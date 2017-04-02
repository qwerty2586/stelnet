//
// Created by qwerty on 25. 3. 2017.
//

#ifndef STELNET_TCP_BASE_OBJECT_H
#define STELNET_TCP_BASE_OBJECT_H

#include <exception>
#include <string>
#include <vector>

class TcpBaseObject {

public:
    void bind(int socket, int port, bool localhost);
    void listen(int socket);
    void set_reuse(int socket);
    int socket();
    int accept(int socket);
    void connect(int socket, const std::string &address, int port);
    void send(int socket, const std::string &message);
    std::vector<int> select(std::vector<int> &socket_group, int ms_timeout);
    virtual const std::string getClassName() = 0;

    std::string recv(int socket);
};

class tcpException : std::exception {
    std::string message;
public:
    tcpException(TcpBaseObject *object, const std::string &message) {
        this->message = object->getClassName() + ": " + message;
    }
    virtual const char* what() {
        return message.c_str();
    }

};



#endif //STELNET_TCP_BASE_OBJECT_H
