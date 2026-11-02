#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include <stdexcept>
using namespace std;
class Graph {
    vector<vector<int>> a;
    void check(int v) const {
        if (v < 0 || v >= int(a.size()))
            throw out_of_range("Vertex");
    }
    void dfs(int v, vector<bool> &seen) const {
        seen[v] = true;
        cout << v << ' ';
        for (int u = 0; u < int(a.size()); ++u)
            if (a[v][u] && !seen[u])
                dfs(u, seen);
    }

  public:
    explicit Graph(int n) : a(n, vector<int>(n)) {}
    void add(int u, int v) {
        check(u);
        check(v);
        a[u][v] = 1;
    }
    void remove(int u, int v) {
        check(u);
        check(v);
        a[u][v] = 0;
    }
    void print() const {
        for (auto &r : a) {
            for (int x : r)
                cout << x << ' ';
            cout << '\n';
        }
    }
    vector<int> bfs(int s, vector<int> &parent, bool show = false) const {
        check(s);
        vector<int> d(a.size(), -1);
        parent.assign(a.size(), -1);
        queue<int> q;
        q.push(s);
        d[s] = 0;
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            if (show)
                cout << v << ' ';
            for (int u = 0; u < int(a.size()); ++u)
                if (a[v][u] && d[u] < 0) {
                    d[u] = d[v] + 1;
                    parent[u] = v;
                    q.push(u);
                }
        }
        if (show)
            cout << '\n';
        return d;
    }
    void dfs(int s) const {
        check(s);
        vector<bool> seen(a.size());
        dfs(s, seen);
        cout << '\n';
    }
    void iterative(int s) const {
        check(s);
        vector<bool> seen(a.size());
        stack<int> q;
        q.push(s);
        while (!q.empty()) {
            int v = q.top();
            q.pop();
            if (seen[v])
                continue;
            seen[v] = true;
            cout << v << ' ';
            for (int u = int(a.size()) - 1; u >= 0; --u)
                if (a[v][u] && !seen[u])
                    q.push(u);
        }
        cout << '\n';
    }
    void components() const {
        vector<bool> seen(a.size());
        for (int s = 0; s < int(a.size()); ++s)
            if (!seen[s]) {
                queue<int> q;
                q.push(s);
                seen[s] = true;
                while (!q.empty()) {
                    int v = q.front();
                    q.pop();
                    cout << v << ' ';
                    for (int u = 0; u < int(a.size()); ++u)
                        if ((a[v][u] || a[u][v]) && !seen[u]) {
                            seen[u] = true;
                            q.push(u);
                        }
                }
                cout << '\n';
            }
    }
    void path(int s, int t) const {
        check(t);
        vector<int> p;
        auto d = bfs(s, p);
        if (d[t] < 0) {
            cout << "No path\n";
            return;
        }
        vector<int> v;
        for (int u = t; u != -1; u = p[u])
            v.push_back(u);
        reverse(v.begin(), v.end());
        for (int u : v)
            cout << u << ' ';
        cout << '\n';
    }
    void diameter() const {
        if (a.empty()) {
            cout << "Empty\n";
            return;
        }
        vector<int> p;
        auto d = bfs(0, p);
        int v = int(max_element(d.begin(), d.end()) - d.begin());
        d = bfs(v, p);
        cout << "Two BFS " << *max_element(d.begin(), d.end()) << '\n';
        int best = 0;
        bool disconnected = false;
        for (int s = 0; s < int(a.size()); ++s) {
            d = bfs(s, p);
            for (int x : d) {
                if (x < 0)
                    disconnected = true;
                best = max(best, x);
            }
        }
        cout << "Finite diameter " << best << '\n';
        if (disconnected)
            cout << "Diameter infinite\n";
    }
};
int main() {
    try {
        int n, m, s, t;
        if (!(cin >> n >> m) || n < 0 || n > 200 || m < 0 || m > 40000)
            throw runtime_error("Size");
        Graph g(n);
        for (int i = 0, u, v; i < m; ++i) {
            if (!(cin >> u >> v))
                throw runtime_error("Input");
            g.add(u, v);
        }
        g.print();
        if (!n) {
            g.diameter();
            return 0;
        }
        if (!(cin >> s >> t))
            throw runtime_error("Input");
        g.dfs(s);
        g.iterative(s);
        vector<int> p;
        g.bfs(s, p, true);
        cout << "Weak components\n";
        g.components();
        g.path(s, t);
        g.diameter();
        g.remove(s, s);
    } catch (const exception &e) {
        cerr << e.what() << '\n';
        return 1;
    }
}
