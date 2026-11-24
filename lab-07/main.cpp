#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

void quickSort(std::vector<int>& values, int left, int right) {
    if (left >= right)
        return;

    // choose middle pivot
    int pivot = values[left + (right - left) / 2];
    int less = left;
    int current = left;
    int greater = right;

    // three part partition
    while (current <= greater) {
        if (values[current] < pivot) {
            std::swap(values[less], values[current]);
            ++less;
            ++current;
        } else if (values[current] > pivot) {
            std::swap(values[current], values[greater]);
            --greater;
        } else {
            ++current;
        }
    }

    // skip equal values
    quickSort(values, left, less - 1);
    quickSort(values, greater + 1, right);
}

int main() {
    std::vector<int> sample = {4, 2, 4, 1, 3, 4, 2};
    quickSort(sample, 0, static_cast<int>(sample.size()) - 1);
    std::cout << "Sorted: ";
    for (int value : sample)
        std::cout << value << ' ';

    std::mt19937 random(12);
    std::cout << "\nn random_us repeats_us\n";

    for (int n : {100, 1000, 10000}) {
        std::cout << n;
        // vary duplicate frequency
        for (int distinct : {100000, 5}) {
            std::vector<int> values(n);
            for (int& value : values)
                value = static_cast<int>(random() % distinct);
            auto expected = values;
            std::sort(expected.begin(), expected.end());

            auto start = std::chrono::steady_clock::now();
            quickSort(values, 0, n - 1);
            auto elapsed = std::chrono::steady_clock::now() - start;
            if (values != expected)
                return 1;

            std::cout << ' ' << std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
        }
        std::cout << '\n';
    }
}
