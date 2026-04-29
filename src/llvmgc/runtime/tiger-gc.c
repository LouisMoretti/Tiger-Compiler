/**
 ** \file llvmgc/runtime/tiger-gc.c
 ** \brief Implementation of the TC gc.
 ** It holds the main algorithm of the collector.
 ** This is a suggestion of skeleton for an implementation of a garbage collector.
 */

#include "tiger-gc.h"
#include <stdlib.h>
#include "fwd.h"

/// Defined by the compiler
extern void tc_main(int);
/// The function that defines the consts array
extern void init_consts(void);

struct gc_ctx gc_ctx_ = {
  // FIXED: Some code was deleted here.
  // Start Fix
  .heap = NULL,
  // End Fix
  .gc_enabled = true,
  .tos = NULL,
};

void gc_collect()
{
  // TODO add DFS to mark from all elements on the stack z
  if (!gc_ctx_.gc_enabled)
    return;

  // FIXED: Some code was deleted here (Run the collector).
  struct list_obj* h = gc_ctx_.heap;
  struct list_obj* prev = NULL;

  // For each element of the linked list, checks if the element is marked
  // if it is, reset the marked at false, if it's not marked, free the element
  // and continue until the end of the list
  while (h != NULL)
    {
      struct gc_object* actual_obj = h->actual;

      if (actual_obj->md->marked)
        {
          actual_obj->md->marked = 0;
          prev = h;
          h = h->next;
        }
      else
        {
          free(actual_obj->md);
          free(actual_obj);
          if (prev == NULL)
            {
              gc_ctx_.heap = h->next;
            }
          else
            {
              prev->next = h->next;
            }

          struct list_obj* tmp = h->next;
          free(h);

          h = tmp;
        }
    }
}

void gc_enter_runtime()
{
  // FIXME: Some code was deleted here (Initialize any context required by the runtime).
}

void gc_exit_runtime()
{
  // FIXME: Some code was deleted here (Any logic required when exiting the runtime and going back to the tiger function).
}

int main(void)
{
  init_consts();

#ifdef GCDEBUG
  // This code will be included if the Tiger program was compiled with the task `--llvm-gc-debug`
#endif

  /// Initialize the top of the stack with main's frame address.
  gc_ctx_.tos = __builtin_frame_address(0);

  // FIXME: Some code was deleted here (Initialize any additional context required by the allocator and garbage collctor).

  tc_main(0);

  return 0;
}
