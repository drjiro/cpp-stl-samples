#include <iostream>
#include <string>
#include <queue>
#include <map>
#include <algorithm>
#include <vector>
#include <climits> // INT_MAX

// Œ^’ZkiC++03 ‚Å‚Í using ‚ª‚È‚¢‚Ì‚Å typedefj
typedef std::pair<int, std::string> P;

std::pair<std::vector<std::string>, int> dijkstra(
    const std::string& start, const std::string& goal,
    const std::map<std::string, std::map<std::string, int> >& graph)
{
    std::map<std::string, int> dist;
    std::map<std::string, std::string> prev;

    // ‰Šú‰»
    std::map<std::string, std::map<std::string, int> >::const_iterator it;
    for (it = graph.begin(); it != graph.end(); ++it) {
        dist[it->first] = INT_MAX;
    }
    dist[start] = 0;

    // priority_queue (¬‚³‚¢‡‚É‚·‚é‚½‚ß greater ‚ğw’è)
    std::priority_queue<
        P, std::vector<P>, std::greater<P>
    > pq;

    pq.push(P(0, start));

    while (!pq.empty()) {
        P top = pq.top(); pq.pop();
        int cost = top.first;
        std::string node = top.second;

        if (node == goal) break;
        if (cost > dist[node]) continue;

        // —×Úƒm[ƒh’Tõ
        const std::map<std::string, int>& edges = graph.find(node)->second;
        std::map<std::string, int>::const_iterator it2;

        for (it2 = edges.begin(); it2 != edges.end(); ++it2) {
            std::string nxt = it2->first;
            int w = it2->second;
            int new_cost = cost + w;

            if (new_cost < dist[nxt]) {
                dist[nxt] = new_cost;
                prev[nxt] = node;
                pq.push(P(new_cost, nxt));
            }
        }
    }

    // Œo˜H•œŒ³
    std::vector<std::string> path;
    std::string cur = goal;
    while (cur != "") {
        path.push_back(cur);
        if (prev.find(cur) == prev.end()) break;
        cur = prev[cur];
    }
    std::reverse(path.begin(), path.end());

    return std::make_pair(path, dist[goal]);
}


// ƒeƒXƒg
void testDijkstra() {
    std::map<std::string, std::map<std::string, int> > graph;

    graph["A"]["B"] = 2;
    graph["A"]["C"] = 5;
    graph["B"]["D"] = 1;
    graph["C"]["D"] = 2;
    graph["D"]["E"] = 3;
    graph["E"]; // o•Ó‚È‚µƒm[ƒh‚ğ“o˜^(bug fix)

    std::pair<std::vector<std::string>, int> result =
        dijkstra("A", "E", graph);

    const std::vector<std::string>& path = result.first;
    int cost = result.second;

    for (size_t i = 0; i < path.size(); ++i) {
        std::cout << path[i] << " ";
    }
    std::cout << " cost=" << cost << std::endl;
}