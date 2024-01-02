#ifndef ADVENTOFCODE2023_LINES_HPP
#define ADVENTOFCODE2023_LINES_HPP

#include <vector>
#include <string>
#include <fstream>
#include <functional>

#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

struct Mmap {
private:
    int fd;
public:
    char* buf;
    off_t len;

    explicit Mmap(const char* fn) {
        this->fd = open(fn, O_RDONLY);
        this->len = lseek(this->fd, 0, SEEK_END);
        lseek(this->fd, 0, SEEK_SET);
        this->buf = static_cast<char*>(mmap(nullptr, this->len, PROT_READ, MAP_PRIVATE | MAP_POPULATE, this->fd, 0));
    }
    Mmap(const Mmap&) = delete;
    Mmap& operator=(const Mmap&) = delete;
    ~Mmap() {
        munmap(this->buf, this->len);
        close(this->fd);
    }
};

template<typename R>
R run_function_on_file(const char* fn, const std::function<int(std::vector<std::string>)>& func) {
    std::vector<std::string> lines;
    std::ifstream in(fn, std::ios_base::in);
    for(std::string line; std::getline(in, line); ) {
        lines.push_back(line);
    }

    return func(lines);
}

#endif //ADVENTOFCODE2023_LINES_HPP