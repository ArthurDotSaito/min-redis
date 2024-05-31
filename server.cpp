//
// Created by arthur on 30/05/24.
//

#include "server.h"
#include "sys/socket.h"
#include <netinet/ip.h>

static void die(const char *msg) {
    int err = errno;
    fprintf(stderr, "[%d] %s\n", err, msg);
    abort();
}


int main(){
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    int val = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

    //bind
    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(1234);
    addr.sin_addr.s_addr = ntohl(0);
    int rv = bind(fd, (const sockaddr *)&addr, sizeof(addr));
    if(rv){
        die("bind()");
    }
}
