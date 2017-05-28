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

        src/aec_cbc.cpp
        src/aec_cbc.h

        src/log.cpp
        src/log.h

        src/base64.cpp
        src/base64.h

        src/key_file.cpp
        src/key_file.h

        src/rsa.cpp
        src/rsa.h)

SET(AES_LIB_DIR "src/3rdparty/tiny_AES128_C")

SET(RSA_LIB_DIR "src/3rdparty/gmp_cygwin")

#INCLUDE_DIRECTORIES(${AES_LIB_DIR} ${RSA_LIB_DIR}/include)

ADD_LIBRARY(
        tiny_aes_lib
        ${AES_LIB_DIR}/aes.c
)


find_library(
        libgmp
        NAMES gmp libgmp
        HINTS "/usr/lib/" "/usr/local/lib/"
        )

INCLUDE_DIRECTORIES(${AES_LIB_DIR} "/usr/include/")


#[[ADD_LIBRARY(
        gmp_lib
      #  ${RSA_LIB_DIR}/lib/libgmp.la
      #  ${RSA_LIB_DIR}/lib/libgmp.la
)]]

LINK_DIRECTORIES(${RSA_LIB_DIR}/lib)



add_executable(stelnet ${SOURCE_FILES})
target_link_libraries(stelnet tiny_aes_lib ${libgmp})