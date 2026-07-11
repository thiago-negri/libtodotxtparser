#include "data_types/array.h"
#include "macros.h"
#include "return_codes.h"
#include <string.h>

#define GROW_QTY 10

#define MAX_SIZE_T ((size_t)-1)

void array_deinit(size_t *count, size_t *capacity, void **array) {
  ASSERT(count != NULL);
  ASSERT(capacity != NULL);
  ASSERT(array != NULL);
  if (*array != NULL) {
    FREE(*array);
    *array = NULL;
  }
  *count = 0;
  *capacity = 0;
}

int array_grow(size_t elem_size, size_t *capacity, void **array) {
  int rc = OK;
  void *new_array = NULL;
  size_t new_capacity = 0;
  size_t new_size = 0;

  ASSERT(elem_size > 0);
  ASSERT(capacity != NULL);
  ASSERT(array != NULL);

  ASSERT(*capacity <= MAX_SIZE_T - GROW_QTY);
  new_capacity = *capacity + GROW_QTY;

  ASSERT(new_capacity > 0 && elem_size <= MAX_SIZE_T / new_capacity);
  new_size = elem_size * new_capacity;

  if (*array == NULL) {
    new_array = MALLOC(new_size);
  } else {
    new_array = REALLOC(*array, new_size);
  }

  if (new_array == NULL) {
    rc = EOOM;
    goto _err;
  }

  *array = new_array;
  *capacity = new_capacity;
  rc = OK;
  goto _ok;

_err:
_ok:
  return rc;
}

int array_append(size_t elem_size, size_t *count, size_t *capacity,
                 void **array, void *elem) {
  void *slot = NULL;
  int rc = OK;
  size_t offset = 0;

  ASSERT(elem_size > 0);
  ASSERT(count != NULL);
  ASSERT(capacity != NULL);
  ASSERT(array != NULL);
  ASSERT(elem != NULL);

  if (*count >= *capacity) {
    rc = array_grow(elem_size, capacity, array);
    if (rc != OK) {
      goto _err;
    }
    ASSERT(*count < *capacity);
  }

  ASSERT(*count == 0 || elem_size <= MAX_SIZE_T / *count);
  offset = elem_size * *count;

  slot = (char *)(*array) + offset;

  memcpy(slot, elem, elem_size);
  *count += 1;
  rc = OK;
  goto _ok;

_err:
_ok:
  return rc;
}
