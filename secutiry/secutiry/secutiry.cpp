// secutiry.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <iostream>

extern void testSymmetric();
extern int testAES();
extern int testRsa2048();
extern int prime256();
extern int testX25519();
extern int testEd25519();

int main()
{
    std::cout << "共通鍵暗号(XOR)\n";
    testSymmetric();

    //std::cout << "共通鍵暗号(AES)\n";
    //testAES();

    //std::cout << "公開鍵暗号(RSA2048)\n";
    //testRsa2048();

    //std::cout << "公開鍵暗号(X25519)\n";
    //testX25519();

    //std::cout << "署名by公開鍵暗号(Ed25519)\n";
    //testEd25519();
}
