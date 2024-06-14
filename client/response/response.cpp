//
// Created by arthur on 13/06/24.
//
#include <response.h>

static int32_t read_response(int fd)
{
    char rbuff[4 + k_max_msg + 1];
    errno = 0;
    int32_t err = read_full(fd, rbuff, 4);

    if (err)
    {
        if (errno == 0)
        {
            msg("EOF");
        }
        else
        {
            msg("read() error");
        }
        return err;
    }

    uint32_t len = 0;
    memcpy(&len, rbuff, 4); // assume little endian
    if (len > k_max_msg)
    {
        msg("too long");
        return -1;
    }
    // reply body
    err = read_full(fd, &rbuff[4], len);
    if (err)
    {
        msg("read() error");
        return err;
    }

    rbuff[4 + len] = '\0';
    printf("server says: %s\n", &rbuff[4]);
    return 0;
}