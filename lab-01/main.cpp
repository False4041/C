#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
using namespace std;
class Session {
    string time;

  public:
    explicit Session(string t) : time(t) {
        if (t.empty())
            throw invalid_argument("Time");
    }
    void print() const {
        cout << time << ' ';
    }
};
class Movie {
    string title;
    vector<Session> sessions;
    static int count;

  public:
    explicit Movie(string t) : title(t) {
        ++count;
    }
    Movie(const Movie &m) : title(m.title), sessions(m.sessions) {
        ++count;
    }
    Movie &operator=(const Movie &) = default;
    ~Movie() {
        --count;
    }
    void add(string t) {
        sessions.emplace_back(t);
    }
    static int total() {
        return count;
    }
    void print() const {
        cout << title << ": ";
        for (const auto &s : sessions)
            s.print();
        cout << '\n';
    }
};
int Movie::count = 0;
class Cinema {
    vector<Movie *> movies;
    Cinema() = default;

  public:
    Cinema(const Cinema &) = delete;
    Cinema &operator=(const Cinema &) = delete;
    static Cinema &instance() {
        static Cinema c;
        return c;
    }
    void add(Movie *m) {
        if (!m)
            throw invalid_argument("Null movie");
        for (auto p : movies)
            if (p == m)
                return;
        movies.push_back(m);
    }
    void clear() {
        movies.clear();
    }
    void print() const {
        for (auto m : movies)
            m->print();
    }
};
int main() {
    cout << "Count " << Movie::total() << '\n';
    {
        Movie a("Dune"), b("Up");
        a.add("18:00");
        a.add("20:00");
        b.add("16:00");
        auto &c = Cinema::instance();
        c.add(&a);
        c.add(&b);
        c.add(&a);
        c.print();
        cout << "Count " << Movie::total() << '\n';
        cout << "Same " << (&c == &Cinema::instance()) << '\n';
        try {
            c.add(nullptr);
        } catch (const exception &e) {
            cout << e.what() << '\n';
        }
        c.clear();
        a.print();
    }
    cout << "Count " << Movie::total() << '\n';
}
