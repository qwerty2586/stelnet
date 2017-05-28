//
// Created by qwerty on 16. 5. 2017.
//


#include <gmp.h>
#include "rsa.h"

Rsa::Rsa(KeyFile *key_file) {
    this->key_file = key_file;
}


void Rsa::encrypt_public(char *output, uint16_t *out_length, char *input, uint16_t *in_length) {
    mpz_t data_mpz, public_exponent, modulus;
    mpz_init (data_mpz);
    mpz_init (public_exponent);
    mpz_init (modulus);
    mpz_import(data_mpz, (size_t) *in_length, 1, 1, 0, 0, input);
    gmp_printf("mpz %Zx\n", data_mpz);
    mpz_import(modulus, key_file->modulus.size(), 1, 1, 0, 0, key_file->modulus.c_str());
    mpz_import(public_exponent, key_file->public_exponent.size(), 1, 1, 0, 0, key_file->public_exponent.c_str());

    mpz_powm_sec(data_mpz, data_mpz, public_exponent, modulus);
    gmp_printf("mpz %Zx\n", data_mpz);
    size_t size;
    mpz_export(output, (size_t *) out_length, 1, 1, 0, 0, data_mpz);
}

void Rsa::decrypt_private(char *output, uint16_t *out_length, char *input, uint16_t *in_length) {
    mpz_t data_mpz, private_exponent, modulus;
    mpz_init (data_mpz);
    mpz_init (private_exponent);
    mpz_init (modulus);
    mpz_import(data_mpz, (size_t) *in_length, 1, 1, 0, 0, input);
    gmp_printf("mpz %Zx\n", data_mpz);
    mpz_import(modulus, key_file->modulus.size(), 1, 1, 0, 0, key_file->modulus.c_str());
    mpz_import(private_exponent, key_file->private_exponent.size(), 1, 1, 0, 0, key_file->private_exponent.c_str());

    mpz_powm_sec(data_mpz, data_mpz, private_exponent, modulus);
    gmp_printf("mpz %Zx\n", data_mpz);
    size_t size;
    mpz_export(output, (size_t *) out_length, 1, 1, 0, 0, data_mpz);
}






