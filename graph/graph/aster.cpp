#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <climits>
#include <algorithm>
#include <cmath>

//#define MANHATTAN
#define EUCLID
//#define CHEBYSHEV

// ヒューリスティック関数（マンハッタン距離）
int heuristic_manhattan(std::pair<int, int> a, std::pair<int, int> b) {
    return std::abs(a.first - b.first) + std::abs(a.second - b.second);
}

// ヒューリスティック関数（ユークリッド距離）
int heuristic_euclid_sq(std::pair<int, int> a,
    std::pair<int, int> b) {
    int dx = a.first - b.first;
    int dy = a.second - b.second;
    return dx * dx + dy * dy;
}

// ヒューリスティック関数（チェビシェフ距離）
int heuristic_chebyshev(std::pair<int, int> a,
    std::pair<int, int> b) {
    int dx = std::abs(a.first - b.first);
    int dy = std::abs(a.second - b.second);
    return std::max(dx, dy);
}

// 座標を見やすく出す
void printPos(const std::pair<int, int>& p) {
    std::cout << "(" << p.first << "," << p.second << ")";
}

// grid を表示（S,G,壁,経路など）
void printGrid(const std::vector<std::vector<int> >& grid,
    const std::pair<int, int>& start,
    const std::pair<int, int>& goal,
    const std::vector<std::vector<int>>& closed,
    const std::vector<std::pair<int, int> >& path) {
	// グリッドサイズ
    int H = (int)grid.size();
    int W = (int)grid[0].size();

    // 経路判定用（簡単にするため map を使う）
    std::map<std::pair<int, int>, bool> inPath;
    for (size_t i = 0; i < path.size(); ++i) {
        inPath[path[i]] = true;
    }

    std::cout << "\n=== Final Map ===\n";
    for (int x = 0; x < H; ++x) {
        for (int y = 0; y < W; ++y) {
            std::pair<int, int> p = std::make_pair(x, y);

            char c = ' ';
            if (grid[x][y] == 1) c = '#';   // 壁
			else if (closed[x][y] == 1) c = 'o'; // 探索済み    
            else c = '.';

            if (inPath.count(p)) c = '*';
            if (p == start) c = 'S';
            if (p == goal)  c = 'G';
            std::cout << c << " ";
        }
        std::cout << "\n";
    }
}

