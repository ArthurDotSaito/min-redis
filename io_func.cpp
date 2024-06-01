//
// Created by arthur on 01/06/24.
//

#include "io_func.h"

static int32_t read_full(int fd, char *buff, size_t n){
    while(n > 0){
        ssize_t rv = read(fd, buff, n);
        if(rv <= 0){
            return -1;
        }
        assert((size_t)rv <= n);
        n -= (size_t)rv;
        buff += rv;
    }
    return 0;
}

static int32_t write_all(int fd, const char *buff, size_t n){
    while (n > 0){
        ssize_t rv = write(fd, buff, n);
        if(rv <= 0){
            return -1;
        }
        assert((size_t)rv <= n);
        n -= (size_t)rv;
        buff += rv;
    }
    return 0;
}