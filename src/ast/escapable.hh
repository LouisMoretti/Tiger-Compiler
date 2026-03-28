/**
 ** \file ast/escapable.hh
 ** \brief Declaration of ast::Escapable.
 */

#pragma once

#include <ast/fwd.hh>

namespace ast
{
  /// Escapable.
  class Escapable
  {
    // FIXME: Some code was deleted here.
  public:
    bool escape_get() const;
    void escape_set(bool b);

  private:
    bool escaped_ = true;
  };
} // namespace ast
#include <ast/escapable.hxx>