std::vector<std::pair<int, int> > astar(
    std::pair<int, int> start,
    std::pair<int, int> goal,
    const std::vector<std::vector<int> >& grid,
    std::vector<std::vector<int> >& closed
) {
    // グリッドサイズ
    int H = (int)grid.size();
    int W = (int)grid[0].size();

	// 探索済みリスト
    closed.assign(H, std::vector<int>(W, 0));

	// 各ノードの最小コスト g(n)
    std::vector<std::vector<int> > cost(H, std::vector<int>(W, INT_MAX));
	// 経路復元用
    std::map<std::pair<int, int>, std::pair<int, int> > prev;

	// 開始ノードのコストは 0
    cost[start.first][start.second] = 0;

    // pq 要素: (f, (x,y))
    typedef std::pair<int, std::pair<int, int> > P;
    std::priority_queue<P, std::vector<P>, std::greater<P> > pq;

    // 最初は f = 0 + h(start)
#if defined(MANHATTAN)
	int f0 = heuristic_manhattan(start, goal);
#elif defined(EUCLID)
	int f0 = heuristic_euclid_sq(start, goal);
#elif defined(CHEBYSHEV)
	int f0 = heuristic_chebyshev(start, goal);
#endif
    pq.push(P(f0, start));

    // 4方向移動
    std::vector<std::pair<int, int> > moves;
    moves.push_back(std::make_pair(1, 0));
    moves.push_back(std::make_pair(-1, 0));
    moves.push_back(std::make_pair(0, 1));
    moves.push_back(std::make_pair(0, -1));

    int step = 0;

    std::cout << "A* start: "; printPos(start);
    std::cout << " goal: ";   printPos(goal);
    std::cout << "\n";
    std::cout << "Initial push: f=" << f0 << " node="; printPos(start);
#if defined(MANHATTAN)
    std::cout << " (g=0, h=" << heuristic_manhattan(start, goal) << ")\n";
#elif defined(EUCLID)
    std::cout << " (g=0, h=" << heuristic_euclid_sq(start, goal) << ")\n";
#elif defined(CHEBYSHEV)
    std::cout << " (g=0, h=" << heuristic_chebyshev(start, goal) << ")\n";
#endif

    while (!pq.empty()) {
		// pq から最小 f のノードを取り出す
        P top = pq.top();
        pq.pop();

        int f = top.first;
        std::pair<int, int> node = top.second;
		// 探索済みリストに追加
        closed[node.first][node.second] = 1;

		// 現在の g(n)
        int g = cost[node.first][node.second];

        // 探索済み（展開済み）として記録
        closed[node.first][node.second] = 1;

		// 現在の h(n)
#if defined(MANHATTAN)
        int h = heuristic_manhattan(node, goal);
#elif defined(EUCLID)
        int h = heuristic_euclid_sq(node, goal);
#elif defined(CHEBYSHEV)
        int h = heuristic_chebyshev(node, goal);
#endif

        std::cout << "\n--- Step " << (++step) << " ---\n";
        std::cout << "Pop  : node="; printPos(node);
        std::cout << "  g=" << g << "  h=" << h << "  f=" << f << "\n";

        // pq には古い候補も残るので、f と g が整合しない場合がある
        // g が INF ならそもそもおかしいのでスキップ
        if (g == INT_MAX) {
            std::cout << "  (skip: g is INF)\n";
            continue;
        }
        // f が g+h と違っても「古い候補」かもしれない。
        if (f != g + h) {
            std::cout << "  note: this entry may be outdated (f != g+h)\n";
        }

        if (node == goal) {
            std::cout << "Goal reached!\n";
            break;
        }

        // 近傍を見る
        for (size_t i = 0; i < moves.size(); ++i) {
			// 近傍ノード座標
            int nx = node.first + moves[i].first;
            int ny = node.second + moves[i].second;

			// 近傍ノード
            std::pair<int, int> nxt = std::make_pair(nx, ny);

            std::cout << "  Check neighbor "; printPos(nxt); std::cout << " : ";

			// 範囲外チェック
            if (nx < 0 || ny < 0 || nx >= H || ny >= W) {
                std::cout << "out of range\n";
                continue;
            }
			// 壁チェック
            if (grid[nx][ny] == 1) {
                std::cout << "wall (#)\n";
                continue;
            }

            int new_cost = g + 1; // 1歩コスト
			// コスト更新判定
            if (new_cost < cost[nx][ny]) {
                cost[nx][ny] = new_cost;
                prev[nxt] = node;

				// 新しい f(n) を計算して pq に追加
#if defined(MANHATTAN)
                int hn = heuristic_manhattan(nxt, goal);
#elif defined(EUCLID)
                int hn = heuristic_euclid_sq(nxt, goal);
#elif defined(CHEBYSHEV)
                int hn = heuristic_chebyshev(nxt, goal);
#endif

                int fn = new_cost + hn;

                std::cout << "UPDATE! new g=" << new_cost
                    << " h=" << hn << " f=" << fn
                    << "  -> push\n";

                pq.push(P(fn, nxt));
            }
            else {
                std::cout << "no update (current best g=" << cost[nx][ny] << ")\n";
            }
        }
    }

    // 経路復元（到達できない場合の保護も追加）
    std::vector<std::pair<int, int> > path;
    if (start == goal) {
        path.push_back(start);
        return path;
    }

    if (!prev.count(goal)) {
        std::cout << "\nNo path found.\n";
        return path; // empty
    }

	// 経路復元
    std::pair<int, int> cur = goal;
    while (!(cur == start)) {
        path.push_back(cur);
        cur = prev[cur];
    }
    path.push_back(start);

	// 逆順にする
    std::reverse(path.begin(), path.end());

	// 結果表示
    std::cout << "\n=== Path ===\n";
    for (size_t i = 0; i < path.size(); ++i) {
        printPos(path[i]);
        std::cout << " ";
    }
    std::cout << "\n";
    return path;
}

void testAster() {
#if defined(MANHATTAN)
    std::cout << "\n=== MANHATTAN ===\n";
#elif defined(EUCLID)
    std::cout << "\n=== EUCLID ===\n";
#elif defined(CHEBYSHEV)
    std::cout << "\n=== CHEBYSHEV ===\n";
#endif

    std::vector<std::vector<int>> grid = {
        {0,0,0,0,0,0,0},
        {0,1,1,1,1,1,0},
        {0,0,0,0,0,1,0},
        {0,1,0,1,0,1,0},
        {0,1,0,1,0,0,0},
        {0,1,0,0,0,1,0},
        {0,0,0,0,0,0,0}
    };

    std::pair<int, int> S = std::make_pair(6, 0);
    std::pair<int, int> G = std::make_pair(0, 6);

    std::vector<std::vector<int>> closed;
    std::vector<std::pair<int, int>> path = astar(S, G, grid, closed);

    // 結果を地図表示
    printGrid(grid, S, G, closed, path);
}
