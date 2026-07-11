/* SPDX-License-Identifier: GPL-3.0-or-later */
/* Copyright (C) 2026 Thiago Negri */

#ifndef __DATA_TYPES__STRING_VIEW_ARRAY_H__
#define __DATA_TYPES__STRING_VIEW_ARRAY_H__

#include <stdlib.h>
#include "string_view.h"

/**
 * An array of string views.
 */
struct string_view_array {
  size_t count;
  size_t capacity;
  struct string_view *string_views;
};

#define STRING_VIEW_ARRAY_ZERO {0, 0, NULL}

void string_view_array_deinit(struct string_view_array *a);

int string_view_array_append(struct string_view_array *a,
                             struct string_view *e);

#endif /* __DATA_TYPES__STRING_VIEW_ARRAY_H__ */
