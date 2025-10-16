#include <iostream>
#include <deque>

// 名前空間の指定
using namespace std;

void testDeque()
{
	cout << "<< deque >>" << endl;
	// vectorの宣言
	deque<int> d = { 10, 20 };
	// 要素数の取得
	cout << "要素数 : " << d.size() << endl;
	// 要素の一覧
	for (int x : d) cout << x << " ";
	cout << endl;
	
	// 末尾に追加
	cout << "<末尾に追加>" << endl;
	d.push_back(30);
	cout << "要素数 : " << d.size() << endl;
	// 要素の一覧
	for (int x : d) cout << x << " ";
	cout << endl;
	
	// 先頭に追加
	cout << "<先頭に追加>" << endl;
	d.push_front(40);
	cout << "要素数 : " << d.size() << endl;
	// 要素の一覧
	for (int x : d) cout << x << " ";
	cout << endl;
	
	// 要素の参照
	cout << "<要素の参照>" << endl;
	cout << d[0] << endl;
	cout << d.at(1) << endl;
	
	// ここから実習を始める
	// 1.要素の参照エラー
	//cout << d[-1] << endl;
	// 2.要素の参照エラー
	//cout << d[4] << endl;
	// 3.要素の参照エラー
	//cout << d.at(-1) << endl;
	// 4.要素の参照エラー
	//cout << d.at(4) << endl;
	
	// 末尾を削除
	cout << "<末尾を削除>" << endl;
	d.pop_back();
	// 要素数の取得
	cout << "要素数 : " << d.size() << endl;
	// 要素の一覧
	for (int x : d) cout << x << " ";
	cout << endl;
	
	// 先頭を削除
	cout << "<先頭を削除>" << endl;
	d.pop_front();
	// 要素数の取得
	cout << "要素数 : " << d.size() << endl;
	// 要素の一覧
	for (int x : d) cout << x << " ";
	cout << endl;
	
	// 全削除
	d.clear();
	cout << "<要素の全削除>" << endl;
	cout << "要素数 : " << d.size() << endl;
}

