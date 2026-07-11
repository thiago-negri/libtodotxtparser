#ifndef __DATA_TYPES__STRING_VIEW_H__
#define __DATA_TYPES__STRING_VIEW_H__

#include <stdlib.h>

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

#endif /* __DATA_TYPES__STRING_VIEW_H__ */
