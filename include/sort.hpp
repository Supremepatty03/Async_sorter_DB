#ifndef SORT_HPP
#define SORT_HPP

#include <vector>
#include <string>

namespace my_sort_lib {
    template <typename T>
    std::vector<T> shaker_sort(std::vector<T> data);

    template <typename T>
    std::vector<T> number_sort(std::vector<T> data);
}


#include "sort.inl" // Реализация шаблонных функций

#endif // SORT_HPP