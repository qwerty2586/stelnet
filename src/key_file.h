//
// Created by qwerty on 14. 5. 2017.
//

#ifndef STELNET_KEY_FILE_H
#define STELNET_KEY_FILE_H

#include <string>
#include <vector>

class KeyFile {
public:
    KeyFile(std::string file_path);
    void parse(std::vector<unsigned char> data);
    int getSize();

    std::string version;
    std::string modulus;
    std::string public_exponent;
    std::string private_exponent;
    std::string prime1;
    std::string prime2;
    std::string exponent1;
    std::string exponent2;
    std::string coefficient;

};

#endif //STELNET_KEY_FILE_H
