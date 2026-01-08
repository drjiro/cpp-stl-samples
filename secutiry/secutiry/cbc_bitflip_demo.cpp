#include <openssl/evp.h>
#include <openssl/rand.h>

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

static bool randomBytes(std::vector<unsigned char>& out)
{
    if (out.empty()) return true;
    return RAND_bytes(out.data(), static_cast<int>(out.size())) == 1;
}

static std::string toHex(const std::vector<unsigned char>& data)
{
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    for (std::size_t i = 0; i < data.size(); ++i)
        oss << std::setw(2) << static_cast<int>(data[i]);
    return oss.str();
}

static bool aes256cbc_encrypt(
    const std::vector<unsigned char>& key, // 32
    const std::vector<unsigned char>& iv,  // 16
    const std::vector<unsigned char>& pt,
    std::vector<unsigned char>& ct_out)
{
    if (key.size() != 32 || iv.size() != 16) return false;

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) return false;

    bool ok = false;
    int len = 0;
    int out_len = 0;

    ct_out.assign(pt.size() + 16, 0); // CBC+PKCS7: 最大1ブロック増える

    do {
        if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key.data(), iv.data()) != 1) break;

        if (!pt.empty()) {
            if (EVP_EncryptUpdate(ctx, ct_out.data(), &len, pt.data(), static_cast<int>(pt.size())) != 1) break;
            out_len = len;
        }

        if (EVP_EncryptFinal_ex(ctx, ct_out.data() + out_len, &len) != 1) break;
        out_len += len;

        ct_out.resize(static_cast<std::size_t>(out_len));
        ok = true;
    } while (false);

    EVP_CIPHER_CTX_free(ctx);
    return ok;
}

static bool aes256cbc_decrypt(
    const std::vector<unsigned char>& key, // 32
    const std::vector<unsigned char>& iv,  // 16
    const std::vector<unsigned char>& ct,
    std::vector<unsigned char>& pt_out)
{
    if (key.size() != 32 || iv.size() != 16) return false;

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) return false;

    bool ok = false;
    int len = 0;
    int out_len = 0;

    pt_out.assign(ct.size(), 0);

    do {
        if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key.data(), iv.data()) != 1) break;

        if (!ct.empty()) {
            if (EVP_DecryptUpdate(ctx, pt_out.data(), &len, ct.data(), static_cast<int>(ct.size())) != 1) break;
            out_len = len;
        }

        // padding が壊れているとここで失敗することがある
        if (EVP_DecryptFinal_ex(ctx, pt_out.data() + out_len, &len) != 1) break;
        out_len += len;

        pt_out.resize(static_cast<std::size_t>(out_len));
        ok = true;
    } while (false);

    EVP_CIPHER_CTX_free(ctx);
    return ok;
}

int cbcBitFlip()
{
    // キーとIV
    std::vector<unsigned char> key(32, 0);
    std::vector<unsigned char> iv(16, 0);
    if (!randomBytes(key) || !randomBytes(iv)) {
        std::cerr << "RAND_bytes failed\n";
        return 1;
    }

    // 平文：わざと長めにして複数ブロックにする（CBCの影響が見えやすい）
    std::string msg =
        "CBC DEMO: This plaintext is long enough to span multiple blocks. "
        "We will flip 1 bit in the ciphertext and try to decrypt.";
    std::vector<unsigned char> pt(msg.begin(), msg.end());

    std::vector<unsigned char> ct;
    if (!aes256cbc_encrypt(key, iv, pt, ct)) {
        std::cerr << "CBC encrypt failed\n";
        return 1;
    }

    // 暗号文の1bitを書き換え（末尾付近を避ける：padding破壊で失敗しにくくするため）
    std::vector<unsigned char> ct_tampered = ct;
    if (ct_tampered.size() > 32) {
        std::size_t pos = 16;         // 2ブロック目の先頭あたり（例）
        ct_tampered[pos] ^= 0x01;     // 1bit flip
    }

    std::vector<unsigned char> dec_ok;
    std::vector<unsigned char> dec_bad;

    bool ok1 = aes256cbc_decrypt(key, iv, ct, dec_ok);
    bool ok2 = aes256cbc_decrypt(key, iv, ct_tampered, dec_bad);

    std::cout << "[CBC] KEY(hex): " << toHex(key) << "\n";
    std::cout << "[CBC] IV (hex): " << toHex(iv) << "\n";
    std::cout << "[CBC] CT (hex): " << toHex(ct) << "\n";
    std::cout << "[CBC] CT'(hex): " << toHex(ct_tampered) << "   (1-bit flipped)\n\n";

    std::cout << "[CBC] Decrypt original: " << (ok1 ? "SUCCESS" : "FAIL") << "\n";
    if (ok1) std::cout << "[CBC] PT : " << std::string(dec_ok.begin(), dec_ok.end()) << "\n\n";

    std::cout << "[CBC] Decrypt tampered: " << (ok2 ? "SUCCESS (!!!)" : "FAIL") << "\n";
    if (ok2) {
        std::cout << "[CBC] PT': " << std::string(dec_bad.begin(), dec_bad.end()) << "\n";
        std::cout << "      ↑ 改ざんされているのに復号が通る（= tag が無い危険）\n";
    }
    else {
        std::cout << "      ※paddingが壊れた位置だと失敗することもある（改ざん検知ではない）\n";
    }

    return 0;
}