#include <assert.h>
#include <stdio.h>
#include "../../src/llvmgc/runtime/gc-alloc.h"
#include "../../src/llvmgc/runtime/tiger-gc.h"

// To prevent undefined reference and double main.
void init_consts(void) {}
void tc_main(int i) { (void)i; }
#define main old_main
#include "../../src/llvmgc/runtime/tiger-gc.c"
#undef main
#include "../../src/llvmgc/runtime/gc-alloc.c"

int linked_list_length(void)
{
  int res = 0;
  struct list_obj* lst = gc_ctx_.heap;
  while (lst)
    {
      res++;
      lst = lst->next;
    }
  return res;
}

int main(void)
{
  // Init
  gc_ctx_.tos = __builtin_frame_address(0);

  // Basic alloc.
  tc_malloc(42);
  assert(linked_list_length() == 1);
  tc_malloc(67);
  assert(linked_list_length() == 2);

  // Collect and none should be left (no ref).
  gc_collect();
  assert(linked_list_length() == 0);

  // Alloc with ref
  size_t* tmp = tc_malloc(42);
  (void)tmp;
  assert(linked_list_length() == 1);
  gc_collect();
  assert(linked_list_length() == 1);

  // Collect with ref removed.
  tmp = NULL;
  gc_collect();
  assert(linked_list_length() == 0);

  // Pointer of pointer ref.
  tmp = tc_malloc(sizeof(void*));
  assert(linked_list_length() == 1);

  *tmp = (size_t)tc_malloc(42);

  assert(linked_list_length() == 2);
  *tmp = 0;
  gc_collect();
  assert(linked_list_length() == 1);

  tmp = NULL;
  gc_collect();
  assert(linked_list_length() == 0);

  // Scope ref (Not working well)
  {
    size_t* tmp2 = tc_malloc(67);
    (void)tmp2;
    assert(linked_list_length() == 1);
    gc_collect();
    assert(linked_list_length() == 1);
    tmp2 = NULL;
  }

  gc_collect();
  assert(linked_list_length() == 0);

  // Many alloc no ref.
#define SixSeven 67
  for (int i = -42; i < SixSeven; i++)
    tc_malloc(SixSeven);

  assert(linked_list_length() == SixSeven + 42);
  gc_collect();
  assert(linked_list_length() == 0);

  // Many alloc ref linked list with no value.
  size_t* base = tc_malloc(sizeof(void*));
  size_t* ptr = base;
  for (int i = -42; i < SixSeven; i++)
    {
      *ptr = (size_t)tc_malloc(SixSeven);
      ptr = (size_t*)*ptr;
    }
  ptr = NULL;

  assert(linked_list_length() == SixSeven + 42 + 1);
  gc_collect();
  assert(linked_list_length() == SixSeven + 42 + 1);

  base = NULL;
  gc_collect();
  assert(linked_list_length() == 0);

  return 0;
}
