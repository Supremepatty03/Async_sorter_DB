#ifndef SORT_HPP
#define SORT_HPP

#include <vector>
#include <string>

namespace my_sort_lib {
    template <typename T>
    std::vector<T> shaker_sort(std::vector<T> data);

    std::vector<std::string> number_sort(std::vector<std::string> data);
}


#include "sort.inl"

#endif // SORT_HPP
