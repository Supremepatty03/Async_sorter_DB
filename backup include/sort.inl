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
}