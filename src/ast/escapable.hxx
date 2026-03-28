/**
 ** \file ast/escapable.hxx
 ** \brief Inline methods of ast::Escapable.
 */

#pragma once

#include <ast/escapable.hh>

namespace ast
{
  // FIXME: Some code was deleted here.
  inline bool Escapable::escape_get() const { return escaped_; }

  inline void Escapable::escape_set(bool b) { escaped_ = b; }
} // namespace ast
