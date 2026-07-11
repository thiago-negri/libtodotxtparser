#!/usr/bin/env bash
# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (C) 2026 Thiago Negri

CFLAGS="-std=c89 -Wall -Wpedantic -Wextra -g -fsanitize=address"
TESTS=$(find test -type f -name '*.c')

if [ "$1" = "amalgamation" ]; then
  ARGS="-I. -D__AMALGAMATION__ libtodotxtparser.c"
else
  OBJECTS=$(find obj -type f -name '*.o')
  ARGS="-Iheader ${OBJECTS[@]}"
fi

mkdir -p bin/test
for f in ${TESTS[@]}; do
  echo "===================== $f"
  gcc $CFLAGS $ARGS $f -o bin/${f%.c}
  chmod +x bin/${f%.c}
  ./bin/${f%.c}
  echo ''
done
