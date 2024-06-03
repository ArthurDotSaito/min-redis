//
// Created by arthur on 30/05/24.
//

#include "server.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "sys/socket.h"
#include <netinet/ip.h>
#include <unistd.h>
#include "io_func.h"
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>

static void die(const char *msg)
{
    int err = errno;
    fprintf(stderr, "[%d] %s\n", err, msg);
    abort();
}

static int32_t one_request(int conn_fd)
{
    char rbuff[4 + k_max_msg + 1];
    errno = 0;
    int32_t err = read_full(conn_fd, rbuff, 4);
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
    memcpy(&len, rbuff, 4);
    if (len > k_max_msg)
    {
        msg("too long");
        return -1;
    }

    rbuff[4 + len] = '\0';
    printf("client: %s\n", &rbuff[4]);

    const char reply[] = "world";
    char wbuff[4 + sizeof(reply)];
    len = (uint32_t)strlen(reply);
    memcpy(wbuff, &len, 4);
    memcpy(&wbuff[4], reply, len);
    return write_all(conn_fd, wbuff, 4 + len);
}

static void fd_set_nonblocking(int fd){
    errno = 0;
    int flags = fcntl(fd, F_GETFL, 0);

    if(errno){
        die("fcntl error");
        return;
    }

    flags |= O_NONBLOCK;

    errno = 0;
    (void) fcntl(fd, F_SETFL, flags);
    if(errno){
        die("fcntl error");
        return;
    }
}

int main()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
    {
        die("socket()");
    }

    int val = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

    // bind
    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(1234);
    addr.sin_addr.s_addr = ntohl(0);
    int rv = bind(fd, (const sockaddr *)&addr, sizeof(addr));
    if (rv)
    {
        die("bind()");
    }

    // listen
    rv = listen(fd, SOMAXCONN);
    if (rv)
    {
        die("listen()");
    }

    // a map of all client connections, keyed by fd
    std::vector<Conn *> fd2conn;

    while (true)
    {
        // accept syscall
        struct sockaddr_in client_addr = {};
        socklen_t addrlen = sizeof(client_addr);
        int conn_fd = accept(fd, (struct sockaddr *)&client_addr, &addrlen);
        if (conn_fd)
        {
            continue;
        }
        while (true)
        {
            int32_t err = one_request(conn_fd);
            if (err)
            {
                break;
            }
        }
    }

    return 0;
}
