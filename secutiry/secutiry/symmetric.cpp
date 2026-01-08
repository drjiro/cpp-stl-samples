#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>

// XOR による暗号化・復号（同じ関数）
std::string xorCrypt(const std::string& text, const std::string& key)
{
    std::string result = text;

    for (std::size_t i = 0; i < text.size(); ++i)
    {
        result[i] = text[i] ^ key[i % key.size()];
    }
    return result;
}

// バイナリを16進文字列で表示するための関数
std::string toHex(const std::string& data)
{
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');

    for (unsigned char c : data)
    {
        oss << std::setw(2) << static_cast<int>(c);
    }
    return oss.str();
}

void testSymmetric()
{
    std::string plainText = "HELLO WORLD";
    std::string key = "SECRET";

    // 暗号化
    std::string cipherText = xorCrypt(plainText, key);

    // 復号
    std::string decryptedText = xorCrypt(cipherText, key);

    std::cout << "Plain Text   : " << plainText << std::endl;
    std::cout << "Key          : " << key << std::endl;
    std::cout << "Cipher (hex) : " << toHex(cipherText) << std::endl;
    std::cout << "Decrypted    : " << decryptedText << std::endl;
}