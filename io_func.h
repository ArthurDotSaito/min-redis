//
// Created by arthur on 01/06/24.
//

#ifndef REDIS_IO_FUNC_H
#define REDIS_IO_FUNC_H

#include <stdint.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "server.h"

const size_t k_max_msg = 4096;

static void msg(const char *msg);

static int32_t read_full(int fd, char *buff, size_t n);
static int32_t write_all(int fd, const char *buff, size_t n);

static void state_req(Conn *conn);
static bool try_fill_buffer(Conn *conn);
static void connection_io(Conn *conn);

#endif //REDIS_IO_FUNC_H
