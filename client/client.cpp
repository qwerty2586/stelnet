//
// Created by qwerty on 23. 3. 2017.
//


#include <rpc.h>
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

    connect(forward_socket, (const sockaddr *) &addr, sizeof(addr));



}

