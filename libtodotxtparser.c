/* SPDX-License-Identifier: GPL-3.0-or-later */
/* Copyright (C) 2026 Thiago Negri */
/* THIS FILE IS GENERATED. DO NOT EDIT. */
#include "libtodotxtparser.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#define GROW_QTY 10
#define MAX_SIZE_T ((size_t)-1)
void array_deinit(size_t *count, size_t *capacity, void **array) {
  ASSERT(count != NULL);
  ASSERT(capacity != NULL);
  ASSERT(array != NULL);
  if (*array != NULL) {
    FREE(*array);
    *array = NULL;
  }
  *count = 0;
  *capacity = 0;
}
int array_grow(size_t elem_size, size_t *capacity, void **array) {
  int rc = OK;
  void *new_array = NULL;
  size_t new_capacity = 0;
  size_t new_size = 0;
  ASSERT(elem_size > 0);
  ASSERT(capacity != NULL);
  ASSERT(array != NULL);
  ASSERT(*capacity <= MAX_SIZE_T - GROW_QTY);
  new_capacity = *capacity + GROW_QTY;
  ASSERT(new_capacity > 0 && elem_size <= MAX_SIZE_T / new_capacity);
  new_size = elem_size * new_capacity;
  if (*array == NULL) {
    new_array = MALLOC(new_size);
  } else {
    new_array = REALLOC(*array, new_size);
  }
  if (new_array == NULL) {
    rc = EOOM;
    goto _err;
  }
  *array = new_array;
  *capacity = new_capacity;
  rc = OK;
  goto _ok;
_err:
_ok:
  return rc;
}
int array_append(size_t elem_size, size_t *count, size_t *capacity,
                 void **array, void *elem) {
  void *slot = NULL;
  int rc = OK;
  size_t offset = 0;
  ASSERT(elem_size > 0);
  ASSERT(count != NULL);
  ASSERT(capacity != NULL);
  ASSERT(array != NULL);
  ASSERT(elem != NULL);
  if (*count >= *capacity) {
    rc = array_grow(elem_size, capacity, array);
    if (rc != OK) {
      goto _err;
    }
    ASSERT(*count < *capacity);
  }
  ASSERT(*count == 0 || elem_size <= MAX_SIZE_T / *count);
  offset = elem_size * *count;
  slot = (char *)(*array) + offset;
  memcpy(slot, elem, elem_size);
  *count += 1;
  rc = OK;
  goto _ok;
_err:
_ok:
  return rc;
}
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
void entry_deinit(struct entry *e) {
  ASSERT(e != NULL);
  string_view_array_deinit(&e->projects);
  string_view_array_deinit(&e->contexts);
  tag_array_deinit(&e->tags);
}
void entry_array_deinit(struct entry_array *a) {
  size_t i = 0;
  ASSERT(a != NULL);
  for (i = 0; i < a->count; i++) {
    entry_deinit(&a->entries[i]);
  }
  array_deinit(&a->count, &a->capacity, (void **)&a->entries);
}
int entry_array_append(struct entry_array *a, struct entry *e) {
  ASSERT(a != NULL);
  ASSERT(e != NULL);
  return array_append(sizeof(struct entry), &a->count, &a->capacity,
                      (void **)&a->entries, (void *)e);
}
void entry_array_print_debug(struct entry_array *a) {
  size_t i = 0;
  size_t j = 0;
  struct entry *e = NULL;
  printf("entry_array:\n");
  printf("- count: %lu\n", a->count);
  for (i = 0; i < a->count; i++) {
    e = &a->entries[i];
    printf("- entry %lu:\n", i);
    printf("  - is_completed...: %d\n", e->is_completed);
    printf("  - priority.......: %d\n", e->priority);
    printf("  - completion_date: ");
    date_print_debug(&e->completion_date);
    printf("\n");
    printf("  - creation_date..: ");
    date_print_debug(&e->creation_date);
    printf("\n");
    ASSERT(e->description.length <= INT_MAX);
    printf("  - description....: %.*s\n", (int)e->description.length,
           e->description.data);
    printf("  - projects.......: ");
    for (j = 0; j < e->projects.count; j++) {
      ASSERT(e->projects.string_views[j].length <= INT_MAX);
      printf("+%.*s ", (int)e->projects.string_views[j].length,
             e->projects.string_views[j].data);
    }
    printf("\n");
    printf("  - contexts.......: ");
    for (j = 0; j < e->contexts.count; j++) {
      ASSERT(e->contexts.string_views[j].length <= INT_MAX);
      printf("@%.*s ", (int)e->contexts.string_views[j].length,
             e->contexts.string_views[j].data);
    }
    printf("\n");
    printf("  - tags...........: ");
    for (j = 0; j < e->tags.count; j++) {
      ASSERT(e->tags.tags[j].key.length <= INT_MAX);
      ASSERT(e->tags.tags[j].value.length <= INT_MAX);
      printf("%.*s:%.*s ", (int)e->tags.tags[j].key.length,
             e->tags.tags[j].key.data, (int)e->tags.tags[j].value.length,
             e->tags.tags[j].value.data);
    }
    printf("\n");
  }
}
void string_view_array_deinit(struct string_view_array *a) {
  ASSERT(a != NULL);
  array_deinit(&a->count, &a->capacity, (void **)&a->string_views);
}
int string_view_array_append(struct string_view_array *a,
                             struct string_view *e) {
  ASSERT(a != NULL);
  ASSERT(e != NULL);
  return array_append(sizeof(struct string_view), &a->count, &a->capacity,
                      (void **)&a->string_views, (void *)e);
}
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
static int parse_line(struct string_view *string, struct entry *ret_entry) {
  int rc = OK;
  size_t line_length = 0;
  const char *line = NULL;
  const char *line_end = NULL;
  int is_completed = 0;
  int priority = NO_PRIORITY;
  struct string_view date = STRING_VIEW_ZERO;
  struct date creation_date = DATE_ZERO;
  struct date completion_date = DATE_ZERO;
  struct string_view description = STRING_VIEW_ZERO;
  const char *word = NULL;
  const char *cur = NULL;
  struct string_view word_alloc = STRING_VIEW_ZERO;
  struct string_view_array projects = STRING_VIEW_ARRAY_ZERO;
  struct string_view_array contexts = STRING_VIEW_ARRAY_ZERO;
  struct tag tag = TAG_ZERO;
  struct tag_array tags = TAG_ARRAY_ZERO;
  ASSERT(string != NULL);
  ASSERT(ret_entry != NULL);
  line_length = string->length;
  line = string->data;
  /* Set line end so we do not read past */
  line_end = &line[line_length];
  /* It's marked as completed if first char is 'x' followed by space */
  if (line_length > 2 && line[0] == 'x' && line[1] == ' ') {
    is_completed = 1;
    /* Skip the completion marker */
    line += 2;
    line_length -= 2;
  }
  /* Parses priority */
  if (line_length > 4 && line[0] == '(' && is_upper_case_letter(line[1]) &&
      line[2] == ')' && line[3] == ' ') {
    priority = line[1] - 'A';
    /* Skip priority */
    line += 4;
    line_length -= 4;
  }
  /* Parses dates */
  if (line_length > 11 && line[10] == ' ') {
    date.length = 10;
    date.data = line;
    if (can_parse_date(&date)) {
      parse_date(&date, &creation_date);
      /* Skip date */
      line += 11;
      line_length -= 11;
      if (line_length > 11 && line[10] == ' ') {
        date.length = 10;
        date.data = line;
        if (can_parse_date(&date)) {
          completion_date = creation_date;
          parse_date(&date, &creation_date);
          /* Skip date */
          line += 11;
          line_length -= 11;
        }
      }
    }
  }
  /* Rest of the line is the description */
  description.data = line;
  description.length = line_length;
  /* Parses contexts, projects, and tags */
  word = line;
  while (word != line_end) {
    /* Skip spaces */
    for (; word != line_end && (*word == ' ' || *word == '\t'); word++)
      ;
    if (word == line_end)
      break;
    /* Parse project / context */
    if ((line_end - word) > 1 && (word[0] == '+' || word[0] == '@')) {
      for (cur = word + 1; cur != line_end && *cur != ' ' && *cur != '\t';
           cur++)
        ;
      word_alloc.length = cur - &word[1];
      word_alloc.data = &word[1];
      switch (word[0]) {
      case '+':
        rc = string_view_array_append(&projects, &word_alloc);
        if (rc != OK) {
          goto _err;
        }
        break;
      case '@':
        rc = string_view_array_append(&contexts, &word_alloc);
        if (rc != OK) {
          goto _err;
        }
        break;
      }
      /* Skip word */
      word = cur;
      continue;
    }
    /* Parse tags */
    for (cur = word;
         cur != line_end && *cur != ' ' && *cur != '\t' && *cur != ':'; cur++)
      ;
    if (cur[0] == ':' && (cur + 1) != line_end && cur[1] != ' ') {
      tag.key.length = cur - word;
      tag.key.data = word;
      /* Skip ':' */
      cur++;
      for (word = cur; cur != line_end && *cur != ' ' && *cur != '\t'; cur++)
        ;
      tag.value.length = cur - word;
      tag.value.data = word;
      rc = tag_array_append(&tags, &tag);
      if (rc != OK) {
        goto _err;
      }
      /* Skip word */
      word = cur;
      continue;
    }
    /* Skip word */
    for (word = cur; word != line_end && *word != ' ' && *word != '\t'; word++)
      ;
  }
  /* Return */
  ret_entry->is_completed = is_completed;
  ret_entry->priority = priority;
  ret_entry->completion_date = completion_date;
  ret_entry->creation_date = creation_date;
  ret_entry->description = description;
  ret_entry->projects = projects;
  ret_entry->contexts = contexts;
  ret_entry->tags = tags;
  rc = OK;
  goto _ok;
_err:
  string_view_array_deinit(&projects);
  string_view_array_deinit(&contexts);
  tag_array_deinit(&tags);
_ok:
  return rc;
}
int parse_file(const char *buffer, size_t size,
               struct entry_array *ret_entry_array) {
  int rc = OK;
  const char *buffer_end = NULL;
  const char *line_start = NULL;
  const char *line_end = NULL;
  size_t line_length = 0;
  struct string_view line = STRING_VIEW_ZERO;
  struct entry e = ENTRY_ZERO;
  struct entry_array entry_array = ENTRY_ARRAY_ZERO;
  /* Set buffer_end so we never read past */
  if (size > 0) {
    buffer_end = &buffer[size];
  }
  line_start = buffer;
  while (line_start != buffer_end && *line_start != '\0') {
    /* Skip spaces */
    for (; line_start != buffer_end &&
           (*line_start == ' ' || *line_start == '\t' || *line_start == '\n' ||
            *line_start == '\0');
         line_start++)
      ;
    /* Stop early if reached end */
    if (line_start == buffer_end || *line_start == '\0') {
      break;
    }
    /* Grabs one line */
    line_length = 0;
    line_end = line_start;
    while (line_end != buffer_end && *line_end != '\0' && *line_end != '\n') {
      ASSERT(line_length <= ((size_t)-1) - 1);
      line_length++;
      line_end++;
    }
    line.data = line_start;
    line.length = line_length;
    printf("line: %.*s\n", (int)line.length, line.data);
    /* Parse one line */
    rc = parse_line(&line, &e);
    if (rc != OK) {
      goto _err;
    }
    /* Append entry */
    rc = entry_array_append(&entry_array, &e);
    if (rc != OK) {
      goto _err;
    }
    line_start = line_end;
  }
  /* Done */
  *ret_entry_array = entry_array;
  rc = OK;
  goto _ok;
_err:
  entry_array_deinit(&entry_array);
_ok:
  return rc;
}
int is_upper_case_letter(char a) { return a >= 'A' && a <= 'Z'; }
int is_digit(char a) { return a >= '0' && a <= '9'; }
int parse_int(struct string_view *string) {
  size_t i = 0;
  size_t length = 0;
  int result = 0;
  int digit = 0;
  const char *data = NULL;
  ASSERT(string != NULL);
  ASSERT(string->length > 0);
  data = string->data;
  length = string->length;
  ASSERT(is_digit(data[0]));
  result = data[0] - '0';
  for (i = 1; i < length; i++) {
    ASSERT(is_digit(data[i]));
    ASSERT(result <= INT_MAX / 10);
    result *= 10;
    digit = data[i] - '0';
    ASSERT(result <= INT_MAX - digit);
    result += digit;
  }
  return result;
}
