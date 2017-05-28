//
// Created by qwerty on 16. 5. 2017.
//

#ifndef STELNET_RSA_H
#define STELNET_RSA_H

#include "key_file.h"

#include "gmp.h"


class Rsa {
public:
    Rsa(KeyFile *key_file);

    // encrypt phrase with public key
    void encrypt_public(char *output, uint16_t *out_length, char *input, uint16_t *in_length);
    // decrypt phrase with private key
    void decrypt_private(char *output, uint16_t *out_length, char *input, uint16_t *in_length);

protected:
    // reference to KeyFile structure
    KeyFile *key_file;
};


#endif //STELNET_RSA_H
