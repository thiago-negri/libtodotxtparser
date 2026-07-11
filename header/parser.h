/* SPDX-License-Identifier: GPL-3.0-or-later */
/* Copyright (C) 2026 Thiago Negri */

#ifndef __PARSER_H__
#define __PARSER_H__

#include "data_types/entry_array.h"
#include <stdlib.h>

int parse_file(const char *buffer, size_t size,
               struct entry_array *ret_entry_array);

#endif /* __PARSER_H__ */
