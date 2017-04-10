//
// Created by qwerty on 10. 4. 2017.
//

#include "padding.h"
#include "my_random.h"

#define BLOCK_SIZE 16

void pad_with_random(std::string &text) {
    // lb lastblock

    uint8_t lb_valid_count = (uint8_t) (text.size() % BLOCK_SIZE);
    uint8_t padding_count = (uint8_t) (BLOCK_SIZE - lb_valid_count);

    char padding_bytes[padding_count];
    generate_random_binary_blob(padding_bytes,padding_count);
    padding_bytes[padding_count-1] = lb_valid_count;

    text.append(padding_bytes, padding_count);

}

void unpad(std::string &text) {
    uint8_t lb_valid_count = (uint8_t) text[text.length() - 1];
    uint8_t padding_count = (uint8_t) (BLOCK_SIZE - lb_valid_count);

    text = text.substr(0,text.length() - padding_count);
}
