//
// Created by jasmine on 1/1/24.
//

#include "day4.h"

#include <cstring>
#include <immintrin.h>

#include "lines.hpp"

inline char get_two_digit_int(const char *line_c) {
    return ((line_c[0] - '0') * 10) + (line_c[1] - '0');
}

// lol we don't even need to loop
int solve_one_day(uint8_t game[32], uint8_t* wins, size_t num_wins) {
    auto dat = _mm256_loadu_si256(reinterpret_cast<__m256i*>(game));
    int total_wins = 0;
    for(size_t i = 0; i < num_wins; ++i) {
        auto cmp = _mm256_set1_epi8(wins[i]);
        auto mask = _mm256_movemask_epi8(_mm256_cmpeq_epi8(dat, cmp));
        int more_wins = __builtin_popcount(mask);
        if(more_wins) {
            ++total_wins;
        }
    }
    int points = (1 << total_wins) >> 1;
    return points;
}

int day4_part1(Mmap input) {
    char* line_c = input.buf;
    char* end = line_c + input.len; // NEVER dereference!

    int score = 0;
    do {
        bool is_test = false;
        if (line_c[8] == ':') [[likely]] {
            line_c += 10;
        } else if (line_c[6] == ':') {
            line_c += 8;
            is_test = true; // use smaller data sizes
        } else [[unlikely]] {
            line_c = strchr(line_c, ':') + 1;
        }

        size_t win_size = is_test ? 5 : 10;
        size_t game_size = is_test ? 8 : 25;
        // allocate enough space for a ymm reg even if we don't need it immediately
        uint8_t winning_nums[win_size];
        uint8_t game[32] = { 0 }; // need zero-init, else we get stack data

        for (size_t i = 0; i < win_size; ++i) {
            winning_nums[i] = get_two_digit_int(line_c);
            line_c += 3;
        }
        line_c += 2;
        for (size_t i = 0; i < game_size; ++i) {
            game[i] = get_two_digit_int(line_c);
            line_c += 3;
        }

        // test game
        score += solve_one_day(game, winning_nums, win_size);
    } while(line_c < end && line_c + 1 < end);

    return score;
}
