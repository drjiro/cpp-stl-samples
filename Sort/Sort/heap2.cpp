#include <iostream>
#include <vector>

extern void printSort2(std::vector<int>, int size);

// ヒープを作る（heapify）
void heapify(std::vector<int>& v, int size, int i)
{
    int largest = i;         // 最大値のインデックス（親）
    int left = 2 * i + 1;    // 左の子
    int right = 2 * i + 2;   // 右の子

    std::cout << "i:" << i << " largest:" << largest << " left: " << left << " right:" << right << " v: ";
    printSort2(v, size);

    // 左の子が親より大きい場合
    if (left < size && v[left] > v[largest])
    {
        largest = left;
    }

    // 右の子が親より大きい場合
    if (right < size && v[right] > v[largest])
    {
        largest = right;
    }

    // 最大が親でない場合は入れ替えて再帰
    if (largest != i)
    {
        std::cout << "swap v[" << i << "](" << v[i] << ")<->v[" << largest << "](" << v[largest] << ")" << std::endl;
        std::swap(v[i], v[largest]);
        heapify(v, size, largest);
    }
}

// ヒープソート
void heap2(std::vector<int>& sortList, int size)
{
    std::cout << "初期状態: ";
    printSort2(sortList, size);

    // --- ヒープ構築フェーズ ---
    // 親ノードから順に最大ヒープを作る
    std::cout << "--- ヒープ構築フェーズ ---" << std::endl;
    for (int i = size / 2 - 1; i >= 0; i--)
    {
        heapify(sortList, size, i);
        std::cout << "ヒープ化 i=" << i << " : ";
        printSort2(sortList, size);
    }

    // --- ソートフェーズ ---
    // ルート（最大値）を末尾に移動し、残りを再ヒープ化
    std::cout << "\n--- ソートフェーズ ---" << std::endl;
    for (int i = size - 1; i > 0; i--)
    {
        std::cout << "swap sortList[0](" << sortList[0] << ")<->sortList[" << i << "](" << sortList[i] << ")" << std::endl;
        std::swap(sortList[0], sortList[i]); // 最大値を末尾へ
        heapify(sortList, i, 0);             // 残りを再ヒープ化
        std::cout << "ソート中 : ";
        printSort2(sortList, size);
    }

    std::cout << "最終結果: ";
    printSort2(sortList, size);
}
