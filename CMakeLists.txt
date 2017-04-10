cmake_minimum_required(VERSION 3.6)
project(stelnet)


set(CMAKE_CXX_STANDARD 14)
if (UNIX)
    set(CMAKE_CXX_FLAGS "-lpthread")
endif()

set(SOURCE_FILES
        src/main.cpp

        src/client.cpp
        src/client.h

        src/server.cpp
        src/server.h

        src/tcp_base_object.cpp
        src/tcp_base_object.h

        src/my_random.cpp
        src/my_random.h
        src/padding.cpp
        src/padding.h
        )

SET(AES_LIB_DIR "src/3rdparty/tiny_AES128_C")

INCLUDE_DIRECTORIES(${AES_LIB_DIR})

ADD_LIBRARY(
        tiny_aes_lib
        ${AES_LIB_DIR}/aes.c
)


add_executable(stelnet ${SOURCE_FILES})
target_link_libraries(stelnet tiny_aes_lib)