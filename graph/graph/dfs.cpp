//
// DFS（深さ優先探索）
//
#include <iostream>
#include <string>
#include <stack>
#include <map>
#include <algorithm>
#include <vector>

std::map<std::string, std::vector<std::string>> graph = {
    {"A", {"B", "C"}},
    {"B", {"D"}},
    {"C", {"E", "F"}},
    {"D", {"G", "H"}},
    {"E", {"I", "J"}},
    {"F", {"K", "L"}},
    {"G", {}}, {"H", {}},
    {"I", {}}, {"J", {}},
    {"K", {}}, {"L", {}}
};

std::vector<std::string> dfs_with_steps(
    const std::string& start,
    const std::string& goal,
    const std::map<std::string, std::vector<std::string>>& graph) {

    std::stack<std::string> st;
    std::map<std::string, std::string> prev;

    std::cout << "Step 1: push start node\n";
    st.push(start);
    prev[start] = "";

    int step = 1;

    while (!st.empty()) {
        std::cout << "\n--- Step " << ++step << " ---\n";

        // スタックの中身表示
        std::cout << "Stack (top -> bottom): ";
        std::stack<std::string> temp = st;
        while (!temp.empty()) {
            std::cout << temp.top() << " ";
            temp.pop();
        }
        std::cout << "\n";

        // pop
        std::string node = st.top();
        st.pop();
        std::cout << "Pop : " << node << "\n";

        if (node == goal) {
            std::cout << "Goal found!\n";
            break;
        }

        // 子ノードを push
        for (auto& nxt : graph.at(node)) {
            if (!prev.count(nxt)) {
                prev[nxt] = node;
                std::cout << "  Push : " << nxt
                    << " (prev[" << nxt << "] = " << node << ")\n";
                st.push(nxt);
            }
        }
    }

    // 経路復元
    std::vector<std::string> path;
    for (std::string cur = goal; cur != ""; cur = prev[cur])
        path.push_back(cur);
    std::reverse(path.begin(), path.end());

    return path;
}

void testDfs() {
    auto path = dfs_with_steps("A", "J", graph);

    std::cout << "\n=== Result Path ===\n";
    for (auto& n : path)
        std::cout << n << " ";
    std::cout << std::endl;
}


