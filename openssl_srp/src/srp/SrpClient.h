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

SRP_API SrpClient *SrpClient_New(const char *id, const char *username, const char *password);
SRP_API void SrpClient_Delete(SrpClient *thiz);

SRP_API TinyRet SrpClient_set_s(SrpClient *thiz, char *s_hex);
SRP_API TinyRet SrpClient_generate_A(SrpClient *thiz, char **A_hex, size_t *A_len);
SRP_API TinyRet SrpClient_compute_u(SrpClient *thiz, const char *B_hex, char **u_hex, size_t *u_len);
SRP_API TinyRet SrpClient_compute_S(SrpClient *thiz, char **S_hex, size_t *S_len);


TINY_END_DECLS

#endif /* __SRP_CLIENT_H__ */
