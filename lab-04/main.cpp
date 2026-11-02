#include <iostream>
#include <stdexcept>
#include <utility>
using namespace std;
template <class T> class SinglyLinkedList {
    struct Node {
        T value;
        Node *next;
    };
    Node *head = nullptr;
    Node *tail = nullptr;
    size_t count = 0;

  public:
    SinglyLinkedList() = default;
    SinglyLinkedList(const SinglyLinkedList &x) {
        for (auto p = x.head; p; p = p->next)
            push_back(p->value);
    }
    SinglyLinkedList &operator=(const SinglyLinkedList &x) {
        if (this != &x) {
            SinglyLinkedList t(x);
            swap(head, t.head);
            swap(tail, t.tail);
            swap(count, t.count);
        }
        return *this;
    }
    ~SinglyLinkedList() {
        clear();
    }
    size_t size() const {
        return count;
    }
    bool empty() const {
        return !head;
    }
    void clear() {
        while (head) {
            auto p = head;
            head = head->next;
            delete p;
        }
        tail = nullptr;
        count = 0;
    }
    void push_front(T x) {
        head = new Node{x, head};
        if (!tail)
            tail = head;
        ++count;
    }
    void push_back(T x) {
        auto p = new Node{x, nullptr};
        if (tail)
            tail->next = p;
        else
            head = p;
        tail = p;
        ++count;
    }
    T pop_front() {
        if (empty())
            throw out_of_range("Empty");
        T x = head->value;
        auto p = head;
        head = head->next;
        delete p;
        if (!head)
            tail = nullptr;
        --count;
        return x;
    }
    T pop_back() {
        if (empty())
            throw out_of_range("Empty");
        if (head == tail)
            return pop_front();
        auto p = head;
        while (p->next != tail)
            p = p->next;
        T x = tail->value;
        delete tail;
        tail = p;
        tail->next = nullptr;
        --count;
        return x;
    }
    T &at(size_t i) {
        if (i >= count)
            throw out_of_range("Index");
        auto p = head;
        while (i--)
            p = p->next;
        return p->value;
    }
    int find(T x) const {
        int i = 0;
        for (auto p = head; p; p = p->next, ++i)
            if (p->value == x)
                return i;
        return -1;
    }
    void insert(size_t i, T x) {
        if (i > count)
            throw out_of_range("Index");
        if (!i) {
            push_front(x);
            return;
        }
        if (i == count) {
            push_back(x);
            return;
        }
        auto p = head;
        for (size_t j = 1; j < i; ++j)
            p = p->next;
        p->next = new Node{x, p->next};
        ++count;
    }
    void erase(size_t i) {
        if (i >= count)
            throw out_of_range("Index");
        if (!i) {
            pop_front();
            return;
        }
        auto p = head;
        for (size_t j = 1; j < i; ++j)
            p = p->next;
        auto q = p->next;
        p->next = q->next;
        if (q == tail)
            tail = p;
        delete q;
        --count;
    }
    void print() const {
        for (auto p = head; p; p = p->next)
            cout << p->value << ' ';
        cout << '\n';
    }
};
template <class T> class DoublyLinkedList {
    struct Node {
        T value;
        Node *prev;
        Node *next;
    };
    Node *head = nullptr;
    Node *tail = nullptr;
    size_t count = 0;
    bool ring = false;
    Node *node(size_t i) const {
        if (i >= count)
            throw out_of_range("Index");
        auto p = head;
        while (i--)
            p = p->next;
        return p;
    }
    void link() {
        if (head) {
            head->prev = ring ? tail : nullptr;
            tail->next = ring ? head : nullptr;
        }
    }

  public:
    explicit DoublyLinkedList(bool circular = false) : ring(circular) {}
    DoublyLinkedList(const DoublyLinkedList &x) : ring(x.ring) {
        auto p = x.head;
        for (size_t i = 0; i < x.count; ++i, p = p->next)
            push_back(p->value);
    }
    DoublyLinkedList &operator=(const DoublyLinkedList &x) {
        if (this != &x) {
            DoublyLinkedList t(x);
            swap(head, t.head);
            swap(tail, t.tail);
            swap(count, t.count);
            swap(ring, t.ring);
        }
        return *this;
    }
    ~DoublyLinkedList() {
        clear();
    }
    size_t size() const {
        return count;
    }
    bool empty() const {
        return !count;
    }
    void clear() {
        while (count)
            erase(0);
    }
    T &at(size_t i) {
        return node(i)->value;
    }
    int find(T x) const {
        auto p = head;
        for (size_t i = 0; i < count; ++i, p = p->next)
            if (p->value == x)
                return int(i);
        return -1;
    }
    void push_front(T x) {
        insert(0, x);
    }
    void push_back(T x) {
        insert(count, x);
    }
    void insert(size_t i, T x) {
        if (i > count)
            throw out_of_range("Index");
        if (!count) {
            head = tail = new Node{x, nullptr, nullptr};
        } else if (!i) {
            auto p = new Node{x, nullptr, head};
            head->prev = p;
            head = p;
        } else if (i == count) {
            auto p = new Node{x, tail, nullptr};
            tail->next = p;
            tail = p;
        } else {
            auto q = node(i);
            auto p = new Node{x, q->prev, q};
            q->prev->next = p;
            q->prev = p;
        }
        ++count;
        link();
    }
    void erase(size_t i) {
        auto p = node(i);
        if (count == 1)
            head = tail = nullptr;
        else {
            if (p == head)
                head = p->next;
            else
                p->prev->next = p->next;
            if (p == tail)
                tail = p->prev;
            else
                p->next->prev = p->prev;
        }
        delete p;
        --count;
        link();
    }
    T pop_front() {
        T x = at(0);
        erase(0);
        return x;
    }
    T pop_back() {
        if (!count)
            throw out_of_range("Empty");
        T x = tail->value;
        erase(count - 1);
        return x;
    }
    void print(size_t rounds = 1) const {
        auto p = head;
        size_t n = ring ? count * rounds : count;
        for (size_t i = 0; i < n; ++i, p = p->next)
            cout << p->value << ' ';
        cout << '\n';
    }
};
int main() {
    SinglyLinkedList<int> a;
    for (int x : {1, 2, 3})
        a.push_back(x);
    a.push_front(0);
    a.insert(2, 9);
    a.erase(2);
    a.print();
    SinglyLinkedList<int> b = a;
    b.pop_back();
    b.pop_front();
    b.print();
    cout << a.find(2) << ' ' << a.at(3) << '\n';
    DoublyLinkedList<int> d;
    d.push_front(2);
    d.push_front(1);
    d.push_back(3);
    d.insert(1, 8);
    d.erase(1);
    d.print();
    DoublyLinkedList<int> r(true);
    for (int x : {1, 2, 3})
        r.push_back(x);
    r.print(2);
    r.pop_front();
    r.pop_back();
    r.print(3);
    auto copy = r;
    r.clear();
    copy.print();
    try {
        r.pop_back();
    } catch (const exception &e) {
        cout << e.what() << '\n';
    }
}
