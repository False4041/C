#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <random>
#include <stdexcept>
using namespace std;
unsigned long long factorial(int n) {
    if (n < 0 || n > 20)
        throw out_of_range("Range");
    return n < 2 ? 1 : n * factorial(n - 1);
}
unsigned long long fib(int n) {
    if (n < 0 || n > 40)
        throw out_of_range("Range");
    return n < 2 ? n : fib(n - 1) + fib(n - 2);
}
bool brackets(const string &s, size_t &i, char end = 0) {
    while (i < s.size()) {
        char c = s[i++];
        if (c == '(' || c == '[' || c == '{') {
            char close = c == '(' ? ')' : c == '[' ? ']' : '}';
            if (!brackets(s, i, close))
                return false;
        } else if (c == ')' || c == ']' || c == '}')
            return c == end;
    }
    return end == 0;
}
void combinations(int n, int k, int start, vector<int> &v) {
    if (k == 0) {
        for (int x : v)
            cout << x << ' ';
        cout << '\n';
        return;
    }
    for (int i = start; i <= n - k + 1; ++i) {
        v.push_back(i);
        combinations(n, k - 1, i + 1, v);
        v.pop_back();
    }
}
void bubble(vector<int> &a) {
    for (size_t n = a.size(); n > 1; --n) {
        bool changed = false;
        for (size_t i = 1; i < n; ++i)
            if (a[i - 1] > a[i]) {
                swap(a[i - 1], a[i]);
                changed = true;
            }
        if (!changed)
            break;
    }
}
void insertion(vector<int> &a) {
    for (size_t i = 1; i < a.size(); ++i) {
        int x = a[i];
        size_t j = i;
        while (j && a[j - 1] > x) {
            a[j] = a[j - 1];
            --j;
        }
        a[j] = x;
    }
}
void selection(vector<int> &a) {
    for (size_t i = 0; i < a.size(); ++i) {
        size_t m = i;
        for (size_t j = i + 1; j < a.size(); ++j)
            if (a[j] < a[m])
                m = j;
        swap(a[i], a[m]);
    }
}
void recursiveInsertion(vector<int> &a, size_t n) {
    if (n < 2)
        return;
    recursiveInsertion(a, n - 1);
    int x = a[n - 1];
    size_t j = n - 1;
    while (j && a[j - 1] > x) {
        a[j] = a[j - 1];
        --j;
    }
    a[j] = x;
}
int main() {
    cout << "Factorial " << factorial(5) << " Fib " << fib(10) << '\n';
    for (string s : {"([]{})", "([)]", "", "(()"}) {
        size_t i = 0;
        cout << "Balanced " << (brackets(s, i) && i == s.size()) << '\n';
    }
    vector<int> c;
    combinations(4, 2, 1, c);
    mt19937 gen(12);
    cout << "n bubble_us insertion_us selection_us recursive_us\n";
    for (int n : {100, 500, 1000}) {
        vector<int> a(n);
        for (int &x : a)
            x = int(gen() % 1000);
        cout << n;
        for (int mode = 0; mode < 4; ++mode) {
            auto b = a;
            auto t = chrono::steady_clock::now();
            if (mode == 0)
                bubble(b);
            if (mode == 1)
                insertion(b);
            if (mode == 2)
                selection(b);
            if (mode == 3)
                recursiveInsertion(b, b.size());
            auto us = chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - t)
                          .count();
            auto expected = a;
            sort(expected.begin(), expected.end());
            if (b != expected)
                return 1;
            cout << ' ' << us;
        }
        cout << '\n';
    }
    vector<int> empty;
    recursiveInsertion(empty, 0);
    try {
        factorial(21);
    } catch (const exception &e) {
        cout << e.what() << '\n';
    }
}
