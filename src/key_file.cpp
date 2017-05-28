#include "key_file.h"
#include "base64.h"
#include <fstream>
#include <sstream>
#include <iostream>

KeyFile::KeyFile(std::string file_path) {

    std::ifstream file(file_path);
    std::string line;
    bool enabled = false;
    std::stringstream ss;

    while (std::getline(file, line)) {
        if (line.at(0) == '-')
            enabled = !enabled;
        else {
            if (enabled) ss << line;
        }
    }

    file.close();

    std::string b64data = ss.str();

    std::vector<BYTE> decodedData =  base64_decode(b64data);

    parse(decodedData);
}

#define OP_SEQUENCE 0x30
#define OP_INT 0x02

void KeyFile::parse(std::vector<unsigned char> data) {
    int iter = 0;
    int seq_length  =0;

    if (data[iter++] != OP_SEQUENCE) return;

    seq_length = data[iter++];

    if (seq_length>0x80) {
        int  rounds = seq_length - 0x80;
        seq_length = 0;
        for (int i = 1; i <= rounds ; ++i) {
            int add =data[iter++];
            for (int j = i; j < rounds; ++j) {
                add *= 256;
            }
            seq_length += add;
        }
    }
    for (int seq_part = 0; seq_part < 9; ++seq_part) {
        if (data[iter++]!=OP_INT) return;

        int length = data[iter++];
        if (length>0x80) {
            int  rounds = length - 0x80;
            length = 0;
            for (int i = 1; i <= rounds ; ++i) {
                int add =data[iter++];
                for (int j = i; j < rounds; ++j) {
                    add *= 256;
                }
                length += add;
            }
        }

        std::string part(data.begin()+iter,data.begin()+iter+length);
        switch (seq_part) {
            case 0: version = part; break;
            case 1: modulus = part; break;
            case 2: public_exponent = part; break;
            case 3: private_exponent = part; break;
            case 4: prime1 = part; break;
            case 5: prime2 = part; break;
            case 6: exponent1 = part; break;
            case 7: exponent2 = part; break;
            case 8: coefficient = part; break;
            default:
                return;
        }
        iter +=length;
    }




}

int KeyFile::getSize() {
    return (int) (modulus.length()-1);
}
