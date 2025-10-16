#include <iostream>
#include <list>

// 名前空間の指定
using namespace std;

void testList()
{
	cout << "<< list >>" << endl;
	// vectorの宣言
	list<int> list1;
	// 要素数の取得
	cout << "要素数 : " << list1.size() << endl;

	// 末尾に追加
	cout << "<末尾に追加>" << endl;
	list1.push_back(10);
	cout << "要素数 : " << list1.size() << endl;
	// 要素の一覧
	cout << "<要素の一覧>" << endl;
	for (int x : list1) cout << x << " ";
	cout << endl;

	// 末尾に追加
	cout << "<末尾に追加>" << endl;
	list1.push_back(20);
	cout << "要素数 : " << list1.size() << endl;
	// 要素の一覧
	for (int x : list1) cout << x << " ";
	cout << endl;

	// 先頭に追加
	cout << "<先頭に追加>" << endl;
	list1.push_front(30);
	cout << "要素数 : " << list1.size() << endl;
	// 要素の一覧
	for (int x : list1) cout << x << " ";
	cout << endl;

	// 指定位置挿入
	cout << "<指定位置に追加>" << endl;
	auto it = list1.begin();  // 先頭を指すイテレータ
	++it;                    // 2番目（10）を指す
	list1.insert(it, 40);
	cout << "要素数 : " << list1.size() << endl;
	// 要素の一覧
	for (int x : list1) cout << x << " ";
	cout << endl;
	// イテレータによる要素の一覧
	// list<int>::iterator it は auto it と同じ
	for (list<int>::iterator it2 = list1.begin(); it2 != list1.end(); ++it2) {
		cout << *it2 << " ";
	}
	cout << endl;

	// 指定位置削除
	cout << "<指定位置削除>" << endl;
	list1.erase(it);
	cout << "要素数 : " << list1.size() << endl;
	// 要素の一覧
	for (int x : list1) cout << x << " ";
	cout << endl;

	cout << "<要素の参照>" << endl;
	cout << "先頭: " << list1.front() << endl;
	cout << "末尾: " << list1.back() << endl;

	// ここから実習を始める
	// 1.要素の参照エラー（指しているデータは削除済み）
	//cout << *it << endl;
	// 2.次の要素を指すエラー（指しているデータは削除済み）
	//++it;
	// 3.要素の参照エラー
	//cout << list[0] << endl;
	// 4.要素の参照エラー
	//cout << list.at(1) << endl;

	// 末尾削除
	cout << "<末尾削除>" << endl;
	list1.pop_back();
	// 要素数の取得
	cout << "要素数 : " << list1.size() << endl;
	// 要素の一覧
	for (int x : list1) cout << x << " ";
	cout << endl;

	// 先頭削除
	cout << "<先頭削除>" << endl;
	list1.pop_front();
	// 要素数の取得
	cout << "要素数 : " << list1.size() << endl;
	// 要素の一覧
	for (int x : list1) cout << x << " ";
	cout << endl;

	// 全削除
	list1.clear();
	cout << "<要素の全削除>" << endl;
	cout << "要素数 : " << list1.size() << endl;
}


