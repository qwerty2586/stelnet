//
// Created by qwerty on 23. 3. 2017.
//

#define CBC 1
#define ECB 1

#include <iostream>
#include "client.h"
#include "padding.h"
#include "aec_cbc.h"

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
        AesCbc aesCbc;

        listening_socket = socket();
        set_reuse(listening_socket);
        bind(listening_socket, listening_port, false);
        listen(listening_socket);

        std::vector<int> socketgroup;
        std::vector<int> sel_group;
        add_socket(socketgroup,listening_socket);

        uint8_t i_buffer[BUFFER_SIZE];
        uint8_t o_buffer[BUFFER_SIZE];
        uint16_t *ret_len = new uint16_t;

        while (!end) {
            sel_group = select(socketgroup, 1000);
            for (int socket : sel_group) {
                if (socket == listening_socket) {
                    telnet_socket = accept(listening_socket);
                    add_socket(socketgroup, telnet_socket); // pridame si ho do skupinky

                    forward_socket = socket();
                    connect(forward_socket, target_address, target_port);

                    uint8_t sym_key[SYM_KEY_LENGTH];
                    uint8_t iv[IV_LENGTH];

                    recv(forward_socket,sym_key,SYM_KEY_LENGTH);
                    recv(forward_socket,iv,IV_LENGTH);

                    aesCbc = AesCbc(sym_key,iv);
                    add_socket(socketgroup, forward_socket);
                }

                if (socket == forward_socket) {
                    uint8_t block_count = recvchar(forward_socket);
                    uint16_t len = recv(forward_socket,i_buffer,block_count*(uint16_t)BLOCK_SIZE);
                    aesCbc.decrypt( o_buffer, ret_len, i_buffer, &len);
                    send(telnet_socket, o_buffer, *ret_len);
                    std::cout << "c >> "    << *ret_len << " " << o_buffer << std::endl;
                }
                if (socket == telnet_socket) {
                    uint16_t len=recv(telnet_socket,  i_buffer, BUFFER_SIZE - BLOCK_SIZE);
                    aesCbc.encrypt(o_buffer,ret_len,i_buffer,&len);
                    uint8_t block_count = (uint8_t)(*ret_len / (uint16_t)BLOCK_SIZE);
                    sendchar(forward_socket,block_count);
                    send(forward_socket, o_buffer, *ret_len);
                    std::cout << "c << " << *ret_len << " " << o_buffer << std::endl;
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

