/**
 * Copyright (C) 2013-2015
 *
 * @author jxfengzi@gmail.com
 * @date   2013-7-9
 *
 * @file   SrpServer.c
 *
 * @remark
 *      set tabstop=4
 *      set shiftwidth=4
 *      set expandtab
 */

#include "SrpServer.h"
#include "SrpCommon.h"
#include <openssl/srp.h>
#include <openssl/rand.h>
#include "tiny_log.h"

#define TAG                 "SrpServer"
#define USERNAME_LEN        64
#define PASSWORD_LEN        64

struct _SrpServer
{
    char         username[USERNAME_LEN];
    char         password[PASSWORD_LEN];
    SRP_gN     * GN;
    BIGNUM     * s;
    BIGNUM     * v;
    BIGNUM     * b;
    BIGNUM     * A;
    BIGNUM     * B;
    BIGNUM     * u;
    BIGNUM     * S;
    BIGNUM     * K;
    BIGNUM     * M1;
    BIGNUM     * M2;
};

static TinyRet SrpServer_Construct(SrpServer *thiz, const char *id, const char *username, const char *password)
{
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_ARG_NULL);
    RETURN_VAL_IF_FAIL(id, TINY_RET_E_ARG_NULL);
    RETURN_VAL_IF_FAIL(username, TINY_RET_E_ARG_NULL);
    RETURN_VAL_IF_FAIL(password, TINY_RET_E_ARG_NULL);

    memset(thiz, 0, sizeof(SrpServer));
    thiz->GN = SRP_get_default_gN(id);
    if (thiz->GN == NULL)
    {
        LOG_E(TAG, "gN invalid: %s", id);
        return TINY_RET_E_ARG_INVALID;
    }

    strncpy(thiz->username, username, USERNAME_LEN);
    strncpy(thiz->password, password, USERNAME_LEN);

    return TINY_RET_OK;
}

static void SrpServer_Dispose(SrpServer *thiz)
{
    RETURN_IF_FAIL(thiz);

    if (thiz->M2 != NULL)
    {
        BN_clear_free(thiz->M2);
    }

    if (thiz->M1 != NULL)
    {
        BN_clear_free(thiz->M1);
    }

    if (thiz->K != NULL)
    {
        BN_clear_free(thiz->K);
    }

    if (thiz->S != NULL)
    {
        BN_clear_free(thiz->S);
    }

    if (thiz->b != NULL)
    {
        BN_clear_free(thiz->b);
    }

    if (thiz->v != NULL)
    {
        BN_clear_free(thiz->v);
    }

    if (thiz->u != NULL)
    {
        BN_clear_free(thiz->u);
    }

    if (thiz->A != NULL)
    {
        BN_clear_free(thiz->A);
    }

    if (thiz->B != NULL)
    {
        BN_clear_free(thiz->B);
    }

    if (thiz->s != NULL)
    {
        BN_clear_free(thiz->s);
    }

    memset(thiz, 0, sizeof(SrpServer));
}

SrpServer *SrpServer_New(const char *id, const char *username, const char *password)
{
    SrpServer *thiz = NULL;

    do
    {
        thiz = (SrpServer *)malloc(sizeof(SrpServer));
        if (thiz == NULL)
        {
            LOG_D(TAG, "tiny_malloc FAILED.");
            break;
        }

        if (RET_FAILED(SrpServer_Construct(thiz, id, username, password)))
        {
            LOG_D(TAG, "SrpServer_Construct FAILED.");
            SrpServer_Delete(thiz);
            thiz = NULL;
            break;
        }
    } while (false);

    return thiz;
}

void SrpServer_Delete(SrpServer *thiz)
{
    RETURN_IF_FAIL(thiz);

    SrpServer_Dispose(thiz);
    free(thiz);
}

TinyRet SrpServer_generate_s(SrpServer *thiz, char **s, size_t *s_len)
{
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_ARG_NULL);
    RETURN_VAL_IF_FAIL(s, TINY_RET_E_ARG_NULL);
    RETURN_VAL_IF_FAIL(s_len, TINY_RET_E_ARG_NULL);

    if (thiz->s != NULL || SRP_create_verifier_BN(thiz->username, thiz->password, &thiz->s, &thiz->v, thiz->GN->N, thiz->GN->g))
    {
        char *salt = BN_bn2hex(thiz->s);
        *s_len = strlen(salt);
        *s = malloc(*s_len + 1);
        memset(*s, 0, *s_len + 1);
        strncpy(*s, salt, *s_len);
        OPENSSL_free(salt);
        return TINY_RET_OK;
    }

    LOG_E(TAG, "Failed to create SRP verifier");

    return TINY_RET_E_INTERNAL;
}

