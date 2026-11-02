#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <stdexcept>
using namespace std;
class Food {
  public:
    string name;
    double calories, price, weight;
    Food(string n, double c, double p, double w) : name(n), calories(c), price(p), weight(w) {
        if (n.empty() || !isfinite(c) || !isfinite(p) || !isfinite(w) || c < 0 || p < 0 || w < 0)
            throw invalid_argument("Value");
    }
    virtual ~Food() = default;
    virtual string type() const = 0;
    virtual void print() const = 0;
    void row() const {
        cout << type() << ' ' << quoted(name) << ' ' << calories << ' ' << price << ' ' << weight
             << '\n';
    }
    void save(ostream &out) const {
        out << type() << ' ' << quoted(name) << ' ' << setprecision(17) << calories << ' ' << price
            << ' ' << weight << '\n';
    }
};
class Fruit : public Food {
  public:
    using Food::Food;
    string type() const override {
        return "fruit";
    }
    void print() const override {
        row();
    }
};
class Vegetable : public Food {
  public:
    using Food::Food;
    string type() const override {
        return "vegetable";
    }
    void print() const override {
        row();
    }
};
class Meat : public Food {
  public:
    using Food::Food;
    string type() const override {
        return "meat";
    }
    void print() const override {
        row();
    }
};
unique_ptr<Food> readFood(istream &in) {
    string type, name;
    double c, p, w;
    if (!(in >> type >> quoted(name) >> c >> p >> w))
        throw runtime_error("Data");
    if (type == "fruit")
        return make_unique<Fruit>(name, c, p, w);
    if (type == "vegetable")
        return make_unique<Vegetable>(name, c, p, w);
    if (type == "meat")
        return make_unique<Meat>(name, c, p, w);
    throw runtime_error("Type");
}
void endLine(istream &in) {
    in >> ws;
    if (!in.eof())
        throw runtime_error("Extra input");
}
int main() {
    vector<unique_ptr<Food>> foods;
    string line;
    cout << "1 Add 2 List 3 Sort 4 Find 5 Remove 6 Save 7 Load 0 Exit\n";
    while (getline(cin, line)) {
        try {
            istringstream in(line);
            int cmd;
            if (!(in >> cmd))
                throw runtime_error("Input");
            if (cmd == 0) {
                endLine(in);
                break;
            }
            if (cmd == 1) {
                auto f = readFood(in);
                endLine(in);
                foods.push_back(move(f));
                cout << "Added\n";
            } else if (cmd == 2) {
                endLine(in);
                for (size_t i = 0; i < foods.size(); ++i) {
                    cout << i << ' ';
                    foods[i]->print();
                }
            } else if (cmd == 3) {
                int field;
                if (!(in >> field) || field < 1 || field > 3)
                    throw runtime_error("Field");
                endLine(in);
                stable_sort(foods.begin(), foods.end(), [field](const auto &a, const auto &b) {
                    if (field == 1)
                        return a->calories < b->calories;
                    if (field == 2)
                        return a->price < b->price;
                    return a->weight < b->weight;
                });
                cout << "Sorted\n";
            } else if (cmd == 4) {
                int field;
                string key;
                if (!(in >> field >> quoted(key)) || (field != 1 && field != 2))
                    throw runtime_error("Field");
                endLine(in);
                bool found = false;
                for (auto &f : foods)
                    if (field == 1 ? f->name.find(key) != string::npos : f->type() == key) {
                        f->print();
                        found = true;
                    }
                if (!found)
                    cout << "Not found\n";
            } else if (cmd == 5) {
                size_t i;
                if (!(in >> i))
                    throw runtime_error("Index");
                endLine(in);
                if (i >= foods.size())
                    throw out_of_range("Index");
                foods.erase(foods.begin() + i);
                cout << "Removed\n";
            } else if (cmd == 6 || cmd == 7) {
                string path;
                if (!(in >> quoted(path)))
                    throw runtime_error("Path");
                endLine(in);
                if (cmd == 6) {
                    ofstream f(path);
                    if (!f)
                        throw runtime_error("Open");
                    f << foods.size() << '\n';
                    for (auto &v : foods)
                        v->save(f);
                    f.close();
                    if (!f)
                        throw runtime_error("Write");
                    cout << "Saved\n";
                } else {
                    ifstream f(path);
                    int n;
                    if (!(f >> n) || n < 0 || n > 10000)
                        throw runtime_error("File");
                    vector<unique_ptr<Food>> loaded;
                    for (int i = 0; i < n; ++i)
                        loaded.push_back(readFood(f));
                    endLine(f);
                    foods.swap(loaded);
                    cout << "Loaded\n";
                }
            } else
                throw runtime_error("Command");
        } catch (const exception &e) {
            cout << "Error: " << e.what() << '\n';
        }
    }
}
