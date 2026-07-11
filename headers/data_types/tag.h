/* SPDX-License-Identifier: GPL-3.0-or-later */
/* Copyright (C) 2026 Thiago Negri */

#ifndef __DATA_TYPES__TAG_H__
#define __DATA_TYPES__TAG_H__

#include "string_view.h"

struct tag {
  struct string_view key;
  struct string_view value;
};

#define TAG_ZERO {STRING_VIEW_ZERO, STRING_VIEW_ZERO}

#endif /* __DATA_TYPES__TAG_H__ */
