/**
 * Copyright (C) 2013-2015
 *
 * @author jxfengzi@gmail.com
 * @date   2013-7-9
 *
 * @file   tiny_typedef.h
 *
 * @remark
 *      set tabstop=4
 *      set shiftwidth=4
 *      set expandtab
 */

#ifndef __TINY_TYPEDEF_H__
#define __TINY_TYPEDEF_H__

#ifdef _WIN32
#include <ws2tcpip.h>
#include <windows.h>
#include <time.h>
#else /* Linux */
#include <unistd.h>
#include <sys/time.h>
#endif /* _WIN32 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <errno.h>

#ifdef __cplusplus
    #define TINY_BEGIN_DECLS extern "C" {
    #define TINY_END_DECLS }
#else
    #define TINY_BEGIN_DECLS
    #define TINY_END_DECLS
#endif

#ifndef STR_EQUAL
    #define STR_EQUAL(a, b) (strcmp(a, b) == 0)
#endif

#define DIM(a) (sizeof(a)/sizeof(a[0]))

#ifdef _WIN32
    #define TINY_INLINE              __inline
    #define tiny_snprintf            _snprintf
    #define tiny_strdup              _strdup
#else
    #define TINY_INLINE              inline
    #define tiny_snprintf            snprintf
    #define tiny_strdup              strdup
#endif /* _WIN32 */


#endif /* __TINY_TYPEDEF_H__ */
