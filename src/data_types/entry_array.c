/* SPDX-License-Identifier: GPL-3.0-or-later */
/* Copyright (C) 2026 Thiago Negri */

#include "data_types/entry_array.h"
#include "data_types/array.h"
#include "data_types/tag_array.h"
#include "macros.h"
#include <limits.h>
#include <stdio.h>

void entry_array_deinit(struct entry_array *a) {
  size_t i = 0;
  ASSERT(a != NULL);
  for (i = 0; i < a->count; i++) {
    entry_deinit(&a->entries[i]);
  }
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
  size_t j = 0;
  struct entry *e = NULL;

  printf("entry_array:\n");
  printf("- count: %lu\n", a->count);
  for (i = 0; i < a->count; i++) {
    e = &a->entries[i];
    printf("- entry %lu:\n", i);
    printf("  - is_completed...: %d\n", e->is_completed);
    printf("  - priority.......: %d\n", e->priority);
    printf("  - completion_date: ");
    date_print_debug(&e->completion_date);
    printf("\n");
    printf("  - creation_date..: ");
    date_print_debug(&e->creation_date);
    printf("\n");
    ASSERT(e->description.length <= INT_MAX);
    printf("  - description....: %.*s\n", (int)e->description.length,
           e->description.data);
    printf("  - projects.......: ");
    for (j = 0; j < e->projects.count; j++) {
      ASSERT(e->projects.string_views[j].length <= INT_MAX);
      printf("+%.*s ", (int)e->projects.string_views[j].length,
             e->projects.string_views[j].data);
    }
    printf("\n");
    printf("  - contexts.......: ");
    for (j = 0; j < e->contexts.count; j++) {
      ASSERT(e->contexts.string_views[j].length <= INT_MAX);
      printf("@%.*s ", (int)e->contexts.string_views[j].length,
             e->contexts.string_views[j].data);
    }
    printf("\n");
    printf("  - tags...........: ");
    for (j = 0; j < e->tags.count; j++) {
      ASSERT(e->tags.tags[j].key.length <= INT_MAX);
      ASSERT(e->tags.tags[j].value.length <= INT_MAX);
      printf("%.*s:%.*s ", (int)e->tags.tags[j].key.length,
             e->tags.tags[j].key.data, (int)e->tags.tags[j].value.length,
             e->tags.tags[j].value.data);
    }
    printf("\n");
  }
}
