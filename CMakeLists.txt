cmake_minimum_required(VERSION 3.7)
project(stelnet)

set(CMAKE_CXX_STANDARD 14)


set(SOURCE_FILES main.cpp client/client.cpp client/client.h)
add_executable(stelnet ${SOURCE_FILES})