//
// Created by arthur on 01/06/24.
//

#ifndef REDIS_IO_FUNC_H
#define REDIS_IO_FUNC_H

#include <stdint.h>
#include <assert.h>
#include <unistd.h>
#include "server.h"
#include "common.h"
#include <commands.h>

struct Conn;

void msg(const char *msg);

int32_t read_full(int fd, char *buff, size_t n);
int32_t write_all(int fd, const char *buff, size_t n);

void state_res(Conn *conn);
void state_req(Conn *conn);
void connection_io(Conn *conn);

#endif // REDIS_IO_FUNC_H
