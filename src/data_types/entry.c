/* SPDX-License-Identifier: GPL-3.0-or-later */
/* Copyright (C) 2026 Thiago Negri */

#include "data_types/entry.h"
#include "macros.h"

void entry_deinit(struct entry *e) {
  ASSERT(e != NULL);
  string_view_array_deinit(&e->projects);
  string_view_array_deinit(&e->contexts);
  tag_array_deinit(&e->tags);
}
