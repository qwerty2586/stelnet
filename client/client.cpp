//
// Created by qwerty on 23. 3. 2017.
//

#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include "client.h"

void Client::setup(int listen_port, int target_port, const std::string &target_address) {
    this->listening_port = listen_port;
    this->target_port = target_port;
    this->target_address = target_address;
}

void Client::live() {


    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons((u_short) target_port);
    addr.sin_addr.s_addr = inet_addr(target_address.c_str());


    forward_socket = socket(AF_INET, SOCK_STREAM, 0);

    //connect(forward_socket,&addr, sizeof(addr));

    int err = 0;
    err = connect(forward_socket, (struct sockaddr*)&addr, sizeof(addr));

    if (err < 0) {
        std::cerr << "cant connect to server... exiting" << std::endl;
        return;
    }

    std::cout << "connected to server... now listening for telnet";


    listening_socket = socket(AF_INET, SOCK_STREAM, 0);
    int enable_flag = 1;
    if (setsockopt(listening_socket, SOL_SOCKET, SO_REUSEADDR, &enable_flag, sizeof(int)) < 0)
        printf("setsockopt(SO_REUSEADDR) failed");

    sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // chceme jenom na localhostu poslouchat
    saddr.sin_port = htons((uint16_t) listening_port);




    err = bind(listening_socket, (struct sockaddr *) &saddr, sizeof(saddr));

    if (err < 0) {
        std::cerr << "ERROR on binding" << std::endl;
        return;
    }

    err =  listen(listening_socket,5);
    if (err < 0) {
        std::cerr << "ERROR on listening" << std::endl;
        return;
    }


    struct sockaddr_in telnet_addr;
    socklen_t telnet_addr_len;

    telnet_socket = accept(listening_socket, (struct sockaddr *) &telnet_addr,&telnet_addr_len);
    if (telnet_socket == 0) {
        std::cerr << "ERROR on accepting" << std::endl;
    }

    send(telnet_socket,"ahoj",4,0);
    close(telnet_socket);








}

