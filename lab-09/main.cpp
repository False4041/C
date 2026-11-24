#include <iostream>
#include <vector>
#include <queue>
#include <random>
#include <algorithm>
#include <stdexcept>
using namespace std;
enum Order { Preorder, Inorder, Postorder };

class BST {
    struct Node {
        int key;
        Node *left = nullptr;
        Node *right = nullptr;
    };
    Node *root = nullptr;
    static Node *add(Node *p, int x) {
        // insert new leaf
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
                // replace with child
                auto q = p->left ? p->left : p->right;
                delete p;
                return q;
            }
            // right subtree minimum
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
    // depth first traversal
    static void walk(Node *p, Order order) {
        if (!p)
            return;
        if (order == Preorder)
            cout << p->key << ' ';
        walk(p->left, order);
        if (order == Inorder)
            cout << p->key << ' ';
        walk(p->right, order);
        if (order == Postorder)
            cout << p->key << ' ';
    }
    static int height(Node *p) {
        return p ? 1 + max(height(p->left), height(p->right)) : 0;
    }
    static int range(Node *p, int lo, int hi) {
        if (!p)
            return 0;
        // skip outside branches
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
    void traversal(Order order) const {
        walk(root, order);
        cout << '\n';
    }
    // breadth first traversal
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
    // record search path
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
    BST tree;
    mt19937 gen(12);
    for (int i = 0; i < 20; ++i)
        tree.insert(int(gen() % 100));

    cout << "Preorder: ";
    tree.traversal(Preorder);
    cout << "Inorder: ";
    tree.traversal(Inorder);
    cout << "Postorder: ";
    tree.traversal(Postorder);
    cout << "Level order: ";
    tree.levelOrder();
    cout << "Min: " << tree.minValue() << " Max: " << tree.maxValue()
         << " Height: " << tree.height() << '\n';
    cout << "Range [20, 70]: " << tree.range(20, 70) << '\n';
    cout << "Path to max:";
    for (int x : tree.path(tree.maxValue()))
        cout << ' ' << x;
    cout << "\nRemove: ";
    int key;
    if (!(cin >> key))
        return 1;
    cout << key << " Found: " << tree.search(key) << '\n';
    tree.remove(key);
    cout << "After removal: ";
    tree.traversal(Inorder);
}
