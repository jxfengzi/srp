/**
 * Copyright (C) 2013-2015
 *
 * @author jxfengzi@gmail.com
 * @date   2013-7-9
 *
 * @file   SrpClient.c
 *
 * @remark
 *      set tabstop=4
 *      set shiftwidth=4
 *      set expandtab
 */

#include "SrpClient.h"
#include <openssl/srp.h>
#include <openssl/rand.h>
#include "tiny_log.h"

#define TAG                 "SrpClient"
#define USERNAME_LEN        64
#define PASSWORD_LEN        64

struct _SrpClient
{
    char         username[USERNAME_LEN];
    char         password[PASSWORD_LEN];
    SRP_gN     * GN;
    BIGNUM     * s;
    BIGNUM     * a;
    BIGNUM     * x;
    BIGNUM     * A;
    BIGNUM     * B;
    BIGNUM     * u;
    BIGNUM     * S;
    BIGNUM     * K;
};

static TinyRet SrpClient_Construct(SrpClient *thiz, const char *id, const char *username, const char *password)
{
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_ARG_NULL);
    RETURN_VAL_IF_FAIL(id, TINY_RET_E_ARG_NULL);
    RETURN_VAL_IF_FAIL(username, TINY_RET_E_ARG_NULL);
    RETURN_VAL_IF_FAIL(password, TINY_RET_E_ARG_NULL);

    memset(thiz, 0, sizeof(SrpClient));
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

static void SrpClient_Dispose(SrpClient *thiz)
{
    RETURN_IF_FAIL(thiz);

    if (thiz->K != NULL)
    {
        BN_clear_free(thiz->K);
    }

    if (thiz->s != NULL)
    {
        BN_clear_free(thiz->s);
    }

    if (thiz->a != NULL)
    {
        BN_clear_free(thiz->a);
    }

    if (thiz->x != NULL)
    {
        BN_clear_free(thiz->x);
    }

    if (thiz->A != NULL)
    {
        BN_clear_free(thiz->A);
    }

    if (thiz->B != NULL)
    {
        BN_clear_free(thiz->B);
    }

    if (thiz->u != NULL)
    {
        BN_clear_free(thiz->u);
    }

    if (thiz->S != NULL)
    {
        BN_clear_free(thiz->S);
    }

    memset(thiz, 0, sizeof(SrpClient));
}

SrpClient *SrpClient_New(const char *id, const char *username, const char *password)
{
    SrpClient *thiz = NULL;

    do
    {
        thiz = (SrpClient *)malloc(sizeof(SrpClient));
        if (thiz == NULL)
        {
            LOG_D(TAG, "tiny_malloc FAILED.");
            break;
        }

        if (RET_FAILED(SrpClient_Construct(thiz, id, username, password)))
        {
            LOG_D(TAG, "SrpServer_Construct FAILED.");
            SrpClient_Delete(thiz);
            thiz = NULL;
            break;
        }
    } while (false);

    return thiz;
}

void SrpClient_Delete(SrpClient *thiz)
{
    RETURN_IF_FAIL(thiz);

    SrpClient_Dispose(thiz);
    free(thiz);
}

TinyRet SrpClient_set_s(SrpClient *thiz, char *s_hex)
{
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_ARG_NULL);
    RETURN_VAL_IF_FAIL(s_hex, TINY_RET_E_ARG_NULL);

    if (thiz->s != NULL)
    {
        BN_free(thiz->s);
        thiz->s = NULL;
    }

    thiz->s = BN_new();
    if (! BN_hex2bn(&thiz->s, s_hex))
    {
        LOG_E(TAG, "Failed to set s");
        return TINY_RET_E_ARG_INVALID;
    }

    return TINY_RET_OK;
}

