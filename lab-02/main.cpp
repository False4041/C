#include <iostream>
#include <string>

template <class T>
int countIf(const T arr[], int n, const T& value) {
    int count = 0;
    // подсчёт равных элементов
    for (int i = 0; i < n; ++i) {
        if (arr[i] == value)
            ++count;
    }
    return count;
}

int main() {
    // проверка двух типов
    int numbers[] = {4, 2, 4, 1, 4};
    std::string words[] = {"cat", "dog", "cat"};

    std::cout << "Number 4: " << countIf(numbers, 5, 4) << '\n';
    std::cout << "Word cat: " << countIf(words, 3, std::string("cat")) << '\n';
    std::cout << "Missing: " << countIf(numbers, 5, 8) << '\n';
    std::cout << "Empty: " << countIf(numbers, 0, 4) << '\n';
}
