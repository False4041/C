#include <cstddef>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <utility>

template <class T>
class Vector {
    std::size_t length = 0;
    std::size_t space = 0;
    std::unique_ptr<T[]> data;

    void grow() {
        std::size_t newSpace = space == 0 ? 1 : 2 * space;
        auto newData = std::make_unique<T[]>(newSpace);
        for (std::size_t i = 0; i < length; ++i)
            newData[i] = data[i];
        data = std::move(newData);
        space = newSpace;
    }

public:
    Vector() = default;

    Vector(const Vector& other) : length(other.length), space(other.space) {
        if (space != 0)
            data = std::make_unique<T[]>(space);
        for (std::size_t i = 0; i < length; ++i)
            data[i] = other.data[i];
    }

    Vector& operator=(const Vector& other) {
        Vector copy(other);
        data.swap(copy.data);
        std::swap(length, copy.length);
        std::swap(space, copy.space);
        return *this;
    }

    void push_back(T value) {
        if (length == space)
            grow();
        data[length] = value;
        ++length;
    }

    void erase(std::size_t index) {
        if (index >= length)
            throw std::out_of_range("Index");
        for (std::size_t i = index; i + 1 < length; ++i)
            data[i] = data[i + 1];
        data[length - 1] = T{};
        --length;
    }

    std::size_t size() const {
        return length;
    }

    std::size_t capacity() const {
        return space;
    }

    void print() const {
        for (std::size_t i = 0; i < length; ++i)
            std::cout << data[i] << ' ';
        std::cout << '\n';
    }
};

int main() {
    Vector<int> numbers;
    numbers.push_back(10);
    numbers.push_back(20);
    numbers.push_back(30);
    std::cout << "Size " << numbers.size() << ", capacity " << numbers.capacity() << '\n';

    Vector<int> copy(numbers);
    copy.erase(1);
    std::cout << "Original: ";
    numbers.print();
    std::cout << "Copy: ";
    copy.print();
    std::cout << "Copy size " << copy.size() << ", capacity " << copy.capacity() << '\n';
}
