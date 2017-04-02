//
// Created by qwerty on 24. 3. 2017.
//

#include <iostream>

#include "server.h"

void Server::setup(int listen_port, int telnetd_port) {
    this->client_port = listen_port;
    this->telnetd_port = telnetd_port;

}

void Server::live() {

    try {
        bool end = false;
        bool client_connected = false;


        std::cout << "connecting to telnetd..." << std::endl;
        telnetd_socket = socket();
        connect(telnetd_socket, "127.0.0.1", telnetd_port);
        std::cout << "connected to telnetd..." << std::endl;


        listening_socket = socket();
        set_reuse(listening_socket);
        bind(listening_socket, client_port, false);
        listen(listening_socket);

        std::vector<int> socketgroup;
        std::vector<int> sel_group;
        socketgroup.push_back(listening_socket);
        socketgroup.push_back(telnetd_socket);

        std::string buffer;

        while (!end) {
            sel_group = select(socketgroup, 1000);
            for ( int socket : sel_group) {
                if (socket == listening_socket) {
                    client_socket = accept(listening_socket);
                    socketgroup.push_back(client_socket);
                    client_connected = true;
                    send(client_socket,buffer);
                    std::cout << "connected client" << std::endl;
                }
                if (socket == client_socket) {
                    buffer = TcpBaseObject::recv(client_socket);
                    std::cout << "s >> " << buffer.size() << std::endl;
                    send(telnetd_socket,buffer);
                }
                if (socket == telnetd_socket) {
                    buffer = TcpBaseObject::recv(telnetd_socket);
                    if (buffer.size()==0) {
                        end = true;
                        continue;
                    }
                    std::cout << "s << " << buffer.size() << std::endl;
                    if (client_connected) send(client_socket,buffer);

                }
            }
        }

        for (int socket : socketgroup) {
            close(socket);
        }



    } catch (tcpException e) {
        std::cerr << e.what() << std::endl;
    }
}

