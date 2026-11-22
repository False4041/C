#include <iostream> // headers
#include <string>
#include <vector>
#include <stdexcept>
using namespace std;
class Session { // session
    string time;

  public:
    explicit Session(string t) : time(t) { // check time
        if (t.empty())
            throw invalid_argument("Time");
    }
    void print() const { // print time
        cout << time << ' ';
    }
};
class Movie { // movie
    string title;
    vector<Session> sessions; // composition
    static int count; // shared counter

  public:
    explicit Movie(string t) : title(t) { // create movie
        ++count;
    }
    Movie(const Movie &m) : title(m.title), sessions(m.sessions) { // copy movie
        ++count;
    }
    Movie &operator=(const Movie &) = default; // copy assignment
    ~Movie() { // destroy movie
        --count;
    }
    void add(string t) { // add session
        sessions.emplace_back(t);
    }
    static int total() { // movie count
        return count;
    }
    void print() const { // print movie
        cout << title << ": ";
        for (const auto &s : sessions)
            s.print();
        cout << '\n';
    }
};
int Movie::count = 0; // initial count
class Cinema { // cinema
    vector<Movie *> movies; // aggregation
    Cinema() = default;

  public:
    Cinema(const Cinema &) = delete; // disable copying
    Cinema &operator=(const Cinema &) = delete;
    static Cinema &instance() { // singleton
        static Cinema c;
        return c;
    }
    void add(Movie *m) { // add movie
        if (!m)
            throw invalid_argument("Null movie");
        for (auto p : movies) // skip duplicates
            if (p == m)
                return;
        movies.push_back(m);
    }
    void clear() { // clear pointers
        movies.clear();
    }
    void print() const { // print movies
        for (auto m : movies)
            m->print();
    }
};
int main() { // demo
    cout << "Count " << Movie::total() << '\n';
    {
        Movie a("Dune"), b("Up"); // sample movies
        a.add("18:00");
        a.add("20:00");
        b.add("16:00");
        auto &c = Cinema::instance(); // get cinema
        c.add(&a);
        c.add(&b);
        c.add(&a);
        c.print();
        cout << "Count " << Movie::total() << '\n';
        cout << "Same " << (&c == &Cinema::instance()) << '\n'; // check singleton
        try { // check error
            c.add(nullptr);
        } catch (const exception &e) {
            cout << e.what() << '\n';
        }
        c.clear(); // clear cinema
        a.print(); // movie remains
    }
    cout << "Count " << Movie::total() << '\n';
}
