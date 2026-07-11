/*
 * Copyright (C) 2026 Thiago Negri
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include "data_types/entry_array.h"
#include "parser.h"
#include <stdio.h>

int main(void) {
  int rc = 0;
  struct entry_array file = ENTRY_ARRAY_ZERO;
  rc = parse_file("x (A) 2026-07-07 hello @world +pf\nfoo +pf +foo due:2026-07-12 +bar +spam", 0, &file);
  printf("rc = %d\n", rc);
  entry_array_print_debug(&file);
  entry_array_deinit(&file);
  return 0;
}
