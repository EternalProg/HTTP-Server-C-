cmake_minimum_required(VERSION 3.22)
project(HTTP_SERVER)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(HTTP_SERVER
        http_tcpServer_linux.cpp
        http_tcpServer_linux.h
        server_linux.cpp)
