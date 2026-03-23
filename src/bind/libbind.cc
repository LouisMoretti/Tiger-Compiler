/**
 ** \file bind/libbind.cc
 ** \brief Define exported bind functions.
 */

// FIXME: Some code was deleted here.

#include <ast/libast.hh>
#include "ast/tasks.hh"
#include "bind/binder.hh"

void launch_bind()
{
  bind::Binder binder;
  binder(*ast::tasks::the_program);
}
