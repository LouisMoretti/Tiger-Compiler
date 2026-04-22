/**
 ** \file llvmtranslate/translator.cc
 ** \brief Implementation of llvmtranslate::Translator
 */

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

#include <llvm/Config/llvm-config.h> // LLVM_VERSION_*
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Verifier.h> // llvm::verifyFunction
#include <llvm/Support/Casting.h>
#include <llvm/TargetParser/Host.h> // llvm::sys
#include <llvm/TargetParser/Triple.h>

#pragma GCC diagnostic pop

#include <ast/all.hh>
#include <llvmtranslate/translator.hh>

namespace llvmtranslate
{
  using namespace std::string_literals;

  namespace
  {
    // Shorthands for integer type and pointer to integer type.
    inline llvm::IntegerType* i64_t(llvm::LLVMContext& ctx)
    {
      return llvm::Type::getInt64Ty(ctx);
    }

    inline llvm::PointerType* i64p_t(llvm::LLVMContext& ctx)
    {
      return llvm::PointerType::get(llvm::Type::getInt64Ty(ctx), 1);
    }

    llvm::AllocaInst* create_alloca(llvm::Function* ll_function,
                                    llvm::Type* ll_type,
                                    const std::string& name)
    {
      // Create an IRBuilder starting at the beginning of the current entry
      // block. LLVM treats allocas as local variables only if they occur at the
      // beginning of a function.
      llvm::IRBuilder<> tmp(&ll_function->getEntryBlock(),
                            ll_function->getEntryBlock().begin());
      return tmp.CreateAlloca(ll_type, nullptr, name);
    }

    // Set default attributes to the functions
    void set_default_attributes(llvm::Function& the_function,
                                const ast::FunctionDec& e)
    {
      the_function.addFnAttr(llvm::Attribute::NoUnwind); // No exceptions in TC
      if (!e.body_get())                                 // Inline primitives
        the_function.addFnAttr(llvm::Attribute::InlineHint);
    }

    std::string function_dec_name(const ast::FunctionDec& e)
    {
      // Rename "_main" to "tc_main"
      if (e.name_get() == "_main")
        return "tc_main";
      // Prefix all the primitives with "tc_"
      if (!e.body_get())
        return "tc_" + e.name_get().get();
      return e.name_get().get();
    }
  } // namespace

  Translator::Translator(llvm::Module& module, escaped_map_type&& escaped)
    : module_{module}
    , ctx_{module_.getContext()}
    , builder_{ctx_}
    , escaped_{std::move(escaped)}
    , type_visitor_{ctx_}
  {
    // Initialize the allocator
    auto function_ltype = llvm::FunctionType::get(
      llvm::PointerType::get(ctx_, 1), {i64_t(ctx_)}, false);

    auto name = "tc_malloc";
    auto linkage = llvm::Function::ExternalLinkage;
    malloc_ = llvm::Function::Create(function_ltype, linkage, name, &module_);

    // The current process triple.
    auto process_triple = llvm::Triple(llvm::sys::getProcessTriple());
    // Set the 64-bit version of the triple.
    module_.setTargetTriple(process_triple.get64BitArchVariant().str());
  }

  void Translator::operator()(const ast::Ast& e)
  {
    translate(e);
    value_ = nullptr;
  }

  llvm::Value* Translator::translate(const ast::Ast& node)
  {
    node.accept(*this);
    return value_;
  }

  llvm::Value* Translator::access_var(const ast::Var& e)
  {
    if (auto var_ast = dynamic_cast<const ast::SimpleVar*>(&e))
      {
        // FIXED: Some code was deleted here.
        return locals_.at(current_function_).at(var_ast->def_get());
      }
    else if (auto arr_ast = dynamic_cast<const ast::SubscriptVar*>(&e))
      {
        // FIXED: Some code was deleted here.
        auto arr = translate(arr_ast->var_get());
        auto index = translate(arr_ast->index_get());
        return builder_.CreateGEP(
          llvm_type(dynamic_cast<const type::Array*>(
                      &arr_ast->var_get().type_get()->actual())
                      ->type_get()),
          arr, index, "");
      }
    else if (auto field_ast = dynamic_cast<const ast::FieldVar*>(&e))
      {
        // FIXED: Some code was deleted here.
        const ast::Var* var = &field_ast->var_get();
        auto var_val = translate(*var);

        // FIXED: Some code was deleted here.
        const type::Record* record_type =
          dynamic_cast<const type::Record*>(&var->type_get()->actual());
        // FIXED: Some code was deleted here.
        misc::symbol field_name = field_ast->name_get();
        // FIXED: Some code was deleted here (Get the index of the field).
        int index = -1;
        for (const auto& f : record_type->fields_get())
          {
            index++;
            if (f.name_get() == field_name)
              break;
          }

        // The GEP instruction provides us with safe pointer arithmetics,
        // usually used with records or arrays.
        // FIXED: Some code was deleted here (Get record's corresponding LLVM type).
        llvm::Type* record_ltype = type_visitor_.get_record_ltype(record_type);
        return builder_.CreateStructGEP(record_ltype, var_val, index,
                                        "fieldptr_"s + field_name.get());
      }
    else
      unreachable();
  }

