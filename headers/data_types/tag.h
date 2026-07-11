#ifndef __DATA_TYPES__TAG_H__
#define __DATA_TYPES__TAG_H__

#include "string_view.h"

struct tag {
  struct string_view key;
  struct string_view value;
};

#define TAG_ZERO {STRING_ZERO, STRING_ZERO}

#endif /* __DATA_TYPES__TAG_H__ */
