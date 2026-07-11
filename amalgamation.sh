#!/usr/bin/env bash
# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (C) 2026 Thiago Negri

HEADER_FILE=libtodotxtparser.h

SOURCE_FILE=libtodotxtparser.c

# In dependency order
HEADERS=(
    header/data_types/array.h
    header/data_types/string_view.h
    header/data_types/string_view_array.h
    header/data_types/date.h
    header/data_types/tag.h
    header/data_types/tag_array.h
    header/data_types/entry.h
    header/data_types/entry_array.h
    header/macros.h
    header/parser.h
    header/return_codes.h
    header/utils.h
  )

SOURCES=$(find src/ -type f -name '*.c')

# HEADER
if [ "$1" = "header" ]; then
  # License
  echo '/* SPDX-License-Identifier: GPL-3.0-or-later */' >$HEADER_FILE
  echo '/* Copyright (C) 2026 Thiago Negri */' >>$HEADER_FILE
  echo '/* THIS FILE IS GENERATED. DO NOT EDIT. */' >>$HEADER_FILE
  # Amalgamation include guard
  echo '#ifndef __LIBTODOTXTPARSER_H__' >>$HEADER_FILE
  echo '#define __LIBTODOTXTPARSER_H__' >>$HEADER_FILE
  # Concatenate all headers
  for f in ${HEADERS[@]}; do
    cat $f >>$HEADER_FILE
  done
  # Remove extra license notes
  sed -i '6,${/.*SPDX-License-Identifier: GPL-3.0-or-later.*/d}' $HEADER_FILE
  sed -i '6,${/.*Copyright (C) 2026 Thiago Negri.*/d}' $HEADER_FILE
  # Remove local includes and group global includes at top
  INCLUDES=$(grep '#include <' $HEADER_FILE | sort -u)
  sed -i '7,${/#include .*/d}' $HEADER_FILE
  IFS=$'\n'; for inc in ${INCLUDES[@]}; do
    sed -i "7i $inc" $HEADER_FILE
  done
  # Remove include guard of individual files
  sed -i '7,${/#ifndef __.*_H__.*/d}' $HEADER_FILE
  sed -i '7,${/#define __.*_H__.*/d}' $HEADER_FILE
  sed -i '7,${/#endif \/\* __.*_H__.*/d}' $HEADER_FILE
  # Add final amalgamation include guard
  echo '#endif /* __LIBTODOTXTPARSER_H__ */' >>$HEADER_FILE
  # Remove empty lines
  sed -i '/^$/d' $HEADER_FILE
fi

# SOURCE
if [ "$1" = "source" ]; then
  # License
  echo '/* SPDX-License-Identifier: GPL-3.0-or-later */' >$SOURCE_FILE
  echo '/* Copyright (C) 2026 Thiago Negri */' >>$SOURCE_FILE
  echo '/* THIS FILE IS GENERATED. DO NOT EDIT. */' >>$SOURCE_FILE
  # Include amalgamation header
  echo "#include "'"'"$HEADER_FILE"'"' >>$SOURCE_FILE
  # Concatenate all sources
  for f in ${SOURCES[@]}; do
    cat $f >>$SOURCE_FILE
  done
  # Remove extra license notes
  sed -i '5,${/.*SPDX-License-Identifier: GPL-3.0-or-later.*/d}' $SOURCE_FILE
  sed -i '5,${/.*Copyright (C) 2026 Thiago Negri.*/d}' $SOURCE_FILE
  # Remove local includes and group global includes at top
  INCLUDES=$(grep '#include <' $SOURCE_FILE | sort -u)
  sed -i '6,${/#include .*/d}' $SOURCE_FILE
  IFS=$'\n'; for inc in ${INCLUDES[@]}; do
    sed -i "6i $inc" $SOURCE_FILE
  done
  # Remove empty lines
  sed -i '/^$/d' $SOURCE_FILE
fi
