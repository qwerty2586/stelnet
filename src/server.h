//
// Created by qwerty on 24. 3. 2017.
//

#ifndef STELNET_SERVER_H
#define STELNET_SERVER_H

#include "tcp_base_object.h"


#define SYM_KEY_LENGTH 16
#define IV_LENGTH 16


class Server : public TcpBaseObject {
    int client_socket,listening_socket,telnetd_socket;
    int client_port,telnetd_port;
public:

    const std::string getClassName() override { return "Server"; };

    void setup(int listen_port, int telnetd_port);

    void live();

};




#endif //STELNET_SERVER_H
