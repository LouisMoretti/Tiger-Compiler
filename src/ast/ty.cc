/**
 ** \file ast/ty.cc
 ** \brief Implementation of ast::Ty.
 */

#include <ast/ty.hh>
#include <ast/visitor.hh>

namespace ast
{
  Ty::Ty(const Location& location)
    : Ast(location)
    , Typable()
    , TypeConstructor()
  {}

  // FIXED
  // Start Fix
  void Ty::type_set(const type::Type* type) { this->type_ = type; }
  const type::Type* Ty::type_get() const { return this->type_; }

  void Ty::created_type_set(const type::Type* created_type)
  {
    this->created_type_ = created_type;
  }

  const type::Type* Ty::created_type_get() const { return this->created_type_; }
  // End Fix

} // namespace ast
