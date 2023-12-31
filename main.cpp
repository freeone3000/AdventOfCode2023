#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "day1.h"

template<typename R>
R run_function_on_file(const char* fn, int(*func)(std::vector<std::string>)) {
    std::vector<std::string> lines;
    std::ifstream in(fn, std::ios_base::in);
    for(std::string line; std::getline(in, line); ) {
        lines.push_back(line);
    }

    return func(lines);
}


int main() {
    auto x = run_function_on_file<int>("data/day1_file.txt", &day1_part1);
    std::cout << x << std::endl;
}