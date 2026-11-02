#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <algorithm>
#include <random>
#include <chrono>
using namespace std;
class Tree {
    struct Node {
        int key, h = 1;
        Node *l = nullptr;
        Node *r = nullptr;
    };
    Node *root = nullptr;
    bool avl;
    static int height(Node *p) {
        return p ? p->h : 0;
    }
    static void fix(Node *p) {
        p->h = 1 + max(height(p->l), height(p->r));
    }
    static Node *right(Node *p) {
        auto q = p->l;
        p->l = q->r;
        q->r = p;
        fix(p);
        fix(q);
        return q;
    }
    static Node *left(Node *p) {
        auto q = p->r;
        p->r = q->l;
        q->l = p;
        fix(p);
        fix(q);
        return q;
    }
    Node *balance(Node *p) {
        if (!p)
            return p;
        fix(p);
        if (!avl)
            return p;
        if (height(p->l) - height(p->r) > 1) {
            if (height(p->l->r) > height(p->l->l))
                p->l = left(p->l);
            return right(p);
        }
        if (height(p->r) - height(p->l) > 1) {
            if (height(p->r->l) > height(p->r->r))
                p->r = right(p->r);
            return left(p);
        }
        return p;
    }
    Node *add(Node *p, int x) {
        if (!p)
            return new Node{x};
        if (x < p->key)
            p->l = add(p->l, x);
        else if (x > p->key)
            p->r = add(p->r, x);
        return balance(p);
    }
    Node *del(Node *p, int x) {
        if (!p)
            return p;
        if (x < p->key)
            p->l = del(p->l, x);
        else if (x > p->key)
            p->r = del(p->r, x);
        else {
            if (!p->l || !p->r) {
                auto q = p->l ? p->l : p->r;
                delete p;
                return q;
            }
            auto q = p->r;
            while (q->l)
                q = q->l;
            p->key = q->key;
            p->r = del(p->r, q->key);
        }
        return balance(p);
    }
    static bool valid(Node *p, long long lo, long long hi) {
        if (!p)
            return true;
        return p->key > lo && p->key < hi && abs(height(p->l) - height(p->r)) <= 1 &&
               p->h == 1 + max(height(p->l), height(p->r)) && valid(p->l, lo, p->key) &&
               valid(p->r, p->key, hi);
    }

  public:
    explicit Tree(bool b = true) : avl(b) {}
    Tree(const Tree &) = delete;
    Tree &operator=(const Tree &) = delete;
    ~Tree() {
        vector<Node *> s;
        if (root)
            s.push_back(root);
        while (!s.empty()) {
            auto p = s.back();
            s.pop_back();
            if (p->l)
                s.push_back(p->l);
            if (p->r)
                s.push_back(p->r);
            delete p;
        }
    }
    void insert(int x) {
        if (avl)
            root = add(root, x);
        else {
            Node **p = &root;
            while (*p) {
                if (x == (*p)->key)
                    return;
                p = x < (*p)->key ? &(*p)->l : &(*p)->r;
            }
            *p = new Node{x};
        }
    }
    void remove(int x) {
        root = del(root, x);
    }
    bool search(int x) const {
        auto p = root;
        while (p && p->key != x)
            p = x < p->key ? p->l : p->r;
        return p;
    }
    bool valid() const {
        return valid(root, -2147483649LL, 2147483648LL);
    }
};
class Trie {
    struct Node {
        bool word = false;
        map<char, unique_ptr<Node>> next;
    };
    Node root;
    static bool erase(Node &n, const string &s, size_t i) {
        if (i == s.size()) {
            bool found = n.word;
            n.word = false;
            return found;
        }
        auto p = n.next.find(s[i]);
        if (p == n.next.end())
            return false;
        bool found = erase(*p->second, s, i + 1);
        if (!p->second->word && p->second->next.empty())
            n.next.erase(p);
        return found;
    }
    static void match(const Node &n, const string &s, size_t i, string path, vector<string> &out) {
        if (i == s.size()) {
            if (n.word)
                out.push_back(path);
            return;
        }
        for (const auto &p : n.next)
            if (s[i] == '?' || s[i] == p.first)
                match(*p.second, s, i + 1, path + p.first, out);
    }

  public:
    void insert(const string &s) {
        Node *p = &root;
        for (char c : s) {
            auto &q = p->next[c];
            if (!q)
                q = make_unique<Node>();
            p = q.get();
        }
        p->word = true;
    }
    bool search(const string &s) const {
        auto p = &root;
        for (char c : s) {
            auto q = p->next.find(c);
            if (q == p->next.end())
                return false;
            p = q->second.get();
        }
        return p->word;
    }
    bool remove(const string &s) {
        return erase(root, s, 0);
    }
    vector<string> mask(const string &s) const {
        vector<string> v;
        match(root, s, 0, "", v);
        return v;
    }
};
int main() {
    Tree t;
    vector<int> a{30, 20, 10, 25, 28, 40, 50, 5};
    for (int x : a) {
        t.insert(x);
        if (!t.valid())
            return 1;
    }
    for (int x : {20, 30, 5, 99}) {
        t.remove(x);
        if (!t.valid())
            return 1;
    }
    cout << "AVL " << t.valid() << ' ' << t.search(25) << '\n';
    Trie tr;
    for (string s : {"cat", "car", "cart", "dog", ""})
        tr.insert(s);
    for (auto &s : tr.mask("ca?"))
        cout << s << ' ';
    cout << '\n';
    tr.remove("car");
    cout << tr.search("car") << ' ' << tr.search("cart") << ' ' << tr.search("") << '\n';
    mt19937 gen(12);
    cout << "n kind avl_insert avl_find bst_insert bst_find\n";
    for (int n : {1000, 5000, 10000})
        for (int kind = 0; kind < 2; ++kind) {
            vector<int> v(n);
            for (int i = 0; i < n; ++i)
                v[i] = i;
            if (!kind)
                shuffle(v.begin(), v.end(), gen);
            cout << n << ' ' << kind;
            for (bool b : {true, false}) {
                Tree tree(b);
                auto start = chrono::steady_clock::now();
                for (int x : v)
                    tree.insert(x);
                auto mid = chrono::steady_clock::now();
                int found = 0;
                for (int x : v)
                    found += tree.search(x);
                auto end = chrono::steady_clock::now();
                if (found != n)
                    return 1;
                cout << ' ' << chrono::duration_cast<chrono::microseconds>(mid - start).count()
                     << ' ' << chrono::duration_cast<chrono::microseconds>(end - mid).count();
            }
            cout << '\n';
        }
}
