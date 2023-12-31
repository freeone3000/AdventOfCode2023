cmake_minimum_required(VERSION 3.27)
#project(AdventOfCode2023 CUDA)
project(AdventOfCode2023)

#set(CMAKE_CUDA_STANDARD 23)

add_executable(AdventOfCode2023 main.cpp
        day1.cpp
        lines.cpp
)
target_compile_options(AdventOfCode2023 PRIVATE -mavx -mavx2)

#set_target_properties(AdventOfCode2023 PROPERTIES
#        CUDA_SEPARABLE_COMPILATION ON)
