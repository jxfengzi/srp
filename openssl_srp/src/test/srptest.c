/*
 * Copyright 2011-2016 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the OpenSSL license (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#include <openssl/opensslconf.h>
#ifdef OPENSSL_NO_SRP

# include <stdio.h>

int main(int argc, char *argv[])
{
    printf("No SRP support\n");
    return (0);
}

#else

# include <openssl/srp.h>
# include <openssl/rand.h>
# include <openssl/err.h>
# include <openssl/sha.h>
# include <openssl/evp.h>

static void showbn(const char *name, const BIGNUM *bn)
{
    fputs(name, stdout);
    fputs(" = ", stdout);
    BN_print_fp(stdout, bn);
    putc('\n', stdout);
}


/* server & client  */
BIGNUM *SRP_Ex_Calc_K(const BIGNUM *S)
{
    unsigned char digest[SHA512_DIGEST_LENGTH];
    unsigned char *tmp = NULL;
    EVP_MD_CTX *ctxt = NULL;
    int longS = BN_num_bytes(S);
    BIGNUM *res = NULL;

    ctxt = EVP_MD_CTX_new();
    if (ctxt == NULL)
    {
        return NULL;
    }

    if ((tmp = OPENSSL_malloc(longS)) == NULL)
    {
        goto err;
    }

    BN_bn2bin(S, tmp);

    if (!EVP_DigestInit_ex(ctxt, EVP_sha512(), NULL) || !EVP_DigestUpdate(ctxt, tmp, longS))
    {
        goto err;
    }

    memset(tmp, 0, longS);

    if (!EVP_DigestFinal_ex(ctxt, digest, NULL))
    {
        goto err;
    }

    res = BN_bin2bn(digest, sizeof(digest), NULL);

err:
    OPENSSL_free(tmp);
    EVP_MD_CTX_free(ctxt);
    return res;
}

# define RANDOM_SIZE 32         /* use 256 bits on each side */

