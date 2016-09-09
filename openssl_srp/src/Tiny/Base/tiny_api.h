/**
 * Copyright (C) 2013-2015
 *
 * @author jxfengzi@gmail.com
 * @date   2013-7-9
 *
 * @file   tiny_api.h
 *
 * @remark
 *      set tabstop=4
 *      set shiftwidth=4
 *      set expandtab
 */

#ifndef __TINY_API_H__
#define __TINY_API_H__


#ifdef _MSC_VER
#   if (defined airtunes_client_shared_EXPORTS) || (defined dlna_shared_EXPORTS)
#       define DLLEXPORT _declspec(dllexport)
#   else
#       define DLLEXPORT _declspec(dllimport)
#   endif
#   define DLLLOCAL
#else
#   ifdef __ANDROID__
#       define DLLEXPORT
#       define DLLLOCAL
#   else
#       define DLLEXPORT __attribute__ ((visibility("default")))
#       define DLLLOCAL __attribute__ ((visibility("hidden")))
#   endif /* __ANDROID__ */
#endif /* _MSC_VER */


#ifdef TINY_STATIC
#   define TINY_API
#else
#   define TINY_API DLLEXPORT
#endif /* TINY_STATIC */


#endif /* __TINY_API_H__ */
