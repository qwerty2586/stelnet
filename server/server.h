//
// Created by qwerty on 24. 3. 2017.
//

#ifndef STELNET_SERVER_H
#define STELNET_SERVER_H

class Server {
    int client_socket,listening_socket,telnetd_socket;
    int client_port,telnetd_port;
public:


    void setup(int listen_port, int telnetd_port);

    void live();
};




#endif //STELNET_SERVER_H
