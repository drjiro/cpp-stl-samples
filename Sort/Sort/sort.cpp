//
// ソートアルゴリズムの実装とテスト
//
#include <iostream>
#include <vector>
#include <chrono>

// 基本課題
extern void bubble(int[], int size);
extern void bubble2(std::vector<int>, int size);
extern void selection(int[], int size);
extern void selection2(std::vector<int>, int size);
extern void insertion(int[], int size);
extern void insertion2(std::vector<int>, int size);

// 応用課題
extern void heap2(std::vector<int>&, int size);
extern void merge2(std::vector<int>&, int size);
extern void quick2(std::vector<int>&, int size);
extern void qsortLib2(std::vector<int>, int size);

// 超応用課題
extern void bench();

//	並べ替え結果の表示関数（配列）
void printSort(int v[], int size)
{
	for (int i = 0; i < size; i++)
	{
		std::cout << v[i] << " ";
	}
	std::cout << std::endl;
}

//	並べ替え結果の表示関数（std::vector）
void printSort2(std::vector<int> v, int size)
{
	for (int i = 0; i < size; i++)
	{
		std::cout << v[i] << " ";
	}
	std::cout << std::endl;
}

int main()
{
	// データ配列
	int sortList[9] = { 5, 9, 3, 1, 2, 8, 4, 7, 6 };
	std::vector<int> base2 = { 5, 9, 3, 1, 2, 8, 4, 7, 6 };
	std::vector<int> sortList2;
	
	int size = sizeof sortList / sizeof(int);
    
    // バブルソート
	//std::cout << "\n## バブルソート" << std::endl;
	//bubble(sortList, size);
	//sortList2 = base2; // 毎回コピー（同じ入力データ）
	//bubble2(sortList2, sortList2.size());

	// 選択ソート
	//std::cout << "\n## 選択ソート" << std::endl;
	//selection(sortList, size);
	//sortList2 = base2; // 毎回コピー（同じ入力データ）
	//selection2(sortList2, sortList2.size());

	// 挿入ソート
	//std::cout << "\n## 挿入ソート" << std::endl;
	//insertion(sortList, size);
	//sortList2 = base2; // 毎回コピー（同じ入力データ）
	//insertion2(sortList2, sortList2.size());

	// ヒープソート
	//std::cout << "\n## ヒープソート" << std::endl;
	//sortList2 = base2; // 毎回コピー（同じ入力データ）
	//heap2(sortList2, sortList2.size());

	// マージソート
	//std::cout << "\n## マージソート" << std::endl;
	//sortList2 = base2; // 毎回コピー（同じ入力データ）
	//merge2(sortList2, sortList2.size());

	// クイックソート
	//std::cout << "\n## クイックソート" << std::endl;
	//sortList2 = base2; // 毎回コピー（同じ入力データ）
	//quick2(sortList2, sortList2.size());

	// qsortを利用
	//std::cout << "\n## qsortを利用" << std::endl;
	//sortList2 = base2; // 毎回コピー（同じ入力データ）
	//qsortLib2(sortList2, sortList2.size());

    // ベンチマーク
	std::cout << "\n## ベンチマーク" << std::endl;
	bench();
}
