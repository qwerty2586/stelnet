//
// Created by qwerty on 14. 4. 2017.
//

#ifndef STELNET_AEC_CBC_H
#define STELNET_AEC_CBC_H

#include <cstdint>
#include <mutex>

#define SYM_KEY_LENGTH 16
#define IV_LENGTH 16
#define BLOCK_SIZE 16

extern std::mutex mutex;

class AesCbc {

public:
    AesCbc(uint8_t *key = nullptr, uint8_t *iv = nullptr);
    void resetIv(uint8_t *iv);
    void resetKey(uint8_t *key);
    void encrypt(uint8_t *output, uint16_t *o_length,uint8_t *input, uint16_t *i_length);
    void decrypt(uint8_t *output, uint16_t *o_length,uint8_t *input, uint16_t *i_length);

private:
    uint8_t key[BLOCK_SIZE];
    uint8_t enc_iv[BLOCK_SIZE];
    uint8_t dec_iv[BLOCK_SIZE];

};

#endif //STELNET_AEC_CBC_H
