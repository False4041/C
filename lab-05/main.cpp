#include <iostream>
#include <vector>
#include <stdexcept>
#include <utility>
using namespace std;
class Stack {
    vector<int> a;

  public:
    bool isEmpty() const {
        return a.empty();
    }
    void push(int x) {
        a.push_back(x);
    }
    int top() const {
        if (isEmpty())
            throw out_of_range("Empty");
        return a.back();
    }
    int pop() {
        int x = top();
        a.pop_back();
        return x;
    }
    int getMiddle() const {
        if (isEmpty())
            throw out_of_range("Empty");
        return a[(a.size() - 1) / 2];
    }
};
class Queue {
    struct Node {
        int value;
        Node *next;
    };
    Node *first = nullptr;
    Node *last = nullptr;

  public:
    Queue() = default;
    Queue(const Queue &) = delete;
    Queue &operator=(const Queue &) = delete;
    ~Queue() {
        while (!isEmpty())
            dequeue();
    }
    bool isEmpty() const {
        return !first;
    }
    void enqueue(int x) {
        auto p = new Node{x, nullptr};
        if (last)
            last->next = p;
        else
            first = p;
        last = p;
    }
    int front() const {
        if (isEmpty())
            throw out_of_range("Empty");
        return first->value;
    }
    int dequeue() {
        int x = front();
        auto p = first;
        first = first->next;
        delete p;
        if (!first)
            last = nullptr;
        return x;
    }
    void split(Queue &odd, Queue &even) {
        if (this == &odd || this == &even || &odd == &even)
            throw invalid_argument("Alias");
        int i = 1;
        while (!isEmpty()) {
            int x = dequeue();
            if (i++ % 2)
                odd.enqueue(x);
            else
                even.enqueue(x);
        }
    }
};
class Deque {
    vector<int> a;

  public:
    bool isEmpty() const {
        return a.empty();
    }
    void insertAt(size_t i, int x) {
        if (i > a.size())
            throw out_of_range("Index");
        a.push_back(x);
        for (size_t j = a.size() - 1; j > i; --j)
            a[j] = a[j - 1];
        a[i] = x;
    }
    void push_front(int x) {
        insertAt(0, x);
    }
    void push_back(int x) {
        a.push_back(x);
    }
    int front() const {
        if (isEmpty())
            throw out_of_range("Empty");
        return a.front();
    }
    int back() const {
        if (isEmpty())
            throw out_of_range("Empty");
        return a.back();
    }
    int pop_front() {
        int x = front();
        for (size_t i = 1; i < a.size(); ++i)
            a[i - 1] = a[i];
        a.pop_back();
        return x;
    }
    int pop_back() {
        int x = back();
        a.pop_back();
        return x;
    }
};
class PriorityQueue {
    vector<int> a;

  public:
    bool isEmpty() const {
        return a.empty();
    }
    static bool isHeap(const vector<int> &v) {
        for (size_t i = 1; i < v.size(); ++i)
            if (v[(i - 1) / 2] < v[i])
                return false;
        return true;
    }
    void push(int x) {
        a.push_back(x);
        size_t i = a.size() - 1;
        while (i && a[(i - 1) / 2] < a[i]) {
            swap(a[i], a[(i - 1) / 2]);
            i = (i - 1) / 2;
        }
    }
    int top() const {
        if (isEmpty())
            throw out_of_range("Empty");
        return a[0];
    }
    int pop() {
        int x = top();
        a[0] = a.back();
        a.pop_back();
        size_t i = 0;
        while (2 * i + 1 < a.size()) {
            size_t j = 2 * i + 1;
            if (j + 1 < a.size() && a[j + 1] > a[j])
                ++j;
            if (a[i] >= a[j])
                break;
            swap(a[i], a[j]);
            i = j;
        }
        return x;
    }
};
int main() {
    Stack s;
    for (int x : {1, 2, 3, 4})
        s.push(x);
    cout << "Middle " << s.getMiddle() << '\n';
    s.pop();
    cout << s.getMiddle() << '\n';
    Queue q, o, e;
    for (int i = 1; i <= 6; ++i)
        q.enqueue(i);
    q.split(o, e);
    while (!o.isEmpty())
        cout << o.dequeue() << ' ';
    cout << '\n';
    while (!e.isEmpty())
        cout << e.dequeue() << ' ';
    cout << '\n';
    Deque d;
    d.push_back(1);
    d.push_back(3);
    d.insertAt(1, 2);
    d.push_front(0);
    cout << d.pop_back() << '\n';
    while (!d.isEmpty())
        cout << d.pop_front() << ' ';
    cout << '\n';
    PriorityQueue p;
    for (int x : {3, 1, 5, 2})
        p.push(x);
    while (!p.isEmpty())
        cout << p.pop() << ' ';
    cout << '\n';
    cout << PriorityQueue::isHeap({9, 7, 8, 1}) << ' ' << PriorityQueue::isHeap({1, 9}) << '\n';
    try {
        s = Stack();
        s.pop();
    } catch (const exception &x) {
        cout << x.what() << '\n';
    }
    try {
        q.dequeue();
    } catch (const exception &x) {
        cout << x.what() << '\n';
    }
    try {
        d.insertAt(1, 0);
    } catch (const exception &x) {
        cout << x.what() << '\n';
    }
    try {
        p.pop();
    } catch (const exception &x) {
        cout << x.what() << '\n';
    }
}
