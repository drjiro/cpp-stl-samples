#include <iostream>
#include <vector>

extern void printSort2(std::vector<int> v, int size);

//
// バブルソート（vector版）
//
void bubble2(std::vector<int> sortList, int size)
{
    // 初期状態の表示
    std::cout << "初期状態: ";
    printSort2(sortList, size);

    // バブルソートで並び替えを行う

    // 外側のループ
    // TODO: 前からどこまで？
    for (int i = 0; i < size - 1; i++)
    {
        // 内側のループ
        // TODO: どこから先頭まで？
        for (int j = size - 2; j >= 0; j--)
        {
            // 隣同士を比較して、順序が逆なら入れ替え
            if (sortList[j] > sortList[j + 1])
            {
                // TODO: 入れ替え処理（３行）
                int tmp = sortList[j];
                sortList[j] = sortList[j + 1];
                sortList[j + 1] = tmp;
            }
            std::cout << "　内側　: ";
            printSort2(sortList, size);
        }
        std::cout << "外側　　: ";
        printSort2(sortList, size);
    }
}