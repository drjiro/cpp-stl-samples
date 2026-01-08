#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/bio.h>

#include <iostream>
#include <string>
#include <vector>

static void printOpenSSLErrors(const char* msg)
{
    std::cerr << msg << "\n";
    unsigned long e = 0;
    while ((e = ERR_get_error()) != 0)
    {
        char buf[256];
        ERR_error_string_n(e, buf, sizeof(buf));
        std::cerr << "  OpenSSL: " << buf << "\n";
    }
}

// 秘密鍵をPEM化
static bool privateKeyToPemString(EVP_PKEY* pkey, std::string& pemOut)
{
    if (!pkey) {
        return false;
    }

    BIO* mem = BIO_new(BIO_s_mem());
    if (!mem) {
        return false;
    }

    // 暗号化なしでPEM化
    int ok = PEM_write_bio_PrivateKey(mem, pkey, NULL, NULL, 0, NULL, NULL);
    if (ok != 1) {
        BIO_free(mem);
        printOpenSSLErrors("PEM_write_bio_PrivateKey failed");
        return false;
    }

    BUF_MEM* bptr = NULL;
    BIO_get_mem_ptr(mem, &bptr);
    if (!bptr || !bptr->data || bptr->length == 0) {
        BIO_free(mem);
        return false;
    }

    pemOut.assign(bptr->data, bptr->length);
    BIO_free(mem);
    return true;
}

// 公開鍵をPEM化
static bool publicKeyToPemString(EVP_PKEY* pkey, std::string& pemOut)
{
    if (!pkey) {
        return false;
    }

    BIO* mem = BIO_new(BIO_s_mem());
    if (!mem) {
        return false;
    }

    int ok = PEM_write_bio_PUBKEY(mem, pkey);
    if (ok != 1) {
        BIO_free(mem);
        printOpenSSLErrors("PEM_write_bio_PUBKEY failed");
        return false;
    }

    BUF_MEM* bptr = NULL;
    BIO_get_mem_ptr(mem, &bptr);
    if (!bptr || !bptr->data || bptr->length == 0) {
        BIO_free(mem);
        return false;
    }

    pemOut.assign(bptr->data, bptr->length);
    BIO_free(mem);
    return true;
}

static EVP_PKEY* generateRSA(int bits)
{
	// RSA鍵生成のためのコンテキストを作成する
    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
    if (!ctx) {
        return NULL;
    }

	// 鍵生成の初期化とパラメータ設定
    EVP_PKEY* pkey = NULL;
    if (EVP_PKEY_keygen_init(ctx) != 1) {
        EVP_PKEY_CTX_free(ctx);
        return NULL; 
    }

	// 鍵サイズを設定
    if (EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, bits) != 1) {
        EVP_PKEY_CTX_free(ctx);
        return NULL;
    }

	// 鍵ペアを生成
    if (EVP_PKEY_keygen(ctx, &pkey) != 1) {
        EVP_PKEY_CTX_free(ctx);
        return NULL;
    }

	// 鍵生成コンテキストを解放
    EVP_PKEY_CTX_free(ctx);
    return pkey;
}

// 暗号化
static bool rsaEncryptOaepSha256(EVP_PKEY* pubKey,
    const std::vector<unsigned char>& pt,
    std::vector<unsigned char>& ctOut)
{
    ctOut.clear();
    if (!pubKey) return false;

    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(pubKey, NULL);
    if (!ctx) {
        printOpenSSLErrors("EVP_PKEY_CTX_new failed");
        return false;
    }

    bool ok = false;
    size_t outLen = 0;

    do {
        if (EVP_PKEY_encrypt_init(ctx) != 1) { printOpenSSLErrors("encrypt_init failed"); break; }

        // OAEP パディング（安全なRSA暗号化）
        if (EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING) != 1) {
            printOpenSSLErrors("set_rsa_padding(OAEP) failed"); break;
        }

        // OAEP のハッシュを SHA-256 に
        if (EVP_PKEY_CTX_set_rsa_oaep_md(ctx, EVP_sha256()) != 1) {
            printOpenSSLErrors("set_rsa_oaep_md(SHA-256) failed"); break;
        }

        // MGF1 ハッシュも SHA-256 に（推奨）
        if (EVP_PKEY_CTX_set_rsa_mgf1_md(ctx, EVP_sha256()) != 1) {
            printOpenSSLErrors("set_rsa_mgf1_md(SHA-256) failed"); break;
        }

        // 出力サイズ取得
        if (EVP_PKEY_encrypt(ctx, NULL, &outLen, pt.data(), pt.size()) != 1) {
            printOpenSSLErrors("encrypt(size) failed"); break;
        }

        ctOut.assign(outLen, 0);

        // 暗号化
        if (EVP_PKEY_encrypt(ctx, ctOut.data(), &outLen, pt.data(), pt.size()) != 1) {
            printOpenSSLErrors("encrypt failed"); break;
        }

        ctOut.resize(outLen);
        ok = true;
    } while (false);

    EVP_PKEY_CTX_free(ctx);
    return ok;
}

