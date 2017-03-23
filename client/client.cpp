//
// Created by qwerty on 23. 3. 2017.
//

#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <iostream>
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

    forward_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    //connect(forward_socket,&addr, sizeof(addr));

    int err = connect(forward_socket, (struct sockaddr*)&addr, sizeof(addr));

    if (err < 0) {
        std::cerr << "cant connect to server... exiting" << std::endl;
        return;
    }

    std::cout << "connected to server... now listening for telnet";


    listening_socket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;
    saddr.sin_port = htons(listening_port);


    bind(listening_socket, (struct sockaddr *) &saddr, sizeof(saddr));
    
    /* Now bind the host address using bind() call.*/
    if ( < 0) {
        perror("ERROR on binding");
        exit(1);
    }





}

