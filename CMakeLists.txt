cmake_minimum_required(VERSION 3.28)
project(Redis)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Redis
        client.cpp
        client.h
        server.cpp
        server.h
        io_func.cpp)
