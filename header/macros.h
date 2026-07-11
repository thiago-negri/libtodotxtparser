/* SPDX-License-Identifier: GPL-3.0-or-later */
/* Copyright (C) 2026 Thiago Negri */

#ifndef __MACROS_H__
#define __MACROS_H__

#ifndef ASSERT
#include <assert.h>
#define ASSERT assert
#endif /* ASSERT */

#ifndef REALLOC
#define REALLOC realloc
#endif /* REALLOC */

#ifndef MALLOC
#define MALLOC malloc
#endif /* MALLOC */

#ifndef FREE
#define FREE free
#endif /* FREE */

#endif /* __MACROS_H__ */
