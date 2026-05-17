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
#include <string.h>

#include "tiger-gc.h"

char* tc_init_string(size_t size)
{
  // // FIXED: Some code was deleted here.
  // void* res = tc_malloc(size);
  // if (!res)
  //   return NULL;

  // char* string = malloc(size);

  // // add the new last element at the end of the heap (maybe need optimisation later)
  // if (gc_ctx_.heap == NULL)
  //   {
  //     gc_ctx_.heap = malloc(sizeof(struct list_obj));

  //     if (!gc_ctx_.heap)
  //       return NULL;

  //     gc_ctx_.heap->actual = malloc(sizeof(struct gc_object));
  //     gc_ctx_.heap->actual->md = res;
  //     memcpy(gc_ctx_.heap->actual->f, string, size);
  //     gc_ctx_.heap->next = NULL;
  //   }
  // else
  //   {
  //     struct list_obj* last_elt = gc_ctx_.heap;
  //     while (last_elt->next != NULL)
  //       {
  //         last_elt = last_elt->next;
  //       }

  //     struct list_obj* new_last = malloc(sizeof(struct list_obj));
  //     if (!new_last)
  //       return NULL;

  //     last_elt->next = new_last;

  //     new_last->actual = malloc(sizeof(struct gc_object));
  //     new_last->actual->md = res;
  //     memcpy(new_last->actual->f, string, size);
  //     new_last->next = NULL;
  //   }

  // return string;
  return tc_malloc(size);
}

size_t* tc_init_array(size_t size, size_t elt)
{
  // // FIXED: Some code was deleted here.
  // void* res = tc_malloc(size * elt);
  // if (!res)
  //   return NULL;

  // size_t* array = malloc(size * elt);

  // // add the new last element at the end of the heap (maybe need optimisation later)
  // if (gc_ctx_.heap == NULL)
  //   {
  //     gc_ctx_.heap = malloc(sizeof(struct list_obj));

  //     if (!gc_ctx_.heap)
  //       return NULL;

  //     gc_ctx_.heap->actual = malloc(sizeof(struct gc_object));
  //     gc_ctx_.heap->actual->md = res;
  //     memcpy(gc_ctx_.heap->actual->f, array,8*size);
  //     gc_ctx_.heap->next = NULL;
  //   }
  // else
  //   {
  //     struct list_obj* last_elt = gc_ctx_.heap;
  //     while (last_elt->next != NULL)
  //       {
  //         last_elt = last_elt->next;
  //       }

  //     struct list_obj* new_last = malloc(sizeof(struct list_obj));
  //     if (!new_last)
  //       return NULL;

  //     last_elt->next = new_last;

  //     new_last->actual = malloc(sizeof(struct gc_object));
  //     new_last->actual->md = res;
  //     memcpy(new_last->actual->f,array, 8*size);
  //     new_last->next = NULL;
  //   }

  // return array;
  size_t* tmp = tc_malloc(size * sizeof(size_t));
  for (size_t i = 0; i < size; i++)
    tmp[i] = elt;
  return tmp;
}

size_t* tc_init_ptr_array(size_t size, struct gc_object* elt)
{
  // FIXME: Some code was deleted here.
  size_t* res = tc_malloc(size * sizeof(size_t));
  for (size_t i = 0; i < size; i++)
    res[i] = (size_t)elt;
  return res;
}

void* tc_init_record(size_t size)
{
  // struct gc_object* res = NULL;
  // FIXME: Some code was deleted here.

  // Return a pointer to the first field
  // struct gc_md, f0, f1, f2 ...
  //                ^
  //                |
  // return res->f;
  return tc_malloc(size);
}

void* tc_malloc(size_t size)
{
  // FIXED: Some code was deleted here.
  struct gc_object* res = calloc(1, sizeof(struct gc_md) + size);

  if (!res)
    {
      gc_collect();
      res = calloc(1, sizeof(struct gc_md) + size);
      if (!res)
        exit(EXIT_FAILURE);
    }

  struct list_obj* node = malloc(sizeof(struct list_obj));
  if (!node)
    exit(EXIT_FAILURE);

  node->actual = res;
  node->next = gc_ctx_.heap;
  gc_ctx_.heap = node;

  res->md.marked = 0;
  res->md.size = size;
  return res->f;
}
