#include <iostream>
#include <vector>

// 名前空間の指定
using namespace std;

void testVector()
{
	cout << "<< vector >>" << endl;
	// vectorの宣言
	vector<int> v = { 10, 20, 30 };
	// 要素数の取得
	cout << "要素数 : " << v.size() << endl;
	// 要素の一覧
	for (int x : v) cout << x << " ";
	cout << endl;

	// 要素の追加
	cout << "<要素の追加>" << endl;
	v.push_back(40);
	cout << "要素数 : " << v.size() << endl;
	// 要素の一覧
	for (int x : v) cout << x << " ";
	cout << endl;

	// 要素の参照
	cout << "<要素の参照>" << endl;
	cout << v[0] << endl;
	cout << v.at(1) << endl;

	// ここから実習を始める
	// 1.要素の参照エラー
	//cout << v[-1] << endl;
	// 2.要素の参照エラー
	//cout << v[4] << endl;
	// 3.要素の参照エラー
	//cout << v.at(-1) << endl;
	// 4.要素の参照エラー
	//cout << v.at(4) << endl;

	// 要素の削除
	cout << "<要素の削除>" << endl;
	v.pop_back();
	// 要素数の取得
	cout << "要素数 : " << v.size() << endl;
	// 要素の一覧
	for (int x : v) cout << x << " ";
	cout << endl;

	// 全削除
	v.clear();
	cout << "<要素の全削除>" << endl;
	cout << "要素数 : " << v.size() << endl;

	cout << "<入れられる要素数の確認>" << endl;
	for (int i = 0; i < 10; i++)
	{
		v.push_back(i);
		// 要素数の取得
		cout << "要素数 : " << v.size() << endl;
		// 入れられる要素数
		cout << "入れられる要素数 : " << v.capacity() << endl;
	}
	cout << "<入れられる要素数の変更>" << endl;
	//v.reserve(10);
	v.reserve(100);
	cout << "入れられる要素数 : " << v.capacity() << endl;
}
