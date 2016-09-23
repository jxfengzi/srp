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

#include "SrpCommon.h"
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <stdbool.h>
#include "tiny_log.h"

#define TAG                 "SrpCommon"


static bool _bn_sha512(const BIGNUM *N, unsigned char digest[SHA512_DIGEST_LENGTH])
{
    bool ret = false;

    do
    {
        unsigned char *tmp = NULL;
        EVP_MD_CTX *ctxt = NULL;
        int longS = BN_num_bytes(N);

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

        BN_bn2bin(N, tmp);

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

        ret = true;
    } while (0);

    return ret;
}

static bool _str_sha512(const char *str, unsigned char digest[SHA512_DIGEST_LENGTH])
{
    bool ret = false;

    do
    {
        EVP_MD_CTX *ctxt = EVP_MD_CTX_new();
        if (ctxt == NULL)
        {
            break;
        }

        if (!EVP_DigestInit_ex(ctxt, EVP_sha512(), NULL) || !EVP_DigestUpdate(ctxt, str, strlen(str)))
        {
            EVP_MD_CTX_free(ctxt);
            break;
        }

        if (!EVP_DigestFinal_ex(ctxt, digest, NULL))
        {
            EVP_MD_CTX_free(ctxt);
            break;
        }

        ret = true;
    } while (0);

    return ret;
}

BIGNUM *SRP_Ex_Calc_K(const BIGNUM *S)
{
    unsigned char digest[SHA512_DIGEST_LENGTH];

    if (! _bn_sha512(S, digest))
    {
        return NULL;
    }

    return BN_bin2bn(digest, sizeof(digest), NULL);
}

BIGNUM *SRP_Ex_Calc_M1(const BIGNUM *N,
                       const BIGNUM *g,
                       const char *username,
                       const BIGNUM *s,
                       const BIGNUM *A,
                       const BIGNUM *B,
                       const BIGNUM *K)
{
    BIGNUM *m1 = NULL;
    EVP_MD_CTX *ctx = NULL;
    unsigned char *bin_s = NULL;
    unsigned char *bin_A = NULL;
    unsigned char *bin_B = NULL;
    unsigned char *bin_K = NULL;

    do
    {
        unsigned char digest[SHA512_DIGEST_LENGTH];
        unsigned char h_N[SHA512_DIGEST_LENGTH];
        unsigned char h_g[SHA512_DIGEST_LENGTH];
        unsigned char h_I[SHA512_DIGEST_LENGTH];
        unsigned char h_xor[SHA512_DIGEST_LENGTH];
        size_t len_s = (size_t)BN_num_bytes(s);
        size_t len_A = (size_t)BN_num_bytes(A);
        size_t len_B = (size_t)BN_num_bytes(B);
        size_t len_K = (size_t)BN_num_bytes(K);
        int i = 0;

        /**
         * RFC 2945
         * M = H(H(N) XOR H(g) | H(U) | s | A | B | K)
         */
        if (! _bn_sha512(N, h_N) || ! _bn_sha512(g, h_g) ||! _str_sha512(username, h_I))
        {
            break;
        }

        for (i = 0; i < SHA512_DIGEST_LENGTH; ++i)
        {
            h_xor[i] = h_N[i] ^ h_g[i];
        }

        bin_s = OPENSSL_malloc(len_s);
        bin_A = OPENSSL_malloc(len_A);
        bin_B = OPENSSL_malloc(len_B);
        bin_K = OPENSSL_malloc(len_K);

        if (bin_s == NULL || bin_A == NULL || bin_B == NULL || bin_K == NULL)
        {
            break;
        }

        BN_bn2bin(s, bin_s);
        BN_bn2bin(A, bin_A);
        BN_bn2bin(B, bin_B);
        BN_bn2bin(K, bin_K);

        ctx = EVP_MD_CTX_new();
        if (ctx == NULL)
        {
            break;
        }

        if (!EVP_DigestInit_ex(ctx, EVP_sha512(), NULL))
        {
            break;
        }

        if (!EVP_DigestUpdate(ctx, h_xor, SHA512_DIGEST_LENGTH))
        {
            break;
        }

        if (!EVP_DigestUpdate(ctx, h_I, SHA512_DIGEST_LENGTH))
        {
            break;
        }

        if (!EVP_DigestUpdate(ctx, bin_s, len_s))
        {
            break;
        }

        if (!EVP_DigestUpdate(ctx, bin_A, len_A))
        {
            break;
        }

        if (!EVP_DigestUpdate(ctx, bin_B, len_B))
        {
            break;
        }

        if (!EVP_DigestUpdate(ctx, bin_K, len_K))
        {
            break;
        }

        if (!EVP_DigestFinal_ex(ctx, digest, NULL))
        {
            break;
        }

        m1 = BN_bin2bn(digest, SHA512_DIGEST_LENGTH, NULL);
    } while (false);

    if (bin_s != NULL)
    {
        OPENSSL_free(bin_s);
    }

    if (bin_A != NULL)
    {
        OPENSSL_free(bin_A);
    }

    if (bin_B != NULL)
    {
        OPENSSL_free(bin_B);
    }

    if (bin_K != NULL)
    {
        OPENSSL_free(bin_K);
    }

    if (ctx != NULL)
    {
        EVP_MD_CTX_free(ctx);
    }

    return m1;
}