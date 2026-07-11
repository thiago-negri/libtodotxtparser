/* SPDX-License-Identifier: GPL-3.0-or-later */
/* Copyright (C) 2026 Thiago Negri */

#ifndef __DATA_TYPES__ENTRY_H__
#define __DATA_TYPES__ENTRY_H__

#include "date.h"
#include "string_view.h"
#include "string_view_array.h"
#include "tag_array.h"

struct entry {
  int is_completed;
  int priority;
  struct date completion_date;
  struct date creation_date;
  struct string_view description;
  struct string_view_array projects;
  struct string_view_array contexts;
  struct tag_array tags;
};

#define ENTRY_ZERO                                                             \
  {0,                                                                          \
   0,                                                                          \
   DATE_ZERO,                                                                  \
   DATE_ZERO,                                                                  \
   STRING_VIEW_ZERO,                                                           \
   STRING_VIEW_ARRAY_ZERO,                                                     \
   STRING_VIEW_ARRAY_ZERO,                                                     \
   TAG_ARRAY_ZERO}

#define NO_PRIORITY (('Z' - 'A') + 1)

void entry_deinit(struct entry *e);

#endif /* __DATA_TYPES__ENTRY_H__ */
