//
// Created by qwerty on 16. 5. 2017.
//

#ifndef STELNET_RSA_H
#define STELNET_RSA_H

#include "key_file.h"

extern "C" {
#include "3rdparty/avr_crypto_lib/rsa_basic.h"
#include "3rdparty/avr_crypto_lib/bigint.h"
}

class Rsa {
public:
    Rsa(KeyFile *key_file);
protected:
    KeyFile* key_file ;
};


#endif //STELNET_RSA_H
