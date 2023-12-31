//
// Created by jasmine on 12/31/23.
//
#ifdef __AVX2__
#include <immintrin.h>
#endif // __AVX2__

#include <vector>
#include <string>

int day1_part1(std::vector<std::string> lines) {
    /*
     * all the instructions I actually need are in avx512. I don't have avx512.
     * I learned a lot about AVX, mostly about how I want AVX512.
     * Buy AMD, peeps. Intel is shit.
     * */
#if defined(__AVX2__) && !defined(__AVX512F__) && false
    char buf_out[1024];
    size_t buf_out_pos = 0;

    size_t i = 0;
    for(; i + 63 < len; i += 64) {
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
#endif // false
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