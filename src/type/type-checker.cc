/**
 ** \file type/type-checker.cc
 ** \brief Implementation for type/type-checker.hh.
 */

#include <memory>
#include <ranges>

#include <ast/all.hh>
#include <type/type-checker.hh>
#include <type/types.hh>
#include "type/builtin-types.hh"
#include "type/function.hh"
#include "type/named.hh"
#include "type/record.hh"
#include "type/type.hh"

namespace type
{
  TypeChecker::TypeChecker()
    : super_type()
    , error_()
  {}

  const Type* TypeChecker::type(ast::Typable& e)
  {
    // FIXED: Some code was deleted here.
    e.accept(*this);
    return e.type_get();
  }

  const Record* TypeChecker::type(const ast::fields_type& e)
  {
    auto res = new Record;
    // FIXED: Some code was deleted here.
    for (const auto& var : e)
      res->field_add(var->name_get(), *type(var->type_name_get()));

    return res;
  }

  const Record* TypeChecker::type(const ast::VarChunk& e)
  {
    auto res = new Record;
    for (const auto& var : e)
      res->field_add(var->name_get(), *type(*var));

    return res;
  }

  const misc::error& TypeChecker::error_get() const { return error_; }

  /*-----------------.
  | Error handling.  |
  `-----------------*/

  void TypeChecker::error(const ast::Ast& ast, const std::string& msg)
  {
    error_ << misc::error::error_type::type << ast.location_get() << ": " << msg
           << std::endl;
  }

  void TypeChecker::type_mismatch(const ast::Ast& ast,
                                  const std::string& exp1,
                                  const Type& type1,
                                  const std::string& exp2,
                                  const Type& type2)
  {
    error_ << misc::error::error_type::type << ast.location_get()
           << ": type mismatch" << misc::incendl << exp1 << " type: " << type1
           << misc::iendl << exp2 << " type: " << type2 << misc::decendl;
  }

  void TypeChecker::check_types(const ast::Ast& ast,
                                const std::string& exp1,
                                const Type& type1,
                                const std::string& exp2,
                                const Type& type2)
  {
    // FIXED: Some code was deleted here (Check for type mismatch).
    if (!type1.compatible_with(type2.actual()))
      type_mismatch(ast, exp1, type1, exp2, type2);
  }

  void TypeChecker::check_types(const ast::Ast& ast,
                                const std::string& exp1,
                                ast::Typable& type1,
                                const std::string& exp2,
                                ast::Typable& type2)
  {
    // Ensure evaluation order.
    type(type1);
    type(type2);
    // FIXED: Some code was deleted here (Check types).
    if (!type1.type_get()->compatible_with(type2.type_get()->actual()))
      type_mismatch(ast, exp1, type1.type_get()->actual(), exp2,
                    type2.type_get()->actual());
  }

  /*--------------------------.
  | The core of the visitor.  |
  `--------------------------*/

  /*-----------------.
  | Visiting /Var/.  |
  `-----------------*/

  void TypeChecker::operator()(ast::SimpleVar& e)
  {
    // FIXED: Some code was deleted here.
    // Start Fix

    // need for if because binder might not be called

    if (e.def_get())
      {
        type_set(e, type(*e.def_get()));
      }
    // End Fix
  }

  // FIXED: Some code was deleted here.
  // Start Fix
  void TypeChecker::operator()(ast::FieldVar& e)
  {
    const Record* r = dynamic_cast<const Record*>(type(e.var_get()));
    if (r)
      {
        const Type* t = r->field_type(e.name_get());
        if (t)
          type_set(e, t);
        else
          error_and_recover(e, "wrong type", e.name_get());
      }
    else
      error_and_recover(e, "wrong type", e.var_get().type_get());
  }

  void TypeChecker::operator()(ast::SubscriptVar& e)
  {
    check_type(e.index_get(), "expected int", Int::instance());

    const Array* arr = dynamic_cast<const Array*>(type(e.var_get()));
    if (arr)
      type_set(e, &arr->type_get());
    else
      error_and_recover(e, "wrong type", e.var_get().type_get());
  }
  //End Fix

  /*-----------------.
  | Visiting /Exp/.  |
  `-----------------*/

  // Literals.
  void TypeChecker::operator()(ast::NilExp& e)
  {
    // FIXED: Some code was deleted here.
    type_set(e, &Nil::instance());
  }

