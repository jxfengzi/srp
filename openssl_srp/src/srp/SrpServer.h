/**
 * Copyright (C) 2013-2015
 *
 * @author jxfengzi@gmail.com
 * @date   2013-7-9
 *
 * @file   SrpServer.h
 *
 * @remark
 *      set tabstop=4
 *      set shiftwidth=4
 *      set expandtab
 */

#ifndef __SRP_SERVER_H__
#define __SRP_SERVER_H__

#include "tiny_base.h"
#include "srp_api.h"


TINY_BEGIN_DECLS


struct _SrpServer;
typedef struct _SrpServer SrpServer;

SRP_API SrpServer *SrpServer_New(void);
SRP_API void SrpServer_Delete(SrpServer *thiz);


TINY_END_DECLS

#endif /* __SRP_SERVER_H__ */
