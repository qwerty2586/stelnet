//
// Created by qwerty on 23. 3. 2017.
//

#include <iostream>
#include "client.h"

void Client::setup(int listen_port, int target_port, const std::string &target_address) {
    this->listening_port = listen_port;
    this->target_port = target_port;
    this->target_address = target_address;
}

void Client::live() {
    try {
        forward_socket = socket();
        connect(forward_socket, target_address, target_port);
        std::cout << "connected to server... now listening for telnet" << std::endl;

        listening_socket = socket();
        set_reuse(listening_socket);
        bind(listening_socket, listening_port, true);
        listen(listening_socket);
        accept(listening_socket);
    } catch (tcpException e) {
        std::cerr << e.what() << std::endl;
    }

    //send(telnet_socket,"ahoj",4,0);
    //close(telnet_socket);








}

