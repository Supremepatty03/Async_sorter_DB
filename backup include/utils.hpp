#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <string>

namespace my_sort_lib 
{

    void show_data(const std::vector<std::string>& data);
    std::vector<std::string> default_data();
    std::string read_until_space();

}

#endif // UTILS_HPP