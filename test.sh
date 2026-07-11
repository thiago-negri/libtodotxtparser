#!/usr/bin/env bash
# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (C) 2026 Thiago Negri

OBJECTS=$(find obj -type f -name '*.o')
CFLAGS=$(cat ./compile_flags.txt)
TESTS=$(find test -type f -name '*.c')

ARGS=${OBJECTS[@]}
if [ "$1" = "amalgamation" ]; then
  ARGS=libtodotxtparser.c
fi

mkdir -p bin/test
for f in ${TESTS[@]}; do
  echo "===================== $f"
  gcc ${CFLAGS[@]} -g -fsanitize=address $f $ARGS -o bin/${f%.c}
  chmod +x bin/${f%.c}
  ./bin/${f%.c}
  echo ''
done