  llvm::Value* Translator::init_array(llvm::Value* count_val,
                                      llvm::Value* init_val)
  {
    // Cast everything so that it is conform to the signature of init_array
    // int *init_array(int, int)

    // We need to separate the pointers and the ints.
    // LLVM requires a ptrtoint instruction for pointers
    // and a bitcast for others.
    auto init_val_cast = init_val->getType()->isPointerTy()
      ? builder_.CreatePtrToInt(init_val, i64_t(ctx_), "init_array_ptrtoint")
      : builder_.CreateBitCast(init_val, i64_t(ctx_), "init_array_bitcast");

    // Create the init_array function:
    // First, the arguments (int*, int, int)
    std::vector<llvm::Type*> arg_type{i64_t(ctx_), init_val_cast->getType()};

    // Then, create the FunctionType.
    auto init_array_ltype =
      llvm::FunctionType::get(i64p_t(ctx_), arg_type, false);

    // Get the llvm::Function from the module related to the name and type
    auto init_array_function = module_.getOrInsertFunction(
      init_val->getType()->isPointerTy() ? "tc_init_ptr_array"
                                         : "tc_init_array",
      init_array_ltype);

    // Prepare the arguments.
    std::vector<llvm::Value*> arg_vals{count_val, init_val_cast};

    // Create the call.
    auto init_array_call =
      builder_.CreateCall(init_array_function, arg_vals, "init_array_call");

    // Cast the result of the call in the desired type.
    return builder_.CreateBitCast(init_array_call,
                                  init_val->getType()->getPointerTo(1),
                                  "init_array_call_cast");
  }

  llvm::Type* Translator::llvm_type(const type::Type& type)
  {
    type_visitor_(type);
    return type_visitor_.llvm_type_get();
  }

  llvm::FunctionType*
  Translator::llvm_function_type(const type::Function& function_type)
  {
    // Prepare the arguments
    std::vector<llvm::Type*> args_types;
    // First, if there are any escaped vars, create ptr arguments for it
    // (Lambda lifting)

    if (auto escapes_it = escaped_.find(&function_type);
        escapes_it != std::end(escaped_))
      {
        auto& escapes = escapes_it->second;
        args_types.reserve(escapes.size()
                           + function_type.formals_get().fields_get().size());
        for (const auto dec : escapes)
          {
            // FIXED: Some code was deleted here (Get the llvm type of the VarDec).
            llvm::Type* var_ltype = llvm_type(*dec->type_get());
            args_types.emplace_back(llvm::PointerType::getUnqual(var_ltype));
          }
      }
    else
      args_types.reserve(function_type.formals_get().fields_get().size());

    // Then, the actual arguments
    for (const auto& field : function_type.formals_get())
      args_types.emplace_back(llvm_type(field.type_get()));

    llvm::Type* result_ltype = nullptr;
    // FIXED: Some code was deleted here (If the result is void typed, we assign llvm void type to result_ltype).

    auto ltype = function_type.result_get() == type::Void::instance()
      ? result_ltype = llvm::Type::getVoidTy(ctx_)
      : result_ltype = llvm_type(function_type.result_get());

    return llvm::FunctionType::get(result_ltype, args_types, false);
  }

  void Translator::operator()(const ast::SimpleVar& e)
  {
    // Void var types are actually Ints represented by a 0
    // FIXED: Some code was deleted here.
    // Start Fix
    auto ltype = e.type_get() == &type::Void::instance()
      ? i64_t(ctx_)
      : llvm_type(*e.type_get());
    value_ = builder_.CreateLoad(ltype, access_var(e), e.name_get().get());
    // End Fix
  }

  void Translator::operator()(const ast::FieldVar& e)
  {
    // FIXED: Some code was deleted here.
    auto ltype = e.type_get() == &type::Void::instance()
      ? i64_t(ctx_)
      : llvm_type(*e.type_get());
    value_ = builder_.CreateLoad(ltype, access_var(e), e.name_get().get());
  }

