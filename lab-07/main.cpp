#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <chrono>
#include <random>
#include <stdexcept>
using namespace std;
void quick(vector<int> &a, int l, int r) {
    if (l >= r)
        return;
    int p = a[(l + r) / 2], i = l, j = r;
    while (i <= j) {
        while (a[i] < p)
            ++i;
        while (a[j] > p)
            --j;
        if (i <= j)
            swap(a[i++], a[j--]);
    }
    if (l < j)
        quick(a, l, j);
    if (i < r)
        quick(a, i, r);
}
vector<int> combine(const vector<int> &a, const vector<int> &b) {
    vector<int> c;
    size_t i = 0, j = 0;
    while (i < a.size() || j < b.size()) {
        if (j == b.size() || (i < a.size() && a[i] <= b[j]))
            c.push_back(a[i++]);
        else
            c.push_back(b[j++]);
    }
    return c;
}
void mergeSort(vector<int> &a) {
    if (a.size() < 2)
        return;
    auto mid = a.begin() + a.size() / 2;
    vector<int> l(a.begin(), mid), r(mid, a.end());
    mergeSort(l);
    mergeSort(r);
    a = combine(l, r);
}
void down(vector<int> &a, int n, int i) {
    while (2 * i + 1 < n) {
        int j = 2 * i + 1;
        if (j + 1 < n && a[j + 1] > a[j])
            ++j;
        if (a[i] >= a[j])
            break;
        swap(a[i], a[j]);
        i = j;
    }
}
void heapSort(vector<int> &a) {
    int n = int(a.size());
    for (int i = n / 2 - 1; i >= 0; --i)
        down(a, n, i);
    for (int i = n - 1; i > 0; --i) {
        swap(a[0], a[i]);
        down(a, i, 0);
    }
}
vector<int> read(const string &file) {
    ifstream f(file);
    if (!f)
        throw runtime_error("Open");
    vector<int> a;
    int x;
    while (f >> x)
        a.push_back(x);
    if (!f.eof())
        throw runtime_error("Data");
    return a;
}
void write(const string &file, const vector<int> &a) {
    ofstream f(file);
    if (!f)
        throw runtime_error("Open");
    for (int x : a)
        f << x << ' ';
    f << '\n';
    if (!f)
        throw runtime_error("Write");
}
int main(int argc, char **argv) {
    try {
        if (argc > 1) {
            string mode = argv[1];
            if (mode == "worker" && argc == 4) {
                auto a = read(argv[2]);
                mergeSort(a);
                write(argv[3], a);
                return 0;
            }
            if (mode == "split" && argc == 4) {
                auto a = read(argv[2]);
                int n = stoi(argv[3]);
                if (n < 1 || n > 100)
                    throw runtime_error("Workers");
                for (int i = 0; i < n; ++i) {
                    auto l = a.begin() + a.size() * i / n, r = a.begin() + a.size() * (i + 1) / n;
                    write("part" + to_string(i) + ".txt", vector<int>(l, r));
                }
                return 0;
            }
            if (mode == "merge" && argc >= 4) {
                vector<int> a;
                for (int i = 3; i < argc; ++i) {
                    auto b = read(argv[i]);
                    if (!is_sorted(b.begin(), b.end()))
                        throw runtime_error("Unsorted");
                    a = combine(a, b);
                }
                write(argv[2], a);
                return 0;
            }
            throw runtime_error("Args");
        }
        mt19937 gen(12);
        cout << "n kind quick_us merge_us heap_us\n";
        for (int n : {100, 1000, 10000})
            for (int kind = 0; kind < 3; ++kind) {
                vector<int> a(n);
                for (int i = 0; i < n; ++i)
                    a[i] = kind == 0 ? int(gen() % 10000) : kind == 1 ? i : n - i;
                auto expected = a;
                sort(expected.begin(), expected.end());
                cout << n << ' ' << kind;
                for (int mode = 0; mode < 3; ++mode) {
                    auto b = a;
                    auto t = chrono::steady_clock::now();
                    if (mode == 0)
                        quick(b, 0, n - 1);
                    if (mode == 1)
                        mergeSort(b);
                    if (mode == 2)
                        heapSort(b);
                    auto us =
                        chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - t)
                            .count();
                    if (b != expected)
                        throw runtime_error("Sort");
                    cout << ' ' << us;
                }
                cout << '\n';
            }
        for (vector<int> a : vector<vector<int>>{{}, {1}, {2, 2, 1, 1}}) {
            auto b = a, c = a;
            quick(a, 0, int(a.size()) - 1);
            mergeSort(b);
            heapSort(c);
            if (a != b || b != c)
                throw runtime_error("Edge");
        }
    } catch (const exception &e) {
        cerr << e.what() << '\n';
        return 1;
    }
}
