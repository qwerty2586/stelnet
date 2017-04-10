//
// Created by qwerty on 23. 3. 2017.
//

#define CBC 1
#define ECB 1

#include <iostream>
#include "client.h"
#include "padding.h"
extern "C" {
#define CBC 1
#include "3rdparty/tiny_AES128_C/aes.h"
}
#include <algorithm>
#include <cstring>

void Client::setup(int listen_port, int target_port, const std::string &target_address) {
    this->listening_port = listen_port;
    this->target_port = target_port;
    this->target_address = target_address;
}



void Client::live() {
    try {
        bool end = false;
        bool client_connected = false;
        bool cbc_started = false;

        char iv[IV_LENGTH];
        char sym_key[SYM_KEY_LENGTH];

        forward_socket = socket();
        connect(forward_socket, target_address, target_port);

        {
            auto keys = recv(forward_socket);
            std::cout << "client keys: " << keys << std::endl;
            memcpy(sym_key,keys.substr(0,SYM_KEY_LENGTH).c_str(),SYM_KEY_LENGTH);
            memcpy(sym_key,keys.substr(SYM_KEY_LENGTH,IV_LENGTH).c_str(),IV_LENGTH);
        }



        std::cout << "connected to server... now listening for telnet" << std::endl;

        listening_socket = socket();
        set_reuse(listening_socket);
        bind(listening_socket, listening_port, false);
        listen(listening_socket);

        std::vector<int> socketgroup;
        std::vector<int> sel_group;
        socketgroup.push_back(listening_socket);
        socketgroup.push_back(forward_socket);

        std::string buffer;

        while (!end) {
            sel_group = select(socketgroup, 1000);
            for (int socket : sel_group) {
                if (socket == listening_socket) {
                    telnet_socket = accept(listening_socket);
                    add_socket(socketgroup,telnet_socket); // pridame si ho do skupinky
                    std::cout << "late " << buffer.size() << " " << buffer << std::endl;
                    send(telnet_socket,buffer);
                    client_connected = true;
                    std::cout << "connected telnet" << std::endl;
                }
                if (socket == forward_socket) {
                    buffer = TcpBaseObject::recv(forward_socket);
                    if (buffer.size()==0) {
                        end = true;
                        continue;
                    }
                    char dec_buffer[buffer.length()];
                    AES128_CBC_decrypt_buffer((uint8_t *) dec_buffer, (uint8_t *) buffer.c_str(),
                                              (uint32_t) buffer.length(), (const uint8_t *) sym_key,
                                              (const uint8_t *) iv);
                    cbc_started = true;
                    buffer = std::string(dec_buffer,buffer.length());
                    unpad(buffer);
                    std::cout << "c << " << buffer.size() << " " << buffer << std::endl;
                    if (client_connected) send(telnet_socket,buffer);

                }
                if (socket == telnet_socket) {
                    buffer = TcpBaseObject::recv(telnet_socket);
                    if (buffer.size()==0) {
                        close(telnet_socket);
                        remove_socket(socketgroup,telnet_socket);
                        client_connected = false;
                        continue;
                    }
                    pad_with_random(buffer);
                    char enc_buffer[buffer.length()];
                    AES128_CBC_encrypt_buffer((uint8_t *) enc_buffer, (uint8_t *) buffer.c_str(),
                                              (uint32_t) buffer.length(), (const uint8_t *) sym_key,
                                              (const uint8_t *) iv);
                    cbc_started = true;
                    std::cout << "c >> " << buffer.size() << " " << buffer << std::endl;
                    send(forward_socket, std::string(enc_buffer,buffer.length()));
                }


            }
        }

        for (int socket : socketgroup) {
            close(socket);
        }

    } catch (tcpException e) {
        std::cerr << e.what() << std::endl;
    }

    //send(telnet_socket,"ahoj",4,0);
    //close(telnet_socket);








}

