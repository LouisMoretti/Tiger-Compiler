#include <cassert>
#include <ast/all.hh>
#include <ast/default-visitor.hh>
#include <ast/non-object-visitor.hh>
#include <llvmtranslate/escapes-collector.hh>

namespace llvmtranslate
{
  /// LLVM IR doesn't support static link and nested functions.
  /// In order to translate those functions to LLVM IR, we use a technique
  /// called Lambda Lifting, which consists in passing a pointer to
  /// the escaped variables to the nested function using that variable.
  ///
  /// In order to do that, we need a visitor to collect these kind of
  /// variables and associate them to each function.

  class EscapesCollector
    : public ast::DefaultConstVisitor
    , public ast::NonObjectConstVisitor
  {
  public:
    /// Super class.
    using super_type = ast::DefaultConstVisitor;
    /// Import overloaded operator() methods.
    using super_type::operator();

    EscapesCollector()
      : did_modify_{false}
      , escaped_{}
    {}

    escaped_map_type& escaped_get() { return escaped_; }

    void operator()(const ast::FunctionChunk& e) override
    {
      bool saved_did_modify = did_modify_;

      // Iterate on the chunk in order to iteratively collect all the callee
      // functions' escaped variables.
      did_modify_ = !e.empty();
      while (did_modify_)
        {
          did_modify_ = false;
          super_type::operator()(e);
        }

      did_modify_ = saved_did_modify;
    }

    void operator()(const ast::FunctionDec& e) override
    {
      // Keep track of the current function
      // FIXED: Some code was deleted here.
      auto tmp = this->actual_func_;
      const type::Function* cast =
        dynamic_cast<const type::Function*>(&e.type_get()->actual());

      assert(cast && "function dec not a function, error in type checker");

      this->actual_func_ = cast;

      if (!this->escaped_.count(this->actual_func_))
        this->escaped_[this->actual_func_] = misc::set<const ast::VarDec*>();

      super_type::operator()(e);

      this->actual_func_ = tmp;
    }

    void operator()(const ast::CallExp& e) override
    {
      super_type::operator()(e);

      // FIXED: Some code was deleted here.
      if (e.def_get() == nullptr)
        return;

      // Check whether there are any newly collected escaped variables.
      // If there are, mark the iteration as modified.
      // FIXED: Some code was deleted here.

      // for (auto arg : e.args_get())
      //   {
      //     const ast::VarDec* cast = dynamic_cast<const ast::VarDec*>(arg);
      //     // assert(cast != nullptr
      //     //        && "var dec not a vardec, error in type checker");
      //     if (cast && cast->escape_get())
      //       {
      //         escaped_.at(this->actual_func_).insert(cast);
      //       }
      //   }

      const type::Function* def_type =
        dynamic_cast<const type::Function*>(&e.def_get()->type_get()->actual());
      if (def_type == nullptr || escaped_.count(def_type) == 0)
        return;

      auto& def_escaped = escaped_.at(actual_func_);

      for (const auto escaped : escaped_.at(def_type))
        if (def_escaped.insert(escaped).second)
          did_modify_ = true;
    }

    void operator()(const ast::SimpleVar& e) override
    {
      // Associate escaped variables declared in parent frames with their
      // functions
      // FIXED: Some code was deleted here.
      const ast::VarDec* cast = dynamic_cast<const ast::VarDec*>(e.def_get());

      assert(cast && "var dec not a vardec, error in type checker");

      if (cast->escape_get())
        {
          escaped_.at(this->actual_func_).insert(cast);
        }
    }

  private:
    /// Whether any modification was done during the iteration.
    bool did_modify_ = false;

    /// Associate a set of variables with their function.
    escaped_map_type escaped_;

    /// Current visiting function.
    // FIXED: Some code was deleted here.
    const type::Function* actual_func_;
  };

  escaped_map_type collect_escapes(const ast::Ast& ast)
  {
    EscapesCollector collect;
    collect(ast);

    return std::move(collect.escaped_get());
  }

} // namespace llvmtranslate
