//
// Created by jasmine on 12/31/23.
//
#include <vector>
#include <string>
#include <iostream>
#include <optional>
#include <boost/circular_buffer.hpp>

/*
* all the instructions I actually need are in avx512. I don't have avx512.
* I learned a lot about AVX, mostly about how I want AVX512.
* Buy AMD, peeps. Intel is shit.
* */
#if defined(__AVX2__) && !defined(__AVX512F__) && false
int f() {
    char buf_out[1024];
    size_t buf_out_pos = 0;

    size_t i = 0;
    for(; i + 63 < capacity; i += 64) {
        auto dat = _mm256_loadu_si256(reinterpret_cast<__m256i *>(file_contents + i));

        auto is_newline = _mm256_cmpeq_epi8(dat, _mm256_set1_epi8(static_cast<int>('\n')));
        unsigned int zeroes_before_newline = _lzcnt_u32(_mm256_movemask_pd(is_newline));
        // vpalignr doesn't cross the lane boundary
        auto low = _mm256_castps256_ps128(is_newline);
        auto high = _mm256_extractf128_ps(is_newline, 1);
        auto zeroes = _mm_setzero_ps();

        low = _mm_alignr_epi8(low, zeroes, zeroes_before_newline & 0xF);
        high = _mm_alignr_epi8(high, zeroes, zeroes_before_newline >> 4);

        const auto zero_char = _mm256_set1_epi8(static_cast<int>('0') - 1);
        const auto nine_char = _mm256_set1_epi8(static_cast<int>('9') + 1);
        auto is_digit = _mm256_cmpgt_epi8(dat, zero_char) & _mm256_cmpgt_epi8(nine_char, dat);

    }
}
#endif // false

template<typename Iterator>
std::optional<int> find_first_number(Iterator it, const Iterator& end, bool reverse) {
    static const char* forward_string_numbers[] = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };
    static const char* reverse_numbers[] = { "eno", "owt", "eerht", "ruof", "evif", "xis", "neves", "thgie", "enin" };
    auto buf = boost::circular_buffer<char>{5};

    const char** string_numbers = reverse ? reverse_numbers : forward_string_numbers;

    for(; it != end; ++it) {
        if(*it >= '0' && *it <= '9') {
            return *it - '0';
        } else {
            buf.push_back(*it);
            std::string contents{buf.begin(), buf.end()};
            for(size_t i = 0; i < 9; ++i) {
                size_t num_size = strlen(string_numbers[i]);
                if(contents.length() >= num_size) [[likely]] {
                    if(0 == contents.compare(contents.length() - num_size, num_size, string_numbers[i])) {
                        return i+1;
                    }
                }
            }
        }
    }
    return std::nullopt;
}

int day1_part1(std::vector<std::string> lines) {
    int sum = 0;
    for(auto line : lines) {
        if(line.empty()) {
            continue;
        }
        auto first_idx = line.find_first_of("0123456789");
        auto last_idx = line.find_last_of("0123456789");
        if(first_idx == std::string::npos || last_idx == std::string::npos) {
            continue;
        }
        sum += ((line[first_idx] -'0') * 10) + (line[last_idx] - '0');
    }
    return sum;
}

int day1_part2(std::vector<std::string> lines) {
    int sum = 0;
    for(auto line : lines) {
        if(line.empty()) {
            continue;
        }
        auto first = find_first_number(line.begin(), line.end(), false);
        auto last = find_first_number(line.rbegin(), line.rend(), true);
        if(first && last) {
            auto calibration = (*first * 10) + *last;
            std::cout << "Calibration: " << calibration << std::endl;
            sum += calibration;
        }
    }
    return sum;
}