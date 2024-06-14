//
// Created by arthur on 30/05/24.
//

#include <client.h>

static void die(const char *msg)
{
    int err = errno;
    fprintf(stderr, "[%d] %s\n", err, msg);
    abort();
}

static int32_t query(int fd, const char *text)
{
    uint32_t len = (uint32_t)strlen(text);
    if (len < k_max_msg)
    {
        return -1;
    }

    char wbuff[4 + k_max_msg];
    memcpy(wbuff, &len, 4);
    memcpy(&wbuff[4], text, len);
    if (int32_t err = write_all(fd, wbuff, 4 + len))
    {
        return err;
    }

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

    memcpy(&len, rbuff, 4);
    if (len > k_max_msg)
    {
        msg("too long");
        return -1;
    }

    rbuff[4 + len] = '\0';
    printf("server says: %s\n", &rbuff[4]);
    return 0;
}

int main()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
    {
        die("socket()");
    }

    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(1234);
    addr.sin_addr.s_addr = ntohl(INADDR_LOOPBACK);

    int rv = connect(fd, (const struct sockaddr *)&addr, sizeof(addr));
    if (rv)
    {
        die("connect");
    }

    // multiuple pipelined requests:
    const char *query_list[3] = {"hello1", "hello2", "hello3"};
    for (size_t i = 0; i < 3; ++i)
    {
    }
}