#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdexcept>
using namespace std;
class Event {
  protected:
    string name;
    int day, minute;
    vector<string> people;

  public:
    Event(string n, int d, int m) : name(n), day(d), minute(m) {
        if (n.empty() || d < 1 || d > 31 || m < 0 || m >= 1440)
            throw invalid_argument("Event");
    }
    virtual ~Event() = default;
    virtual string kind() const = 0;
    virtual string extra() const = 0;
    virtual bool due(int, int) const {
        return false;
    }
    void add(string p) {
        if (p.empty())
            throw invalid_argument("Name");
        for (auto &s : people)
            if (s == p)
                return;
        people.push_back(p);
    }
    virtual void print() const {
        cout << kind() << ' ' << name << ' ' << day << ' ' << minute << ' ' << extra();
        for (auto &p : people)
            cout << ' ' << p;
        cout << '\n';
    }
    void save(ostream &out) const {
        out << kind() << ' ' << quoted(name) << ' ' << day << ' ' << minute << ' '
            << quoted(extra()) << ' ' << people.size();
        for (auto &p : people)
            out << ' ' << quoted(p);
        out << '\n';
    }
};
class Meeting : public Event {
    string room;

  public:
    Meeting(string n, int d, int m, string r) : Event(n, d, m), room(r) {
        if (r.empty())
            throw invalid_argument("Room");
    }
    string kind() const override {
        return "meeting";
    }
    string extra() const override {
        return room;
    }
    void print() const override {
        Event::print();
    }
};
class Reminder : public Event {
    int lead;

  public:
    Reminder(string n, int d, int m, int l) : Event(n, d, m), lead(l) {
        if (l < 0 || l > 1440)
            throw invalid_argument("Lead");
    }
    string kind() const override {
        return "reminder";
    }
    string extra() const override {
        return to_string(lead);
    }
    bool due(int d, int m) const override {
        int now = (d - 1) * 1440 + m, start = (day - 1) * 1440 + minute;
        return now >= start - lead && now <= start;
    }
    void print() const override {
        Event::print();
    }
};
unique_ptr<Event> readEvent(istream &in) {
    string type, name, extra;
    int day, minute, count;
    if (!(in >> type >> quoted(name) >> day >> minute >> quoted(extra) >> count) || count < 0 ||
        count > 100)
        throw runtime_error("Data");
    unique_ptr<Event> e;
    if (type == "meeting")
        e = make_unique<Meeting>(name, day, minute, extra);
    else if (type == "reminder") {
        size_t used;
        int lead = stoi(extra, &used);
        if (used != extra.size())
            throw runtime_error("Lead");
        e = make_unique<Reminder>(name, day, minute, lead);
    } else
        throw runtime_error("Type");
    for (int i = 0; i < count; ++i) {
        string p;
        if (!(in >> quoted(p)))
            throw runtime_error("Person");
        e->add(p);
    }
    return e;
}
void endLine(istream &in) {
    in >> ws;
    if (!in.eof())
        throw runtime_error("Extra input");
}
int main() {
    vector<unique_ptr<Event>> events;
    string line;
    cout << "1 Add 2 List 3 Remove 4 Save 5 Load 6 Due 7 Person 0 Exit\n";
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
                auto e = readEvent(in);
                endLine(in);
                events.push_back(move(e));
                cout << "Added\n";
            } else if (cmd == 2) {
                endLine(in);
                for (size_t i = 0; i < events.size(); ++i) {
                    cout << i << ' ';
                    events[i]->print();
                }
            } else if (cmd == 3) {
                size_t i;
                if (!(in >> i))
                    throw runtime_error("Index");
                endLine(in);
                if (i >= events.size())
                    throw out_of_range("Index");
                events.erase(events.begin() + i);
                cout << "Removed\n";
            } else if (cmd == 4 || cmd == 5) {
                string path;
                if (!(in >> quoted(path)))
                    throw runtime_error("Path");
                endLine(in);
                if (cmd == 4) {
                    ofstream f(path);
                    if (!f)
                        throw runtime_error("Open");
                    f << events.size() << '\n';
                    for (auto &e : events)
                        e->save(f);
                    f.close();
                    if (!f)
                        throw runtime_error("Write");
                    cout << "Saved\n";
                } else {
                    ifstream f(path);
                    int n;
                    if (!(f >> n) || n < 0 || n > 10000)
                        throw runtime_error("File");
                    vector<unique_ptr<Event>> loaded;
                    for (int i = 0; i < n; ++i)
                        loaded.push_back(readEvent(f));
                    endLine(f);
                    events.swap(loaded);
                    cout << "Loaded\n";
                }
            } else if (cmd == 6) {
                int d, m;
                if (!(in >> d >> m) || d < 1 || d > 31 || m < 0 || m >= 1440)
                    throw runtime_error("Time");
                endLine(in);
                for (auto &e : events)
                    if (e->due(d, m))
                        e->print();
            } else if (cmd == 7) {
                size_t i;
                string person;
                if (!(in >> i >> quoted(person)))
                    throw runtime_error("Input");
                endLine(in);
                if (i >= events.size())
                    throw out_of_range("Index");
                events[i]->add(person);
                cout << "Added\n";
            } else
                throw runtime_error("Command");
        } catch (const exception &e) {
            cout << "Error: " << e.what() << '\n';
        }
    }
}
