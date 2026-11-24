#include <iostream>
#include <stdexcept>
#include <utility>

template <class T>
class SinglyLinkedList {
    struct Node {
        T value;
        Node* next;
    };

    Node* head = nullptr;
    Node* tail = nullptr;

public:
    SinglyLinkedList() = default;

    // copy each node
    SinglyLinkedList(const SinglyLinkedList& other) {
        SinglyLinkedList copy;
        for (Node* node = other.head; node != nullptr; node = node->next)
            copy.push_back(node->value);
        std::swap(head, copy.head);
        std::swap(tail, copy.tail);
    }

    SinglyLinkedList& operator=(const SinglyLinkedList& other) {
        SinglyLinkedList copy(other);
        std::swap(head, copy.head);
        std::swap(tail, copy.tail);
        return *this;
    }

    // release all nodes
    ~SinglyLinkedList() {
        while (head != nullptr) {
            Node* node = head;
            head = head->next;
            delete node;
        }
    }

    bool empty() const {
        return head == nullptr;
    }

    // attach new tail
    void push_back(const T& value) {
        Node* node = new Node{value, nullptr};
        if (empty())
            head = node;
        else
            tail->next = node;
        tail = node;
    }

    T pop_back() {
        if (empty())
            throw std::out_of_range("Empty list");
        T value = tail->value;
        if (head == tail) {
            delete tail;
            head = nullptr;
            tail = nullptr;
        } else {
            // find previous node
            Node* previous = head;
            while (previous->next != tail)
                previous = previous->next;
            delete tail;
            tail = previous;
            tail->next = nullptr;
        }
        return value;
    }

    void print() const {
        for (Node* node = head; node != nullptr; node = node->next)
            std::cout << node->value << ' ';
        std::cout << '\n';
    }
};

int main() {
    SinglyLinkedList<int> numbers;
    std::cout << "Empty: " << std::boolalpha << numbers.empty() << '\n';
    numbers.push_back(10);
    numbers.push_back(20);
    numbers.push_back(30);

    SinglyLinkedList<int> copy(numbers);
    std::cout << "Removed from copy: " << copy.pop_back() << '\n';
    std::cout << "Original: ";
    numbers.print();
    std::cout << "Copy: ";
    copy.print();
}
