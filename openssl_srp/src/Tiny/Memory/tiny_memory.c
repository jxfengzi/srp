/**
 * Copyright (C) 2013-2015
 *
 * @author coding.tom@gmail.com
 * @date   2013-7-9
 *
 * @file   tiny_malloc.c
 *
 * @remark
 *      set tabstop=4
 *      set shiftwidth=4
 *      set expandtab
 *
 */

#include "tiny_memory.h"
#include <stdlib.h>
#include <stdio.h>

void * tiny_malloc(uint32_t size)
{
    return malloc(size);
}

void * tiny_realloc(void *p, uint32_t size)
{
    return realloc(p, size);
}

void tiny_free(void *p)
{
    free(p);
}
