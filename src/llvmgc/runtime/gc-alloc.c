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
  char* res = tc_malloc(size);
  if (!res)
    return NULL;

  return res;
}

size_t* tc_init_array(size_t size, size_t elt)
{
  // FIXED: Some code was deleted here.
  size_t* res = tc_malloc(size);
  if (!res)
    return NULL;

  for (size_t i = 0; i < size; i++)
    {
      res[i] = elt;
    }

  return res;
}

size_t* tc_init_ptr_array(size_t size, struct gc_object* elt)
{
  // FIXED: Some code was deleted here.
  size_t* res = tc_malloc(size * sizeof(size_t));
  if (!res)
    return NULL;

  for (size_t i = 0; i < size; i++)
    {
      res[i] = (size_t)elt;
    }

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
  void* res = malloc(size);
  return res;
}