// 復号
static bool rsaDecryptOaepSha256(EVP_PKEY* privKey,
    const std::vector<unsigned char>& ct,
    std::vector<unsigned char>& ptOut)
{
    ptOut.clear();
    if (!privKey) return false;

    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(privKey, NULL);
    if (!ctx) {
        printOpenSSLErrors("EVP_PKEY_CTX_new failed");
        return false;
    }

    bool ok = false;
    size_t outLen = 0;

    do {
        if (EVP_PKEY_decrypt_init(ctx) != 1) { printOpenSSLErrors("decrypt_init failed"); break; }

        if (EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING) != 1) {
            printOpenSSLErrors("set_rsa_padding(OAEP) failed"); break;
        }

        if (EVP_PKEY_CTX_set_rsa_oaep_md(ctx, EVP_sha256()) != 1) {
            printOpenSSLErrors("set_rsa_oaep_md(SHA-256) failed"); break;
        }

        if (EVP_PKEY_CTX_set_rsa_mgf1_md(ctx, EVP_sha256()) != 1) {
            printOpenSSLErrors("set_rsa_mgf1_md(SHA-256) failed"); break;
        }

        // 復号後サイズ取得
        if (EVP_PKEY_decrypt(ctx, NULL, &outLen, ct.data(), ct.size()) != 1) {
            printOpenSSLErrors("decrypt(size) failed"); break;
        }

        ptOut.assign(outLen, 0);

        // 復号
        if (EVP_PKEY_decrypt(ctx, ptOut.data(), &outLen, ct.data(), ct.size()) != 1) {
            printOpenSSLErrors("decrypt failed"); break;
        }

        ptOut.resize(outLen);
        ok = true;
    } while (false);

    EVP_PKEY_CTX_free(ctx);
    return ok;
}

int testRsa2048()
{
    EVP_PKEY* rsa = generateRSA(2048);
    if (!rsa) {
        printOpenSSLErrors("RSA keygen failed");
        return 1;
    }

    std::string privPem;
    std::string pubPem;

    if (!privateKeyToPemString(rsa, privPem)) {
        EVP_PKEY_free(rsa);
        return 1;
    }
    if (!publicKeyToPemString(rsa, pubPem)) {
        EVP_PKEY_free(rsa);
        return 1;
    }

    // RSA 暗号化/復号テスト
    {
        const char* msg = "Hello RSA-OAEP(SHA-256)!";
        std::vector<unsigned char> pt(msg, msg + std::strlen(msg));

        std::vector<unsigned char> ct;
        if (!rsaEncryptOaepSha256(rsa, pt, ct)) {
            printOpenSSLErrors("RSA encrypt failed");
            EVP_PKEY_free(rsa);
            return 1;
        }

        std::vector<unsigned char> dec;
        if (!rsaDecryptOaepSha256(rsa, ct, dec)) {
            printOpenSSLErrors("RSA decrypt failed");
            EVP_PKEY_free(rsa);
            return 1;
        }

        std::cout << "----- RSA ENCRYPT/DECRYPT TEST -----\n";
        std::cout << "Plaintext : " << std::string(pt.begin(), pt.end()) << "\n";
        std::cout << "Ciphertext bytes: " << ct.size() << "\n";
        std::cout << "Decrypted : " << std::string(dec.begin(), dec.end()) << "\n";

        if (pt == dec) {
            std::cout << "[OK] decrypt == plaintext\n";
        }
        else {
            std::cout << "[NG] mismatch\n";
        }
    }

    EVP_PKEY_free(rsa);

    std::cout << "----- PRIVATE KEY PEM -----\n";
    std::cout << privPem << "\n";
    std::cout << "----- PUBLIC  KEY PEM -----\n";
    std::cout << pubPem << "\n";

    return 0;
}