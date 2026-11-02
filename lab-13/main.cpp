#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <numeric>
#include <functional>
#include <cmath>
#include <chrono>
#include <stdexcept>
using namespace std;
struct Edge {
    int u, v, id;
    double w;
};
struct DSU {
    vector<int> p, r;
    explicit DSU(int n) : p(n), r(n) {
        iota(p.begin(), p.end(), 0);
    }
    int find(int x) {
        return p[x] == x ? x : p[x] = find(p[x]);
    }
    bool join(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b)
            return false;
        if (r[a] < r[b])
            swap(a, b);
        p[b] = a;
        if (r[a] == r[b])
            ++r[a];
        return true;
    }
};
pair<double, vector<int>> kruskal(int n, vector<Edge> e, int banned = -1) {
    sort(e.begin(), e.end(), [](auto &a, auto &b) { return a.w < b.w; });
    DSU d(n);
    double sum = 0;
    vector<int> ids;
    for (auto x : e)
        if (x.id != banned && d.join(x.u, x.v)) {
            sum += x.w;
            ids.push_back(x.id);
        }
    return {sum, ids};
}
pair<double, vector<int>> prim(int n, const vector<Edge> &e) {
    vector<vector<pair<double, int>>> g(n);
    for (auto x : e) {
        g[x.u].push_back({x.w, x.id});
        g[x.v].push_back({x.w, x.id});
    }
    vector<bool> seen(n);
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> q;
    double sum = 0;
    vector<int> ids;
    for (int s = 0; s < n; ++s) {
        if (seen[s])
            continue;
        seen[s] = true;
        for (auto x : g[s])
            q.push(x);
        while (!q.empty()) {
            auto [w, id] = q.top();
            q.pop();
            auto x = e[id];
            if (seen[x.u] && seen[x.v])
                continue;
            int v = seen[x.u] ? x.v : x.u;
            seen[v] = true;
            sum += w;
            ids.push_back(id);
            for (auto y : g[v])
                q.push(y);
        }
    }
    return {sum, ids};
}
int main() {
    try {
        int n, m;
        if (!(cin >> n >> m) || n < 1 || n > 200 || m < 0 || m > 10000)
            throw runtime_error("Size");
        vector<Edge> e;
        for (int i = 0; i < m; ++i) {
            int u, v;
            double w;
            if (!(cin >> u >> v >> w) || u < 0 || v < 0 || u >= n || v >= n || !isfinite(w))
                throw runtime_error("Edge");
            e.push_back({u, v, i, w});
        }
        auto start = chrono::steady_clock::now();
        auto [sum, ids] = kruskal(n, e);
        auto mid = chrono::steady_clock::now();
        auto p = prim(n, e);
        auto end = chrono::steady_clock::now();
        cout << "kruskal_ns prim_ns\n"
             << chrono::duration_cast<chrono::nanoseconds>(mid - start).count() << ' '
             << chrono::duration_cast<chrono::nanoseconds>(end - mid).count() << '\n';
        if (abs(sum - p.first) > 1e-8 * max(1.0, abs(sum)))
            throw runtime_error("Mismatch");
        cout << "Cost " << sum << " Prim " << p.first << '\n';
        for (int id : ids)
            cout << e[id].u << ' ' << e[id].v << ' ' << e[id].w << '\n';
        if (int(ids.size()) != n - 1) {
            cout << "Disconnected\n";
            DSU d(n);
            for (auto x : e)
                d.join(x.u, x.v);
            for (int r = 0; r < n; ++r)
                if (d.find(r) == r) {
                    for (int i = 0; i < n; ++i)
                        if (d.find(i) == r)
                            cout << i << ' ';
                    cout << '\n';
                }
            return 0;
        }
        bool found = false;
        double second = 0;
        vector<int> alt;
        for (int id : ids) {
            auto result = kruskal(n, e, id);
            if (int(result.second.size()) == n - 1 && (!found || result.first < second)) {
                found = true;
                second = result.first;
                alt = result.second;
            }
        }
        if (found) {
            cout << "Second " << second << '\n';
            for (int id : alt)
                cout << e[id].u << ' ' << e[id].v << ' ' << e[id].w << '\n';
        } else
            cout << "No second tree\n";
    } catch (const exception &e) {
        cerr << e.what() << '\n';
        return 1;
    }
}
