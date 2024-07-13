//
// Created by arthur on 13/06/24.
//

#ifndef REQUEST_H
#define REQUEST_H
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <io_func.h>

int32_t send_request(int fd, const std::vector<std::string> &command);

#endif // REQUEST_H
