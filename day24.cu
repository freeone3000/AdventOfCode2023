//
// Created by jasmine on 12/31/23.
// This obviously is implementing ray tracing. So, let's see about RTX here.
//
#include <iostream>

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

int main() {
    return 0;
}