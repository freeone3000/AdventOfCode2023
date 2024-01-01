cmake_minimum_required(VERSION 3.27)
project(AdventOfCode2023 CUDA)

# Day 1 is text processing. What if we do that in AVX?
add_executable(AdventOfCode2023 main.cpp
        day1.cpp
        lines.hpp
)
set_target_properties(AdventOfCode2023 PROPERTIES LINKER_LANGUAGE CXX)
target_compile_options(AdventOfCode2023 PRIVATE -mavx -mavx2)

# Day 24 is obviously raytracing, so let's do CUDA
set(CMAKE_CUDA_STANDARD 23)

add_executable(AdventOfCode2023Day24
        day24.cu)
set_target_properties(AdventOfCode2023Day24 PROPERTIES
        CUDA_SEPARABLE_COMPILATION ON)
target_compile_definitions(AdventOfCode2023Day24 PRIVATE
        # Pass DEBUG as a compile flag if we're in debug release
        $<$<CONFIG:Debug>:DEBUG>
)