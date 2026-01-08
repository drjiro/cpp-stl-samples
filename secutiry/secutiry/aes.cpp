#include <openssl/evp.h>
#include <openssl/rand.h>

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>

// バイナリを16進文字列で表示するための関数
static std::string toHex(const std::vector<unsigned char>& data)
{
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    for (std::size_t i = 0; i < data.size(); ++i)
    {
        oss << std::setw(2) << static_cast<int>(data[i]);
    }
    return oss.str();
}

// 指定サイズのランダムバイト列を生成
static bool randomBytes(std::vector<unsigned char>& out)
{
    if (out.empty()) return true;
    return RAND_bytes(out.data(), static_cast<int>(out.size())) == 1;
}

// AES-GCM 暗号化結果
struct AesGcmResult
{
    std::vector<unsigned char> iv;         // 12 bytes recommended
    std::vector<unsigned char> ciphertext; // same length as plaintext
    std::vector<unsigned char> tag;        // 16 bytes
};

// AES-256-GCM 暗号化
static bool aes256gcm_encrypt(
    const std::vector<unsigned char>& key,           // 32 bytes
    const std::vector<unsigned char>& plaintext,
    const std::vector<unsigned char>& aad,           // can be empty
    AesGcmResult& out)
{
	// 鍵サイズのチェック
    if (key.size() != 32) return false;

	// 出力バッファの用意
    out.iv.assign(12, 0);
    out.tag.assign(16, 0);
	// 暗号化テキストは元のテキストと同じサイズ
    out.ciphertext.assign(plaintext.size(), 0);

	// ランダムなIVを生成
    if (!randomBytes(out.iv)) return false;

	// コンテキストの作成
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) return false;

	// 暗号化処理
    bool ok = false;
    int len = 0;
    int ciphertext_len = 0;

    do
    {
		// 暗号化処理の初期化
        if (EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL) != 1) break;

        // IVの長さを設定 (GCMのデフォルトは 12だけど明示的に設定)
        if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN,
            static_cast<int>(out.iv.size()), NULL) != 1) break;

		// 鍵とIVを設定
        if (EVP_EncryptInit_ex(ctx, NULL, NULL, key.data(), out.iv.data()) != 1) break;

        // AAD (オプション、暗号化はしないが、改ざんされたら必ず検出されるデータ)
        if (!aad.empty())
        {
			// 追加認証データを設定
            if (EVP_EncryptUpdate(ctx, NULL, &len, aad.data(),
                static_cast<int>(aad.size())) != 1) break;
        }

        if (!plaintext.empty())
        {
			// 暗号化データの更新
            if (EVP_EncryptUpdate(ctx, out.ciphertext.data(), &len,
                plaintext.data(), static_cast<int>(plaintext.size())) != 1) break;
            ciphertext_len = len;
        }

        // ファイナライズ（GCM は余分なバイトを追加しませんが、とにかく呼び出します）
        if (EVP_EncryptFinal_ex(ctx, out.ciphertext.data() + ciphertext_len, &len) != 1) break;
        ciphertext_len += len;

		// サイズを調整
        out.ciphertext.resize(static_cast<std::size_t>(ciphertext_len));

        // 認証タグを取得する
        if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG,
            static_cast<int>(out.tag.size()), out.tag.data()) != 1) break;

        ok = true;
    } while (false);

	// 解放
    EVP_CIPHER_CTX_free(ctx);
    return ok;
}

static bool aes256gcm_decrypt(
    const std::vector<unsigned char>& key,              // 32 bytes
    const std::vector<unsigned char>& iv,               // 12 bytes recommended
    const std::vector<unsigned char>& ciphertext,
    const std::vector<unsigned char>& tag,              // 16 bytes
    const std::vector<unsigned char>& aad,              // must match encrypt
    std::vector<unsigned char>& plaintext_out)
{
    if (key.size() != 32) return false;
    if (tag.size() != 16) return false;
    if (iv.empty()) return false;

    plaintext_out.assign(ciphertext.size(), 0);

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) return false;

    bool ok = false;
    int len = 0;
    int plaintext_len = 0;

    do
    {
        if (EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL) != 1) break;

        if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN,
            static_cast<int>(iv.size()), NULL) != 1) break;

        if (EVP_DecryptInit_ex(ctx, NULL, NULL, key.data(), iv.data()) != 1) break;

        // AAD must be the same as used in encryption
        if (!aad.empty())
        {
            if (EVP_DecryptUpdate(ctx, NULL, &len, aad.data(),
                static_cast<int>(aad.size())) != 1) break;
        }

        if (!ciphertext.empty())
        {
            if (EVP_DecryptUpdate(ctx, plaintext_out.data(), &len,
                ciphertext.data(), static_cast<int>(ciphertext.size())) != 1) break;
            plaintext_len = len;
        }

        // Set expected tag BEFORE final
        if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG,
            static_cast<int>(tag.size()),
            const_cast<unsigned char*>(tag.data())) != 1) break;

        // Final verifies tag: returns 1 if OK, 0 if auth failed
        if (EVP_DecryptFinal_ex(ctx, plaintext_out.data() + plaintext_len, &len) != 1) break;
        plaintext_len += len;

        plaintext_out.resize(static_cast<std::size_t>(plaintext_len));
        ok = true;
    } while (false);

    EVP_CIPHER_CTX_free(ctx);
    return ok;
}

int testAES()
{
    // 32-byte key (demo). In real apps: derive from password using PBKDF2/scrypt/Argon2, or generate & store securely.
    std::vector<unsigned char> key(32, 0);
    if (!randomBytes(key))
    {
        std::cerr << "RAND_bytes failed\n";
        return 1;
    }

    std::string msg = "Hello AES-256-GCM via OpenSSL EVP!";
    std::vector<unsigned char> plaintext(msg.begin(), msg.end());

    // AAD is optional. If you use it, it must match exactly on decrypt.
    std::vector<unsigned char> aad;
    {
        std::string aadStr = "header-metadata";
        aad.assign(aadStr.begin(), aadStr.end());
    }

    AesGcmResult enc;
    if (!aes256gcm_encrypt(key, plaintext, aad, enc))
    {
        std::cerr << "encrypt failed\n";
        return 1;
    }

    std::vector<unsigned char> dec;
    if (!aes256gcm_decrypt(key, enc.iv, enc.ciphertext, enc.tag, aad, dec))
    {
        std::cerr << "decrypt failed (tag mismatch?)\n";
        return 1;
    }

    std::string recovered(dec.begin(), dec.end());

    std::cout << "Plain    : " << msg << "\n";
    std::cout << "KEY (hex): " << toHex(key) << "\n";
    std::cout << "IV  (hex): " << toHex(enc.iv) << "\n";
    std::cout << "TAG (hex): " << toHex(enc.tag) << "\n";
    std::cout << "CT  (hex): " << toHex(enc.ciphertext) << "\n";
    std::cout << "RECOVERED: " << recovered << "\n";

    return 0;
}