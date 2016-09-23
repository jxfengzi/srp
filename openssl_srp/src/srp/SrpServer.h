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

SRP_API SrpServer *SrpServer_New(const char *id, const char *username, const char *password);
SRP_API void SrpServer_Delete(SrpServer *thiz);

SRP_API TinyRet SrpServer_generate_s(SrpServer *thiz, char **s_hex, size_t *s_len);
SRP_API TinyRet SrpServer_generate_B(SrpServer *thiz, char **B_hex, size_t *B_len);
SRP_API TinyRet SrpServer_compute_u(SrpServer *thiz, const char *A_hex, char **u_hex, size_t *u_len);
SRP_API TinyRet SrpServer_compute_S(SrpServer *thiz, char **S_hex, size_t *S_len);
SRP_API TinyRet SrpServer_compute_K(SrpServer *thiz, char **K_hex, size_t *K_len);
SRP_API TinyRet SrpServer_compute_M1(SrpServer *thiz, char **M1_hex, size_t *M1_len);


TINY_END_DECLS

#endif /* __SRP_SERVER_H__ */
