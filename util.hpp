//
// Created by jasmine on 1/1/24.
//

#ifndef ADVENTOFCODE2023_UTIL_HPP
#define ADVENTOFCODE2023_UTIL_HPP

#include <vector>
#include <string>
#include <sstream>

using std::vector, std::string, std::stringstream;

template<typename R>
vector<R> extract_repeat(const string& in) {
    stringstream ss{in};
    vector<R> out;
    R tmp;
    while(ss >> tmp) {
        out.push_back(tmp);
    }
    return out;
}

#endif //ADVENTOFCODE2023_UTIL_HPP
