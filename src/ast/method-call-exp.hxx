/**
 ** \file ast/method-call-exp.hxx
 ** \brief Inline methods of ast::MethodCallExp.
 */

#pragma once

#include <ast/method-call-exp.hh>

namespace ast
{

  inline const Var& MethodCallExp::object_get() const { return *object_; }
  inline Var& MethodCallExp::object_get() { return *object_; }

  // FIXED: Some code was deleted here.
  // Start Fix
  inline MethodDec* MethodCallExp::def_get() const { return def_; }
  inline void MethodCallExp::def_set(MethodDec* def) { def_ = def; }
  // End Fix

} // namespace ast
