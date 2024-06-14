//
// Created by arthur on 30/05/24.
//

#ifndef REDIS_SERVER_H
#define REDIS_SERVER_H
#include "io_func.h"

enum
{
    STATE_REQ = 0,
    STATE_RES = 1,
    STATE_END = 2 // mark the connection to be closed
};

struct Conn
{
    int fd = -1;
    uint32_t state = 0;

    // buffer for reading
    size_t rbuff_size = 0;
    uint8_t rbuff[4 + k_max_msg];

    // buffer for writing
    size_t wbuff_size = 0;
    size_t wbuff_sent = 0;
    uint8_t wbuff[4 + k_max_msg];
};

#endif // REDIS_SERVER_H