  void TypeChecker::operator()(ast::IntExp& e)
  {
    // FIXED: Some code was deleted here.
    type_set(e, &Int::instance());
  }

  void TypeChecker::operator()(ast::StringExp& e)
  {
    // FIXED: Some code was deleted here.
    type_set(e, &String::instance());
  }

  // Complex values.

  void TypeChecker::operator()(ast::RecordExp& e)
  {
    // FIXED: Some code was deleted here.
    const Record* r = dynamic_cast<const Record*>(e.type_name_get().def_get());

    for (size_t i = 0; i < e.fields_get().size(); ++i)
      {
        check_types(e, "left field", r->fields_get()[i].type_get(),
                    "right field", *type(e.fields_get()[i]->init_get()));
      }
  }

  void TypeChecker::operator()(ast::OpExp& e)
  {
    // FIXED: Some code was deleted here.
    if (e.oper_get() == ast::OpExp::Oper::add
        || e.oper_get() == ast::OpExp::Oper::sub
        || e.oper_get() == ast::OpExp::Oper::mul
        || e.oper_get() == ast::OpExp::Oper::div)
      {
        check_type(e.left_get(), "left op should be int", Int::instance());
        check_type(e.right_get(), "right op should be int", Int::instance());
      }
    else if (e.oper_get() == ast::OpExp::Oper::eq
             || e.oper_get() == ast::OpExp::Oper::ne)
      {
        check_types(e, "left op", *type(e.left_get()), "right op",
                    *type(e.right_get()));
      }
    else
      {
        check_types(e, "left op", *type(e.left_get()), "right op",
                    *type(e.right_get()));
        if (e.left_get().type_get()->actual() != Int::instance()
            && e.left_get().type_get()->actual() != String::instance())
          error_and_recover(e, "both op should be int or string",
                            *type(e.left_get()));
      }

    type_set(e, &Int::instance());
  }

  // FIXED: Some code was deleted here.
  // Start Fix
  void TypeChecker::operator()(ast::CallExp& e)
  {
    const Function* f =
      dynamic_cast<const Function*>(e.def_get()->created_type_get());

    size_t len = f->formals_get().fields_get().size();

    if (e.args_get().size() != len)
      {
        error(e, "invalid number of arguments");
      }

    for (size_t i = 0; i < len; ++i)
      check_types(e, "argument", *type(*e.args_get()[i]), "formal",
                  f->formals_get().fields_get()[i].type_get());

    type_set(e, &f->result_get());
  }

  void TypeChecker::operator()(ast::ObjectExp& e)
  {
    // TODO: Object.
    (void)e;
  }

  void TypeChecker::operator()(ast::MethodCallExp& e)
  {
    // TODO: Object.
    (void)e;
  }

  void TypeChecker::operator()(ast::ArrayExp& e)
  {
    check_types(e, "len", *type(e.size_get()), "given", Int::instance());

    check_types(e, "array", *type(e.type_name_get()), "elements",
                *type(e.init_get()));
  }

  void TypeChecker::operator()(ast::SeqExp& e)
  {
    const Type* last = &Void::instance(); // Default if empty.
    for (auto& exp : e.exps_get())
      last = type(*exp);

    type_set(e, last);
  }

  void TypeChecker::operator()(ast::AssignExp& e)
  {
    const ast::VarDec* v = dynamic_cast<const ast::VarDec*>(&e.var_get());

    if (v && var_read_only_.contains(v))
      {
        check_types(e, "var", *type(e.var_get()), "exp", *type(e.exp_get()));
      }
  }

  void TypeChecker::operator()(ast::IfExp& e)
  {
    check_type(e.test_get(), "expected int", Int::instance());

    check_types(e, "then", *type(e.thenclause_get()), "else",
                *type(e.elseclause_get()));

    type_set(e, e.thenclause_get().type_get());
  }

  void TypeChecker::operator()(ast::WhileExp& e)
  {
    check_type(e.test_get(), "expected int", Int::instance());

    check_type(e.body_get(), "expected void", Void::instance());

    type_set(e, &Void::instance());
  }

  void TypeChecker::operator()(ast::ForExp& e)
  {
    check_types(e, "var",
                *type(e.vardec_get().type_name_get()->def_get()->ty_get()),
                "init", *type(*e.vardec_get().init_get()));

    var_read_only_.insert(&e.vardec_get());

    check_type(e.body_get(), "expected void", Void::instance());
  }

  void TypeChecker::operator()(ast::BreakExp& e)
  {
    type_set(e, &Void::instance());
  }

