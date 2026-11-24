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
void insertAt(vector<int> &a, size_t i) {
    int x = a[i];
    while (i && a[i - 1] > x) {
        a[i] = a[i - 1];
        --i;
    }
    a[i] = x;
}
void insertion(vector<int> &a) {
    for (size_t i = 1; i < a.size(); ++i)
        insertAt(a, i);
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
    insertAt(a, n - 1);
}
void recursiveSort(vector<int> &a) {
    recursiveInsertion(a, a.size());
}
int main() {
    cout << "Factorial: " << factorial(5) << "\nFibonacci: " << fib(10) << '\n';
    cout << "Brackets:";
    for (string s : {"([]{})", "([)]", "(()"}) {
        size_t i = 0;
        cout << ' ' << brackets(s, i);
    }
    cout << "\nCombinations:\n";
    vector<int> chosen;
    combinations(4, 2, 1, chosen);

    void (*algorithms[])(vector<int> &) = {bubble, insertion, selection, recursiveSort};
    mt19937 gen(12);
    cout << "n bubble_us insertion_us selection_us recursive_us\n";
    for (int n : {100, 500, 1000}) {
        vector<int> input(n);
        for (int &x : input)
            x = int(gen() % 1000);
        auto expected = input;
        sort(expected.begin(), expected.end());
        cout << n;
        for (auto algorithm : algorithms) {
            auto a = input;
            auto start = chrono::steady_clock::now();
            algorithm(a);
            auto elapsed = chrono::steady_clock::now() - start;
            if (a != expected)
                return 1;
            cout << ' ' << chrono::duration_cast<chrono::microseconds>(elapsed).count();
        }
        cout << '\n';
    }
}
