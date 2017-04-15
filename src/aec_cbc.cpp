//
// Created by qwerty on 14. 4. 2017.
//

#include <cstring>
extern "C" {
#define CBC 1
#include "3rdparty/tiny_AES128_C/aes.h"
}
#include "aec_cbc.h"
#include "my_random.h"

AesCbc::AesCbc(uint8_t *key, uint8_t *iv) {

    if (key) resetKey(key);
    if (iv) resetIv(iv);

}

void AesCbc::resetIv(uint8_t *iv) {
    memcpy(this->dec_iv,iv,BLOCK_SIZE);
    memcpy(this->enc_iv,iv,BLOCK_SIZE);
}

void AesCbc::resetKey(uint8_t *key) {
    memcpy(this->key,key,BLOCK_SIZE);
}

void AesCbc::encrypt(uint8_t *output, uint16_t *o_length, uint8_t *input, uint16_t *i_length) {
    uint8_t lb_valid_count = (uint8_t) (*i_length % BLOCK_SIZE);
    uint8_t padding_count = (uint8_t) (BLOCK_SIZE - lb_valid_count);
    uint16_t new_length = *i_length + padding_count;

    // doplnime nahodnymi znaky, posledni znak obsahuje pocet validnich znaku v poslednim bloku
    // pokud je delka zpravy mod 16 pak se doplni 16 znaku pricemz v poslednim je nula
    generate_random_binary_blob((char *)(input+ *i_length),padding_count);
    input[new_length - 1] = lb_valid_count;


    // zavolame enkrypci
    AES128_CBC_encrypt_buffer(output,input,new_length,key,enc_iv);
    *o_length = new_length;

    // aktualizace IV
    memcpy(enc_iv,output+new_length-BLOCK_SIZE,BLOCK_SIZE);
}

void AesCbc::decrypt(uint8_t *output, uint16_t *o_length, uint8_t *input, uint16_t *i_length) {
    //ulozime starou delku
    uint16_t old_length = *i_length;

    // dekryptujeme
    AES128_CBC_decrypt_buffer(output,input,old_length,key,dec_iv);

    //schovame si IV pro pristi pouziti
    memcpy(dec_iv,input+old_length-BLOCK_SIZE,BLOCK_SIZE);


    // spocitame delku
    uint8_t lb_valid_count = output[old_length - 1];
    uint8_t padding_count = (uint8_t) (BLOCK_SIZE - lb_valid_count);
    uint16_t  new_length = old_length - padding_count;
    // ulozime novou delku
    *o_length = new_length;

}
