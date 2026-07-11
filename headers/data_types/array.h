/* SPDX-License-Identifier: GPL-3.0-or-later */
/* Copyright (C) 2026 Thiago Negri */

#ifndef __ARRAY_H__
#define __ARRAY_H__

#include <stdlib.h>

void array_deinit(size_t *count, size_t *capacity, void **array);

int array_append(size_t elem_size, size_t *count, size_t *capacity,
                 void **array, void *elem);

int array_grow(size_t elem_size, size_t *capacity, void **array);

#endif /* __ARRAY_H__ */
