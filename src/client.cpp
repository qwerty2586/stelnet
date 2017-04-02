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
        bool end = false;
        bool client_connected = false;
        forward_socket = socket();
        connect(forward_socket, target_address, target_port);
        std::cout << "connected to server... now listening for telnet" << std::endl;

        listening_socket = socket();
        set_reuse(listening_socket);
        bind(listening_socket, listening_port, false);
        listen(listening_socket);



        std::vector<int> socketgroup;
        std::vector<int> sel_group;
        socketgroup.push_back(listening_socket);
        socketgroup.push_back(forward_socket);

        std::string buffer;

        while (!end) {
            sel_group = select(socketgroup, 1000);
            for (int socket : sel_group) {
                if (socket == listening_socket) {
                    telnet_socket = accept(listening_socket);
                    socketgroup.push_back(telnet_socket); // pridame si ho do skupinky
                    std::cout << "late " << buffer.size() << std::endl;
                    send(telnet_socket,buffer);
                    client_connected = true;
                    std::cout << "connected telnet" << std::endl;
                }
                if (socket == forward_socket) {
                    buffer = TcpBaseObject::recv(forward_socket);

                    std::cout << "c << " << buffer.size() << std::endl;
                    if (client_connected) send(telnet_socket,buffer);
                    //send(telnet_socket,buffer);

                }
                if (socket == telnet_socket) {
                    buffer = TcpBaseObject::recv(telnet_socket);
                    std::cout << "c >> " << buffer.size() << std::endl;
                    send(forward_socket, buffer);
                }


            }
        }


        listening_socket = socket();
        set_reuse(listening_socket);
        bind(listening_socket, listening_port, false);
        listen(listening_socket);
        telnet_socket = accept(listening_socket);
        send(telnet_socket, "hello client, I live you...\n\r");
        send(telnet_socket, "telnet is love, telnet is lyfe\n\r");


    } catch (tcpException e) {
        std::cerr << e.what() << std::endl;
    }

    //send(telnet_socket,"ahoj",4,0);
    //close(telnet_socket);








}

