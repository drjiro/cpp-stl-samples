//
// BFS（幅優先探索）
//
#include <iostream>
#include <string>
#include <queue>
#include <map>
#include <algorithm>
#include <vector>

struct Node {
    int value;
    Node* left;
    Node* right;

    Node(int v) : value(v), left(nullptr), right(nullptr) {}
};

Node* buildTree() {
    Node* root = new Node(5);

    root->left = new Node(12);
    root->right = new Node(13);

    root->left->left = new Node(7);

    root->right->left = new Node(14);
    root->right->right = new Node(2);

    root->left->left->left = new Node(17);
    root->left->left->right = new Node(23);

    root->right->left->left = new Node(27);
    root->right->left->right = new Node(3);

    root->right->right->left = new Node(8);
    root->right->right->right = new Node(11);

    return root;
}

void bfs(Node* root) {
    std::queue<Node*> q;

    std::cout << "Step 1: ルートをキューに追加\n";
    q.push(root);

    int step = 1;

    while (!q.empty()) {
        std::cout << "\n--- Step " << ++step << " ---\n";

        // キューの中身表示
        std::cout << "queue: ";
        std::queue<Node*> temp = q;
        while (!temp.empty()) {
            std::cout << temp.front()->value << " ";
            temp.pop();
        }
        std::cout << std::endl;

        // ノード取り出し
        Node* current = q.front();
        q.pop();
        std::cout << "Dequeue: " << current->value << std::endl;
        std::cout << "Output : " << current->value << std::endl;

        // 左の子
        if (current->left) {
            std::cout << "  Enqueue left child: " << current->left->value << std::endl;
            q.push(current->left);
        }

        // 右の子
        if (current->right) {
            std::cout << "  Enqueue right child: " << current->right->value << std::endl;
            q.push(current->right);
        }
    }
}

void testBfs() {
    std::cout << "BFSのテスト" << std::endl;

    Node* root = buildTree();
    bfs(root);
}
