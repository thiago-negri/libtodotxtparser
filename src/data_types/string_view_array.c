#include "data_types/string_view_array.h"
#include "data_types/array.h"
#include "data_types/string_view.h"
#include "macros.h"

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
