#include <iostream>
#include <queue>

// 名前空間の指定
using namespace std;

void testQueue()
{
	cout << "<< queue >>" << endl;
	// vectorの宣言
	queue<int> q;
	// 要素数の取得
	cout << "要素数 : " << q.size() << endl;

	// 追加
	cout << "<追加>" << endl;
	q.push(10);
	cout << "要素数 : " << q.size() << endl;
	// トップ要素の参照
	cout << "トップ要素の参照" << q.front() << endl;
	// 最後要素の参照
	cout << "最後要素の参照" << q.back() << endl;

	// 追加
	cout << "<追加>" << endl;
	q.push(20);
	cout << "要素数 : " << q.size() << endl;
	// トップ要素の参照
	cout << "トップ要素の参照" << q.front() << endl;
	// 最後要素の参照
	cout << "最後要素の参照" << q.back() << endl;

	// ここから実習を始める
	// 1.要素の参照エラー
	//cout << q[0] << endl;
	// 2.要素の参照エラー
	//cout << q.at(1) << endl;

	// 削除
	cout << "<削除>" << endl;
	q.pop();
	cout << "要素数 : " << q.size() << endl;
	// トップ要素の参照
	cout << "トップ要素の参照" << q.front() << endl;
	// 最後要素の参照
	cout << "最後要素の参照" << q.back() << endl;

	// 削除
	cout << "<削除>" << endl;
	q.pop();
	cout << "要素数 : " << q.size() << endl;

	// ここから実習を始める
	// 1,トップ要素の参照エラー
	//cout << "トップ要素の参照" << q.front() << endl;
	// 2.最後要素の参照エラー
	//cout << "最後要素の参照" << q.back() << endl;
}

