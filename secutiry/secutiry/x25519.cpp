// ---------------------------------------------------------
// 重要: NuGetのlibsodium(static版)を使う場合、この定義が必要です。
// 必ず #include <sodium.h> の前に記述してください。
#define SODIUM_STATIC
// ---------------------------------------------------------

#include <sodium.h>
#include <iostream>
#include <vector>
#include <string>

// バイナリデータを16進数で見やすく表示する関数
void print_hex(const std::string& label, const std::vector<unsigned char>& data) {
    std::cout << label << " (" << data.size() << " bytes): ";
    for (unsigned char c : data) {
        printf("%02x", c);
    }
    std::cout << std::endl;
}

int testX25519() {
    // 1. Libsodiumの初期化
    if (sodium_init() < 0) {
        std::cerr << "Libsodiumの初期化に失敗しました。" << std::endl;
        return -1;
    }
    std::cout << "Libsodium initialized successfully." << std::endl;

    // --- 【鍵の準備】 (受信者側) ---
    // 暗号化には Ed25519 ではなく Curve25519 (X25519) を使います

    std::vector<unsigned char> recipient_pk(crypto_box_PUBLICKEYBYTES);
    std::vector<unsigned char> recipient_sk(crypto_box_SECRETKEYBYTES);

    // 鍵ペア生成
    crypto_box_keypair(recipient_pk.data(), recipient_sk.data());

    print_hex("受信者の公開鍵", recipient_pk);
    // print_hex("受信者の秘密鍵", recipient_sk); // 秘密鍵は厳重に保管


    // --- 【暗号化】 (送信者側) ---
    // 送信者は「受信者の公開鍵」を使ってメッセージをロックします

    std::string message = "Message from Visual Studio!";
    std::cout << "\n[送信] 平文: " << message << std::endl;

    // 暗号文のサイズ計算 (メッセージ長 + SEALBYTES)
    size_t ciphertext_len = crypto_box_SEALBYTES + message.size();
    std::vector<unsigned char> ciphertext(ciphertext_len);

    // 暗号化 (Sealed Box)
    if (crypto_box_seal(
        ciphertext.data(),
        reinterpret_cast<const unsigned char*>(message.c_str()),
        message.size(),
        recipient_pk.data()) != 0) {
        std::cerr << "暗号化エラー" << std::endl;
        return -1;
    }

    print_hex("[送信] 暗号文", ciphertext);


    // --- 【復号】 (受信者側) ---
    // 受信者は「自分の秘密鍵」と「自分の公開鍵」を使ってロックを解除します

    std::vector<unsigned char> decrypted(message.size());

    // 復号 (Open Sealed Box)
    if (crypto_box_seal_open(
        decrypted.data(),
        ciphertext.data(),
        ciphertext_len,
        recipient_pk.data(),
        recipient_sk.data()) != 0) {
        std::cerr << "復号エラー: 鍵が間違っているか、データが破損しています。" << std::endl;
        return -1;
    }

    // 結果表示
    std::string decrypted_text(decrypted.begin(), decrypted.end());
    std::cout << "\n[受信] 復号結果: " << decrypted_text << std::endl;

    return 0;
}