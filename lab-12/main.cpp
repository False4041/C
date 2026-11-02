#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <random>
#include <chrono>
#include <functional>
using namespace std;
const long long INF = 1000000000000LL;
using Matrix = vector<vector<long long>>;
using Adj = vector<vector<pair<int, long long>>>;
Adj adjacency(const Matrix &a) {
    int n = int(a.size());
    Adj g(n);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            if (i != j && a[i][j] < INF)
                g[i].push_back({j, a[i][j]});
    return g;
}
vector<long long> dijkstra(const Adj &g, int s) {
    int n = int(g.size());
    vector<long long> d(n, INF);
    priority_queue<pair<long long, int>, vector<pair<long long, int>>,
                   greater<pair<long long, int>>>
        q;
    d[s] = 0;
    q.push({0, s});
    while (!q.empty()) {
        auto [dist, v] = q.top();
        q.pop();
        if (dist != d[v])
            continue;
        for (auto [u, w] : g[v])
            if (d[u] > dist + w) {
                d[u] = dist + w;
                q.push({d[u], u});
            }
    }
    return d;
}
Matrix floyd(Matrix d) {
    int n = int(d.size());
    for (int k = 0; k < n; ++k)
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                if (d[i][k] < INF && d[k][j] < INF)
                    d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
    return d;
}
void paths(int v, int s, const vector<vector<int>> &p, vector<int> &path, vector<bool> &seen) {
    if (seen[v])
        return;
    seen[v] = true;
    path.push_back(v);
    if (v == s) {
        for (auto i = path.rbegin(); i != path.rend(); ++i)
            cout << *i << ' ';
        cout << '\n';
    } else
        for (int u : p[v])
            paths(u, s, p, path, seen);
    path.pop_back();
    seen[v] = false;
}
void allPaths(const Matrix &a, int s, int t) {
    auto d = dijkstra(adjacency(a), s);
    if (d[t] == INF) {
        cout << "No path\n";
        return;
    }
    vector<vector<int>> p(a.size());
    for (int u = 0; u < int(a.size()); ++u)
        for (int v = 0; v < int(a.size()); ++v)
            if (u != v && d[u] < INF && a[u][v] < INF && d[u] + a[u][v] == d[v])
                p[v].push_back(u);
    vector<int> path;
    vector<bool> seen(a.size());
    paths(t, s, p, path, seen);
}
int main() {
    Matrix a(4, vector<long long>(4, INF));
    for (int i = 0; i < 4; ++i)
        a[i][i] = 0;
    a[0][1] = a[0][2] = a[1][3] = a[2][3] = 1;
    cout << "Paths\n";
    allPaths(a, 0, 3);
    cout << "Distances\n";
    for (auto &r : floyd(a)) {
        for (auto x : r) {
            if (x == INF)
                cout << "INF ";
            else
                cout << x << ' ';
        }
        cout << '\n';
    }
    mt19937 gen(12);
    cout << "n source dijkstra_us floyd_us equal\n";
    for (int n : {10, 20, 50, 100}) {
        Matrix g(n, vector<long long>(n, INF));
        for (int i = 0; i < n; ++i) {
            g[i][i] = 0;
            for (int j = 0; j < n; ++j)
                if (i != j && gen() % 4 == 0)
                    g[i][j] = 1 + gen() % 20;
        }
        int s = int(gen() % n);
        auto list = adjacency(g);
        auto start = chrono::steady_clock::now();
        auto d = dijkstra(list, s);
        auto mid = chrono::steady_clock::now();
        auto f = floyd(g);
        auto end = chrono::steady_clock::now();
        if (d != f[s])
            return 1;
        cout << n << ' ' << s << ' '
             << chrono::duration_cast<chrono::microseconds>(mid - start).count() << ' '
             << chrono::duration_cast<chrono::microseconds>(end - mid).count() << ' ' << (d == f[s])
             << '\n';
    }
}
