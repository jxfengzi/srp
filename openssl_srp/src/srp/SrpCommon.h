/**
 * Copyright (C) 2013-2015
 *
 * @author jxfengzi@gmail.com
 * @date   2013-7-9
 *
 * @file   SrpCommon.h
 *
 * @remark
 *      set tabstop=4
 *      set shiftwidth=4
 *      set expandtab
 */

#ifndef __SRP_COMMON_H__
#define __SRP_COMMON_H__

#include "tiny_base.h"
#include <openssl/ossl_typ.h>

TINY_BEGIN_DECLS


BIGNUM *SRP_Ex_Calc_K(const BIGNUM *S);
BIGNUM *SRP_Ex_Calc_M1(const BIGNUM *N,
                       const BIGNUM *g,
                       const char *username,
                       const BIGNUM *s,
                       const BIGNUM *A,
                       const BIGNUM *B,
                       const BIGNUM *K);


TINY_END_DECLS

#endif /* __SRP_COMMON_H__ */