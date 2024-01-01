cmake_minimum_required(VERSION 3.27)
project(AdventOfCode2023)

find_package(Boost REQUIRED)

# Day 1 is text processing. What if we do that in AVX?
add_executable(AdventOfCode2023 main.cpp
        day1.cpp
        lines.hpp
        day1.h
)
set_target_properties(AdventOfCode2023 PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(AdventOfCode2023 PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(AdventOfCode2023 PRIVATE ${Boost_LIBRARIES})
target_compile_definitions(AdventOfCode2023 PRIVATE
        # Pass DEBUG as a compile flag if we're in debug release
        $<$<CONFIG:Debug>:DEBUG>
)

# Day 24 is obviously raytracing, so let's do CUDA
#set(CMAKE_CUDA_STANDARD 23)
#
#add_executable(AdventOfCode2023Day24
#        day24.cu)
#set_target_properties(AdventOfCode2023Day24 PROPERTIES
#        CUDA_SEPARABLE_COMPILATION ON)
#target_compile_definitions(AdventOfCode2023Day24 PRIVATE
#        # Pass DEBUG as a compile flag if we're in debug release
#        $<$<CONFIG:Debug>:DEBUG>
#)