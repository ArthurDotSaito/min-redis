cmake_minimum_required(VERSION 3.28)
project(Redis)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Redis
        client/client/client.cpp
        server/server.cpp
        io/io_func.cpp
        server/server.cpp
        libs/client.h
        libs/io_func.h
        libs/request.h
        libs/server.h
        libs/client.h
        libs/io_func.h

)
