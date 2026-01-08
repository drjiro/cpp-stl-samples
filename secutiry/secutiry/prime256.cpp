// fopenのunsafe警告回避
#define _CRT_SECURE_NO_WARNINGS

#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/err.h>

#include <cstdio>
#include <iostream>

static void printOpenSSLErrors(const char* msg)
{
    std::cerr << msg << std::endl;
    ERR_print_errors_fp(stderr);
}

static bool writePrivateKeyPem(const char* path, EVP_PKEY* pkey)
{
    FILE* fp = std::fopen(path, "wb");
    if (!fp) return false;
    int ok = PEM_write_PrivateKey(fp, pkey, NULL, NULL, 0, NULL, NULL);
    std::fclose(fp);
    return ok == 1;
}

static bool writePublicKeyPem(const char* path, EVP_PKEY* pkey)
{
    FILE* fp = std::fopen(path, "wb");
    if (!fp) return false;
    int ok = PEM_write_PUBKEY(fp, pkey);
    std::fclose(fp);
    return ok == 1;
}

static EVP_PKEY* generateEC(int curveNid)
{
    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_EC, NULL);
    if (!ctx) return NULL;

    EVP_PKEY* pkey = NULL;

    if (EVP_PKEY_paramgen_init(ctx) != 1) { EVP_PKEY_CTX_free(ctx); return NULL; }
    if (EVP_PKEY_CTX_set_ec_paramgen_curve_nid(ctx, curveNid) != 1) { EVP_PKEY_CTX_free(ctx); return NULL; }

    // まずパラメータ生成
    EVP_PKEY* params = NULL;
    if (EVP_PKEY_paramgen(ctx, &params) != 1) { EVP_PKEY_CTX_free(ctx); return NULL; }

    EVP_PKEY_CTX_free(ctx);

    // 生成したパラメータを元に鍵生成
    EVP_PKEY_CTX* kctx = EVP_PKEY_CTX_new(params, NULL);
    if (!kctx) { EVP_PKEY_free(params); return NULL; }

    if (EVP_PKEY_keygen_init(kctx) != 1) { EVP_PKEY_CTX_free(kctx); EVP_PKEY_free(params); return NULL; }
    if (EVP_PKEY_keygen(kctx, &pkey) != 1) { EVP_PKEY_CTX_free(kctx); EVP_PKEY_free(params); return NULL; }

    EVP_PKEY_CTX_free(kctx);
    EVP_PKEY_free(params);
    return pkey;
}

int prime256()
{
    // P-256 = prime256v1
    EVP_PKEY* ec = generateEC(NID_X9_62_prime256v1);
    if (!ec) {
        printOpenSSLErrors("EC keygen failed");
        return 1;
    }

    if (!writePrivateKeyPem("ec_private.pem", ec)) {
        printOpenSSLErrors("write ec_private.pem failed");
        EVP_PKEY_free(ec);
        return 1;
    }

    if (!writePublicKeyPem("ec_public.pem", ec)) {
        printOpenSSLErrors("write ec_public.pem failed");
        EVP_PKEY_free(ec);
        return 1;
    }

    EVP_PKEY_free(ec);

    std::cout << "Generated EC keys: ec_private.pem / ec_public.pem\n";
    return 0;
}