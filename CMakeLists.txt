cmake_minimum_required(VERSION 3.6)
project(stelnet)


set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_CXX_FLAGS "-g")

set(SOURCE_FILES src/main.cpp src/client.cpp src/client.h src/server.cpp src/server.h src/tcp_base_object.cpp src/tcp_base_object.h)
add_executable(stelnet ${SOURCE_FILES})