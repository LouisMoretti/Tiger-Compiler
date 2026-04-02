/**
 ** \file ast/dec.cc
 ** \brief Implementation of ast::Dec.
 */

#include <ast/dec.hh>
#include <ast/visitor.hh>

namespace ast
{
  Dec::Dec(const Location& location, misc::symbol name)
    : Ast(location)
    , Typable()
    , name_(name)
  {}

  // FIXED
  // Start Fix
  void Dec::type_set(const type::Type* type) { this->type_ = type; }
  const type::Type* Dec::type_get() const { return this->type_; }
  // End Fix

} // namespace ast
