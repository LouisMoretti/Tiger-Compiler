/**
 ** \file type/array.cc
 ** \brief Implementation for type/array.hh.
 */

#include <type/array.hh>
#include <type/visitor.hh>

namespace type
{
  // FIXME: Some code was deleted here.
  // TODO: Check if need to store type inside.
  void Array::accept(ConstVisitor& v) const { v(*this); }
  void Array::accept(Visitor& v) { v(*this); }

} // namespace type