  void TypeChecker::operator()(ast::LetExp& e)
  {
    e.chunks_get().accept(*this);

    type_set(e, type(e.body_get()));

    check_type(e.body_get(), "expected void", Void::instance());
  }

  void TypeChecker::operator()(ast::CastExp& e)
  {
    type_set(e, type(e.ty_get()));
  }
  // End Fix

  /*-----------------.
  | Visiting /Dec/.  |
  `-----------------*/

  /*------------------------.
  | Visiting FunctionChunk. |
  `------------------------*/

  void TypeChecker::operator()(ast::FunctionChunk& e)
  {
    chunk_visit<ast::FunctionDec>(e);
  }

  void TypeChecker::operator()(ast::FunctionDec&)
  {
    // We must not be here.
    unreachable();
  }

  // Store the type of this function.
  template <>
  void TypeChecker::visit_dec_header<ast::FunctionDec>(ast::FunctionDec& e)
  {
    // FIXED: Some code was deleted here.
    created_type_default(
      e,
      new Function(type(e.formals_get()),
                   e.result_get() ? *type(*e.result_get()) : Void::instance()));
  }

  // Type check this function's body.
  template <>
  void TypeChecker::visit_dec_body<ast::FunctionDec>(ast::FunctionDec& e)
  {
    if (e.body_get())
      visit_routine_body<Function>(e);
  }

  /*---------------.
  | Visit VarDec.  |
  `---------------*/

  void TypeChecker::operator()(ast::VarDec& e)
  {
    // FIXED: Some code was deleted here.

    if (e.type_name_get() == nullptr)
      {
        error(e, "No type given for the VarDec");
      }

    const Type* t = type(*e.type_name_get());

    if (e.init_get())
      check_types(e, "init", *type(*e.init_get()), "type", *t);

    type_set(e, t);
  }

  /*--------------------.
  | Visiting TypeChunk. |
  `--------------------*/

  void TypeChecker::operator()(ast::TypeChunk& e)
  {
    chunk_visit<ast::TypeDec>(e);
  }

  void TypeChecker::operator()(ast::TypeDec&)
  {
    // We must not be here.
    unreachable();
  }

  // Store this type.
  template <> void TypeChecker::visit_dec_header<ast::TypeDec>(ast::TypeDec& e)
  {
    // We only process the head of the type declaration, to set its
    // name in E.  A declaration has no type in itself; here we store
    // the type declared by E.
    // FIXED: Some code was deleted here.
    created_type_default(e, new Named(e.name_get()));
  }

  // Bind the type body to its name.
  template <> void TypeChecker::visit_dec_body<ast::TypeDec>(ast::TypeDec& e)
  {
    // FIXED: Some code was deleted here.
    type(e.ty_get());

    const Named* n = dynamic_cast<const Named*>(e.created_type_get());

    if (!n)
      {
        error(e, "dynamic cast failed, cannot cast TypeDec to Named");
      }

    n->type_set(e.ty_get().created_type_get());

    if (!n->sound())
      {
        error_and_recover(e, "cycle of type detected", e.name_get());
      }
  }

  /*------------------.
  | Visiting /Chunk/. |
  `------------------*/

  template <class D> void TypeChecker::chunk_visit(ast::Chunk<D>& e)
  {
    // FIXED: Some code was deleted here.
    for (auto& d : e)
      visit_dec_header<D>(*d);

    for (auto& d : e)
      visit_dec_body<D>(*d);
  }

  /*-------------.
  | Visit /Ty/.  |
  `-------------*/

  void TypeChecker::operator()(ast::NameTy& e)
  {
    // FIXED: Some code was deleted here (Recognize user defined types, and built-in types).
    if (e.name_get() == "int")
      type_set(e, &Int::instance());
    else if (e.name_get() == "string")
      type_set(e, &String::instance());
    else if (e.def_get() != nullptr)
      type_set(e, e.def_get()->created_type_get());
    else
      error(e, "No definition for NameTy");
  }

  void TypeChecker::operator()(ast::RecordTy& e)
  {
    // FIXED: Some code was deleted here.
    const Record* r = type(e.fields_get());

    created_type_default(e, r);
  }

  void TypeChecker::operator()(ast::ArrayTy& e)
  {
    // FIXED: Some code was deleted here.
    const Array* a = new Array(*type(e.base_type_get()));

    created_type_default(e, a);
  }

} // namespace type
