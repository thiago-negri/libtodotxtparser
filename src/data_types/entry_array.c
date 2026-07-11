#include "data_types/entry_array.h"
#include "data_types/array.h"
#include "macros.h"
#include <stdio.h>

void entry_array_deinit(struct entry_array *a) {
  ASSERT(a != NULL);
  array_deinit(&a->count, &a->capacity, (void **)&a->entries);
}

int entry_array_append(struct entry_array *a, struct entry *e) {
  ASSERT(a != NULL);
  ASSERT(e != NULL);
  return array_append(sizeof(struct entry), &a->count, &a->capacity,
                      (void **)&a->entries, (void *)e);
}

void entry_array_print_debug(struct entry_array *a) {
  size_t i = 0;

  printf("entry_array:\n");
  printf("- count: %lu\n", a->count);
  for (i = 0; i < a->count; i++) {
    printf("- entry %lu:\n", i);
    /* TODO: entry_array_print_debug */
    printf("\n");
  }
}
