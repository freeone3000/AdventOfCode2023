//
// Created by jasmine on 12/31/23.
// This obviously is implementing ray tracing. So, let's see about RTX here.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"
#pragma ide diagnostic ignored "bugprone-reserved-identifier"
// for sscanf_s
#define __STDC_WANT_LIB_EXT1__ 1
#include <cstdio>
#pragma clang diagnostic pop

#include <unistd.h>
#include <iostream>
#include <utility>
#include <vector>
#include <string>

// cuda
#include <cuda_runtime.h>

#include "lines.hpp"

using std::nullopt;

#define CHECK_CUDA(val) check_cuda( (val), #val, __FILE__, __LINE__ )
void check_cuda(cudaError_t result, char const* const func, const char* const file, int const line) {
    if (result) {
        std::cerr << "CUDA error = " << static_cast<unsigned int>(result) << " at " <<
                  file << ":" << line << " '" << func << "' \n";
        // Make sure we call CUDA Device Reset before exiting
        cudaDeviceReset();
        exit(99);
    }
}

// max size for position real problem is 400000000000000, which is 2**48.5. We need 64 bit.
// but we can fit velocity in a short!
// for alignas, see https://docs.nvidia.com/cuda/cuda-c-programming-guide/index.html#vector-types-alignment-requirements-in-device-code
struct alignas(8) Hailstone {
public:
    long3 position;
    short3 velocity;
    /** for printing */
    friend std::ostream& operator<<(std::ostream& os, const Hailstone& hailstone) {
        os << hailstone.position.x << ", " << hailstone.position.y << ", " << hailstone.position.z << " @ " <<
           hailstone.velocity.x << ", " << hailstone.velocity.y << ", " << hailstone.velocity.z;
        return os;
    }
};

float time_intersect(long p0, long p1, short v0, short v1) {
    // p0 + v0 * t = p1 + v1 * t
    // p0 - p1 = (v1 - v0) * t
    // (p0 - p1) / (v1 - v0) = t
    // if v0 == v1, then we have a collision
    // if v0 != v1, then we have a collision at t = (p0 - p1) / (v1 - v0)
    // if v0 == v1, then we have a collision at t = 0
    if(v0 == v1) {
        return 0;
    }
    return static_cast<float>(p0 - p1) / static_cast<float>(v1 - v0);
}

float position_at_time(long p, short v, float t) {
    return static_cast<float>(p) + static_cast<float>(v) * t;
}

std::optional<float2> check_intersection(Hailstone a, Hailstone b, const long2& disc) {
    //check x
    float t = time_intersect(a.position.x, b.position.x, a.velocity.x, b.velocity.x);
    if(t < 0) {
        return nullopt;
    }
    float x0 = position_at_time(a.position.x, a.velocity.x, t);
    float x1 = position_at_time(b.position.x, b.velocity.x, t);
    if(x0 != x1 || static_cast<long>(x0) < disc.x || static_cast<long>(x0) > disc.y) {
        return nullopt;
    }
    //check y
    t = time_intersect(a.position.y, b.position.y, a.velocity.y, b.velocity.y);
    if(t < 0) {
        return nullopt;
    }
    float y0 = position_at_time(a.position.y, a.velocity.y, t);
    float y1 = position_at_time(b.position.y, b.velocity.y, t);
    if(y0 != y1 || static_cast<long>(y0) < disc.x || static_cast<long>(y0) > disc.y) {
        return nullopt;
    }
    return make_float2(x0, y0);
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err34-c" // ignore sscanf
int raytrace_on_file(std::vector<std::string> items, long2 target_minmax) {
    const size_t num_hailstones = items.size();
    Hailstone hailstones[num_hailstones];

    size_t hail_idx = 0;
    for(auto it = items.begin(); it != items.end(); ++it, ++hail_idx) {
        int64_t x, y, z;
        int16_t vx, vy, vz;
#ifdef __STDC_LIB_EXT1__
        sscanf_s((*it).c_str(), "%ld, %ld, %ld @ %hd, %hd, %hd", &x, &y, &z, &vx, &vy, &vz, item.length());
#else
        sscanf((*it).c_str(), "%ld, %ld, %ld @ %hd, %hd, %hd", &x, &y, &z, &vx, &vy, &vz);
#endif //__STDC_LIB_EXT1__
        hailstones[hail_idx] = Hailstone{make_long3(x, y, z), make_short3(vx, vy, vz)};
    }


    int count = 0;
    for(size_t i = 0; i < num_hailstones; ++i) {
        for(size_t j = i + 1; j < num_hailstones; ++j) {
            auto intersection = check_intersection(hailstones[i], hailstones[j], target_minmax);
            if(intersection) {
                std::cout << "Hailstone A: " << hailstones[i] << std::endl;
                std::cout << "Hailstone B: " << hailstones[j] << std::endl;
                std::cout << "Intersect at (" << intersection->x << ", " << intersection->y << ")\n" << std::endl;
                ++count;
            }
        }
    }
    return count;
}
#pragma clang diagnostic pop

int main(int argc, const char* argv[]) {
    if(argc != 4) {
        std::cerr << "Usage: " << argv[0] << " [file] [target-min] [target-max]" << std::endl;
        return 1;
    }
    auto target = make_long2(atol(argv[2]), atol(argv[3]));

    int res = run_function_on_file<int>(argv[1], [target](std::vector<std::string> vec){
        return raytrace_on_file(std::move(vec), target);
    });
    std::cout << "Result: " << res << std::endl;
}