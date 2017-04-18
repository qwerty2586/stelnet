//
// Created by qwerty on 18. 4. 2017.
//

#include <string>
#include <iostream>
#include <iomanip>
#include "log.h"

void printdata(char *tag, void *c, unsigned long n) {
    std::string s((char *)c,n);
    std::cout << tag << " " << n << " " << s << std::endl;
}

void printdatahex(char *tag, char *c, long n) {

    std::cout << tag << " " << n << " ";
    char const hex_chars[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

    for( int i = 0; i < n; ++i )
    {
        char const byte = c[i];

        std::cout << hex_chars[ ( byte & 0xF0 ) >> 4 ];
        std::cout << hex_chars[ ( byte & 0x0F ) >> 0 ];
    }
    std::cout << std::endl;

}
