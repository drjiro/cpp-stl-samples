#include <iostream>
#include <vector>

// 名前空間の指定
//using namespace std;

void testVector()
{
	int a[] = { 10,20,30 };
	// これは実行時もエラーにならない
	std::cout << a[-1] << std::endl;

	std::cout << "<< vector >>" << std::endl;
	// vectorの宣言
	std::vector<int> v = { 10, 20, 30 };
	// 要素数の取得
	std::cout << "要素数 : " << v.size() << std::endl;
	// 要素の一覧
	for (int x : v) std::cout << x << " ";
	std::cout << std::endl;

	// 要素の追加
	std::cout << "<要素の追加>" << std::endl;
	v.push_back(40);
	std::cout << "要素数 : " << v.size() << std::endl;
	// 要素の一覧
	for (int x : v) std::cout << x << " ";
	std::cout << std::endl;

	// 要素の参照
	std::cout << "<要素の参照>" << std::endl;
	std::cout << v[0] << std::endl;
	std::cout << v.at(1) << std::endl;

	// ここから実習を始める
	// 1.要素の参照エラー
	//std::cout << v[-1] << std::endl;
	// 2.要素の参照エラー
	//std::cout << v[4] << std::endl;
	// 3.要素の参照エラー
	//std::cout << v.at(-1) << std::endl;
	// 4.要素の参照エラー
	//std::cout << v.at(4) << std::endl;

	// 要素の削除
	std::cout << "<要素の削除>" << std::endl;
	v.pop_back();
	// 要素数の取得
	std::cout << "要素数 : " << v.size() << std::endl;
	// 要素の一覧
	for (int x : v) std::cout << x << " ";
	std::cout << std::endl;

	// 全削除
	v.clear();
	std::cout << "<要素の全削除>" << std::endl;
	std::cout << "要素数 : " << v.size() << std::endl;

	std::cout << "<入れられる要素数の確認>" << std::endl;
	for (int i = 0; i < 10; i++)
	{
		v.push_back(i);
		// 要素数の取得
		std::cout << "要素数 : " << v.size() << std::endl;
		// 入れられる要素数
		std::cout << "入れられる要素数 : " << v.capacity() << std::endl;
	}
	std::cout << "<入れられる要素数の変更>" << std::endl;
	//v.reserve(10);
	v.reserve(100);
	std::cout << "入れられる要素数 : " << v.capacity() << std::endl;
}
