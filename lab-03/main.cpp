#include <iostream>
#include <memory>
#include <algorithm>
#include <stdexcept>
#include <utility>
using namespace std;
template <class T> class Vector {
    struct Data {
        size_t n = 0, cap;
        unique_ptr<T[]> a;
        explicit Data(size_t c) : cap(c), a(new T[c]{}) {}
    };
    shared_ptr<Data> d;
    bool exposed = false;
    shared_ptr<Data> clone(size_t cap) const {
        auto p = make_shared<Data>(cap);
        if (d) {
            p->n = d->n;
            copy_n(d->a.get(), d->n, p->a.get());
        }
        return p;
    }
    void detach() {
        if (!d)
            d = make_shared<Data>(0);
        else if (d.use_count() > 1)
            d = clone(d->cap);
    }

  public:
    Vector() : d(make_shared<Data>(0)) {}
    Vector(const Vector &v) : d(v.exposed ? v.clone(v.capacity()) : v.d) {}
    Vector(Vector &&v) noexcept : d(move(v.d)), exposed(v.exposed) {
        v.exposed = false;
    }
    Vector &operator=(const Vector &v) {
        if (this != &v) {
            Vector t(v);
            swap(d, t.d);
            swap(exposed, t.exposed);
        }
        return *this;
    }
    Vector &operator=(Vector &&v) noexcept {
        if (this != &v) {
            d = move(v.d);
            exposed = v.exposed;
            v.exposed = false;
        }
        return *this;
    }
    ~Vector() = default;
    size_t size() const {
        return d ? d->n : 0;
    }
    size_t capacity() const {
        return d ? d->cap : 0;
    }
    long owners() const {
        return d.use_count();
    }
    void reserve(size_t c) {
        if (c > capacity()) {
            d = clone(c);
            exposed = false;
        }
    }
    void push_back(const T &value) {
        T saved = value;
        detach();
        if (size() == capacity())
            reserve(capacity() ? 2 * capacity() : 1);
        d->a[d->n++] = saved;
    }
    void pop_back() {
        if (!size())
            throw out_of_range("Empty");
        detach();
        d->a[--d->n] = T{};
    }
    void erase(size_t i) {
        if (i >= size())
            throw out_of_range("Index");
        detach();
        for (size_t j = i + 1; j < size(); ++j)
            d->a[j - 1] = d->a[j];
        d->a[--d->n] = T{};
    }
    const T &operator[](size_t i) const {
        if (i >= size())
            throw out_of_range("Index");
        return d->a[i];
    }
    T &operator[](size_t i) {
        if (i >= size())
            throw out_of_range("Index");
        detach();
        exposed = true;
        return d->a[i];
    }
    const T *begin() const {
        return d ? d->a.get() : nullptr;
    }
    const T *end() const {
        return d ? d->a.get() + d->n : nullptr;
    }
    T *begin() {
        detach();
        exposed = true;
        return d->a.get();
    }
    T *end() {
        detach();
        exposed = true;
        return d->a.get() + d->n;
    }
};
int main() {
    Vector<int> a;
    for (int x : {1, 2, 3})
        a.push_back(x);
    Vector<int> b = a;
    cout << "Shared " << a.owners() << '\n';
    b[0] = 9;
    for (int x : static_cast<const Vector<int> &>(a))
        cout << x << ' ';
    cout << '\n';
    for (int x : static_cast<const Vector<int> &>(b))
        cout << x << ' ';
    cout << '\n';
    int &ref = a[0];
    Vector<int> c = a;
    ref = 8;
    cout << "Safe " << c[0] << '\n';
    Vector<int> d = move(b);
    b.push_back(4);
    d.erase(1);
    d.pop_back();
    cout << "Sizes " << d.size() << ' ' << b.size() << '\n';
    a = a;
    c = d;
    c = move(d);
    try {
        d.pop_back();
    } catch (const exception &e) {
        cout << e.what() << '\n';
    }
}
