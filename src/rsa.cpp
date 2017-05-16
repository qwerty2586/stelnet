//
// Created by qwerty on 16. 5. 2017.
//


#include "rsa.h"

Rsa::Rsa(KeyFile *key_file) {
    this->key_file= key_file;

    rsa_publickey_t pub;

    pub.exponent.length_W = key_file->public_exponent.size();
    pub.exponent.wordv = (bigint_word_t *) malloc(pub.exponent.length_W);
    for (int i = 0; i < pub.exponent.length_W ; ++i) {
        pub.exponent.wordv[i] = (uint8_t)key_file->public_exponent[i];
    }

    pub.modulus.length_W = key_file->modulus.size();
    pub.modulus.wordv = (bigint_word_t *) malloc(pub.modulus.length_W);
    for (int i = 0; i < pub.modulus.length_W ; ++i) {
        pub.modulus.wordv[i] = (uint8_t)key_file->modulus[i];
    }


    bigint_t x;
    x.length_W = 1;
    x.wordv = (bigint_word_t *) malloc(1);
    x.wordv[0] = 10;

    rsa_enc(&x,&pub);

    rsa_enc(&x,&pub);
}
