//
// Created by arthur on 30/05/24.
//

#include "server.h"
#include "sys/socket.h"

int main(){
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    int val = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
}
