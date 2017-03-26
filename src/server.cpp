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

    try {
        listening_socket = socket();
        set_reuse(listening_socket);
        bind(listening_socket, client_port, false);
        listen(listening_socket);
        client_socket = accept(listening_socket);
    } catch (tcpException e) {
        std::cerr << e.what() << std::endl;
    }
}

