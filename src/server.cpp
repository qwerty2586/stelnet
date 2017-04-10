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


void Server::setup(int listen_port, int telnetd_port) {
    this->client_port = listen_port;
    this->telnetd_port = telnetd_port;

}

void Server::live() {

    try {
        bool end = false;
        bool client_connected = false;
        bool cbc_started = false;

        char iv[IV_LENGTH];
        char sym_key[SYM_KEY_LENGTH];


        std::cout << "connecting to telnetd..." << std::endl;
        telnetd_socket = socket();
        connect(telnetd_socket, "127.0.0.1", telnetd_port);
        std::cout << "connected to telnetd..." << std::endl;


        listening_socket = socket();
        set_reuse(listening_socket);
        bind(listening_socket, client_port, false);
        listen(listening_socket);

        std::vector<int> socketgroup;
        std::vector<int> sel_group;
        socketgroup.push_back(listening_socket);
        socketgroup.push_back(telnetd_socket);

        std::string buffer;

        while (!end) {
            sel_group = select(socketgroup, 1000);
            for (int socket : sel_group) {
                if (socket == listening_socket) {
                    client_socket = accept(listening_socket);
                    socketgroup.push_back(client_socket);
                    client_connected = true;
                    generate_random_binary_blob(sym_key, SYM_KEY_LENGTH);
                    generate_random_binary_blob(iv, IV_LENGTH);
                    std::string keys(sym_key, SYM_KEY_LENGTH);
                    keys.append(iv, IV_LENGTH);
                    std::cout << "iv " << iv << std::endl;
                    std::cout << "sym_key " << sym_key << std::endl;

                    send(client_socket, keys);
                    cbc_started = false;


                    pad_with_random(buffer);
                    char enc_buffer[buffer.length()];
                    AES128_CBC_encrypt_buffer((uint8_t *) enc_buffer, (uint8_t *) buffer.c_str(),
                                              (uint32_t) buffer.length(), (const uint8_t *) sym_key,
                                              (const uint8_t *) iv);
                    cbc_started = true;
                    send(client_socket,std::string(enc_buffer,buffer.length()));
                    std::cout << "connected client" << std::endl;
                }
                if (socket == client_socket) {
                    buffer = TcpBaseObject::recv(client_socket);
                    char dec_buffer[buffer.length()];
                    AES128_CBC_decrypt_buffer((uint8_t *) dec_buffer, (uint8_t *) buffer.c_str(),
                                              (uint32_t) buffer.length(), (const uint8_t *) sym_key,
                                              (const uint8_t *) iv);
                    cbc_started = true;
                    buffer = std::string(dec_buffer,buffer.length());
                    unpad(buffer);
                    std::cout << "s >> "    << buffer.size() << " " << buffer << std::endl;
                    send(telnetd_socket, buffer);
                }
                if (socket == telnetd_socket) {
                    buffer = TcpBaseObject::recv(telnetd_socket);
                    if (buffer.size() == 0) {
                        end = true;
                        continue;
                    }
                    pad_with_random(buffer);
                    char enc_buffer[buffer.length()];
                    AES128_CBC_encrypt_buffer((uint8_t *) enc_buffer, (uint8_t *) buffer.c_str(),
                                              (uint32_t) buffer.length(), (const uint8_t *) sym_key,
                                              (const uint8_t *) iv);
                    cbc_started = true;
                    std::cout << "s << " << buffer.size() << " " << buffer << std::endl;
                    if (client_connected) send(client_socket, std::string(enc_buffer,buffer.length()));

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

