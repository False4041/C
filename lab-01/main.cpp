#include <iostream> // заголовки
#include <string>
#include <vector>
#include <stdexcept>
using namespace std;
class Session { // сеанс
    string time;

  public:
    explicit Session(string t) : time(t) { // проверка времени
        if (t.empty())
            throw invalid_argument("Time");
    }
    void print() const { // вывод времени
        cout << time << ' ';
    }
};
class Movie { // фильм
    string title;
    vector<Session> sessions; // композиция
    static int count; // общий счётчик

  public:
    explicit Movie(string t) : title(t) { // создание фильма
        ++count;
    }
    Movie(const Movie &m) : title(m.title), sessions(m.sessions) { // копирование фильма
        ++count;
    }
    Movie &operator=(const Movie &) = default; // присваивание копии
    ~Movie() { // деструктор фильма
        --count;
    }
    void add(string t) { // добавление сеанса
        sessions.emplace_back(t);
    }
    static int total() { // число фильмов
        return count;
    }
    void print() const { // вывод фильма
        cout << title << ": ";
        for (const auto &s : sessions)
            s.print();
        cout << '\n';
    }
};
int Movie::count = 0; // начальное значение
class Cinema { // кинотеатр
    vector<Movie *> movies; // агрегация
    Cinema() = default;

  public:
    Cinema(const Cinema &) = delete; // запрет копирования
    Cinema &operator=(const Cinema &) = delete;
    static Cinema &instance() { // одиночка
        static Cinema c;
        return c;
    }
    void add(Movie *m) { // добавление фильма
        if (!m)
            throw invalid_argument("Null movie");
        for (auto p : movies) // проверка дубликатов
            if (p == m)
                return;
        movies.push_back(m);
    }
    void clear() { // очистка указателей
        movies.clear();
    }
    void print() const { // вывод фильмов
        for (auto m : movies)
            m->print();
    }
};
int main() { // пример
    cout << "Count " << Movie::total() << '\n';
    {
        Movie a("Dune"), b("Up"); // создание фильмов
        a.add("18:00");
        a.add("20:00");
        b.add("16:00");
        auto &c = Cinema::instance(); // получение кинотеатра
        c.add(&a);
        c.add(&b);
        c.add(&a);
        c.print();
        cout << "Count " << Movie::total() << '\n';
        cout << "Same " << (&c == &Cinema::instance()) << '\n'; // проверка экземпляра
        try { // проверка ошибки
            c.add(nullptr);
        } catch (const exception &e) {
            cout << e.what() << '\n';
        }
        c.clear(); // очистка связей
        a.print(); // фильм остался
    }
    cout << "Count " << Movie::total() << '\n';
}
