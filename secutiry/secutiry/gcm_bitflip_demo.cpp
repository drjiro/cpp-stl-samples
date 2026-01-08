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

struct GcmOut {
    std::vector<unsigned char> iv;  // 12
    std::vector<unsigned char> ct;
    std::vector<unsigned char> tag; // 16
};

static bool aes256gcm_encrypt(
    const std::vector<unsigned char>& key, // 32
    const std::vector<unsigned char>& pt,
    const std::vector<unsigned char>& aad,
    GcmOut& out)
{
    if (key.size() != 32) return false;

    out.iv.assign(12, 0);
    out.tag.assign(16, 0);
    out.ct.assign(pt.size(), 0);
    if (!randomBytes(out.iv)) return false;

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) return false;

    bool ok = false;
    int len = 0;
    int out_len = 0;

    do {
        if (EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL) != 1) break;
        if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, static_cast<int>(out.iv.size()), NULL) != 1) break;
        if (EVP_EncryptInit_ex(ctx, NULL, NULL, key.data(), out.iv.data()) != 1) break;

        if (!aad.empty()) {
            if (EVP_EncryptUpdate(ctx, NULL, &len, aad.data(), static_cast<int>(aad.size())) != 1) break;
        }

        if (!pt.empty()) {
            if (EVP_EncryptUpdate(ctx, out.ct.data(), &len, pt.data(), static_cast<int>(pt.size())) != 1) break;
            out_len = len;
        }

        if (EVP_EncryptFinal_ex(ctx, out.ct.data() + out_len, &len) != 1) break;
        out_len += len;
        out.ct.resize(static_cast<std::size_t>(out_len));

        if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, static_cast<int>(out.tag.size()), out.tag.data()) != 1) break;

        ok = true;
    } while (false);

    EVP_CIPHER_CTX_free(ctx);
    return ok;
}

static bool aes256gcm_decrypt(
    const std::vector<unsigned char>& key, // 32
    const std::vector<unsigned char>& iv,
    const std::vector<unsigned char>& ct,
    const std::vector<unsigned char>& tag,
    const std::vector<unsigned char>& aad,
    std::vector<unsigned char>& pt_out)
{
    if (key.size() != 32 || iv.empty() || tag.size() != 16) return false;

    pt_out.assign(ct.size(), 0);

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) return false;

    bool ok = false;
    int len = 0;
    int out_len = 0;

    do {
        if (EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL) != 1) break;
        if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, static_cast<int>(iv.size()), NULL) != 1) break;
        if (EVP_DecryptInit_ex(ctx, NULL, NULL, key.data(), iv.data()) != 1) break;

        if (!aad.empty()) {
            if (EVP_DecryptUpdate(ctx, NULL, &len, aad.data(), static_cast<int>(aad.size())) != 1) break;
        }

        if (!ct.empty()) {
            if (EVP_DecryptUpdate(ctx, pt_out.data(), &len, ct.data(), static_cast<int>(ct.size())) != 1) break;
            out_len = len;
        }

        // 期待タグをセット
        if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG,
            static_cast<int>(tag.size()),
            const_cast<unsigned char*>(tag.data())) != 1) break;

        // ここでタグ検証：改ざんされていたら失敗(0)
        if (EVP_DecryptFinal_ex(ctx, pt_out.data() + out_len, &len) != 1) break;
        out_len += len;

        pt_out.resize(static_cast<std::size_t>(out_len));
        ok = true;
    } while (false);

    EVP_CIPHER_CTX_free(ctx);
    return ok;
}

int gcmBitFlip()
{
    std::vector<unsigned char> key(32, 0);
    if (!randomBytes(key)) {
        std::cerr << "RAND_bytes failed\n";
        return 1;
    }

    std::string msg = "GCM DEMO: flip 1 bit in ciphertext => tag verification must fail.";
    std::vector<unsigned char> pt(msg.begin(), msg.end());

    std::vector<unsigned char> aad;
    {
        std::string hdr = "demo-aad-header";
        aad.assign(hdr.begin(), hdr.end());
    }

    GcmOut enc;
    if (!aes256gcm_encrypt(key, pt, aad, enc)) {
        std::cerr << "GCM encrypt failed\n";
        return 1;
    }

    // 1bit改ざん
    std::vector<unsigned char> ct_tampered = enc.ct;
    if (!ct_tampered.empty()) {
        ct_tampered[0] ^= 0x01;
    }

    std::vector<unsigned char> dec_ok;
    std::vector<unsigned char> dec_bad;

    bool ok1 = aes256gcm_decrypt(key, enc.iv, enc.ct, enc.tag, aad, dec_ok);
    bool ok2 = aes256gcm_decrypt(key, enc.iv, ct_tampered, enc.tag, aad, dec_bad);

    std::cout << "[GCM] KEY(hex): " << toHex(key) << "\n";
    std::cout << "[GCM] IV (hex): " << toHex(enc.iv) << "\n";
    std::cout << "[GCM] TAG(hex): " << toHex(enc.tag) << "\n";
    std::cout << "[GCM] CT (hex): " << toHex(enc.ct) << "\n";
    std::cout << "[GCM] CT'(hex): " << toHex(ct_tampered) << "   (1-bit flipped)\n\n";

    std::cout << "[GCM] Decrypt original: " << (ok1 ? "SUCCESS" : "FAIL") << "\n";
    if (ok1) std::cout << "[GCM] PT : " << std::string(dec_ok.begin(), dec_ok.end()) << "\n\n";

    std::cout << "[GCM] Decrypt tampered: " << (ok2 ? "SUCCESS (should NOT happen)" : "FAIL (as expected)") << "\n";
    if (!ok2) {
        std::cout << "      ↑ 1bitでも改ざんするとタグ検証で落ちる（= tag が必要な理由）\n";
    }
    return 0;
}