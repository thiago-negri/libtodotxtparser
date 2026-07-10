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

#include "libtodotxtparser.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef REALLOC
#define REALLOC _realloc
#endif

static void *_realloc(void *p, size_t s) {
  if (p == NULL) {
    if (s != 0) {
      return malloc(s);
    }
    return NULL;
  } else if (s == 0) {
    free(p);
    return NULL;
  } else {
    return realloc(p, s);
  }
}

static int is_uppercase_letter(char a) { return a >= 'A' && a <= 'Z'; }

static int is_digit(char a) { return a >= '0' && a <= '9'; }

static int is_date(struct string *string) {
  if (string == NULL || string->length < 10) {
    return 0;
  }
  return
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

static int parse_int(struct string *string) {
  size_t i = 0;
  size_t length = 0;
  int result = 0;
  char *data = NULL;

  assert(string != NULL);
  assert(string->length > 0);

  data = string->data;
  length = string->length;

  assert(is_digit(data[0]));
  result = data[0] - '0';
  for (i = 1; i < length; i++) {
    assert(is_digit(data[i]));
    result *= 10;
    result += data[i] - '0';
  }

  return result;
}

static void parse_date(struct string *string, struct date *ret_date) {
  struct string slice = STRING_ZERO;
  int year = 0;
  int month = 0;
  int day = 0;

  assert(string != NULL);
  assert(is_date(string));

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

static void print_date_debug(struct date *date) {
  if (date == NULL || (date->year == 0 && date->month == 0 && date->day == 0)) {
    return;
  }
  printf("%d-", date->year);
  if (date->month < 10) {
    printf("0");
  }
  printf("%d-", date->month);
  if (date->day < 10) {
    printf("0");
  }
  printf("%d", date->day);
}

struct da {
  char **data;
  size_t size;
  size_t cap;
};
#define DA_ZERO {NULL, 0, 0}

#define DA_GROW_QTY (4)

static int da_grow(struct da *da) {
  size_t cap_new = 0;
  char **data_new = NULL;

  assert(da != NULL);

  cap_new = da->cap + DA_GROW_QTY;
  data_new = REALLOC(da->data, sizeof(char *) * cap_new);
  if (data_new == NULL) {
    return EOOM;
  }

  da->data = data_new;
  da->cap += cap_new;
  return OK;
}

static int da_append(struct da *da, char *datum) {
  int rc = 0;

  if (da->size >= da->cap) {
    rc = da_grow(da);
    if (rc != TODOTXT_OK) {
      return rc;
    }
  }

  da->data[da->size++] = datum;

  return TODOTXT_OK;
}

static void da_free(struct da *da) {
  TODOTXT_REALLOC(da->data, 0);
  da->cap = 0;
  da->size = 0;
}

char *str_alloc(const char *start, const char *end) {
  size_t length = end - start;
  char *str = TODOTXT_REALLOC(NULL, length);
  if (str == NULL) {
    return NULL;
  }
  strncpy(str, start, length);
  return str;
}

char **da_to_array_alloc(struct da *da) {
  size_t size = sizeof(char *) * da->size;
  char **result = TODOTXT_REALLOC(NULL, size);
  if (result == NULL) {
    return NULL;
  }
  memcpy(result, da->data, size);
  return result;
}

static void todotxt_tag_free(struct todotxt_tag *tag) {
  TODOTXT_REALLOC(tag->key, 0);
  TODOTXT_REALLOC(tag->value, 0);
}

static void todotxt_entry_free(struct todotxt_entry *entry) {
  size_t i = 0;

  TODOTXT_REALLOC(entry->description, 0);
  entry->description = NULL;

  if (entry->projects != NULL || entry->project_count > 0) {
    for (i = 0; i < entry->project_count; i++) {
      TODOTXT_REALLOC(entry->projects[i], 0);
    }
    TODOTXT_REALLOC(entry->projects, 0);
    entry->projects = NULL;
    entry->project_count = 0;
  }

  if (entry->contexts != NULL || entry->context_count > 0) {
    for (i = 0; i < entry->context_count; i++) {
      TODOTXT_REALLOC(entry->contexts[i], 0);
    }
    TODOTXT_REALLOC(entry->contexts, 0);
    entry->contexts = NULL;
    entry->context_count = 0;
  }

  if (entry->tags != NULL || entry->tag_count > 0) {
    for (i = 0; i < entry->tag_count; i++) {
      todotxt_tag_free(&entry->tags[i]);
    }
    TODOTXT_REALLOC(entry->tags, 0);
    entry->tags = NULL;
    entry->tag_count = 0;
  }
}

static int parse_line_alloc(const char *line, const char *line_end,
                            struct todotxt_entry *ret_entry) {
  int rc = TODOTXT_OK;
  size_t line_length = 0;
  const char *word = NULL;
  const char *cur = NULL;
  int is_completed = 0;
  int priority = TODOTXT_NO_PRIORITY;
  const char *description = NULL;
  struct todotxt_date creation_date = TODOTXT_DATE_ZERO;
  struct todotxt_date completion_date = TODOTXT_DATE_ZERO;
  struct da tag_keys = DA_ZERO;
  struct da tag_values = DA_ZERO;
  struct da projects = DA_ZERO;
  struct da contexts = DA_ZERO;
  struct todotxt_entry entry = TODOTXT_ENTRY_ZERO;
  char *word_alloc = NULL;
  size_t i = 0;

  line_length = line_end - line;

  /* It's marked as completed if first char is 'x' followed by space */
  if (line_length > 2 && line[0] == 'x' && line[1] == ' ') {
    is_completed = 1;

    /* Skip the completion marker */
    line += 2;
    line_length -= 2;
  } else {
    is_completed = 0;
  }

  /* Parses priority */
  if (line_length > 4 && line[0] == '(' && is_uppercase_letter(line[1]) &&
      line[2] == ')' && line[3] == ' ') {
    priority = line[1] - 'A';

    /* Skip priority */
    line += 4;
    line_length -= 4;
  } else {
    priority = TODOTXT_NO_PRIORITY;
  }

  /* Parses dates */
  if (line_length > 11 && is_date(line) && line[10] == ' ') {
    parse_date(line, &creation_date);

    /* Skip date */
    line += 11;
    line_length -= 11;

    if (line_length > 11 && is_date(line) && line[10] == ' ') {
      completion_date = creation_date;
      parse_date(line, &creation_date);

      /* Skip date */
      line += 11;
      line_length -= 11;
    } else {
      completion_date.year = 0;
      completion_date.month = 0;
      completion_date.day = 0;
    }
  } else {
    creation_date.year = 0;
    creation_date.year = 0;
    creation_date.month = 0;
    completion_date.day = 0;
    completion_date.month = 0;
    completion_date.day = 0;
  }

  /* Rest of the line is the description */
  description = line;

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

      word_alloc = str_alloc(&word[1], cur);
      if (word_alloc == NULL) {
        rc = TODOTXT_ERR_OOM;
        goto _err;
      }

      switch (word[0]) {
      case '+':
        rc = da_append(&projects, word_alloc);
        if (rc != TODOTXT_OK) {
          goto _err;
        }
        word_alloc = NULL;
        break;
      case '@':
        rc = da_append(&contexts, word_alloc);
        if (rc != TODOTXT_OK) {
          goto _err;
        }
        word_alloc = NULL;
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
      word_alloc = str_alloc(word, cur);
      if (word_alloc == NULL) {
        rc = TODOTXT_ERR_OOM;
        goto _err;
      }

      rc = da_append(&tag_keys, word_alloc);
      if (rc != TODOTXT_OK) {
        goto _err;
      }
      word_alloc = NULL;

      /* Skip ':' */
      cur++;

      for (word = cur; cur != line_end && *cur != ' ' && *cur != '\t'; cur++)
        ;

      word_alloc = str_alloc(word, cur);
      if (word_alloc == NULL) {
        rc = TODOTXT_ERR_OOM;
        goto _err;
      }

      rc = da_append(&tag_values, word_alloc);
      if (rc != TODOTXT_OK) {
        goto _err;
      }
      word_alloc = NULL;

      /* Skip word */
      word = cur;

      continue;
    }

    /* Skip word */
    for (word = cur; word != line_end && *word != ' ' && *word != '\t'; word++)
      ;
  }

  /* Allocate and return */
  entry.is_completed = is_completed;
  entry.priority = priority;
  entry.completion_date = completion_date;
  entry.creation_date = creation_date;
  entry.description = str_alloc(description, line_end);
  if (entry.description == NULL) {
    rc = TODOTXT_ERR_OOM;
    goto _err;
  }
  if (projects.size > 0) {
    entry.projects = da_to_array_alloc(&projects);
    if (entry.projects == NULL) {
      rc = TODOTXT_ERR_OOM;
      goto _err;
    }
    entry.project_count = projects.size;
  } else {
    entry.projects = NULL;
    entry.project_count = 0;
  }
  if (contexts.size > 0) {
    entry.contexts = da_to_array_alloc(&contexts);
    if (entry.contexts == NULL) {
      rc = TODOTXT_ERR_OOM;
      goto _err;
    }
    entry.context_count = contexts.size;
  } else {
    entry.contexts = NULL;
    entry.context_count = 0;
  }
  if (tag_keys.size > 0) {
    entry.tags =
        TODOTXT_REALLOC(NULL, sizeof(struct todotxt_tag) * tag_keys.size);
    if (entry.tags == NULL) {
      rc = TODOTXT_ERR_OOM;
      goto _err;
    }
    for (i = 0; i < tag_keys.size; i++) {
      entry.tags[i].key = tag_keys.data[i];
      entry.tags[i].value = tag_values.data[i];
    }
    entry.tag_count = tag_keys.size;
  } else {
    entry.tags = NULL;
    entry.tag_count = 0;
  }

  *ret_entry = entry;
  goto _ok;

_err:
  TODOTXT_REALLOC(word_alloc, 0);
  todotxt_entry_free(&entry);

_ok:
  da_free(&tag_keys);
  da_free(&tag_values);
  da_free(&contexts);
  da_free(&projects);
  return rc;
}

int todotxt_parse_alloc(const char *buffer, size_t size,
                        struct todotxt_file *ret_file) {
  const char *buffer_end = NULL;
  const char *line = NULL;
  const char *line_end = NULL;
  struct todotxt_entry *entries = NULL;
  struct todotxt_entry *entries_new = NULL;
  size_t entries_size = 0;
  size_t entries_cap = 0;
  struct todotxt_file file = TODOTXT_FILE_ZERO;
  size_t i = 0;
  int rc = TODOTXT_OK;

  /* Set buffer_end so we never read past */
  if (size > 0) {
    buffer_end = &buffer[size];
  }

  line = buffer;

  while (line != buffer_end && *line != '\0') {
    /* Skip spaces */
    for (; line != buffer_end &&
           (*line == ' ' || *line == '\t' || *line == '\n' || *line == '\0');
         line++)
      ;

    if (line == buffer_end || *line == '\0') {
      break;
    }

    /* Grabs one line */
    for (line_end = line;
         line_end != buffer_end && *line_end != '\0' && *line_end != '\n';
         line_end++)
      ;
    printf("line: %.*s\n", (int)(line_end - line), line);

    if (entries_size >= entries_cap) {
      entries_new = TODOTXT_REALLOC(entries, sizeof(struct todotxt_entry) *
                                                 (entries_cap + 10) * 1.3);
      if (entries_new == NULL) {
        rc = TODOTXT_ERR_OOM;
        goto _err;
      }
      entries = entries_new;
      entries_new = NULL;
      entries_cap = (entries_cap + 10) * 1.3;
    }

    /* Parses one line */
    rc = parse_line_alloc(line, line_end, &entries[entries_size]);
    if (rc != TODOTXT_OK) {
      goto _err;
    }
    entries_size++;

    line = line_end;
  }

  file.entries =
      TODOTXT_REALLOC(NULL, sizeof(struct todotxt_entry) * entries_size);
  if (file.entries == NULL) {
    rc = TODOTXT_ERR_OOM;
    goto _err;
  }
  memcpy(file.entries, entries, sizeof(struct todotxt_entry) * entries_size);
  file.count = entries_size;

  rc = TODOTXT_OK;
  *ret_file = file;
  goto _ok;

_err:
  for (i = 0; i < entries_size; i++) {
    todotxt_entry_free(&entries[i]);
  }

_ok:
  TODOTXT_REALLOC(entries, 0);
  entries = NULL;
  return rc;
}

void todotxt_file_free(struct todotxt_file *file) {
  size_t i = 0;
  for (i = 0; i < file->count; i++) {
    todotxt_entry_free(&file->entries[i]);
  }
  TODOTXT_REALLOC(file->entries, 0);
  file->entries = NULL;
  file->count = 0;
}

void todotxt_print_debug(struct todotxt_file *file) {
  size_t i = 0;
  size_t j = 0;

  printf("entry count: %lu\n", file->count);
  for (i = 0; i < file->count; i++) {
    printf("entry %lu\n", i);
    printf("  description....: %s\n", file->entries[i].description);
    printf("  is_completed...: %d\n", file->entries[i].is_completed);
    printf("  priority.......: %d\n", file->entries[i].priority);
    printf("  completion_date: ");
    print_date_debug(&file->entries[i].completion_date);
    printf("\n");
    printf("  creation_date..: ");
    print_date_debug(&file->entries[i].creation_date);
    printf("\n");
    printf("  projects.......: ");
    for (j = 0; j < file->entries[i].project_count; j++) {
      printf("+%s ", file->entries[i].projects[j]);
    }
    printf("\n");
    printf("  contexts.......: ");
    for (j = 0; j < file->entries[i].context_count; j++) {
      printf("@%s ", file->entries[i].contexts[j]);
    }
    printf("\n");
    printf("  tags...........: ");
    for (j = 0; j < file->entries[i].tag_count; j++) {
      printf("%s:%s ", file->entries[i].tags[j].key,
             file->entries[i].tags[j].value);
    }
    printf("\n");
  }
}
