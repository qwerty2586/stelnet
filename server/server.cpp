//
// Created by qwerty on 24. 3. 2017.
//

#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

#include "server.h"

void Server::setup(int listen_port, int telnetd_port) {
    this->client_port = listen_port;
    this->telnetd_port = telnetd_port;

}

void Server::live() {


    listening_socket = socket(AF_INET, SOCK_STREAM, 0);
    int enable_flag = 1;
    if (setsockopt(listening_socket, SOL_SOCKET, SO_REUSEADDR, &enable_flag, sizeof(int)) < 0)
        printf("setsockopt(SO_REUSEADDR) failed");

    sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY; // chceme poslouchat vsude
    saddr.sin_port = htons((uint16_t) client_port);



    int err = bind(listening_socket, (struct sockaddr *) &saddr, sizeof(saddr));

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

    client_socket = accept(listening_socket, (struct sockaddr *) &telnet_addr,&telnet_addr_len);
    if (client_socket == 0) {
        std::cerr << "ERROR on accepting" << std::endl;
    }

    std::cout << "connected"<< std::endl;
/*

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons((u_short) target_port);
    addr.sin_addr.s_addr = inet_addr(target_address.c_str());

    forward_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    //connect(forward_socket,&addr, sizeof(addr));

    int err = 0;
    err = connect(forward_socket, (struct sockaddr*)&addr, sizeof(addr));

    if (err < 0) {
        std::cerr << "cant connect to server... exiting" << std::endl;
        return;
    }

    std::cout << "connected to server... now listening for telnet";


    listening_socket = socket(AF_INET, SOCK_STREAM, 0);
*/


}
