/**
 * Copyright (C) 2013-2015
 *
 * @author coding.tom@gmail.com
 * @date   2013-7-9
 *
 * @file   tiny_memory.h
 *
 * @remark
 *      set tabstop=4
 *      set shiftwidth=4
 *      set expandtab
 *
 */

#ifndef __TINY_MALLOC_H__
#define __TINY_MALLOC_H__

#include "tiny_base.h"

TINY_BEGIN_DECLS


void * tiny_malloc(uint32_t size);
void * tiny_realloc(void *p, uint32_t size);
void tiny_free(void *p);


TINY_END_DECLS

#endif /* __TINY_MALLOC_H__ */
