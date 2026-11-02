#include <iostream>
#include <vector>
#include <queue>
#include <random>
#include <algorithm>
#include <stdexcept>
using namespace std;
class BST {
    struct Node {
        int key;
        Node *left = nullptr;
        Node *right = nullptr;
    };
    Node *root = nullptr;
    static Node *add(Node *p, int x) {
        if (!p)
            return new Node{x};
        if (x < p->key)
            p->left = add(p->left, x);
        else if (x > p->key)
            p->right = add(p->right, x);
        return p;
    }
    static Node *del(Node *p, int x) {
        if (!p)
            return p;
        if (x < p->key)
            p->left = del(p->left, x);
        else if (x > p->key)
            p->right = del(p->right, x);
        else {
            if (!p->left || !p->right) {
                auto q = p->left ? p->left : p->right;
                delete p;
                return q;
            }
            auto q = p->right;
            while (q->left)
                q = q->left;
            p->key = q->key;
            p->right = del(p->right, q->key);
        }
        return p;
    }
    static void clear(Node *p) {
        if (p) {
            clear(p->left);
            clear(p->right);
            delete p;
        }
    }
    static void walk(Node *p, int mode) {
        if (!p)
            return;
        if (mode == 0)
            cout << p->key << ' ';
        walk(p->left, mode);
        if (mode == 1)
            cout << p->key << ' ';
        walk(p->right, mode);
        if (mode == 2)
            cout << p->key << ' ';
    }
    static int height(Node *p) {
        return p ? 1 + max(height(p->left), height(p->right)) : 0;
    }
    static int range(Node *p, int lo, int hi) {
        if (!p)
            return 0;
        if (p->key < lo)
            return range(p->right, lo, hi);
        if (p->key > hi)
            return range(p->left, lo, hi);
        return 1 + range(p->left, lo, hi) + range(p->right, lo, hi);
    }

  public:
    BST() = default;
    BST(const BST &) = delete;
    BST &operator=(const BST &) = delete;
    ~BST() {
        clear(root);
    }
    void insert(int x) {
        root = add(root, x);
    }
    void remove(int x) {
        root = del(root, x);
    }
    bool search(int x) const {
        auto p = root;
        while (p && p->key != x)
            p = x < p->key ? p->left : p->right;
        return p;
    }
    void traversal(int mode) const {
        walk(root, mode);
        cout << '\n';
    }
    void levelOrder() const {
        queue<Node *> q;
        if (root)
            q.push(root);
        while (!q.empty()) {
            auto p = q.front();
            q.pop();
            cout << p->key << ' ';
            if (p->left)
                q.push(p->left);
            if (p->right)
                q.push(p->right);
        }
        cout << '\n';
    }
    vector<int> path(int x) const {
        vector<int> v;
        auto p = root;
        while (p) {
            v.push_back(p->key);
            if (p->key == x)
                return v;
            p = x < p->key ? p->left : p->right;
        }
        return {};
    }
    int height() const {
        return height(root);
    }
    int range(int lo, int hi) const {
        return lo > hi ? 0 : range(root, lo, hi);
    }
    int minValue() const {
        if (!root)
            throw out_of_range("Empty");
        auto p = root;
        while (p->left)
            p = p->left;
        return p->key;
    }
    int maxValue() const {
        if (!root)
            throw out_of_range("Empty");
        auto p = root;
        while (p->right)
            p = p->right;
        return p->key;
    }
};
int main() {
    BST t;
    mt19937 gen(12);
    for (int i = 0; i < 20; ++i)
        t.insert(int(gen() % 100));
    for (int m = 0; m < 3; ++m)
        t.traversal(m);
    t.levelOrder();
    cout << "Min " << t.minValue() << " Max " << t.maxValue() << " Height " << t.height() << '\n';
    cout << "Range " << t.range(20, 70) << '\n';
    for (int x : t.path(t.maxValue()))
        cout << x << ' ';
    cout << '\n';
    int x;
    cout << "Remove: ";
    if (!(cin >> x)) {
        cerr << "Input\n";
        return 1;
    }
    t.remove(x);
    t.traversal(1);
    BST check;
    for (int v : {4, 2, 6, 1, 3, 5, 7})
        check.insert(v);
    check.remove(1);
    check.remove(2);
    check.remove(4);
    if (check.search(1) || check.search(2) || check.search(4) || check.range(0, 9) != 4)
        return 1;
    BST empty;
    empty.remove(0);
    empty.levelOrder();
}