TinyRet SrpServer_generate_B(SrpServer *thiz, char **B, size_t *B_len)
{
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_ARG_NULL);
    RETURN_VAL_IF_FAIL(B, TINY_RET_E_ARG_NULL);
    RETURN_VAL_IF_FAIL(B_len, TINY_RET_E_ARG_NULL);

    if (thiz->B == NULL)
    {
        if (thiz->b == NULL)
        {
            # define RANDOM_SIZE 32         /* use 256 bits on each side */
            unsigned char rand_tmp[RANDOM_SIZE];
            RAND_bytes(rand_tmp, sizeof(rand_tmp));
            thiz->b = BN_bin2bn(rand_tmp, sizeof(rand_tmp), NULL);
        }

        thiz->B = SRP_Calc_B(thiz->b, thiz->GN->N, thiz->GN->g, thiz->v);

        if (!SRP_Verify_B_mod_N(thiz->B, thiz->GN->N))
        {
            LOG_E(TAG, "Invalid B");
            return TINY_RET_E_INTERNAL;
        }
    }

    if (thiz->B != NULL)
    {
        char *hex = BN_bn2hex(thiz->B);
        *B_len = strlen(hex);
        *B = malloc(*B_len + 1);
        memset(*B, 0, *B_len + 1);
        strncpy(*B, hex, *B_len);
        OPENSSL_free(hex);
        return TINY_RET_OK;
    }

    return TINY_RET_E_INTERNAL;
}

TinyRet SrpServer_compute_u(SrpServer *thiz, const char *A_hex, char **u, size_t *u_len)
{
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_ARG_NULL);
    RETURN_VAL_IF_FAIL(A_hex, TINY_RET_E_ARG_NULL);
    RETURN_VAL_IF_FAIL(u, TINY_RET_E_ARG_NULL);
    RETURN_VAL_IF_FAIL(u_len, TINY_RET_E_ARG_NULL);

    if (thiz->B == NULL)
    {
        LOG_E(TAG, "B is NULL");
        return TINY_RET_E_INTERNAL;
    }

    if (thiz->A != NULL)
    {
        BN_free(thiz->A);
        thiz->A = NULL;
    }

    thiz->A = BN_new();
    if (! BN_hex2bn(&thiz->A, A_hex))
    {
        LOG_E(TAG, "Failed to set A");
        return TINY_RET_E_ARG_INVALID;
    }

    if (thiz->u != NULL)
    {
        BN_clear_free(thiz->u);
        thiz->u = NULL;
    }

    thiz->u = SRP_Calc_u(thiz->A, thiz->B, thiz->GN->N);
    if (thiz->u == NULL)
    {
        LOG_E(TAG, "Failed to compute u");
        return TINY_RET_E_INTERNAL;
    }

    {
        char *hex = BN_bn2hex(thiz->u);
        *u_len = strlen(hex);
        *u = malloc(*u_len + 1);
        memset(*u, 0, *u_len + 1);
        strncpy(*u, hex, *u_len);
        OPENSSL_free(hex);
    }

    return TINY_RET_OK;
}

TinyRet SrpServer_compute_S(SrpServer *thiz, char **S, size_t *S_len)
{
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_ARG_NULL);
    RETURN_VAL_IF_FAIL(S, TINY_RET_E_ARG_NULL);
    RETURN_VAL_IF_FAIL(S_len, TINY_RET_E_ARG_NULL);

    if (thiz->b == NULL)
    {
        LOG_E(TAG, "b is NULL");
        return TINY_RET_E_INTERNAL;
    }

    if (thiz->v == NULL)
    {
        LOG_E(TAG, "v is NULL");
        return TINY_RET_E_INTERNAL;
    }

    if (thiz->u == NULL)
    {
        LOG_E(TAG, "u is NULL");
        return TINY_RET_E_INTERNAL;
    }

    if (thiz->A == NULL)
    {
        LOG_E(TAG, "A is NULL");
        return TINY_RET_E_INTERNAL;
    }

    if (thiz->S != NULL)
    {
        BN_clear_free(thiz->S);
        thiz->S = NULL;
    }

    thiz->S = SRP_Calc_server_key(thiz->A, thiz->v, thiz->u, thiz->b, thiz->GN->N);
    if (thiz->S == NULL)
    {
        LOG_E(TAG, "Failed to compute S");
        return TINY_RET_E_INTERNAL;
    }

    {
        char *hex = BN_bn2hex(thiz->S);
        *S_len = strlen(hex);
        *S = malloc(*S_len + 1);
        memset(*S, 0, *S_len + 1);
        strncpy(*S, hex, *S_len);
        OPENSSL_free(hex);
    }

    return TINY_RET_OK;
}

