/**
 ** \file bind/tasks.cc
 ** \brief Bind module tasks implementation.
 */

// FIXME: Some code was deleted here.

// Start Fix

#include "bind/tasks.hh"

#include "ast/libast.hh"
#include "bind/libbind.hh"

#include "bind/binder.hh"

namespace bind::tasks
{
  void bind() { launch_bind(); }

  void bind_print() { ast::bindings_display(std::cout) = true; }

} // namespace bind::tasks

// End Fix
