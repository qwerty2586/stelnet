cmake_minimum_required(VERSION 3.6)
project(stelnet)


set(CMAKE_CXX_STANDARD 14)
if (UNIX)
    set(CMAKE_CXX_FLAGS "-lpthread")
endif()

set(STELNET_SOURCES
        src/main.cpp

        src/client.cpp
        src/client.h

        src/server.cpp
        src/server.h

        src/tcp_base_object.cpp
        src/tcp_base_object.h src/my_random.cpp src/my_random.h)

set(TINY_AES_SOURCES
        src/3rdparty/tiny-AES128-C/aes.c
        src/3rdparty/tiny-AES128-C/aes.h)



add_executable(stelnet ${STELNET_SOURCES} ${TINY_AES_SOURCES})