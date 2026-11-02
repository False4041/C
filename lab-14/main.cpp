#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>
using namespace std;
vector<int> row(const vector<int> &w, const vector<int> &v, int n, int cap) {
    vector<int> d(cap + 1);
    for (int i = 0; i < n; ++i)
        for (int c = cap; c >= w[i]; --c)
            d[c] = max(d[c], d[c - w[i]] + v[i]);
    return d;
}
int best(const vector<int> &w, const vector<int> &v, int n, int cap) {
    return row(w, v, n, cap)[cap];
}
void sets(const vector<int> &w, const vector<int> &v, int n, int cap, int value,
          vector<int> &chosen) {
    if (!n) {
        if (!value) {
            if (chosen.empty())
                cout << "Empty";
            for (auto i = chosen.rbegin(); i != chosen.rend(); ++i)
                cout << *i << ' ';
            cout << '\n';
        }
        return;
    }
    if (best(w, v, n - 1, cap) == value)
        sets(w, v, n - 1, cap, value, chosen);
    if (w[n - 1] <= cap && best(w, v, n - 1, cap - w[n - 1]) + v[n - 1] == value) {
        chosen.push_back(n);
        sets(w, v, n - 1, cap - w[n - 1], value - v[n - 1], chosen);
        chosen.pop_back();
    }
}
int main() {
    try {
        int n, cap;
        cout << "Items capacity: ";
        if (!(cin >> n >> cap) || n < 0 || n > 25 || cap < 0 || cap > 10000)
            throw runtime_error("Size");
        vector<int> w(n), v(n);
        for (int i = 0; i < n; ++i)
            if (!(cin >> w[i] >> v[i]) || w[i] <= 0 || v[i] < 0 || v[i] > 1000000)
                throw runtime_error("Item");
        int value = best(w, v, n, cap);
        cout << "Value " << value << "\nSets\n";
        vector<int> chosen;
        sets(w, v, n, cap, value, chosen);
        cout << "Knapsack row (up to 50)\n";
        auto dp = row(w, v, n, cap);
        for (int c = 0; c <= min(cap, 50); ++c)
            cout << dp[c] << ' ';
        cout << '\n';
        string a, b;
        cin.ignore(10000, '\n');
        cout << "Strings:\n";
        if (!getline(cin, a) || !getline(cin, b) || a.size() > 1000 || b.size() > 1000)
            throw runtime_error("Text");
        int x = int(a.size()), y = int(b.size());
        vector<vector<int>> d(x + 1, vector<int>(y + 1)), c = d;
        int len = 0, end = 0;
        for (int i = 1; i <= x; ++i)
            for (int j = 1; j <= y; ++j) {
                d[i][j] =
                    a[i - 1] == b[j - 1] ? d[i - 1][j - 1] + 1 : max(d[i - 1][j], d[i][j - 1]);
                if (a[i - 1] == b[j - 1]) {
                    c[i][j] = c[i - 1][j - 1] + 1;
                    if (c[i][j] > len) {
                        len = c[i][j];
                        end = i;
                    }
                }
            }
        cout << "LCS table (up to 20 x 20)\n";
        for (int r = 0; r <= min(x, 20); ++r) {
            for (int col = 0; col <= min(y, 20); ++col)
                cout << d[r][col] << ' ';
            cout << '\n';
        }
        string lcs;
        int i = x, j = y;
        while (i && j) {
            if (a[i - 1] == b[j - 1]) {
                lcs += a[--i];
                --j;
            } else if (d[i - 1][j] >= d[i][j - 1])
                --i;
            else
                --j;
        }
        reverse(lcs.begin(), lcs.end());
        cout << "LCS " << lcs.size() << ' ' << lcs << "\nSubstring " << len << ' '
             << a.substr(end - len, len) << '\n';
    } catch (const exception &e) {
        cerr << e.what() << '\n';
        return 1;
    }
}