  void Translator::operator()(const ast::SubscriptVar& e)
  {
    // FIXED: Some code was deleted here.
    auto ltype = e.type_get() == &type::Void::instance()
      ? i64_t(ctx_)
      : llvm_type(*e.type_get());
    // value_ = builder_.CreateLoad(ltype, access_var(e), e.name_get().get());
    value_ = builder_.CreateLoad(ltype, access_var(e), "subscript");
  }

  void Translator::operator()(const ast::NilExp& e)
  {
    // FIXED: Some code was deleted here (Create a null pointer).
    auto pointer_type =
      llvm::dyn_cast<llvm::PointerType>(llvm_type(*e.type_get()));

    value_ = llvm::ConstantPointerNull::get(pointer_type);
  }

  void Translator::operator()(const ast::IntExp& e)
  {
    // FIXED: Some code was deleted here (Integers in Tiger are all 64bit signed).
    value_ =
      llvm::ConstantInt::get(llvm::Type::getInt64Ty(ctx_), e.value_get());
  }

  void Translator::operator()(const ast::StringExp& e)
  {
    // FIXED: Some code was deleted here (Strings are translated as `i8*` values, like C's `char*`).
    value_ = builder_.CreateGlobalStringPtr(e.value_get(), "Str", 1, &module_);
  }

  void Translator::operator()(const ast::RecordExp& e)
  {
    // Get the record type
    // FIXED: Some code was deleted here.
    const type::Record* record_type = dynamic_cast<const type::Record*>(
      &e.type_name_get().type_get()->actual());

    assert(record_type != nullptr
           && "error cannot get record type from record exp in translator");

    // Type the record and use get_record_ltype() to get its LLVM type
    llvm_type(*record_type);
    auto struct_ltype = type_visitor_.get_record_ltype(record_type);

    // The size of the structure and cast it to int
    auto sizeof_val = llvm::ConstantExpr::getSizeOf(struct_ltype);
    sizeof_val = llvm::ConstantExpr::getTruncOrBitCast(sizeof_val, i64_t(ctx_));

    // Generate the instruction calling Malloc
    auto malloc_val = builder_.CreateMalloc(
      i64_t(ctx_), struct_ltype, sizeof_val, nullptr, malloc_, "malloccall");

    // Init the fields
    // FIXED: Some code was deleted here.

    for (auto f : e.fields_get())
      {
        auto field = translate(f->init_get());
        builder_.CreateStore(field,
                             builder_.CreateStructGEP(
                               struct_ltype, malloc_val,
                               record_type->field_index(f->name_get()), ""));
      }

    value_ = malloc_val;
  }

  void Translator::operator()(const ast::OpExp& e)
  {
    // FIXED : Some code was deleted here.
    // The comparison instructions returns an i1, and we need an i64, since everything
    // is an i64 in Tiger. Use a zero-extension to avoid this.
    auto left = translate(e.left_get());
    auto right = translate(e.right_get());

    switch (e.oper_get())
      {
      case ast::OpExp::Oper::add:
        value_ = builder_.CreateAdd(left, right, "add");
        break;
      case ast::OpExp::Oper::sub:
        value_ = builder_.CreateSub(left, right, "sub");
        break;
      case ast::OpExp::Oper::mul:
        value_ = builder_.CreateMul(left, right, "mul");
        break;
      case ast::OpExp::Oper::div:
        value_ = builder_.CreateSDiv(left, right, "div");
        break;
      case ast::OpExp::Oper::eq:
        value_ = builder_.CreateICmpEQ(left, right, "eq");
        break;
      case ast::OpExp::Oper::ne:
        value_ = builder_.CreateICmpNE(left, right, "ne");
        break;
      case ast::OpExp::Oper::lt:
        value_ = builder_.CreateICmpSLT(left, right, "lt");
        break;
      case ast::OpExp::Oper::le:
        value_ = builder_.CreateICmpSLE(left, right, "le");
        break;
      case ast::OpExp::Oper::gt:
        value_ = builder_.CreateICmpSGT(left, right, "gt");
        break;
      case ast::OpExp::Oper::ge:
        value_ = builder_.CreateICmpSGE(left, right, "ge");
        break;
      }

    value_ = builder_.CreateZExtOrTrunc(value_, i64_t(ctx_), "op_zext");
  }

  void Translator::operator()(const ast::SeqExp& e)
  {
    // An empty SeqExp is an empty expression, so we should return an int
    // containing 0, since its type is void.
    // FIXED: Some code was deleted here.
    if (e.exps_get().empty())
      {
        value_ = llvm::ConstantInt::get(i64_t(ctx_), 0);
      }
    else
      {
        for (auto exp : e.exps_get())
          value_ = translate(*exp);
      }
  }

