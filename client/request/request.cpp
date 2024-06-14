//
// Created by arthur on 13/06/24.
//
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <io_func.h>

static int32_t send_request(int fd, const char *text)
{
    uint32_t len = (uint32_t)strlen(text);
    if (len > k_max_msg)
    {
        return -1;
    }

    char wbuff[4 + k_max_msg];
    memcpy(wbuff, &len, 4); // here I'm assuming little endian
    memcpy(&wbuff[4], text, len);
    return write_all(fd, wbuff, 4 + len);
}