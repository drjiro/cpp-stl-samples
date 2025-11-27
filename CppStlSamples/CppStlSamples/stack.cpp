#include <iostream>
#include <stack>

// 名前空間の指定
//using namespace std;

void testStack()
{
	std::cout << "<< stack >>" << std::endl;
	// vectorの宣言
	std::stack<int> s;
	// 要素数の取得
	std::cout << "要素数 : " << s.size() << std::endl;

	// 追加
	std::cout << "<追加>" << std::endl;
	s.push(10);
	std::cout << "要素数 : " << s.size() << std::endl;
	// 最上位要素の参照
	std::cout << "最上位要素の参照" << std::endl;
	std::cout << s.top() << std::endl;

	// 追加
	std::cout << "<追加>" << std::endl;
	s.push(20);
	std::cout << "要素数 : " << s.size() << std::endl;
	// 最上位要素の参照
	std::cout << "最上位要素の参照" << std::endl;
	std::cout << s.top() << std::endl;

	// ここから実習を始める
	// 1.要素の参照エラー
	//std::cout << s[0] << std::endl;
	// 2.要素の参照エラー
	//std::cout << s.at(1) << std::endl;

	// 削除
	std::cout << "<削除>" << std::endl;
	s.pop();
	std::cout << "要素数 : " << s.size() << std::endl;
	std::cout << "最上位要素の参照" << std::endl;
	std::cout << s.top() << std::endl;

	// 削除
	std::cout << "<削除>" << std::endl;
	s.pop();
	std::cout << "要素数 : " << s.size() << std::endl;
	std::cout << "最上位要素の参照" << std::endl;

	// ここから実習を始める
	// 1.要素なしの参照エラー
	//std::cout << s.top() << std::endl;
	// 2.要素なしの削除エラー
	//std::cout << "<削除>" << std::endl;
	//s.pop();
}

