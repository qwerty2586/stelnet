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

void printdatahex(char *tag, void *c, long n) {
    {
        using namespace std;
        std::cout << tag << " " << n << " ";
        cout << setfill('0');
        for (size_t i = 0; i < n; ++i) {
            cout << std::hex << std::setw(2) << (int) *((char *) c+i);

                cout << (((i + 1) % 16 == 0) ? "\n" : " ");

        }
        cout << std::endl;
    }
}
