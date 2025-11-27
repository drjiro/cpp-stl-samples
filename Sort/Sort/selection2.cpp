#include <iostream>
#include <string>
#include <vector>

extern void printSort2(std::vector<int> v, int size);

//
// 選択ソート
//
void selection2(std::vector<int> sortList, int size)
{
    // 初期状態の表示
    std::cout << "初期状態: ";
    printSort2(sortList, size);

    // 外側のループ
    // TODO: 前からどこまで？
    for (int i = 0; i < size - 1; i++)
    {
        // 内側のループ。
        // 範囲内で最小値を探す
        // 最小値を確認するための変数。
        int min = sortList[i];
        // minに入れる値が見つかったら、添え字も記録したいので変数を準備
        int minIndex = i;

        // 内側のループ。
        // 最小の値を確認するためのループとなる
        // TODO: どこから最後まで？
        for (int j = i + 1; j < size; j++)
        {
            // jの指す場所にある数値と変数minを比較する
            if (sortList[j] < min)
            {
                // TODO: この時点の最小値は？
                min = sortList[j];
                // TODO: この時点の最小値のインデックスは？
                minIndex = j;
            }
            std::cout << "　内側 min=" << min << " minIndex=" << minIndex << ": ";
            printSort2(sortList, size);
        }
        // iの指す場所とminIndexの指す場所を入れ替える
        // 何かを足すともっと良くなる？
        if (min > 0)
        {
            // TODO: 入れ替え処理（３行）
            int tmp = sortList[i];
            sortList[i] = sortList[minIndex];
            sortList[minIndex] = tmp;
        }

        std::cout << "外側　　: ";
        printSort2(sortList, size);
    }
}