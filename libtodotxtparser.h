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

struct todotxt_tag {
  char *key;
  char *value;
};

struct todotxt_date {
  int year;
  int month;
  int day;
};
#define TODOTXT_DATE_ZERO {0, 0, 0}

struct todotxt_entry {
  int is_completed;
  int priority;
  struct todotxt_date completion_date;
  struct todotxt_date creation_date;
  char *description;
  char **projects;
  char **contexts;
  struct todotxt_tag *tags;
  size_t tag_count;
};
#define TODOTXT_ENTRY_ZERO {0, 0, TODOTXT_DATE_ZERO, TODOTXT_DATE_ZERO, NULL, NULL, NULL, NULL, 0}

struct todotxt_file {
  struct todotxt_entry *entries;
  size_t count;
};
#define TODOTXT_FILE_ZERO {NULL, 0}

#define TODOTXT_NO_PRIORITY (('Z' - 'A') + 1)

#define TODOTXT_OK 0
#define TODOTXT_ERR_OOM 1

int todotxt_parse_alloc(const char *buffer, size_t size,
                        struct todotxt_file *ret_file);

void todotxt_file_free(struct todotxt_file *file);

void todotxt_print_debug(struct todotxt_file *file);

#endif /* __LIBTODOTXTPARSER_H__ */