  void Translator::operator()(const ast::AssignExp& e)
  {
    // FIXED: Some code was deleted here.
    auto exp = translate(e.exp_get());
    builder_.CreateStore(exp, access_var(e.var_get()));
    value_ = llvm::ConstantInt::get(i64_t(ctx_), 0);
  }

  void Translator::operator()(const ast::IfExp& e)
  {
    // FIXME: Some code was deleted here (IfExps are handled in a similar way to Kaleidoscope (see LangImpl5.html)).
  }

  void Translator::operator()(const ast::WhileExp& e)
  {
    // Bb containing the test and the branching
    auto test_bb = llvm::BasicBlock::Create(ctx_, "test", current_function_);
    auto body_bb = llvm::BasicBlock::Create(ctx_, "body", current_function_);
    auto after_bb =
      llvm::BasicBlock::Create(ctx_, "afterloop", current_function_);

    // Save the after block for breaks
    loop_end_[&e] = after_bb;

    // Explicitly fall through from the current block
    builder_.CreateBr(test_bb);

    // Start inside the test BasicBlock
    builder_.SetInsertPoint(test_bb);

    auto cond_val = translate(e.test_get());
    auto zero_val = llvm::ConstantInt::getSigned(cond_val->getType(), 0);
    auto cmp_val = builder_.CreateICmpNE(cond_val, zero_val, "loopcond");

    // Create the branching
    builder_.CreateCondBr(cmp_val, body_bb, after_bb);

    // Translate the body inside the body BasicBlock
    builder_.SetInsertPoint(body_bb);
    // Don't store the return value, is should be void.
    translate(e.body_get());

    // Go back to the Test BasicBlock
    builder_.CreateBr(test_bb);

    // Continue after the loop BasicBlock
    builder_.SetInsertPoint(after_bb);
  }

  void Translator::operator()(const ast::BreakExp& e)
  {
    // FIXED: Some code was deleted here.
    const ast::WhileExp* while_node =
      dynamic_cast<const ast::WhileExp*>(e.def_get());

    assert(while_node != nullptr
           && "cannot access to while definition of breakexp in translator, "
              "error in type_checker or no for-desugar done");

    value_ = builder_.CreateBr(loop_end_.at(while_node));
  }

  void Translator::operator()(const ast::ArrayExp& e)
  {
    // Translate the number of elements,
    // fill the array with the default value, then
    // return the pointer to the allocated zone.
    // FIXED: Some code was deleted here (Use `init_array`).
    value_ = init_array(translate(e.size_get()), translate(e.init_get()));
  }

  void Translator::operator()(const ast::CastExp& e)
  {
    auto exp_val = translate(e.exp_get());
    // FIXED: Some code was deleted here (Destination llvm type).
    llvm::Type* ltype = llvm_type(*e.ty_get().type_get());
    value_ = builder_.CreateBitCast(exp_val, ltype, "cast_exp");
  }

  void Translator::operator()(const ast::FunctionChunk& e)
  {
    for (const auto& fdec : e)
      visit_function_dec_header(*fdec);

    for (const auto& fdec : e)
      // There is nothing to translate for primitives.
      if (fdec->body_get())
        visit_function_dec_body(*fdec);
  }

  void Translator::visit_function_dec_header(const ast::FunctionDec& e)
  {
    bool is_main = e.name_get() == "_main";
    bool is_primitive = e.body_get() == nullptr;
    auto name = function_dec_name(e);

    // FIXED: Some code was deleted here.
    const type::Type* node_type = e.type_get();

    auto& function_type =
      static_cast<const type::Function&>(node_type->actual());
    auto function_ltype = llvm_function_type(function_type);

    // Main and primitives have External linkage.
    // Other Tiger functions are treated as "static" functions in C.
    auto linkage = is_main || is_primitive ? llvm::Function::ExternalLinkage
                                           : llvm::Function::InternalLinkage;

    auto the_function =
      llvm::Function::Create(function_ltype, linkage, name, &module_);
    set_default_attributes(*the_function, e);

    auto& escaped = escaped_[&function_type];

    // Name each argument of the function
    for (auto arg_it = the_function->arg_begin();
         arg_it != the_function->arg_end(); ++arg_it)
      {
        auto i = std::distance(the_function->arg_begin(), arg_it);
        auto var = escaped.size() && static_cast<size_t>(i) < escaped.size()
          ? *std::next(escaped_[&function_type].begin(), i)
          : e.formals_get()[i - escaped.size()];

        arg_it->setName(var->name_get().get());
      }
  }

