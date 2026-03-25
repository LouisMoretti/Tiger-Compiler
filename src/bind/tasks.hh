/**
 ** \file bind/tasks.hh
 ** \brief Bind module related tasks.
 */

// FIXME: Some code was deleted here.

// Start Fix

#include <misc/fwd.hh>
#include <task/libtask.hh>

namespace bind::tasks
{
  TASK_GROUP("3. Binding");

  TASK_DECLARE("b|bindings-compute", "binding a file", bind, "parse");
  DISJUNCTIVE_TASK_DECLARE("bound",
                           "Make sure bindings (regular or taking overloading "
                           "or objects constructs into account) are computed",
                           "bindings-compute");
  TASK_DECLARE("B|bindings-display",
               "binding and printing a file",
               bind_print,
               "parse");
  TASK_DECLARE("rename",
               "TODO: Trouver quoi dire.",
               rename,
               "bindings-compute");
} // namespace bind::tasks

// End Fix
