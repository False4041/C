#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <stdexcept>
using namespace std;
struct Student {
    string name;
    int grade = 0;
};
class HashTable {
    struct Slot {
        int key = 0, state = 0;
        Student value;
    };
    vector<Slot> a;
    size_t used = 0;
    size_t hash(int k) const {
        return size_t((static_cast<long long>(k) % static_cast<long long>(a.size()) + a.size()) %
                      a.size());
    }

  public:
    explicit HashTable(size_t n) : a(n) {
        if (!n)
            throw invalid_argument("Size");
    }
    void put(int k, Student v) {
        size_t start = hash(k), free = a.size();
        for (size_t t = 0; t < a.size(); ++t) {
            size_t i = (start + t) % a.size();
            if (a[i].state == 1 && a[i].key == k) {
                a[i].value = v;
                return;
            }
            if (a[i].state != 1 && free == a.size())
                free = i;
            if (a[i].state == 0)
                break;
        }
        if (free == a.size())
            throw overflow_error("Full");
        a[free] = {k, 1, v};
        ++used;
    }
    const Student *get(int k) const {
        size_t start = hash(k);
        for (size_t t = 0; t < a.size(); ++t) {
            auto &s = a[(start + t) % a.size()];
            if (!s.state)
                return nullptr;
            if (s.state == 1 && s.key == k)
                return &s.value;
        }
        return nullptr;
    }
    bool remove(int k) {
        size_t start = hash(k);
        for (size_t t = 0; t < a.size(); ++t) {
            auto &s = a[(start + t) % a.size()];
            if (!s.state)
                return false;
            if (s.state == 1 && s.key == k) {
                s.state = 2;
                --used;
                return true;
            }
        }
        return false;
    }
    size_t size() const {
        return used;
    }
};
int bound(const vector<int> &a, int x, bool upper) {
    int l = 0, r = int(a.size());
    while (l < r) {
        int m = l + (r - l) / 2;
        if (a[m] < x || (upper && a[m] == x))
            l = m + 1;
        else
            r = m;
    }
    return l;
}
int count(const vector<int> &a, int x) {
    return bound(a, x, true) - bound(a, x, false);
}
int main() {
    cout << "Count " << count({1, 2, 2, 2, 5}, 2) << ' ' << count({}, 2) << '\n';
    HashTable h(3);
    h.put(1, {"Ann", 5});
    h.put(4, {"Bob", 4});
    h.remove(1);
    h.put(4, {"Bob", 5});
    h.put(-2, {"Eve", 3});
    cout << h.get(4)->grade << ' ' << h.size() << ' ' << (h.get(1) == nullptr) << '\n';
    cout << "n binary_us hash_us found\n";
    for (int n : {1000, 10000, 100000}) {
        vector<int> a(n);
        HashTable t(2 * n + 1);
        for (int i = 0; i < n; ++i) {
            a[i] = 2 * i;
            t.put(2 * i, {"S", i % 6});
        }
        long long c = 0, d = 0;
        auto start = chrono::steady_clock::now();
        for (int q = 0; q < 2 * n; ++q) {
            int i = bound(a, q, false);
            c += i < n && a[i] == q;
        }
        auto mid = chrono::steady_clock::now();
        for (int q = 0; q < 2 * n; ++q)
            d += t.get(q) != nullptr;
        auto end = chrono::steady_clock::now();
        if (c != n || d != n)
            return 1;
        cout << n << ' ' << chrono::duration_cast<chrono::microseconds>(mid - start).count() << ' '
             << chrono::duration_cast<chrono::microseconds>(end - mid).count() << ' ' << c << '\n';
    }
}