static int run_srp(const char *username, const char *client_pass,
                   const char *server_pass)
{
    int ret = -1;
    BIGNUM *s = NULL;
    BIGNUM *v = NULL;
    BIGNUM *a = NULL;
    BIGNUM *b = NULL;
    BIGNUM *u = NULL;
    BIGNUM *x = NULL;
    BIGNUM *A = NULL;
    BIGNUM *B = NULL;
    BIGNUM *Kclient = NULL;
    BIGNUM *Kserver = NULL;
    BIGNUM *K = NULL;
    unsigned char rand_tmp[RANDOM_SIZE];

    /* use builtin 1024-bit params */
    //const SRP_gN *GN = SRP_get_default_gN("1024");
    const SRP_gN *GN = SRP_get_default_gN("3072");

    fprintf(stderr, "------ %s : %s : %s\n", username, client_pass, server_pass);

    if (GN == NULL) {
        fprintf(stderr, "Failed to get SRP parameters\n");
        return -1;
    }

#if 0
    /* Set up server's password entry */
    if (!SRP_create_verifier_BN(username, server_pass, &s, &v, GN->N, GN->g)) {
        fprintf(stderr, "Failed to create SRP verifier\n");
        return -1;
    }
#else
    s = BN_new();
    ret = BN_hex2bn(&s, "BEB25379D1A8581EB5A727673A2441EE");
    if (!ret)
    {
        fprintf(stderr, "Failed to set S\n");
        return -1;
    }

    v = BN_new();
    ret = BN_hex2bn(&v, 
            "9B5E061701EA7AEB39CF6E3519655A853CF94C75CAF2555EF1FAF759BB79CB47"
            "7014E04A88D68FFC05323891D4C205B8DE81C2F203D8FAD1B24D2C109737F1BE"
            "BBD71F912447C4A03C26B9FAD8EDB3E780778E302529ED1EE138CCFC36D4BA31"
            "3CC48B14EA8C22A0186B222E655F2DF5603FD75DF76B3B08FF8950069ADD03A7"
            "54EE4AE88587CCE1BFDE36794DBAE4592B7B904F442B041CB17AEBAD1E3AEBE3"
            "CBE99DE65F4BB1FA00B0E7AF06863DB53B02254EC66E781E3B62A8212C86BEB0"
            "D50B5BA6D0B478D8C4E9BBCEC21765326FBD14058D2BBDE2C33045F03873E539"
            "48D78B794F0790E48C36AED6E880F557427B2FC06DB5E1E2E1D7E661AC482D18"
            "E528D7295EF7437295FF1A72D402771713F16876DD050AE5B7AD53CCB90855C9"
            "3956648358ADFD966422F52498732D68D1D7FBEF10D78034AB8DCB6F0FCF885C"
            "C2B2EA2C3E6AC86609EA058A9DA8CC63531DC915414DF568B09482DDAC1954DE"
            "C7EB714F6FF7D44CD5B86F6BD115810930637C01D0F6013BC9740FA2C633BA89");
    if (!ret)
    {
        fprintf(stderr, "Failed to set V\n");
        return -1;
    }
#endif

    showbn("N", GN->N);
    showbn("g", GN->g);
    showbn("Salt", s);
    showbn("Verifier", v);

    /* Server random */
#if 0
    RAND_bytes(rand_tmp, sizeof(rand_tmp));
    b = BN_bin2bn(rand_tmp, sizeof(rand_tmp), NULL);
#else
    b = BN_new();
    ret = BN_hex2bn(&b, "E487CB59D31AC550471E81F00F6928E01DDA08E974A004F49E61F5D105284D20");
    if (!ret)
    {
        fprintf(stderr, "Failed to set b\n");
        return -1;
    }
#endif
    /* TODO - check b != 0 */
    showbn("b", b);

    /* Server's first message */
#if 1
    B = SRP_Calc_B(b, GN->N, GN->g, v);
#else
    B = BN_new();
    ret = BN_hex2bn(&B, 
                "40F57088A482D4C7733384FE0D301FDDCA9080AD7D4F6FDF09A01006C3CB6D56"
                "2E41639AE8FA21DE3B5DBA7585B275589BDB279863C562807B2B99083CD1429C"
                "DBE89E25BFBD7E3CAD3173B2E3C5A0B174DA6D5391E6A06E465F037A40062548"
                "39A56BF76DA84B1C94E0AE208576156FE5C140A4BA4FFC9E38C3B07B88845FC6"
                "F7DDDA93381FE0CA6084C4CD2D336E5451C464CCB6EC65E7D16E548A273E8262"
                "84AF2559B6264274215960FFF47BDD63D3AFF064D6137AF769661C9D4FEE4738"
                "2603C88EAA0980581D07758461B777E4356DDA5835198B51FEEA308D70F75450"
                "B71675C08C7D8302FD7539DD1FF2A11CB4258AA70D234436AA42B6A0615F3F91"
                "5D55CC3B966B2716B36E4D1A06CE5E5D2EA3BEE5A1270E8751DA45B60B997B0F"
                "FDB0F9962FEE4F03BEE780BA0A845B1D9271421783AE6601A61EA2E342E4F2E8"
                "BC935A409EAD19F221BD1B74E2964DD19FC845F60EFC09338B60B6B256D8CAC8"
                "89CCA306CC370A0B18C8B886E95DA0AF5235FEF4393020D2B7F3056904759042");
    if (!ret)
    {
        fprintf(stderr, "Failed to set B\n");
        return -1;
    }
#endif
    showbn("B", B);

    if (!SRP_Verify_B_mod_N(B, GN->N)) {
        fprintf(stderr, "Invalid B\n");
        return -1;
    }

    /* Client random */
#if 0
    RAND_bytes(rand_tmp, sizeof(rand_tmp));
    a = BN_bin2bn(rand_tmp, sizeof(rand_tmp), NULL);
#else
    a = BN_new();
    ret = BN_hex2bn(&a, "60975527035CF2AD1989806F0407210BC81EDC04E2762A56AFD529DDDA2D4393");
    if (!ret)
    {
        fprintf(stderr, "Failed to set a\n");
        return -1;
    }
#endif
    /* TODO - check a != 0 */
    showbn("a", a);

    /* Client's response */
#if 1
    A = SRP_Calc_A(a, GN->N, GN->g);
#else
    A = BN_new();
    ret = BN_hex2bn(&A, 
                "FAB6F5D2615D1E323512E7991CC37443F487DA604CA8C9230FCB04E541DCE628"
                "0B27CA4680B0374F179DC3BDC7553FE62459798C701AD864A91390A28C93B644"
                "ADBF9C00745B942B79F9012A21B9B78782319D83A1F8362866FBD6F46BFC0DDB"
                "2E1AB6E4B45A9906B82E37F05D6F97F6A3EB6E182079759C4F6847837B62321A"
                "C1B4FA68641FCB4BB98DD697A0C73641385F4BAB25B793584CC39FC8D48D4BD8"
                "67A9A3C10F8EA12170268E34FE3BBE6FF89998D60DA2F3E4283CBEC1393D52AF"
                "724A57230C604E9FBCE583D7613E6BFFD67596AD121A8707EEC4694495703368"
                "6A155F644D5C5863B48F61BDBF19A53EAB6DAD0A186B8C152E5F5D8CAD4B0EF8"
                "AA4EA5008834C3CD342E5E0F167AD04592CD8BD279639398EF9E114DFAAAB919"
                "E14E850989224DDD98576D79385D2210902E9F9B1F2D86CFA47EE244635465F7"
                "1058421A0184BE51DD10CC9D079E6F1604E7AA9B7CF7883C7D4CE12B06EBE160"
                "81E23F27A231D18432D7D1BB55C28AE21FFCF005F57528D15A88881BB3BBB7FE");
    if (!ret)
    {
        fprintf(stderr, "Failed to set A\n");
        return -1;
    }
#endif
    showbn("A", A);

    if (!SRP_Verify_A_mod_N(A, GN->N)) {
        fprintf(stderr, "Invalid A\n");
        return -1;
    }

    /* Both sides calculate u */
#if 1
    u = SRP_Calc_u(A, B, GN->N);
#else
    u = BN_new();
    ret = BN_hex2bn(&u, "03AE5F3C3FA9EFF1A50D7DBB8D2F60A1EA66EA712D50AE976EE34641A1CD0E51"
                        "C4683DA383E8595D6CB56A15D5FBC7543E07FBDDD316217E01A391A18EF06DFF");
    if (!ret)
    {
        fprintf(stderr, "Failed to set u\n");
        return -1;
    }

#endif
    showbn("u", u);

#if 1
    /* Client's key */
    x = SRP_Calc_x(s, username, client_pass);
    Kclient = SRP_Calc_client_key(GN->N, B, GN->g, x, a, u);
    showbn("Client's key", Kclient);

    /* Server's key */
    Kserver = SRP_Calc_server_key(A, v, u, b, GN->N);
    showbn("Server's key", Kserver);

    K = SRP_Ex_Calc_K(Kserver); 
    if (K != NULL) 
    { 
        showbn("Session Key", K); 
    }

    if (BN_cmp(Kclient, Kserver) == 0) {
        ret = 0;
    } else {
        fprintf(stderr, "Keys mismatch\n");
        ret = 1;
    }


#endif

    BN_clear_free(K);
    BN_clear_free(Kclient);
    BN_clear_free(Kserver);
    BN_clear_free(x);
    BN_clear_free(a);
    BN_clear_free(b);
    BN_clear_free(v);

    BN_free(u);
    BN_free(A);
    BN_free(B);
    BN_free(s);

    return ret;
}

