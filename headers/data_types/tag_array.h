#ifndef __DATA_TYPES__TAG_ARRAY_H__
#define __DATA_TYPES__TAG_ARRAY_H__

#include <stdlib.h>
#include "tag.h"

struct tag_array {
  size_t count;
  size_t capacity;
  struct tag *tags;
};

#define TAG_ARRAY_ZERO {0, 0, NULL}

void tag_array_deinit(struct tag_array *a);

int tag_array_append(struct tag_array *a, struct tag *e);

#endif /* __DATA_TYPES__TAG_ARRAY_H__ */