  void Translator::visit_function_dec_body(const ast::FunctionDec& e)
  {
    auto the_function = module_.getFunction(function_dec_name(e));

    // Save the old function in case a nested function occurs.
    auto old_insert_point = builder_.saveIP();
    auto old_function = current_function_;
    current_function_ = the_function;

    // Create a new basic block to start the function.
    auto bb = llvm::BasicBlock::Create(ctx_, "entry_"s + e.name_get().get(),
                                       the_function);
    builder_.SetInsertPoint(bb);

    // const type::Type* node_type = nullptr;
    // FIXED: Some code was deleted here.
    auto& function_type =
      static_cast<const type::Function&>(e.type_get()->actual());
    auto& escaped = escaped_[&function_type];
    auto& formals = e.formals_get();

    auto arg_it = the_function->arg_begin();

    for (const auto var : escaped)
      {
        locals_[current_function_][var] = &*arg_it;
        ++arg_it;
      }

    // FIXED: Some code was deleted here (Create alloca instructions for each variable).

    for (const auto f : formals)
      {
        auto ltype = f->type_get() == &type::Void::instance()
          ? i64_t(ctx_)
          : llvm_type(*f->type_get());

        auto alloca = create_alloca(the_function, ltype, f->name_get().get());
        locals_[current_function_][f] = alloca;
        builder_.CreateStore(&*arg_it, alloca);
        arg_it++;
      }

    // FIXED: Some code was deleted here (Create a return instruction).

    auto body = translate(*e.body_get());

    if (function_type.result_get() == type::Void::instance())
      builder_.CreateRetVoid();
    else
      builder_.CreateRet(body);

    // Validate the generated code, checking for consistency.
    llvm::verifyFunction(*the_function);

    // Restore the context of the old function.
    current_function_ = old_function;
    builder_.restoreIP(old_insert_point);
  }

  void Translator::operator()(const ast::CallExp& e)
  {
    // Look up the name in the global module table.
    // If it's a primitive, rename the call to tc_name.
    //
    // Then, add the escaped variables and the rest of the arguments to the
    // list of arguments, and return the correct value.
    // FIXED: Some code was deleted here.
    auto name = module_.getNamedGlobal(e.name_get().get());

    if (!e.def_get()->body_get())
      e.def_get()->name_set("tc_name");

    const type::Function* cast =
      dynamic_cast<const type::Function*>(&e.def_get()->type_get()->actual());

    std::vector<llvm::Type*> var_vect;

    for (auto esc : escaped_.at(cast))
      {
        var_vect.emplace_back(llvm_type(*esc->type_get()));
      }

    for (auto parameter : e.def_get()->formals_get())
      {
        var_vect.emplace_back(llvm_type(*parameter->type_get()));
      }

    llvm::ArrayRef<llvm::Type*> args{var_vect.data(),
                                     var_vect.data() + var_vect.size()};

    auto ltype = e.def_get()->type_get() == &type::Void::instance()
      ? i64_t(ctx_)
      : llvm_type(*e.def_get()->type_get());

    auto func_type = llvm::FunctionType::get(ltype, args, false);

    llvm::GlobalValue::LinkageTypes link_type =
      llvm::GlobalValue::LinkageTypes::
        ExternalLinkage; // TODO needs to check with yaka for the enum value

    auto func =
      llvm::Function::Create(func_type, link_type, "funcCall", nullptr);
    /* TODO fix FunctionCallee error and CreatCall error with types

    (it's working, just needs to check types)


    llvm::FunctionCallee* callee =
      llvm::FunctionCallee::FunctionCallee{func_type, func};

    if (e.def_get()->type_get() == &type::Void::instance())
      {
        value_ = builder_.CreateCall(*callee, args, "", nullptr);
      }
    else
      {
        value_ = builder_.CreateCall(*callee, args, "funcCall", nullptr);
      }*/
  }

  void Translator::operator()(const ast::VarDec& e)
  {
    // Void var types are actually Ints represented by a 0
    // FIXED: Some code was deleted here.
    auto ltype = e.type_get() == &type::Void::instance()
      ? i64_t(ctx_)
      : llvm_type(*e.type_get());

    auto alloca = create_alloca(current_function_, ltype, e.name_get().get());
    locals_[current_function_][&e] = alloca;

    value_ = builder_.CreateStore(translate(*e.init_get()), alloca);
    // value_ = builder_.CreateAlloca(ltype, 1, nullptr, e.name_get().get());
  }

} // namespace llvmtranslate
