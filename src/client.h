//
// Created by qwerty on 23. 3. 2017.
//

#ifndef STELNET_CLIENT_H
#define STELNET_CLIENT_H


#include <string>
#include "tcp_base_object.h"


class Client : public TcpBaseObject{
    int listening_socket,telnet_socket, forward_socket;
    int listening_port;
    int target_port;
    std::string target_address;


public:
    const std::string getClassName() override { return "Client"; };
    void setup(int listen_port, int target_port, const std::string& target_address);

    void live();

private:

};


#endif //STELNET_CLIENT_H
