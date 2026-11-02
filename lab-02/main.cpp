#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <chrono>
#include <stdexcept>
using namespace std;
template <class T> class SparseArray {
    vector<pair<int, T>> items;

  public:
    void set(int i, const T &value) {
        if (i < 0)
            throw out_of_range("Index");
        for (auto &p : items)
            if (p.first == i) {
                p.second = value;
                return;
            }
        items.push_back({i, value});
    }
    T get(int i) const {
        if (i < 0)
            throw out_of_range("Index");
        for (const auto &p : items)
            if (p.first == i)
                return p.second;
        return T{};
    }
    bool remove(int i) {
        if (i < 0)
            throw out_of_range("Index");
        for (size_t j = 0; j < items.size(); ++j)
            if (items[j].first == i) {
                items[j] = items.back();
                items.pop_back();
                return true;
            }
        return false;
    }
    size_t size() const {
        return items.size();
    }
};
template <class F> long long measure(F f) {
    auto t = chrono::steady_clock::now();
    f();
    return chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - t).count();
}
int main() {
    SparseArray<int> a;
    a.set(12, 4);
    a.set(1000, 7);
    a.set(12, 9);
    cout << a.size() << ' ' << a.get(12) << ' ' << a.get(13) << '\n';
    a.remove(12);
    cout << a.get(12) << ' ' << a.size() << '\n';
    SparseArray<string> b;
    b.set(5, "Hi");
    cout << b.get(5) << '\n';
    try {
        a.set(-1, 0);
    } catch (const exception &e) {
        cout << e.what() << '\n';
    }
    cout << "n sparse_us stl_us sum\n";
    for (int n : {1000, 5000, 10000}) {
        SparseArray<int> x;
        unordered_map<int, int> y;
        long long sx = 0, sy = 0;
        auto tx = measure([&] {
            for (int i = 0; i < n; ++i)
                x.set(i * 10, i);
            for (int i = 0; i < n; ++i)
                sx += x.get(i * 10);
        });
        auto ty = measure([&] {
            for (int i = 0; i < n; ++i)
                y[i * 10] = i;
            for (int i = 0; i < n; ++i)
                sy += y.at(i * 10);
        });
        if (sx != sy)
            return 1;
        cout << n << ' ' << tx << ' ' << ty << ' ' << sx << '\n';
    }
}
