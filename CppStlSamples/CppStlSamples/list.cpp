#include <iostream>
#include <list>

// 名前空間の指定
//using namespace std;

void testList()
{
	std::cout << "<< list >>" << std::endl;
	// vectorの宣言
	std::list<int> list1;
	// 要素数の取得
	std::cout << "要素数 : " << list1.size() << std::endl;

	// 末尾に追加
	std::cout << "<末尾に追加>" << std::endl;
	list1.push_back(10);
	std::cout << "要素数 : " << list1.size() << std::endl;
	// 要素の一覧
	std::cout << "<要素の一覧>" << std::endl;
	for (int x : list1) std::cout << x << " ";
	std::cout << std::endl;

	// 末尾に追加
	std::cout << "<末尾に追加>" << std::endl;
	list1.push_back(20);
	std::cout << "要素数 : " << list1.size() << std::endl;
	// 要素の一覧
	for (int x : list1) std::cout << x << " ";
	std::cout << std::endl;

	// 先頭に追加
	std::cout << "<先頭に追加>" << std::endl;
	list1.push_front(30);
	std::cout << "要素数 : " << list1.size() << std::endl;
	// 要素の一覧
	for (int x : list1) std::cout << x << " ";
	std::cout << std::endl;

	// 指定位置挿入
	std::cout << "<指定位置に追加>" << std::endl;
	auto it = list1.begin();  // 先頭を指すイテレータ
	++it;                    // 2番目（10）を指す
	list1.insert(it, 40);
	std::cout << "要素数 : " << list1.size() << std::endl;
	// 要素の一覧
	for (int x : list1) std::cout << x << " ";
	std::cout << std::endl;
	// イテレータによる要素の一覧
	// list<int>::iterator it は auto it と同じ
	for (std::list<int>::iterator it2 = list1.begin(); it2 != list1.end(); ++it2) {
		std::cout << *it2 << " ";
	}
	std::cout << std::endl;

	// 指定位置削除
	std::cout << "<指定位置削除>" << std::endl;
	list1.erase(it);
	std::cout << "要素数 : " << list1.size() << std::endl;
	// 要素の一覧
	for (int x : list1) std::cout << x << " ";
	std::cout << std::endl;

	std::cout << "<要素の参照>" << std::endl;
	std::cout << "先頭: " << list1.front() << std::endl;
	std::cout << "末尾: " << list1.back() << std::endl;

	// ここから実習を始める
	// 1.要素の参照エラー（指しているデータは削除済み）
	//std::cout << *it << std::endl;
	// 2.次の要素を指すエラー（指しているデータは削除済み）
	//++it;
	// 3.要素の参照エラー
	//std::cout << list[0] << std::endl;
	// 4.要素の参照エラー
	//std::cout << list.at(1) << std::endl;

	// 末尾削除
	std::cout << "<末尾削除>" << std::endl;
	list1.pop_back();
	// 要素数の取得
	std::cout << "要素数 : " << list1.size() << std::endl;
	// 要素の一覧
	for (int x : list1) std::cout << x << " ";
	std::cout << std::endl;

	// 先頭削除
	std::cout << "<先頭削除>" << std::endl;
	list1.pop_front();
	// 要素数の取得
	std::cout << "要素数 : " << list1.size() << std::endl;
	// 要素の一覧
	for (int x : list1) std::cout << x << " ";
	std::cout << std::endl;

	// 全削除
	list1.clear();
	std::cout << "<要素の全削除>" << std::endl;
	std::cout << "要素数 : " << list1.size() << std::endl;
}


