#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <algorithm>
using namespace std;
vector<int> prefix(const string &s) {
    vector<int> p(s.size());
    for (int i = 1; i < int(s.size()); ++i) {
        int j = p[i - 1];
        while (j && s[i] != s[j])
            j = p[j - 1];
        if (s[i] == s[j])
            ++j;
        p[i] = j;
    }
    return p;
}
int main() {
    string text, pattern;
    cout << "Text:\n";
    if (!getline(cin, text))
        return 1;
    cout << "Pattern:\n";
    if (!getline(cin, pattern))
        return 1;
    if (pattern.empty()) {
        cerr << "Empty pattern\n";
        return 1;
    }
    auto p = prefix(pattern);
    cout << "Prefix ";
    for (int x : p)
        cout << x << ' ';
    cout << "\nPositions ";
    int j = 0, longest = 0, last = 0;
    for (int i = 0; i < int(text.size()); ++i) {
        while (j && text[i] != pattern[j])
            j = p[j - 1];
        if (text[i] == pattern[j])
            ++j;
        longest = max(longest, j);
        last = j;
        if (j == int(pattern.size())) {
            cout << i - j + 1 << ' ';
            j = p[j - 1];
        }
    }
    cout << "\nBest prefix " << longest << "\nSuffix prefix " << last << '\n';
    long long shift;
    cout << "Shift: ";
    if (!(cin >> shift)) {
        cerr << "Input\n";
        return 1;
    }
    deque<char> d(text.begin(), text.end());
    if (!d.empty()) {
        long long n = static_cast<long long>(d.size());
        shift = (shift % n + n) % n;
        rotate(d.begin(), d.begin() + shift, d.end());
    }
    cout << "Rotated ";
    for (char c : d)
        cout << c;
    cout << '\n';
}
