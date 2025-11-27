// main.cpp
#include <vector>
#include <cstdlib>
#include <ctime>
#include <emscripten/emscripten.h>

const int H = 12;
const int W = 6;
const int COLORS = 4; // 色数（1〜4を使う）

int board[H][W];   // board[y][x]

// 探索用
bool visited[H][W];
int dx[4] = {1, -1, 0, 0};
int dy[4] = {0, 0, 1, -1};

void dfs(int x, int y, int color, std::vector<std::pair<int,int>>& group) {
    visited[y][x] = true;
    group.push_back({y, x});
    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (nx < 0 || nx >= W || ny < 0 || ny >= H) continue;
        if (visited[ny][nx]) continue;
        if (board[ny][nx] != color) continue;
        dfs(nx, ny, color, group);
    }
}

// ぷよを下に落とす（重力）
void applyGravity() {
    for (int x = 0; x < W; x++) {
        int writeY = H - 1;
        for (int y = H - 1; y >= 0; y--) {
            if (board[y][x] != 0) {
                board[writeY][x] = board[y][x];
                if (writeY != y) {
                    board[y][x] = 0;
                }
                writeY--;
            }
        }
        // 残りは空に
        for (int y = writeY; y >= 0; y--) {
            board[y][x] = 0;
        }
    }
}

extern "C" {

// 盤面初期化（ランダム）
EMSCRIPTEN_KEEPALIVE
void initBoard() {
    std::srand((unsigned)std::time(nullptr));
    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
          board[y][x] = 0; // 上部は空にしても良い
        }
    }
}

// JavaScript から盤面を読む用
EMSCRIPTEN_KEEPALIVE
int getCell(int x, int y) {
    if (x < 0 || x >= W || y < 0 || y >= H) return 0;
    return board[y][x];
}

// 1ステップ：同色4個以上のグループを全部消して、重力をかける
// 戻り値：消えた個数（0ならこれ以上消える場所なし）
EMSCRIPTEN_KEEPALIVE
int step() {
    for (int y = 0; y < H; y++)
        for (int x = 0; x < W; x++)
            visited[y][x] = false;

    std::vector<std::pair<int,int>> toErase;
    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            int color = board[y][x];
            if (color == 0 || visited[y][x]) continue;
            std::vector<std::pair<int,int>> group;
            dfs(x, y, color, group);
            if (group.size() >= 4) {
                // 消去対象に追加
                toErase.insert(toErase.end(), group.begin(), group.end());
            }
        }
    }

    if (toErase.empty()) return 0;

    for (auto& p : toErase) {
        int y = p.first;
        int x = p.second;
        board[y][x] = 0;
    }

    applyGravity();

    return (int)toErase.size();
}

// 高さ・幅を JS から参照するための関数
EMSCRIPTEN_KEEPALIVE
int getWidth()  { return W; }

EMSCRIPTEN_KEEPALIVE
int getHeight() { return H; }

} // extern "C"

// ===== 操作ぷよ用 =====
struct ActivePuyo {
    int x, y;     // 基準ぷよ
    int rot;      // 0↑,1→,2↓,3←
    int colorA, colorB;
    bool active;
} cur;

bool canPut(int x, int y) {
    if (x < 0 || x >= W || y < 0 || y >= H) return false;
    return (board[y][x] == 0);
}

void getPuyoPos(int rot, int &dx, int &dy) {
    static int t[4][2] = {{0,-1},{1,0},{0,1},{-1,0}};
    dx = t[rot][0];
    dy = t[rot][1];
}

extern "C" {

// --- 2連ぷよ生成 ---
EMSCRIPTEN_KEEPALIVE
void spawnPuyo() {
    cur.x = W / 2;
    cur.y = 0;
    cur.rot = 2;  // 下向き
    cur.colorA = 1 + std::rand() % COLORS;
    cur.colorB = 1 + std::rand() % COLORS;
    cur.active = true;
}

// --- 下に移動（着地したら false） ---
EMSCRIPTEN_KEEPALIVE
int moveDown() {
    if (!cur.active) return 0;

    int dx, dy;
    getPuyoPos(cur.rot, dx, dy);
    if (canPut(cur.x, cur.y + 1) && canPut(cur.x + dx, cur.y + dy + 1)) {
        cur.y++;
        return 1;
    } else {
        // 着地
        board[cur.y][cur.x] = cur.colorA;
        board[cur.y + dy][cur.x + dx] = cur.colorB;
        cur.active = false;
        return 0;
    }
}

// 左右 & 回転
EMSCRIPTEN_KEEPALIVE
void moveLeft()  { int dx, dy; getPuyoPos(cur.rot, dx, dy); if (canPut(cur.x - 1, cur.y) && canPut(cur.x + dx - 1, cur.y + dy)) cur.x--; }
EMSCRIPTEN_KEEPALIVE
void moveRight() { int dx, dy; getPuyoPos(cur.rot, dx, dy); if (canPut(cur.x + 1, cur.y) && canPut(cur.x + dx + 1, cur.y + dy)) cur.x++; }
EMSCRIPTEN_KEEPALIVE
void rotate()    { int dx, dy; getPuyoPos((cur.rot+1)%4, dx, dy); if (canPut(cur.x + dx, cur.y + dy)) cur.rot = (cur.rot + 1) % 4; }

// JS描画用 getter
EMSCRIPTEN_KEEPALIVE int puyo_get_x() { return cur.x; }
EMSCRIPTEN_KEEPALIVE int puyo_get_y() { return cur.y; }
EMSCRIPTEN_KEEPALIVE int puyo_get_rot() { return cur.rot; }
EMSCRIPTEN_KEEPALIVE int puyo_get_colorA() { return cur.colorA; }
EMSCRIPTEN_KEEPALIVE int puyo_get_colorB() { return cur.colorB; }

} // extern "C"
