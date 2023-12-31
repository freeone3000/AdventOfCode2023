cmake_minimum_required(VERSION 3.27)
project(AdventOfCode2023 CUDA)

set(CMAKE_CUDA_STANDARD 23)

add_executable(AdventOfCode2023 main.cu)

set_target_properties(AdventOfCode2023 PROPERTIES
        CUDA_SEPARABLE_COMPILATION ON)
