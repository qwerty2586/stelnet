//
// Created by qwerty on 14. 5. 2017.
//

#ifndef STELNET_BASE64_H
#define STELNET_BASE64_H

#include <vector>
#include <string>
typedef unsigned char BYTE;

std::string base64_encode(BYTE const* buf, unsigned int bufLen);
std::vector<BYTE> base64_decode(std::string const&);
#endif //STELNET_BASE64_H
