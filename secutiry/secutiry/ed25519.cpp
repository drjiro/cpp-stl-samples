#include <sodium.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

static std::string toHex(const unsigned char* p, std::size_t n)
{
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    for (std::size_t i = 0; i < n; ++i) {
        oss << std::setw(2) << static_cast<int>(p[i]);
    }
    return oss.str();
}

int testEd25519()
{
    if (sodium_init() < 0) {
        std::cerr << "sodium_init failed\n";
        return 1;
    }

    // 鍵ペア生成（Ed25519）
    unsigned char pk[crypto_sign_PUBLICKEYBYTES];
    unsigned char sk[crypto_sign_SECRETKEYBYTES];
    crypto_sign_keypair(pk, sk);

    // 署名対象メッセージ
    std::string message = "Ed25519 signature test with libsodium.";
    std::vector<unsigned char> msg(message.begin(), message.end());

    // 署名（detached signature）
    unsigned char sig[crypto_sign_BYTES]; // 64 bytes
    unsigned long long siglen = 0;

    if (crypto_sign_detached(sig, &siglen, msg.data(), (unsigned long long)msg.size(), sk) != 0) {
        std::cerr << "crypto_sign_detached failed\n";
        return 1;
    }

    // 検証
    int vr = crypto_sign_verify_detached(sig, msg.data(), (unsigned long long)msg.size(), pk);

    // 改ざんテスト：1byte変える
    std::vector<unsigned char> tampered = msg;
    if (!tampered.empty()) tampered[0] ^= 0x01;
    int vr2 = crypto_sign_verify_detached(sig, tampered.data(), (unsigned long long)tampered.size(), pk);

    std::cout << "Text: " << message << "\n";
    std::cout << "PublicKey(hex): " << toHex(pk, sizeof(pk)) << "\n";
    std::cout << "Signature(hex): " << toHex(sig, (std::size_t)siglen) << "\n";
    std::cout << "Verify(original): " << (vr == 0 ? "OK" : "NG") << "\n";
    std::cout << "Verify(tampered): " << (vr2 == 0 ? "OK" : "NG") << "\n";

    return 0;
}