TinyRet SrpClient_generate_A(SrpClient *thiz, char **A_hex, size_t *A_len)
{
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_ARG_NULL);
    RETURN_VAL_IF_FAIL(A_hex, TINY_RET_E_ARG_NULL);
    RETURN_VAL_IF_FAIL(A_len, TINY_RET_E_ARG_NULL);

    if (thiz->B == NULL)
    {
        if (thiz->a == NULL)
        {
            # define RANDOM_SIZE 32         /* use 256 bits on each side */
            unsigned char rand_tmp[RANDOM_SIZE];
            RAND_bytes(rand_tmp, sizeof(rand_tmp));
            thiz->a = BN_bin2bn(rand_tmp, sizeof(rand_tmp), NULL);
        }

        thiz->A = SRP_Calc_A(thiz->a, thiz->GN->N, thiz->GN->g);
        if (!SRP_Verify_A_mod_N(thiz->A, thiz->GN->N))
        {
            LOG_E(TAG, "Invalid A");
            return TINY_RET_E_INTERNAL;
        }
    }

    if (thiz->A != NULL)
    {
        char *hex = BN_bn2hex(thiz->A);
        *A_len = strlen(hex);
        *A_hex = malloc(*A_len + 1);
        memset(*A_hex, 0, *A_len + 1);
        strncpy(*A_hex, hex, *A_len);
        OPENSSL_free(hex);
        return TINY_RET_OK;
    }

    return TINY_RET_E_INTERNAL;
}

TinyRet SrpClient_compute_u(SrpClient *thiz, const char *B_hex, char **u_hex, size_t *u_len)
{
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_ARG_NULL);
    RETURN_VAL_IF_FAIL(B_hex, TINY_RET_E_ARG_NULL);
    RETURN_VAL_IF_FAIL(u_hex, TINY_RET_E_ARG_NULL);
    RETURN_VAL_IF_FAIL(u_len, TINY_RET_E_ARG_NULL);

    if (thiz->A == NULL)
    {
        LOG_E(TAG, "A is NULL");
        return TINY_RET_E_INTERNAL;
    }

    if (thiz->B != NULL)
    {
        BN_free(thiz->B);
        thiz->B = NULL;
    }

    thiz->B = BN_new();
    if (! BN_hex2bn(&thiz->B, B_hex))
    {
        LOG_E(TAG, "Failed to set B");
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
        *u_hex = malloc(*u_len + 1);
        memset(*u_hex, 0, *u_len + 1);
        strncpy(*u_hex, hex, *u_len);
        OPENSSL_free(hex);
    }

    return TINY_RET_OK;
}

TinyRet SrpClient_compute_S(SrpClient *thiz, char **S_hex, size_t *S_len)
{
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_ARG_NULL);
    RETURN_VAL_IF_FAIL(S_hex, TINY_RET_E_ARG_NULL);
    RETURN_VAL_IF_FAIL(S_len, TINY_RET_E_ARG_NULL);

    if (thiz->a == NULL)
    {
        LOG_E(TAG, "b is NULL");
        return TINY_RET_E_INTERNAL;
    }

    if (thiz->u == NULL)
    {
        LOG_E(TAG, "u is NULL");
        return TINY_RET_E_INTERNAL;
    }

    if (thiz->B == NULL)
    {
        LOG_E(TAG, "B is NULL");
        return TINY_RET_E_INTERNAL;
    }

    thiz->x = SRP_Calc_x(thiz->s, thiz->username, thiz->password);
    if (thiz->x == NULL)
    {
        LOG_E(TAG, "Failed to compute x");
        return TINY_RET_E_INTERNAL;
    }

    thiz->S = SRP_Calc_client_key(thiz->GN->N, thiz->B, thiz->GN->g, thiz->x, thiz->a, thiz->u);
    if (thiz->S == NULL)
    {
        LOG_E(TAG, "S is NULL");
        return TINY_RET_E_INTERNAL;
    }

    {
        char *hex = BN_bn2hex(thiz->S);
        *S_len = strlen(hex);
        *S_hex = malloc(*S_len + 1);
        memset(*S_hex, 0, *S_len + 1);
        strncpy(*S_hex, hex, *S_len);
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

TinyRet SrpClient_compute_K(SrpClient *thiz, char **K_hex, size_t *K_len)
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
