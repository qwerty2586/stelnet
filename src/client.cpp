//
// Created by qwerty on 23. 3. 2017.
//

#define CBC 1
#define ECB 1

#include <iostream>
#include <cstring>
#include "client.h"
#include "aec_cbc.h"
#include "log.h"
#include "rsa.h"

void Client::setup(int listen_port, int target_port, const std::string &target_address) {
    this->listening_port = listen_port;
    this->target_port = target_port;
    this->target_address = target_address;
}


void Client::live() {
    try {
        bool end = false;
        AesCbc aesCbc;
        Rsa rsa(key_file);

        listening_socket = csocket();
        set_reuse(listening_socket);
        bind(listening_socket, listening_port, false);
        listen(listening_socket);

        std::vector<int> socketgroup;
        std::vector<int> sel_group;
        add_socket(socketgroup, listening_socket);

        uint8_t i_buffer[BUFFER_SIZE];
        uint8_t o_buffer[BUFFER_SIZE];
        uint16_t ret_len;

        while (!end) {
            sel_group = select(socketgroup, 1000);
            for (int socket : sel_group) {
                if (socket == listening_socket) {
                    telnet_socket = accept(listening_socket);
                    add_socket(socketgroup, telnet_socket); // pridame si ho do skupinky

                    forward_socket = csocket();
                    connect(forward_socket, target_address, target_port);

                    uint8_t sym_key[SYM_KEY_LENGTH];
                    uint8_t iv[IV_LENGTH];
                    uint8_t password[PASS_LENGTH];

                    uint16_t *blob_l = new uint16_t(key_file->getSize());

                    uint8_t l;
                    f_recv(forward_socket,&l,1);
                    uint8_t *blob = new uint8_t[l];
                    f_recv(forward_socket,blob,l);

                    rsa.decrypt_private((char *) blob, blob_l, (char *) blob, blob_l);

                    memcpy(sym_key,blob,SYM_KEY_LENGTH);
                    memcpy(iv,blob+SYM_KEY_LENGTH,IV_LENGTH);
                    memcpy(password,blob+SYM_KEY_LENGTH+IV_LENGTH,PASS_LENGTH);

                    std::cout << "iv " << iv << std::endl;
                    std::cout << "sym_key " << sym_key << std::endl;
                    std::cout << "password " << password << std::endl;

                    aesCbc = AesCbc(sym_key, iv);
                    *blob_l = PASS_LENGTH;
                    aesCbc.encrypt(blob,blob_l,password,blob_l);
                    aesCbc.resetIv(iv);

                    l = (uint8_t) *blob_l;
                    send(forward_socket,&l,1);
                    send(forward_socket,blob,*blob_l);

                    add_socket(socketgroup, forward_socket);
                }
                try {
                    if (socket == forward_socket) {
                        uint8_t block_count = f_recvchar(forward_socket);
                        uint16_t len = block_count * (uint16_t) BLOCK_SIZE;
                        f_recv(forward_socket, i_buffer, len);
                        printdatahex("c {{", (char *) i_buffer, len);
                        aesCbc.decrypt(o_buffer, &ret_len, i_buffer, &len);
                        printdatahex("c <<", (char *) o_buffer, ret_len);
                        send(telnet_socket, o_buffer, ret_len);


                    }
                    if (socket == telnet_socket) {
                        uint16_t len = recv(telnet_socket, i_buffer, BUFFER_SIZE - BLOCK_SIZE);
                        printdatahex("c }}", (char *) i_buffer, len);
                        aesCbc.encrypt(o_buffer, &ret_len, i_buffer, &len);
                        uint8_t block_count = (uint8_t) (ret_len / (uint16_t) BLOCK_SIZE);
                        sendchar(forward_socket, block_count);
                        printdatahex("c >>", (char *) o_buffer, ret_len);
                        send(forward_socket, o_buffer, ret_len);
                    }
                } catch (sendRecvException e) {
                    close(forward_socket);
                    close(telnet_socket);
                    remove_socket(socketgroup, forward_socket);
                    remove_socket(socketgroup, telnet_socket);
                }


            }
        }

    } catch (tcpException e) {
        std::cerr << e.what() << std::endl;
    }

}

