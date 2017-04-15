//
// Created by qwerty on 24. 3. 2017.
//
#define CBC 1
#define ECB 1

#include <iostream>
#include <cstring>
extern "C" {
#define CBC 1
#include "3rdparty/tiny_AES128_C/aes.h"
}

#include "server.h"
#include "my_random.h"
#include "padding.h"
#include "aec_cbc.h"


void Server::setup(int listen_port, int telnetd_port) {
    this->client_port = listen_port;
    this->telnetd_port = telnetd_port;

}

void Server::live() {

    try {
        bool end = false;
        AesCbc aesCbc;

        listening_socket = socket();
        set_reuse(listening_socket);
        bind(listening_socket, client_port, false);
        listen(listening_socket);

        std::vector<int> socketgroup;
        std::vector<int> sel_group;
        socketgroup.push_back(listening_socket);

        uint8_t i_buffer[BUFFER_SIZE];
        uint8_t o_buffer[BUFFER_SIZE];
        uint16_t *ret_len = new uint16_t;

        while (!end) {
            sel_group = select(socketgroup, 1000);
            for (int socket : sel_group) {
                if (socket == listening_socket) {
                    client_socket = accept(listening_socket);
                    socketgroup.push_back(client_socket);
                    std::cout << "connected client" << std::endl;

                    uint8_t iv[IV_LENGTH];
                    uint8_t sym_key[SYM_KEY_LENGTH];

                    generate_random_binary_blob((char *) sym_key, SYM_KEY_LENGTH);
                    generate_random_binary_blob((char *) iv, IV_LENGTH);

                    std::cout << "iv " << iv << std::endl;
                    std::cout << "sym_key " << sym_key << std::endl;

                    send(client_socket, sym_key,SYM_KEY_LENGTH);
                    send(client_socket, iv,IV_LENGTH);

                    aesCbc = AesCbc(sym_key,iv);

                    std::cout << "connecting to telnetd..." << std::endl;
                    telnetd_socket = socket();
                    connect(telnetd_socket, "127.0.0.1", telnetd_port);
                    std::cout << "connected to telnetd..." << std::endl;
                    socketgroup.push_back(telnetd_socket);

                }
                if (socket == client_socket) {
                    uint8_t block_count = recvchar(client_socket);
                    uint16_t len = recv(client_socket,i_buffer,block_count*(uint16_t)BLOCK_SIZE);
                    aesCbc.decrypt( o_buffer, ret_len, i_buffer, &len);
                    send(telnetd_socket, o_buffer, *ret_len);
                    std::cout << "s >> "    << *ret_len << " " << o_buffer << std::endl;
                }
                if (socket == telnetd_socket) {
                    uint16_t len=recv(telnetd_socket,  i_buffer, BUFFER_SIZE - BLOCK_SIZE);
                    aesCbc.encrypt(o_buffer,ret_len,i_buffer,&len);
                    uint8_t block_count = (uint8_t)(*ret_len / (uint16_t)BLOCK_SIZE);
                    sendchar(client_socket,block_count);
                    send(client_socket, o_buffer, *ret_len);
                   /* if (buffer.size() == 0) {
                        end = true;
                        continue;
                    }*/ // pro pozdejsi pouziti s exceptionama
                    std::cout << "s << " << *ret_len << " " << o_buffer << std::endl;
                }
            }
        }

        for (int socket : socketgroup) {
            close(socket);
        }




    } catch (tcpException e) {
        std::cerr << e.what() << std::endl;
    }
}

