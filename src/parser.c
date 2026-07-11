#include <stdio.h>
#include "parser.h"
#include "data_types/date.h"
#include "data_types/entry.h"
#include "data_types/entry_array.h"
#include "data_types/tag.h"
#include "data_types/tag_array.h"
#include "macros.h"
#include "return_codes.h"
#include "utils.h"

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
