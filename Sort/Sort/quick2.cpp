#include <iostream>
#include <vector>

extern void printSort2(std::vector<int> v, int size);

// 配列の分割（パーティション）
int partition(std::vector<int>& v, int left, int right, int size)
{
    int pivot = v[right]; // ピボットを右端に設定
    int i = left - 1;     // 小さい要素の位置を管理

    for (int j = left; j < right; j++)
    {
        if (v[j] <= pivot)
        {
            i++;
            std::swap(v[i], v[j]);
        }
    }
    std::swap(v[i + 1], v[right]); // ピボットを正しい位置に

    std::cout << "分割中 : ";
    printSort2(v, size);

    return i + 1; // ピボットの最終位置を返す
}

// 再帰的クイックソート
void quickSortRecursive(std::vector<int>& v, int left, int right, int size)
{
    if (left < right)
    {
        int pivotIndex = partition(v, left, right, size);
        quickSortRecursive(v, left, pivotIndex - 1, size);   // 左側
        quickSortRecursive(v, pivotIndex + 1, right, size);  // 右側
    }
}

// 
// クイックソート全体
//
void quick2(std::vector<int>& sortList, int size)
{
    std::cout << "初期状態: ";
    printSort2(sortList, size);

    quickSortRecursive(sortList, 0, size - 1, size);

    std::cout << "最終結果: ";
    printSort2(sortList, size);
}
