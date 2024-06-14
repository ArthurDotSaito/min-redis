//
// Created by arthur on 01/06/24.
//

#include <io_func.h>

int32_t read_full(int fd, char *buff, size_t n)
{
    while (n > 0)
    {
        ssize_t rv = read(fd, buff, n);
        if (rv <= 0)
        {
            return -1;
        }
        assert((size_t)rv <= n);
        n -= (size_t)rv;
        buff += rv;
    }
    return 0;
}

int32_t write_all(int fd, const char *buff, size_t n)
{
    while (n > 0)
    {
        ssize_t rv = write(fd, buff, n);
        if (rv <= 0)
        {
            return -1;
        }
        assert((size_t)rv <= n);
        n -= (size_t)rv;
        buff += rv;
    }
    return 0;
}

void msg(const char *msg)
{
    fprintf(stderr, "%s\n", msg);
}

static bool try_one_request(Conn *conn)
{
    if (conn->rbuff_size < 4)
    {
        return false;
    }

    uint32_t len = 0;
    memcpy(&len, &conn->rbuff[0], 4);
    if (len > k_max_msg)
    {
        msg("too long");
        conn->state = STATE_END;
        return false;
    }

    if (4 + len > conn->rbuff_size)
    {
        // Here we have not enough data in the buffer. Try in next iteration.
        return false;
    }

    // Just doing something to feedback.
    printf("client says: %.*s\n", len, &conn->rbuff[4]);

    memcpy(&conn->wbuff[0], &len, 4);
    memcpy(&conn->wbuff[4], &conn->rbuff[4], len);
    conn->wbuff_size = 4 + len;

    size_t remain = conn->rbuff_size - 4 - len;
    if (remain)
    {
        memmove(conn->rbuff, &conn->rbuff[4 + len], remain);
    }
    conn->rbuff_size = remain;

    conn->state = STATE_RES;
    state_res(conn);

    return (conn->state == STATE_REQ);
}

static bool try_fill_buffer(Conn *conn)
{
    assert(conn->rbuff_size < sizeof(conn->rbuff));
    ssize_t rv = 0;
    do
    {
        size_t cap = sizeof(conn->rbuff) - conn->rbuff_size;
        rv = read(conn->fd, &conn->rbuff[conn->rbuff_size], cap);
    } while (rv < 0 && errno == EINTR);

    if (rv < 0)
    {
        msg("read() error");
        conn->state = STATE_END;
        return false;
    }

    if (rv == 0)
    {
        if (conn->rbuff_size > 0)
        {
            msg("unexpected EOF");
        }
        else
        {
            msg("EOF");
        }
        conn->state = STATE_END;
        return false;
    }

    conn->rbuff_size += (size_t)rv;
    assert(conn->rbuff_size <= sizeof(conn->rbuff));

    while (try_one_request(conn))
    {
    }
    return (conn->state == STATE_REQ);
}

static bool try_flush_buffer(Conn *conn)
{
    ssize_t rv = 0;
    do
    {
        ssize_t remain = conn->wbuff_size - conn->wbuff_sent;
        rv = write(conn->fd, &conn->wbuff[conn->wbuff_sent], remain);
    } while (rv < 0 && errno == EINTR);

    if (rv < 0 && errno == EAGAIN)
    {
        return false;
    }

    if (rv < 0)
    {
        msg("write() error");
        conn->state = STATE_END;
        return false;
    }

    conn->wbuff_sent += (size_t)rv;
    assert(conn->wbuff_sent <= conn->wbuff_size);
    if (conn->wbuff_sent == conn->wbuff_size)
    {
        // response was fully sent
        conn->state = STATE_REQ;
        conn->wbuff_sent = 0;
        conn->wbuff_size = 0;
        return false;
    }

    return true;
}

void state_res(Conn *conn)
{
    while (try_flush_buffer(conn))
    {
    }
}

void state_req(Conn *conn)
{
    while (try_fill_buffer(conn))
    {
    }
}

void connection_io(Conn *conn)
{
    if (conn->state == STATE_REQ)
    {
        state_req(conn);
    }
    else if (conn->state == STATE_RES)
    {
        state_res(conn);
    }
}
