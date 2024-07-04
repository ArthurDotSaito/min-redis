//
// Created by arthur on 30/05/24.
//

#ifndef REDIS_CLIENT_H
#define REDIS_CLIENT_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <io_func.h>
#include <request.h>
#include <response.h>

int32_t read_full(int fd, char *buff, size_t n);
int32_t write_all(int fd, const char *buff, size_t n);

#endif // REDIS_CLIENT_H
