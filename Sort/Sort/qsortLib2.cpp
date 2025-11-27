#include <iostream>
#include <cstdlib>  // qsortを使うために必要
#include <vector>

extern void printSort2(std::vector<int> v, int size);

// 比較関数（昇順）
// qsortはvoid*を使うため、キャストが必要
int compareIntAsc(const void* a, const void* b)
{
    int arg1 = *(const int*)a;
    int arg2 = *(const int*)b;
    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;
}

// 
// qsortを使ったソート
//
void qsortLib2(std::vector<int> sortList, int size)
{
    std::cout << "初期状態: ";
    printSort2(sortList, size);

    // qsortでソート実行
    qsort(sortList.data(), sortList.size(), sizeof(int), compareIntAsc);

    std::cout << "最終結果: ";
    printSort2(sortList, size);
}
