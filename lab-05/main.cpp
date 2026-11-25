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
        // первый из двух средних
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
    // разделение по позициям
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
        // сдвиг элементов вправо
        a.insert(a.begin() + i, x);
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
        a.erase(a.begin());
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
    // проверка порядка кучи
    static bool isHeap(const vector<int> &v) {
        for (size_t i = 1; i < v.size(); ++i)
            if (v[(i - 1) / 2] < v[i])
                return false;
        return true;
    }
    void push(int x) {
        a.push_back(x);
        size_t i = a.size() - 1;
        // просеивание при вставке
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
        // просеивание после удаления
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
    Stack stack;
    for (int x : {1, 2, 3, 4})
        stack.push(x);
    cout << "Stack middle: " << stack.getMiddle() << '\n';

    Queue queue, odd, even;
    for (int x : {10, 20, 30, 40, 50, 60})
        queue.enqueue(x);
    queue.split(odd, even);
    cout << "Odd positions:";
    while (!odd.isEmpty())
        cout << ' ' << odd.dequeue();
    cout << "\nEven positions:";
    while (!even.isEmpty())
        cout << ' ' << even.dequeue();

    Deque deque;
    deque.push_back(1);
    deque.push_back(3);
    deque.insertAt(1, 2);
    deque.push_front(0);
    cout << "\nDeque:";
    while (!deque.isEmpty())
        cout << ' ' << deque.pop_front();

    PriorityQueue priority;
    for (int x : {3, 1, 5, 2})
        priority.push(x);
    cout << "\nPriority:";
    while (!priority.isEmpty())
        cout << ' ' << priority.pop();
    cout << "\nIs heap: " << PriorityQueue::isHeap({9, 7, 8, 1}) << ' '
         << PriorityQueue::isHeap({1, 9}) << '\n';
}