int main(int argc, char **argv)
{
    BIO *bio_err;
    bio_err = BIO_new_fp(stderr, BIO_NOCLOSE | BIO_FP_TEXT);

    CRYPTO_set_mem_debug(1);
    CRYPTO_mem_ctrl(CRYPTO_MEM_CHECK_ON);

#if 0
    /* "Negative" test, expect a mismatch */
    if (run_srp("alice", "password1", "password2") == 0) {
        fprintf(stderr, "Mismatched SRP run failed\n");
        return 1;
    }

    /* "Positive" test, should pass */
    if (run_srp("alice", "password", "password") != 0) {
        fprintf(stderr, "Plain SRP run failed\n");
        return 1;
    }
#else
    /* "Negative" test, expect a mismatch */
    if (run_srp("alice", "password123", "password2") == 0) {
        fprintf(stderr, "Mismatched SRP run failed\n");
        return 1;
    }

    /* "Positive" test, should pass */
    if (run_srp("alice", "password123", "password123") != 0) {
        fprintf(stderr, "Plain SRP run failed\n");
        return 1;
    }
#endif

#ifndef OPENSSL_NO_CRYPTO_MDEBUG
    if (CRYPTO_mem_leaks(bio_err) <= 0)
        return 1;
#endif
    BIO_free(bio_err);

    return 0;
}
#endif
