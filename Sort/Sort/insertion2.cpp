#include <iostream>
#include <string>
#include <vector>

extern void printSort2(std::vector<int> v, int size);

//
// 挿入ソート
//
void insertion2(std::vector<int> sortList, int size)
{
    //	初期状態の表示
    std::cout << "初期状態: ";
    printSort2(sortList, size);

    int tmp = 0;

    // 外側のループ
    // TODO: どこから？
    for (int i = 1; i < size; i++)
    {
        // 挿入する値をkeyに保存
        int key = sortList[i];
        // TODO: どこから？
        int j = i - 1;
        // 内側のループ
        // 前に向かって、挿入位置を探す
        // TODO: ループ条件は？
        while (j >= 0 && sortList[j] > key)
        {
            // TODO: 一つ後ろにずらす
            sortList[j + 1] = sortList[j];
            --j;
            std::cout << "　内側　: ";
            printSort2(sortList, size);
        }
        // TODO: 挿入すべき位置にkeyを入れる
        sortList[j + 1] = key;

        std::cout << "外側　　: ";
        printSort2(sortList, size);
    }
}