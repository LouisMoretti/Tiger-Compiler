/**
 ** \file ast/type-dec.cc
 ** \brief Implementation of ast::TypeDec.
 */

#include <ast/type-dec.hh>
#include <ast/visitor.hh>

namespace ast
{
  TypeDec::TypeDec(const Location& location, misc::symbol name, Ty* ty)
    : Dec(location, name)
    , TypeConstructor()
    , ty_(ty)
  {}

  TypeDec::~TypeDec() { delete ty_; }

  void TypeDec::accept(ConstVisitor& v) const { v(*this); }

  void TypeDec::accept(Visitor& v) { v(*this); }

  // FIXED
  // Start Fix
  void TypeDec::created_type_set(const type::Type* created_type)
  {
    this->created_type_ = created_type;
  }
  const type::Type* TypeDec::created_type_get() const
  {
    return this->created_type_;
  }
  // End Fix
} // namespace ast
