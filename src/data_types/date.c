/* SPDX-License-Identifier: GPL-3.0-or-later */
/* Copyright (C) 2026 Thiago Negri */

#include "data_types/date.h"
#include "macros.h"
#include "utils.h"
#include <stdio.h>

int can_parse_date(struct string_view *string) {
  ASSERT(string != NULL);
  return string->length >= 10 &&
         /* YYYY */
         is_digit(string->data[0]) && is_digit(string->data[1]) &&
         is_digit(string->data[2]) && is_digit(string->data[3]) &&
         /* - */
         string->data[4] == '-' &&
         /* MM */
         is_digit(string->data[5]) && is_digit(string->data[6]) &&
         /* - */
         string->data[7] == '-' &&
         /* DD */
         is_digit(string->data[8]) && is_digit(string->data[9]);
}

void parse_date(struct string_view *string, struct date *ret_date) {
  struct string_view slice = STRING_VIEW_ZERO;
  int year = 0;
  int month = 0;
  int day = 0;

  ASSERT(string != NULL);
  ASSERT(ret_date != NULL);
  ASSERT(can_parse_date(string));

  /* (YYYY)-MM-DD */
  slice.length = 4;
  slice.data = string->data;
  year = parse_int(&slice);

  /* YYYY-(MM)-DD */
  slice.length = 2;
  slice.data = &string->data[5];
  month = parse_int(&slice);

  /* YYYY-MM-(DD) */
  slice.length = 2;
  slice.data = &string->data[8];
  day = parse_int(&slice);

  /* Return */
  ret_date->year = year;
  ret_date->month = month;
  ret_date->day = day;
}

void date_print_debug(struct date *date) {
  if (date == NULL || (date->year == 0 && date->month == 0 && date->day == 0)) {
    return;
  }
  printf("%d-", date->year);
  if (date->month < 10)
    printf("0");
  printf("%d-", date->month);
  if (date->day < 10)
    printf("0");
  printf("%d", date->day);
}
