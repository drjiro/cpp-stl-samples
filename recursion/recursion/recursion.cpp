#include <iostream>

// 階乗
int fact(int n) {
    if (n <= 1) {
        return 1;
    }
    return n * fact(n - 1);
}

// 1からnまでの和
int sum(int n) {
    if (n <= 0) {
        return 0;
    }
    return n + sum(n - 1);
}


// --- 動作確認 ---
int main() {
    // 階乗
    int res1 = sum(3);
    std::cout << "fact(3) = " << res1 << std::endl;
    // 階乗
    int res2 = sum(10);
    std::cout << "sum(10) = " << res2 << std::endl;

    return 0;
}