TinyRet SrpServer_compute_K(SrpServer *thiz, char **K_hex, size_t *K_len)
{
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_ARG_NULL);
    RETURN_VAL_IF_FAIL(K_hex, TINY_RET_E_ARG_NULL);
    RETURN_VAL_IF_FAIL(K_len, TINY_RET_E_ARG_NULL);

    if (thiz->S == NULL)
    {
        LOG_E(TAG, "S is NULL");
        return TINY_RET_E_INTERNAL;
    }

    if (thiz->K != NULL)
    {
        BN_clear_free(thiz->K);
        thiz->K = NULL;
    }

    thiz->K = SRP_Ex_Calc_K(thiz->S);
    if (thiz->K == NULL)
    {
        LOG_E(TAG, "Failed to compute K");
        return TINY_RET_E_INTERNAL;
    }

    {
        char *hex = BN_bn2hex(thiz->K);
        *K_len = strlen(hex);
        *K_hex = malloc(*K_len + 1);
        memset(*K_hex, 0, *K_len + 1);
        strncpy(*K_hex, hex, *K_len);
        OPENSSL_free(hex);
    }

    return TINY_RET_OK;
}

TinyRet SrpServer_compute_M1(SrpServer *thiz, char **M1_hex, size_t *M1_len)
{
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_ARG_NULL);
    RETURN_VAL_IF_FAIL(M1_hex, TINY_RET_E_ARG_NULL);
    RETURN_VAL_IF_FAIL(M1_len, TINY_RET_E_ARG_NULL);

    if (thiz->s == NULL)
    {
        LOG_E(TAG, "s is NULL");
        return TINY_RET_E_INTERNAL;
    }

    if (thiz->A == NULL)
    {
        LOG_E(TAG, "A is NULL");
        return TINY_RET_E_INTERNAL;
    }

    if (thiz->B == NULL)
    {
        LOG_E(TAG, "B is NULL");
        return TINY_RET_E_INTERNAL;
    }

    if (thiz->K == NULL)
    {
        LOG_E(TAG, "K is NULL");
        return TINY_RET_E_INTERNAL;
    }

    if (thiz->M1 != NULL)
    {
        BN_clear_free(thiz->M1);
        thiz->M1 = NULL;
    }

    thiz->M1 = SRP_Ex_Calc_M1(thiz->GN->N, thiz->GN->g, thiz->username, thiz->s, thiz->A, thiz->B, thiz->K);
    if (thiz->M1 == NULL)
    {
        LOG_E(TAG, "Failed to compute M1");
        return TINY_RET_E_INTERNAL;
    }

    {
        char *hex = BN_bn2hex(thiz->M1);
        *M1_len = strlen(hex);
        *M1_hex = malloc(*M1_len + 1);
        memset(*M1_hex, 0, *M1_len + 1);
        strncpy(*M1_hex, hex, *M1_len);
        OPENSSL_free(hex);
    }

    return TINY_RET_OK;
}

TinyRet SrpServer_compute_M2(SrpServer *thiz, char **M2_hex, size_t *M2_len)
{
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_ARG_NULL);
    RETURN_VAL_IF_FAIL(M2_hex, TINY_RET_E_ARG_NULL);
    RETURN_VAL_IF_FAIL(M2_len, TINY_RET_E_ARG_NULL);

    if (thiz->A == NULL)
    {
        LOG_E(TAG, "A is NULL");
        return TINY_RET_E_INTERNAL;
    }

    if (thiz->M1 == NULL)
    {
        LOG_E(TAG, "M1 is NULL");
        return TINY_RET_E_INTERNAL;
    }

    if (thiz->K == NULL)
    {
        LOG_E(TAG, "K is NULL");
        return TINY_RET_E_INTERNAL;
    }

    if (thiz->M2 != NULL)
    {
        BN_clear_free(thiz->M2);
        thiz->M2 = NULL;
    }

    thiz->M2 = SRP_Ex_Calc_M2(thiz->A, thiz->M1, thiz->K);
    if (thiz->M2 == NULL)
    {
        LOG_E(TAG, "Failed to compute M2");
        return TINY_RET_E_INTERNAL;
    }

    {
        char *hex = BN_bn2hex(thiz->M2);
        *M2_len = strlen(hex);
        *M2_hex = malloc(*M2_len + 1);
        memset(*M2_hex, 0, *M2_len + 1);
        strncpy(*M2_hex, hex, *M2_len);
        OPENSSL_free(hex);
    }

    return TINY_RET_OK;
}
