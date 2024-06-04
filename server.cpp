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
#include <poll.h>
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

static void conn_put(std::vector<Conn *> &fd2conn, struct Conn *conn){
    if(fd2conn.size() <= (size_t)conn->fd){
        fd2conn.resize(conn->fd + 1);
    }

    fd2conn[conn->fd] = conn;
}

static int32_t accept_new_connections(std::vector<Conn *> &fd2conn, int fd){

    struct sockaddr_in client_addr = {};
    socklen_t socklen = sizeof(client_addr);

    int conn_fd = accept(fd, (struct sockaddr *)&client_addr, &socklen);
    if(conn_fd < 0){
        msg("accept error");
        return -1;
    }

    fd_set_nonblocking(conn_fd);

    struct Conn *conn = (struct Conn*) malloc(sizeof(struct Conn));
    if(!conn){
        close(conn_fd);
        return -1;
    }

    conn->fd = conn_fd;
    conn->state = STATE_REQ;
    conn->rbuff_size = 0;
    conn->wbuff_size = 0;
    conn->wbuff_sent = 0;

    conn_put(fd2conn, conn);
    return 0;
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

    //set to nonblocking fd
    fd_set_nonblocking(fd);

    std::vector<struct pollfd> poll_args;

    while (true)
    {
        poll_args.clear();

        struct pollfd pfd = {fd, POLLIN, 0};
        poll_args.push_back(pfd);

        //connection fds
        for(Conn *conn : fd2conn){
            if(!conn){
                continue;
            }

            struct pollfd pfd = {};
            pfd.fd = conn->fd;
            pfd.events = (conn->state == STATE_REQ) ? POLLIN : POLLOUT;
            pfd.events = pfd.events | POLLERR;
            poll_args.push_back(pfd);
        }

        //poll for active fds. Timeout is not important
        int rv = poll(poll_args.data(), (nfds_t)poll_args.size(), 1000);
        if(rv < 0){
            die("poll");
        }

        for(size_t i = 1; i < poll_args.size(); ++i){
            if(poll_args[i].revents){
                Conn *conn = fd2conn[poll_args[i].fd];
            }
        }


        if(poll_args[0].revents){
            (void) accept_new_connections(fd2conn, fd);
        }
    }

    return 0;
}
