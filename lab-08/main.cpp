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
    enum State { Empty, Filled, Deleted };
    struct Slot {
        int key = 0;
        Student value;
        State state = Empty;
    };
    vector<Slot> slots;

    size_t hash(int key) const {
        auto size = static_cast<long long>(slots.size());
        auto index = key % size;
        // handle negative keys
        return static_cast<size_t>(index < 0 ? index + size : index);
    }
    // linear probing
    size_t findIndex(int key) const {
        size_t index = hash(key);
        for (size_t i = 0; i < slots.size(); ++i) {
            const auto &slot = slots[index];
            if (slot.state == Empty)
                break;
            if (slot.state == Filled && slot.key == key)
                return index;
            index = (index + 1) % slots.size();
        }
        return slots.size();
    }

  public:
    explicit HashTable(size_t size) : slots(size) {
        if (size == 0)
            throw invalid_argument("Size");
    }
    void put(int key, const Student &value) {
        size_t index = findIndex(key);
        if (index != slots.size()) {
            slots[index].value = value;
            return;
        }
        index = hash(key);
        // find reusable slot
        for (size_t i = 0; i < slots.size(); ++i) {
            if (slots[index].state != Filled) {
                slots[index] = {key, value, Filled};
                return;
            }
            index = (index + 1) % slots.size();
        }
        throw overflow_error("Full");
    }
    const Student *get(int key) const {
        size_t index = findIndex(key);
        return index == slots.size() ? nullptr : &slots[index].value;
    }
    bool remove(int key) {
        size_t index = findIndex(key);
        if (index == slots.size())
            return false;
        // mark as deleted
        slots[index].state = Deleted;
        return true;
    }
};
// binary search bounds
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
    // equal range length
    return bound(a, x, true) - bound(a, x, false);
}
int main() {
    cout << "Count: " << count({1, 2, 2, 2, 5}, 2) << ' ' << count({}, 2) << '\n';
    HashTable h(3);
    h.put(1, {"Ann", 5});
    h.put(4, {"Bob", 4});
    h.remove(1);
    h.put(4, {"Bob", 5});
    h.put(-2, {"Eve", 3});
    cout << "Updated grade: " << h.get(4)->grade << '\n';
    cout << "Reused slot: " << h.get(-2)->name << '\n';
    cout << "Removed: " << (h.get(1) == nullptr) << '\n';
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
