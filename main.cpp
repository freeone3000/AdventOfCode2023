#include <iostream>

#include "day1.h"
#include "lines.hpp"


int main() {
    auto x = run_function_on_file<int>("data/day1_file.txt", &day1_part1);
    std::cout << "Part 1: " << x << std::endl;
    int y = run_function_on_file<int>("data/day1_file.txt", &day1_part2);
    std::cout << "Part 2 (test): " << y << std::endl;
}