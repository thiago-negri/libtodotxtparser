#include "utils.h"
#include "data_types/string_view.h"
#include "macros.h"
#include <limits.h>
#include <stdlib.h>

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
