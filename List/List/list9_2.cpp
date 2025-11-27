#include <iostream>

// ノード構造体
struct Node {
    int data;      // データ
    Node* next;    // 次ノードへのポインタ
};

// 単方向リストクラス
class List {
private:
    Node* head; // 先頭ノード

public:
    List() : head(nullptr) {}
    ~List() {
        clear();
    }

    // --- 先頭に追加 ---
    void push(int value) {
        Node* node = new Node;
        node->data = value;
        node->next = head;
        head = node;
    }

    // --- 先頭を削除 ---
    void pop() {
        Node* node;
        node = head;
        head = head->next;
        delete node;
    }

    // --- 全削除 ---
    void clear() {
        Node* cur = head;
        while (cur != nullptr) {
            head = cur->next;
            delete cur;
            cur = head;
        }
    }

    // --- 末尾に追加 ---
    void append(int value) {
        // 最後のノードまで移動
        Node* prev = head;
        Node* cur = head;
        while (cur != nullptr) {
            prev = cur;
            cur = cur->next;
        }
        // ノードを作成
        Node* node = new Node;
        node->data = value;
        node->next = nullptr;
        if (prev == nullptr) {
            head = node;
        } else {
            prev->next = node;
        }
    }

    // --- 指定値を削除 ---
    void remove(int value) {
        Node* prev = head;
        Node* cur = head;
        while (cur != nullptr) {
            if (cur->data == value) {
                if (prev == head) {
                    head = cur->next;
                }
                else {
                    prev->next = cur->next;
                }
                delete cur;
                break;
            }
            prev = head;
            cur = cur->next;
        }
    }

    // --- 出力 ---
    void print() const {
        Node* current = head;
        while (current != nullptr) {
            std::cout << current->data << " -> ";
            current = current->next;
        }
        std::cout << "NULL\n";
    }

    // --- 最終ノードから逆に出力 ---
    void printReverseHelper(Node* node) const {
        if (node == nullptr) return;
        printReverseHelper(node->next); // ① 先に後ろへたどる
        std::cout << node->data << " -> "; // ② 戻りながら表示
    }

    void printReverse() const {
        printReverseHelper(head);
        std::cout << "NULL\n";
    }
};

// --- 動作確認 ---
int main() {
    List list;

    std::cout << "先頭に追加\n";
    list.push(10);
    list.push(20);
    list.push(30);
    std::cout << "初期リスト: ";
    list.print();
    std::cout << "逆順リスト: ";
    list.printReverse();

    std::cout << "先頭を削除\n";
    list.pop();
    list.print();

    std::cout << "全削除\n";
    list.clear();
    list.print();

    std::cout << "最後に追加\n";
    list.append(10);
    list.append(20);
    list.append(30);
    list.print();

    std::cout << "指定データ 20 を削除(データあり)\n";
    list.remove(20);
    list.print();
    std::cout << "指定データを削除(先頭を削除)\n";
    list.remove(10);
    list.print();
    std::cout << "指定データを削除(データなし)\n";
    list.remove(100);
    list.print();

    return 0;
}