//
// Created by qwerty on 10. 4. 2017.
//

#include <cstdio>
#include <sys/fcntl.h>
#include <unistd.h>
#include "my_random.h"

void generate_random_binary_blob(char *addr, int length) {
    int urandom = open("/dev/urandom",O_RDONLY);
    size_t read_len = 0;
    while (read_len<length) {
        ssize_t read_bytes = read(urandom,addr+read_len,length-read_len);
        read_len+=read_bytes;
    }
    close(urandom);


}
