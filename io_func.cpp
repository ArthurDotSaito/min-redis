//
// Created by arthur on 01/06/24.
//

#include "io_func.h"
#include "server.h"

static int32_t read_full(int fd, char *buff, size_t n){
    while(n > 0){
        ssize_t rv = read(fd, buff, n);
        if(rv <= 0){
            return -1;
        }
        assert((size_t)rv <= n);
        n -= (size_t)rv;
        buff += rv;
    }
    return 0;
}

static int32_t write_all(int fd, const char *buff, size_t n){
    while (n > 0){
        ssize_t rv = write(fd, buff, n);
        if(rv <= 0){
            return -1;
        }
        assert((size_t)rv <= n);
        n -= (size_t)rv;
        buff += rv;
    }
    return 0;
}

static void msg(const char *msg) {
    fprintf(stderr, "%s\n", msg);
}

static bool try_fill_buffer(Conn *conn){
    assert(conn->rbuff_size < sizeof(conn->rbuff));
    ssize_t rv = 0;
    do{
        size_t cap = sizeof(conn->rbuff) - conn->rbuff_size;
        rv = read(conn->fd, &conn->rbuff[conn->rbuff_size], cap);
    } while (rv < 0 && errno == EINTR);

    if(rv < 0){
        msg("read() error");
        conn->state = STATE_END;
        return false;
    }

    if(rv == 0){
        if(conn->rbuff_size > 0){
            msg("unexpected EOF");
        }else{
            msg("EOF");
        }
        conn->state = STATE_END;
        return false;
    }

    conn->rbuff_size += (size_t)rv;
    assert(conn->rbuff_size <= sizeof(conn->rbuff));

    //while(try_one_request)
    return (conn->state == STATE_REQ);
}

static bool try_flush_buffer(Conn *conn){
    
}

static void state_req(Conn *conn) {
    while (try_fill_buffer(conn)) {}
}

static void connection_io(Conn *conn){
    if(conn->state == STATE_REQ){
        state_req(conn);
    } else if(conn->state == STATE_RES){
        //state_res(conn);
    }
}
