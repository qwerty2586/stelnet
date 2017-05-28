//
// Created by qwerty on 24. 3. 2017.
//
#include <algorithm>
#include <iostream>
#include <cstring>
#include "server.h"
#include "my_random.h"
#include "aec_cbc.h"
#include "log.h"
#include "rsa.h"


void Server::setup(int listen_port, int telnetd_port) {
    this->client_port = listen_port;
    this->telnetd_port = telnetd_port;

}

void Server::live() {

    try {
        bool end = false;
        AesCbc aesCbc;
        Rsa rsa(key_file);


        listening_socket = csocket();
        set_reuse(listening_socket);
        bind(listening_socket, client_port, false);
        listen(listening_socket);

        std::vector<int> socketgroup;
        std::vector<int> sel_group;
        add_socket(socketgroup,listening_socket);

        uint8_t i_buffer[BUFFER_SIZE];
        uint8_t o_buffer[BUFFER_SIZE];
        uint16_t ret_len;

        while (!end) {
            sel_group = select(socketgroup, 1000);
            for (int socket : sel_group) {
                if (socket == listening_socket) {
                    client_socket = accept(listening_socket);
                    add_socket(socketgroup,client_socket);
                    std::cout << "connected client" << std::endl;

                    uint8_t sym_key[SYM_KEY_LENGTH];
                    uint8_t iv[IV_LENGTH];
                    uint8_t password[PASS_LENGTH];


                    do {
                        generate_random_binary_blob((char *) sym_key, SYM_KEY_LENGTH);
                    } while (sym_key[0]==0); // cant be zero it would shorter rsa encoded number
                    generate_random_binary_blob((char *) iv, IV_LENGTH);
                    generate_random_binary_blob((char *) password, PASS_LENGTH);


                    uint16_t *blob_l = new uint16_t(SYM_KEY_LENGTH+IV_LENGTH+PASS_LENGTH);
                    uint8_t *blob = new uint8_t[*blob_l];


                    // now we create long phare to encrypt
                    memcpy(blob,sym_key,SYM_KEY_LENGTH);
                    memcpy(blob+SYM_KEY_LENGTH,iv,IV_LENGTH);
                    memcpy(blob+SYM_KEY_LENGTH+IV_LENGTH,password,PASS_LENGTH);

                    std::cout << "iv " << iv << std::endl;
                    std::cout << "sym_key " << sym_key << std::endl;
                    std::cout << "password " << password << std::endl;


                    rsa.encrypt_public((char *) blob, blob_l, (char *) blob, blob_l);

                    uint8_t first_l = (uint8_t) *blob_l;
                    send(client_socket, &first_l,1); //delka
                    send(client_socket, blob, (uint16_t) *blob_l);

                    aesCbc = AesCbc(sym_key,iv);

                    f_recv(client_socket,blob,PASS_LENGTH);
                    *blob_l = PASS_LENGTH;
                    aesCbc.decrypt(blob,blob_l,blob,blob_l);

                    if (memcmp(blob,password,PASS_LENGTH)) {
                        telnetd_socket = csocket();
                        connect(telnetd_socket, "127.0.0.1", telnetd_port);
                        std::cout << "connected to telnetd..." << std::endl;
                        add_socket(socketgroup,telnetd_socket);
                    } else {
                        // wrong password
                        close(client_socket);
                        remove_socket(socketgroup,client_socket);
                    }
                }
                try {
                    if (socket == client_socket) {
                        uint8_t block_count = f_recvchar(client_socket);
                        uint16_t len = block_count * (uint16_t) BLOCK_SIZE;
                        f_recv(client_socket, i_buffer, len);
                        printdatahex("s }}", (char *) i_buffer, len);
                        aesCbc.decrypt(o_buffer, &ret_len, i_buffer, &len);
                        printdatahex("s >>", (char *) o_buffer, ret_len);
                        send(telnetd_socket, o_buffer, ret_len);
                    }
                    if (socket == telnetd_socket) {

                        uint16_t len = recv(telnetd_socket, i_buffer, BUFFER_SIZE - BLOCK_SIZE * 2);
                        printdatahex("s {{", (char *) i_buffer, len);
                        aesCbc.encrypt(o_buffer, &ret_len, i_buffer, &len);
                        uint8_t block_count = (uint8_t) (ret_len / (uint16_t) BLOCK_SIZE);
                        sendchar(client_socket, block_count);
                        printdatahex("s <<", (char *) o_buffer, ret_len);
                        send(client_socket, o_buffer, ret_len);
                    }
                } catch (sendRecvException e) {
                    close(client_socket);
                    close(telnetd_socket);
                    remove_socket(socketgroup,client_socket);
                    remove_socket(socketgroup,telnetd_socket);
                }
            }
        }
    } catch (tcpException e) {
        std::cerr << e.what() << std::endl;
    }
}

