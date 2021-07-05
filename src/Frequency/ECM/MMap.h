//
// Created by z yd on 2020-08-09.
//

#ifndef SLIDINGMEM_MMAP_H
#define SLIDINGMEM_MMAP_H

#include <fcntl.h>
#include <stdint.h>

#include <sys/stat.h>
#include <sys/mman.h>

#include <iostream>

struct LoadResult{
    void* start;
    uint64_t length;
};

LoadResult Load(const char* PATH){
    LoadResult ret;

    int32_t fd = open(PATH, O_RDONLY);
    if(fd == -1) {
        std::cerr << "Cannot open " << PATH << std::endl;
        throw;
    }

    struct stat sb;
    if(fstat(fd, &sb) == -1)
        std::cerr << "Fstat Error\n";

    ret.length = sb.st_size;
    ret.start = mmap(nullptr, ret.length, PROT_READ, MAP_PRIVATE, fd, 0u);

    if (ret.start == MAP_FAILED) {
        std::cerr << "Cannot mmap " << PATH << " of length " << ret.length << std::endl;
        throw;
    }

    return ret;
}

void UnLoad(LoadResult result){
    munmap(result.start, result.length);
}

#endif //SLIDINGMEM_MMAP_H
