#include <iostream>
#include <string>

class Document {
    class Page {
        int number;
        std::string content;

    public:
        Page(int pageNumber, const std::string& text) : number(pageNumber), content(text) {
            std::cout << "Page " << number << " created\n";
        }

        ~Page() {
            std::cout << "Page " << number << " destroyed\n";
        }

        void print() const {
            std::cout << number << ": " << content << '\n';
        }
    };

    // страницы внутри документа
    Page first;
    Page second;

public:
    // начальные значения страниц
    Document(const std::string& intro, const std::string& summary)
        : first(1, intro), second(2, summary) {
        std::cout << "Document created\n";
    }

    ~Document() {
        std::cout << "Document destroyed\n";
    }

    void print() const {
        first.print();
        second.print();
    }
};

int main() {
    // время жизни документа
    {
        Document document("Introduction", "Summary");
        document.print();
    }
    std::cout << "End\n";
}
