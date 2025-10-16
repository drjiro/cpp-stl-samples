#include <iostream>
#include <deque>

// 名前空間の指定
//using namespace std;

void testDeque()
{
	std::cout << "<< deque >>" << std::endl;
	// vectorの宣言
	std::deque<int> d = { 10, 20 };
	// 要素数の取得
	std::cout << "要素数 : " << d.size() << std::endl;
	// 要素の一覧
	for (int x : d) std::cout << x << " ";
	std::cout << std::endl;
	
	// 末尾に追加
	std::cout << "<末尾に追加>" << std::endl;
	d.push_back(30);
	std::cout << "要素数 : " << d.size() << std::endl;
	// 要素の一覧
	for (int x : d) std::cout << x << " ";
	std::cout << std::endl;
	
	// 先頭に追加
	std::cout << "<先頭に追加>" << std::endl;
	d.push_front(40);
	std::cout << "要素数 : " << d.size() << std::endl;
	// 要素の一覧
	for (int x : d) std::cout << x << " ";
	std::cout << std::endl;
	
	// 要素の参照
	std::cout << "<要素の参照>" << std::endl;
	std::cout << d[0] << std::endl;
	std::cout << d.at(1) << std::endl;
	
	// ここから実習を始める
	// 1.要素の参照エラー
	//std::cout << d[-1] << std::endl;
	// 2.要素の参照エラー
	//std::cout << d[4] << std::endl;
	// 3.要素の参照エラー
	//std::cout << d.at(-1) << std::endl;
	// 4.要素の参照エラー
	//std::cout << d.at(4) << std::endl;
	
	// 末尾を削除
	std::cout << "<末尾を削除>" << std::endl;
	d.pop_back();
	// 要素数の取得
	std::cout << "要素数 : " << d.size() << std::endl;
	// 要素の一覧
	for (int x : d) std::cout << x << " ";
	std::cout << std::endl;
	
	// 先頭を削除
	std::cout << "<先頭を削除>" << std::endl;
	d.pop_front();
	// 要素数の取得
	std::cout << "要素数 : " << d.size() << std::endl;
	// 要素の一覧
	for (int x : d) std::cout << x << " ";
	std::cout << std::endl;
	
	// 全削除
	d.clear();
	std::cout << "<要素の全削除>" << std::endl;
	std::cout << "要素数 : " << d.size() << std::endl;
}
