#include <iostream>
#include <vector>

extern void printSort2(std::vector<int> v, int size);

// 配列をマージ（統合）
void merge(std::vector<int>& v, int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // 一時配列
    std::vector<int> L(n1), R(n2);

    // 左右に分割コピー
    for (int i = 0; i < n1; i++)
    {
        L[i] = v[left + i];
    }
    for (int j = 0; j < n2; j++)
    {
        R[j] = v[mid + 1 + j];
    }

    // マージ処理
    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            v[k] = L[i];
            i++;
        }
        else
        {
            v[k] = R[j];
            j++;
        }
        k++;
    }

    // 残りをコピー
    while (i < n1)
    {
        v[k++] = L[i++];
    }
    while (j < n2)
    {
        v[k++] = R[j++];
    }
}

// 再帰的マージソート
void mergeSortRecursive(std::vector<int>& v, int left, int right, int size)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;

        // 左右を分割してソート
        mergeSortRecursive(v, left, mid, size);
        mergeSortRecursive(v, mid + 1, right, size);

        // 統合
        merge(v, left, mid, right);

        std::cout << "マージ中 : ";
        printSort2(v, size);
    }
}
//
// マージソート全体
//
void merge2(std::vector<int>& sortList, int size)
{
    std::cout << "初期状態: ";
    printSort2(sortList, size);

    mergeSortRecursive(sortList, 0, size - 1, size);

    std::cout << "最終結果: ";
    printSort2(sortList, size);
}
