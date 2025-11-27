#include <iostream>
#include <queue>

// 名前空間の指定
//using namespace std;

void testQueue()
{
	std::cout << "<< queue >>" << std::endl;
	// vectorの宣言
	std::queue<int> q;
	// 要素数の取得
	std::cout << "要素数 : " << q.size() << std::endl;

	// 追加
	std::cout << "<追加>" << std::endl;
	q.push(10);
	std::cout << "要素数 : " << q.size() << std::endl;
	// トップ要素の参照
	std::cout << "トップ要素の参照" << q.front() << std::endl;
	// 最後要素の参照
	std::cout << "最後要素の参照" << q.back() << std::endl;

	// 追加
	std::cout << "<追加>" << std::endl;
	q.push(20);
	std::cout << "要素数 : " << q.size() << std::endl;
	// トップ要素の参照
	std::cout << "トップ要素の参照" << q.front() << std::endl;
	// 最後要素の参照
	std::cout << "最後要素の参照" << q.back() << std::endl;

	// ここから実習を始める
	// 1.要素の参照エラー
	//std::cout << q[0] << std::endl;
	// 2.要素の参照エラー
	//std::cout << q.at(1) << std::endl;

	// 削除
	std::cout << "<削除>" << std::endl;
	q.pop();
	std::cout << "要素数 : " << q.size() << std::endl;
	// トップ要素の参照
	std::cout << "トップ要素の参照" << q.front() << std::endl;
	// 最後要素の参照
	std::cout << "最後要素の参照" << q.back() << std::endl;

	// 削除
	std::cout << "<削除>" << std::endl;
	q.pop();
	std::cout << "要素数 : " << q.size() << std::endl;

	// ここから実習を始める
	// 1,トップ要素の参照エラー
	//std::cout << "トップ要素の参照" << q.front() << std::endl;
	// 2.最後要素の参照エラー
	//std::cout << "最後要素の参照" << q.back() << std::endl;
}

