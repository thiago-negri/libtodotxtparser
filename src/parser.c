#include "parser.h"
#include "data_types/entry.h"
#include "data_types/entry_array.h"
#include "macros.h"
#include "return_codes.h"

static int parse_line(struct string_view *line, struct entry *entry) {
  /* TODO: parse_line, check libtodotxtparser.c parse_line_alloc */
  (void)line;
  (void)entry;
  return OK;
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

    /* Parse one line */
    rc = parse_line(&line, &e);
    if (rc != OK) {
      goto _err;
    }

    /* Append entry */
    rc = entry_array_append(ret_entry_array, &e);
    if (rc != OK) {
      goto _err;
    }
  }

  /* Done */
  rc = OK;
  goto _ok;

_err:
  entry_array_deinit(ret_entry_array);

_ok:
  return rc;
}
