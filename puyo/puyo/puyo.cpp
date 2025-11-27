// puyo.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <iostream>
#include <vector>

const int H = 12;
const int W = 6;

// マスデータ
int board[H][W] = {
    {1, 2, 3, 3, 3, 1},
    {1, 2, 2, 3, 2, 1},
    {1, 1, 2, 4, 4, 1},
    {0, 0, 0, 4, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
};

// すでに探索したマスを記録
bool visited[H][W];

// 探索用の方向（上下左右）
int dx[4] = { 1, -1, 0, 0 };
int dy[4] = { 0, 0, 1, -1 };

// 同色の連結した部分を DFS で探索し、座標を group に追加する
void dfs(int x, int y, int color, std::vector<std::pair<int, int>>& group) {
	// 既に訪問済みにする
    visited[y][x] = true;
    // グループデータに位置を(row, col) で保存
    group.push_back({ y, x });
    for (int i = 0; i < 4; i++) {
		// 隣接マスの座標
        int nx = x + dx[i];
        int ny = y + dy[i];
        // 範囲外チェック
        if (nx < 0 || nx >= W || ny < 0 || ny >= H) {
            continue;
        }
        // 色が違う or 既に訪問済みならスキップ
        if (visited[ny][nx] || board[ny][nx] != color) {
            continue;
        }
        dfs(nx, ny, color, group);
    }
}

int main() {
    // visitedを初期化
    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            visited[y][x] = false;
        }
    }

    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            // 空は無視
            if (board[y][x] == 0) {
                continue;
            }
            // すでに調べたら無視
            if (visited[y][x]) {
                continue;
            }

			// 新しいグループを探索
            std::vector<std::pair<int, int>> group;
            dfs(x, y, board[y][x], group);

            // 同じ色の連結グループが group に入る
            std::cout << "Color " << board[y][x]
                << " group size = " << group.size() << "\n";

            // 4つ以上なら「消える」などの判定ができる
            if (group.size() >= 4) {
                std::cout << " -> 消える!\n";
                // 実際に消す処理もここに書ける
                for (auto& p : group) {
                    // 消す
                    board[p.first][p.second] = 0;
				}
            }
        }
    }
	// 結果表示
    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            std::cout << board[y][x] << " ";
        }
        std::cout << "\n";
	}
}