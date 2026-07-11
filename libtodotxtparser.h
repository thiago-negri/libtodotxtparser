/* SPDX-License-Identifier: GPL-3.0-or-later */
/* Copyright (C) 2026 Thiago Negri */
/* THIS FILE IS GENERATED. DO NOT EDIT. */
#ifndef __LIBTODOTXTPARSER_H__
#define __LIBTODOTXTPARSER_H__
#include <stdlib.h>
#include <assert.h>
void array_deinit(size_t *count, size_t *capacity, void **array);
int array_append(size_t elem_size, size_t *count, size_t *capacity,
                 void **array, void *elem);
int array_grow(size_t elem_size, size_t *capacity, void **array);
/**
 * A slice of an array of chars.
 *
 * It does not own the pointer, hence it's a "view".
 */
struct string_view {
  size_t length;
  const char *data;
};
#define STRING_VIEW_ZERO {0, NULL}
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
struct date {
  int year;
  int month;
  int day;
};
#define DATE_ZERO {0, 0, 0}
int can_parse_date(struct string_view *string);
void parse_date(struct string_view *string, struct date *ret_date);
void date_print_debug(struct date *date);
struct tag {
  struct string_view key;
  struct string_view value;
};
#define TAG_ZERO {STRING_VIEW_ZERO, STRING_VIEW_ZERO}
struct tag_array {
  size_t count;
  size_t capacity;
  struct tag *tags;
};
#define TAG_ARRAY_ZERO {0, 0, NULL}
void tag_array_deinit(struct tag_array *a);
int tag_array_append(struct tag_array *a, struct tag *e);
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
struct entry_array {
  size_t count;
  size_t capacity;
  struct entry *entries;
};
#define ENTRY_ARRAY_ZERO {0, 0, NULL}
void entry_array_deinit(struct entry_array *a);
int entry_array_append(struct entry_array *a, struct entry *e);
void entry_array_print_debug(struct entry_array *a);
#ifndef ASSERT
#define ASSERT assert
#endif /* ASSERT */
#ifndef REALLOC
#define REALLOC realloc
#endif /* REALLOC */
#ifndef MALLOC
#define MALLOC malloc
#endif /* MALLOC */
#ifndef FREE
#define FREE free
#endif /* FREE */
int parse_file(const char *buffer, size_t size,
               struct entry_array *ret_entry_array);
#define OK 0
#define EOOM 1
int is_upper_case_letter(char c);
int is_digit(char c);
int parse_int(struct string_view *string);
#endif /* __LIBTODOTXTPARSER_H__ */
