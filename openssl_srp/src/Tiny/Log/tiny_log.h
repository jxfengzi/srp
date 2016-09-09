/**
 * Copyright (C) 2013-2015
 *
 * @author jxfengzi@gmail.com
 * @date   2013-7-9
 *
 * @file   tiny_log.h
 *
 * @remark
 *
 */

#ifndef __tiny_log_H__
#define __tiny_log_H__


#ifdef __ANDROID__
    #include <android/log.h>
    #define LOG_OPEN(file)
    #define LOG_CLOSE()
    #define LOG_D(tag, format, ...) __android_log_print(ANDROID_LOG_DEBUG, tag, format, ##__VA_ARGS__)
    #define LOG_E(tag, format, ...) __android_log_print(ANDROID_LOG_ERROR, tag, format, ##__VA_ARGS__)
    #define LOG_I(tag, format, ...) __android_log_print(ANDROID_LOG_INFO, tag, format, ##__VA_ARGS__)
    #define LOG_V(tag, format, ...) __android_log_print(ANDROID_LOG_VERBOSE, tag, format, ##__VA_ARGS__)
    #define LOG_W(tag, format, ...) __android_log_print(ANDROID_LOG_WARN, tag, format, ##__VA_ARGS__)
#else
    #include "tiny_log_print.h"
    #define LOG_OPEN(file)  __tiny_log_open(file)
    #define LOG_CLOSE() __tiny_log_close()

    #ifdef TINY_DEBUG
        #define LOG_D(tag, format, ...) __tiny_log_print(TINY_LOG_DEBUG, tag, format, ##__VA_ARGS__)
    #else
        #define LOG_D(tag, format, ...)
    #endif /* TINY_DEBUG */

    #define LOG_E(tag, format, ...) __tiny_log_print(TINY_LOG_ERROR, tag, format, ##__VA_ARGS__)
    #define LOG_I(tag, format, ...) __tiny_log_print(TINY_LOG_INFO, tag, format, ##__VA_ARGS__)
    #define LOG_V(tag, format, ...) __tiny_log_print(TINY_LOG_VERBOSE, tag, format, ##__VA_ARGS__)
    #define LOG_W(tag, format, ...) __tiny_log_print(TINY_LOG_WARN, tag, format, ##__VA_ARGS__)
#endif /* __ANDROID__ */

#ifdef TINY_DEBUG
    #define LOG_TIME_BEGIN(tag, func) uint64_t usec_##func = tiny_getusec()

    #ifdef __MAC_OSX__
        #define LOG_TIME_END(tag, func) LOG_D(tag, "%s, consuming: %lldms", #func, (tiny_getusec() - usec_##func) / 1000)
    #else
        #define LOG_TIME_END(tag, func) LOG_D(tag, "%s, consuming: %lums", #func, (tiny_getusec() - usec_##func) / 1000)
    #endif
#else
    #define LOG_TIME_BEGIN(tag, func)
    #define LOG_TIME_END(tag, func)
#endif


#endif /* __tiny_log_H__ */
