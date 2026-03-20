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
  TASK_GROUP("2. Binding");

  TASK_DECLARE("bindings-compute", "binding a file", bind, "parse");
  TASK_DECLARE("bindings-display",
               "binding and printing a file",
               bind_print,
               "parse");
} // namespace bind::tasks

// End Fix