/**
 * Copyright (C) 2013-2015
 *
 * @author jxfengzi@gmail.com
 * @date   2013-7-9
 *
 * @file   tiny_log_print.h
 *
 * @remark
 *
 */

#ifndef __tiny_log_PRINT_H__
#define __tiny_log_PRINT_H__

#include <stdarg.h>
#include "tiny_base.h"

TINY_BEGIN_DECLS

int __tiny_log_open(const char *log_file);

int __tiny_log_close(void);

/*
 * mi log priority values, in ascending priority order.
 */
typedef enum _tiny_log_priority {
    TINY_LOG_UNKNOWN = 0,
    TINY_LOG_VERBOSE,
    TINY_LOG_DEBUG,
    TINY_LOG_INFO,
    TINY_LOG_WARN,
    TINY_LOG_ERROR,
} TINY_LOG_priority;

/*
 * Send a simple string to the log.
 */
int __tiny_log_write(int prio, const char *tag, const char *text);

/*
 * Send a formatted string to the log, used like printf(fmt,...)
 */
#if defined(__GNUC__)
int __tiny_log_print(int prio, const char *tag, const char *fmt, ...) __attribute__((format(printf, 3, 4)));
#else
int __tiny_log_print(int prio, const char *tag, const char *fmt, ...);
#endif

/*
 * A variant of __tiny_log_print() that takes a va_list to list
 * additional parameters.
 */
int __tiny_log_vprint(int prio, const char *tag, const char *fmt, va_list ap);


TINY_END_DECLS

#endif /* __tiny_log_PRINT_H__ */
