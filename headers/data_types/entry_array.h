#ifndef __DATA_TYPES__ENTRY_ARRAY_H__
#define __DATA_TYPES__ENTRY_ARRAY_H__

#include "data_types/entry.h"
#include <stdlib.h>

struct entry_array {
  size_t count;
  size_t capacity;
  struct entry *entries;
};

#define ENTRY_ARRAY_ZERO {NULL, 0}

void entry_array_deinit(struct entry_array *a);

int entry_array_append(struct entry_array *a, struct entry *e);

void entry_array_print_debug(struct entry_array *a);

#endif /* __DATA_TYPES__ENTRY_ARRAY_H__ */
