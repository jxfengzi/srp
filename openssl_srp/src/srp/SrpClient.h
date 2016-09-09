/**
 * Copyright (C) 2013-2015
 *
 * @author jxfengzi@gmail.com
 * @date   2013-7-9
 *
 * @file   SrpClient.h
 *
 * @remark
 *      set tabstop=4
 *      set shiftwidth=4
 *      set expandtab
 */

#ifndef __SRP_CLIENT_H__
#define __SRP_CLIENT_H__

#include "tiny_base.h"
#include "srp_api.h"


TINY_BEGIN_DECLS


struct _SrpClient;
typedef struct _SrpClient SrpClient;

SRP_API SrpClient *SrpClient_New(void);
SRP_API void SrpClient_Delete(SrpClient *thiz);


TINY_END_DECLS

#endif /* __SRP_CLIENT_H__ */
