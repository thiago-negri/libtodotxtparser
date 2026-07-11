/* SPDX-License-Identifier: GPL-3.0-or-later */
/* Copyright (C) 2026 Thiago Negri */

#ifdef __AMALGAMATION__
#include "libtodotxtparser.h"
#else
#include "data_types/entry_array.h"
#include "parser.h"
#endif
#include <stdio.h>

int main(void) {
  int rc = 0;
  struct entry_array file = ENTRY_ARRAY_ZERO;
  rc = parse_file("x (A) 2026-07-07 hello @world +pf\nfoo +pf +foo "
                  "due:2026-07-12 +bar +spam",
                  0, &file);
  printf("rc = %d\n", rc);
  entry_array_print_debug(&file);
  entry_array_deinit(&file);
  return 0;
}
