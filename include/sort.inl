#include <vector>
#include <string>

namespace my_sort_lib {
    template <typename T>
    std::vector<T> shaker_sort(std::vector<T> data) {
        bool swapped = true;
        int n = 0;
        while (swapped) {
            swapped = false;
            for (int i = 0; i < data.size() - n - 1; i++) {
                if (data[i] > data[i + 1]) {
                    std::swap(data[i], data[i + 1]);
                    swapped = true;
                }
            }
            if (!swapped) break;
            n++;
            for (int i = static_cast<int>(data.size()) - n - 1; i > 0; i--) {
                if (data[i] < data[i - 1]) {
                    std::swap(data[i], data[i - 1]);
                    swapped = true;
                }
            }
        }
        return data;
    }
    template <typename T>
    std::vector<T> number_sort(std::vector<T> data) {
        std::vector<int> numbers;
        int first_index = -1;

        for (size_t i = 0; i < data.size(); i++) {
            try {
                int tmp = std::stoi(data[i]);  
                if (std::to_string(tmp) == data[i]) { 
                    numbers.emplace_back(tmp);
                    if (first_index == -1) {
                        first_index = static_cast<int>(i);
                    }
                }
            }
            catch (...) {
                continue;
            }
        }

        if (first_index != -1) {
            std::vector<int> sorted_numbers = shaker_sort(numbers);
            data.erase(data.begin() + first_index, data.begin() + first_index + numbers.size());
            for (size_t i = 0; i < sorted_numbers.size(); i++) {
                data.insert(data.begin() + first_index + i, std::to_string(sorted_numbers[i]));  // Используем std::to_string
            }
        }

        return data;
    }
}