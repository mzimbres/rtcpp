#include <iostream>
#include <limits>

#include "rtcpp.hpp"

using namespace rt;

void sort_benchmark(int size, int repeat)
{
    auto first = std::numeric_limits<int>::min();
    auto last = std::numeric_limits<int>::max();

    auto vec = make_rand_data(size * repeat, first, last, 1);

    { // std::sort
        auto vec_copy = vec;
        timer t;
        for (auto i = 0; i < repeat; ++i) {
            auto begin = std::begin(vec_copy) + i * size;
            auto end = begin + size;
            std::sort(begin, end);
        }
        auto c = t.get_count();
        std::cout << c << " ";
    }

    { // Insertion sort
        auto vec_copy = vec;
        timer t;
        for (auto i = 0; i < repeat; ++i) {
            auto begin = std::begin(vec_copy) + i * size;
            auto end = begin + size;
            rt::straight_insertion(begin, end);
        }
        auto c = t.get_count();
        std::cout << c << " ";
    }

    { // Straight selection
        auto vec_copy = vec;
        timer t;
        for (auto i = 0; i < repeat; ++i) {
            auto begin = std::begin(vec_copy) + i * size;
            auto end = begin + size;
            rt::straight_selection(begin, end);
        }
        auto c = t.get_count();
        std::cout << c << " ";
    }

    std::cout << std::endl;
}

int main()
{
    auto size = 15;
    auto repeat = 80000;

    for (auto i = 0; i < 30; ++i) {
        auto s = size + i * 5;
        std::cout << s << " ";
        sort_benchmark(s, repeat);
    }
    std::cin.ignore();
}

