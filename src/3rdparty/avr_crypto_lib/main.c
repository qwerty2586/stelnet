#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include "bigint.h"
#include "bigint_io.h"
#define SIZE_BYTES 32

void generate_random_binary_blob(char *addr, int length) {
    int urandom = open("/dev/urandom",O_RDONLY);
    size_t read_len = 0;
    while (read_len<length) {
        ssize_t read_bytes = read(urandom,addr+read_len,length-read_len);
        read_len+=read_bytes;
    }
    close(urandom);
}

void random_prime(void * addr,int lenght) {

}

int main() {
    bigint_t *p;
    p = malloc(sizeof(bigint_t));

    printf("hello");

    p->wordv = malloc(100*sizeof(bigint_word_t));
    //bigint_clip(p,(size_t )16u);
    bigint_set_zero(p);
    generate_random_binary_blob(p->wordv,4);
    p->length_W = 6;
    bigint_adjust(p);
    bigint_print_hex(p);




    return 0;
}