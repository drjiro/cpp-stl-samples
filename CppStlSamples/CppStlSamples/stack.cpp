#include <iostream>
#include <stack>

// 名前空間の指定
using namespace std;

void testStack()
{
	cout << "<< stack >>" << endl;
	// vectorの宣言
	stack<int> s;
	// 要素数の取得
	cout << "要素数 : " << s.size() << endl;

	// 追加
	cout << "<追加>" << endl;
	s.push(10);
	cout << "要素数 : " << s.size() << endl;
	// 最上位要素の参照
	cout << "最上位要素の参照" << endl;
	cout << s.top() << endl;

	// 追加
	cout << "<追加>" << endl;
	s.push(20);
	cout << "要素数 : " << s.size() << endl;
	// 最上位要素の参照
	cout << "最上位要素の参照" << endl;
	cout << s.top() << endl;

	// ここから実習を始める
	// 1.要素の参照エラー
	//cout << s[0] << endl;
	// 2.要素の参照エラー
	//cout << s.at(1) << endl;

	// 削除
	cout << "<削除>" << endl;
	s.pop();
	cout << "要素数 : " << s.size() << endl;
	cout << "最上位要素の参照" << endl;
	cout << s.top() << endl;

	// 削除
	cout << "<削除>" << endl;
	s.pop();
	cout << "要素数 : " << s.size() << endl;
	cout << "最上位要素の参照" << endl;

	// ここから実習を始める
	// 1.要素なしの参照エラー
	//cout << s.top() << endl;
	// 2.要素なしの削除エラー
	//cout << "<削除>" << endl;
	//s.pop();
}

