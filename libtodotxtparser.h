/*
 * Copyright (C) 2026 Thiago Negri
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef __LIBTODOTXTPARSER_H__
#define __LIBTODOTXTPARSER_H__

#include <stdlib.h>

struct string {
  size_t length;
  char *data;
};
#define STRING_ZERO {0, NULL}

struct string_array {
  size_t count;
  struct string *data;
};
#define STRING_ARRAY_ZERO {0, NULL}

struct tag {
  struct string key;
  struct string value;
};
#define TAG_ZERO {STRING_ZERO, STRING_ZERO}

struct tag_array {
  size_t count;
  struct tag *tags;
};
#define TAG_ARRAY_ZERO {0, NULL}

struct date {
  int year;
  int month;
  int day;
};
#define DATE_ZERO {0, 0, 0}

struct entry {
  int is_completed;
  int priority;
  struct date completion_date;
  struct date creation_date;
  struct string description;
  struct string_array projects;
  struct string_array contexts;
  struct tag_array tags;
};
#define ENTRY_ZERO                                                             \
  {0,                                                                          \
   0,                                                                          \
   DATE_ZERO,                                                                  \
   DATE_ZERO,                                                                  \
   STRING_ZERO,                                                                \
   STRING_ARRAY_ZERO,                                                          \
   STRING_ARRAY_ZERO,                                                          \
   TAG_ARRAY_ZERO}

struct entry_array {
  size_t count;
  struct entry *entries;
};
#define ENTRY_ARRAY_ZERO {NULL, 0}

#define NO_PRIORITY (('Z' - 'A') + 1)

#define OK 0
#define EOOM 1

int parse(const char *buffer, size_t size, struct entry_array *ret_entry_array);

void entry_array_clear(struct entry_array *entry_array);

void entry_array_print_debug(struct entry_array *entry_array);

#endif /* __LIBTODOTXTPARSER_H__ */
