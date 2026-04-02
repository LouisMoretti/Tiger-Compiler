/**
 ** \file type/array.hh
 ** \brief The class Array.
 */
#pragma once

#include <type/fwd.hh>
#include <type/type.hh>

namespace type
{
  /// Array types.
  class Array : public Type
  {
    // FIXED: Some code was deleted here.
  public:
    void accept(ConstVisitor& v) const override;
    void accept(Visitor& v) override;
  };

} // namespace type

#include <type/array.hxx>
