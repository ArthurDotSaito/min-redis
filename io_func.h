//
// Created by arthur on 01/06/24.
//

#ifndef REDIS_IO_FUNC_H
#define REDIS_IO_FUNC_H

#include <stdint.h>
#include <assert.h>
#include <unistd.h>

static int32_t read_full(int fd, char *buff, size_t n);
static int32_t write_all(int fd, const char *buff, size_t n);

#endif //REDIS_IO_FUNC_H
