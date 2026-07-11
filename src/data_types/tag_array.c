/* SPDX-License-Identifier: GPL-3.0-or-later */
/* Copyright (C) 2026 Thiago Negri */

#include "data_types/tag_array.h"
#include "data_types/array.h"
#include "data_types/tag.h"
#include "macros.h"

void tag_array_deinit(struct tag_array *a) {
  ASSERT(a != NULL);
  array_deinit(&a->count, &a->capacity, (void **)&a->tags);
}

int tag_array_append(struct tag_array *a, struct tag *e) {
  ASSERT(a != NULL);
  ASSERT(e != NULL);
  return array_append(sizeof(struct tag), &a->count, &a->capacity,
                      (void **)&a->tags, (void *)e);
}
