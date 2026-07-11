/* SPDX-License-Identifier: GPL-3.0-or-later */
/* Copyright (C) 2026 Thiago Negri */

#ifndef __DATA_TYPES__DATE_H__
#define __DATA_TYPES__DATE_H__

#include "string_view.h"

struct date {
  int year;
  int month;
  int day;
};

#define DATE_ZERO {0, 0, 0}

int can_parse_date(struct string_view *string);

void parse_date(struct string_view *string, struct date *ret_date);

void date_print_debug(struct date *date);

#endif /* __DATA_TYPES__DATE_H__ */
