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

    thiz->u = SRP_Calc_u(thiz->A, thiz->B, thiz->GN->N);
    if (thiz->u == NULL)
    {
        LOG_E(TAG, "Failed to compute U");
        return TINY_RET_E_INTERNAL;
    }

    {
        char *hex = BN_bn2hex(thiz->B);
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

    thiz->S = SRP_Calc_server_key(thiz->A, thiz->v, thiz->u, thiz->b, thiz->GN->N);
    if (thiz->S == NULL)
    {
        LOG_E(TAG, "S is NULL");
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

# include <openssl/sha.h>
# include <openssl/evp.h>
static BIGNUM *SRP_Ex_Calc_K(const BIGNUM *S)
{
    BIGNUM *res = NULL;

    do
    {
        unsigned char digest[SHA512_DIGEST_LENGTH];
        unsigned char *tmp = NULL;
        EVP_MD_CTX *ctxt = NULL;
        int longS = BN_num_bytes(S);

        ctxt = EVP_MD_CTX_new();
        if (ctxt == NULL)
        {
            break;
        }

        if ((tmp = OPENSSL_malloc(longS)) == NULL)
        {
            EVP_MD_CTX_free(ctxt);
            break;
        }

        BN_bn2bin(S, tmp);

        if (!EVP_DigestInit_ex(ctxt, EVP_sha512(), NULL) || !EVP_DigestUpdate(ctxt, tmp, longS))
        {
            OPENSSL_free(tmp);
            EVP_MD_CTX_free(ctxt);
            break;
        }

        memset(tmp, 0, longS);

        if (!EVP_DigestFinal_ex(ctxt, digest, NULL))
        {
            OPENSSL_free(tmp);
            EVP_MD_CTX_free(ctxt);
            break;
        }

        res = BN_bin2bn(digest, sizeof(digest), NULL);
    } while (0);

    return res;
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

    thiz->K = SRP_Ex_Calc_K(thiz->S);
    if (thiz->K == NULL)
    {
        LOG_E(TAG, "K is NULL");
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
