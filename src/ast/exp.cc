/**
 ** \file ast/exp.cc
 ** \brief Implementation of ast::Exp.
 */

#include <ast/exp.hh>
#include <ast/visitor.hh>

namespace ast
{
  Exp::Exp(const Location& location)
    : Ast(location)
    , Typable()
  {}

  // FIXED
  // Start Fix
  void Exp::type_set(const type::Type* type) { this->type_ = type; }
  const type::Type* Exp::type_get() const { return this->type_; }
  // End Fix

} // namespace ast
