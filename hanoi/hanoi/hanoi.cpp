#include <iostream>
#include <vector>
#include <string>

// 各タワー
std::vector<int> A, B, C;
int step = 1;

// 棒の状態を表示する関数
void printTowers() {
    std::cout << "Step " << step++ << ":\n";
    auto print = [](const std::string& name, const std::vector<int>& v) {
        std::cout << name << ": ";
        for (int d : v) std::cout << d << " ";
        std::cout << std::endl;
    };

    print("A", A);
    print("B", B);
    print("C", C);

    std::cout << "---------------------------\n";
}

// 円盤を 1 枚動かす
void moveDisk(char from, char to) {
    std::vector<int>* src;
    std::vector<int>* dst;

    if (from == 'A') {
        src = &A;
    }
    else if (from == 'B') {
        src = &B;
    }
    else {
        src = &C;
    }

    if (to == 'A') {
        dst = &A;
    }
    else if (to == 'B') {
        dst = &B;
    }
    else {
        dst = &C;
    }

    // 移動元のディスクを取得
    int disk = src->back();
    // 移動元から削除
    src->pop_back();
	// 移動先に追加
    dst->push_back(disk);

    std::cout << "Move disk " << disk << " from " << from << " to " << to << std::endl;
    printTowers();
}

// ハノイの塔の再帰処理
void hanoi(int n, char from, char to, char aux) {
    if (n == 1) {
        moveDisk(from, to);
        return;
    }

    hanoi(n - 1, from, aux, to);
    moveDisk(from, to);
    hanoi(n - 1, aux, to, from);
}

int main() {
    int n;
    std::cout << "Number of disks: ";
    std::cin >> n;

    // A に n 個（大→小の順）で積む
    for (int i = n; i >= 1; i--) {
        A.push_back(i);
    }

    // 初期状態
    printTowers();

    hanoi(n, 'A', 'C', 'B');

    return 0;
}
