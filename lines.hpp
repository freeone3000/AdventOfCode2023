#ifndef ADVENTOFCODE2023_LINES_HPP
#define ADVENTOFCODE2023_LINES_HPP

#include <vector>
#include <string>
#include <fstream>
#include <functional>

template<typename R>
R run_function_on_file(const char* fn, const std::function<int(std::vector<std::string>)>& func) {
    std::vector<std::string> lines;
    std::ifstream in(fn, std::ios_base::in);
    for(std::string line; std::getline(in, line); ) {
        lines.push_back(line);
    }

    return func(lines);
}

#endif //ADVENTOFCODE2023_LINES_HPP