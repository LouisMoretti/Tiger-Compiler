/**
 ** \file llvmgc/runtime/gc-alloc.c
 ** \brief Implementation of the TC allocation functions.
 **
 ** This file holds the functions used directly and indirectly by the Tiger program and the runtime
 ** to allocate arrays, records and strings.
 ** These functions should attempt to allocate memory according to their specifications and
 ** call the garbage collector when necessary.
 */

#include "gc-alloc.h"
#include <stdlib.h>
#include "fwd.h"
#include "tiger-gc.h"

char* tc_init_string(size_t size)
{
  // FIXED: Some code was deleted here.
  void* res = tc_malloc(size);
  if (!res)
    return NULL;

  char* string = malloc(size);

  // add the new last element at the end of the heap (maybe need optimisation later)
  if (gc_ctx_.heap == NULL)
    {
      gc_ctx_.heap = malloc(sizeof(struct list_obj));

      if (!gc_ctx_.heap)
        return NULL;

      gc_ctx_.heap->actual = malloc(sizeof(struct gc_object));
      gc_ctx_.heap->actual->md = res;
      gc_ctx_.heap->actual->f = (tc_word_t)string;
      gc_ctx_.heap->next = NULL;
    }
  else
    {
      struct list_obj* last_elt = gc_ctx_.heap;
      while (last_elt->next != NULL)
        {
          last_elt = last_elt->next;
        }

      struct list_obj* new_last = malloc(sizeof(struct list_obj));
      if (!gc_ctx_.heap)
        return NULL;

      last_elt->next = new_last;

      new_last->actual = malloc(sizeof(struct gc_object));
      new_last->actual->md = res;
      new_last->actual->f = (tc_word_t)string;
      new_last->next = NULL;
    }

  return string;
}

size_t* tc_init_array(size_t size, size_t elt)
{
  // FIXED: Some code was deleted here.
  void* res = tc_malloc(size * elt);
  if (!res)
    return NULL;

  size_t* array = malloc(size * elt);

  // add the new last element at the end of the heap (maybe need optimisation later)
  if (gc_ctx_.heap == NULL)
    {
      gc_ctx_.heap = malloc(sizeof(struct list_obj));

      if (!gc_ctx_.heap)
        return NULL;

      gc_ctx_.heap->actual = malloc(sizeof(struct gc_object));
      gc_ctx_.heap->actual->md = res;
      gc_ctx_.heap->actual->f = (tc_word_t)array;
      gc_ctx_.heap->next = NULL;
    }
  else
    {
      struct list_obj* last_elt = gc_ctx_.heap;
      while (last_elt->next != NULL)
        {
          last_elt = last_elt->next;
        }

      struct list_obj* new_last = malloc(sizeof(struct list_obj));
      if (!gc_ctx_.heap)
        return NULL;

      last_elt->next = new_last;

      new_last->actual = malloc(sizeof(struct gc_object));
      new_last->actual->md = res;
      new_last->actual->f = (tc_word_t)array;
      new_last->next = NULL;
    }

  return array;
}

size_t* tc_init_ptr_array(size_t size, struct gc_object* elt)
{
  // FIXME: Some code was deleted here.
  size_t* res = NULL;
  return res;
}

void* tc_init_record(
  // FIXME: Some code was deleted here.
)
{
  struct gc_object* res = NULL;
  // FIXME: Some code was deleted here.

  // Return a pointer to the first field
  // struct gc_md, f0, f1, f2 ...
  //                ^
  //                |
  return res->f;
}

void* tc_malloc(size_t size)
{
  // FIXED: Some code was deleted here.
  struct gc_md* res = malloc(sizeof(struct gc_md));
  res->marked = 0;
  res->size = size;
  return res;
}
