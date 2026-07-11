/* SPDX-License-Identifier: GPL-3.0-or-later */
/* Copyright (C) 2026 Thiago Negri */

#ifndef __UTILS_H__
#define __UTILS_H__

#include "data_types/string_view.h"

int is_upper_case_letter(char c);

int is_digit(char c);

int parse_int(struct string_view *string);

#endif /* __UTILS_H__ */
