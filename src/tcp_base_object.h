//
// Created by qwerty on 25. 3. 2017.
//

#ifndef STELNET_TCP_BASE_OBJECT_H
#define STELNET_TCP_BASE_OBJECT_H

#include <exception>
#include <string>
#include <vector>
#include "key_file.h"

#define BUFFER_SIZE 4096

class TcpBaseObject {

public:
    void bind(int socket, int port, bool localhost);
    void listen(int socket);
    void set_reuse(int socket);
    int csocket();
    int accept(int socket);
    void connect(int socket, const std::string &address, int port);
    void send(int socket, const std::string &message);
    std::vector<int> select(std::vector<int> &socket_group, int ms_timeout);
    void remove_socket(std::vector<int> &socket_group,int socket);
    void add_socket(std::vector<int> &socket_group,int socket);
    virtual const std::string getClassName() = 0;

    uint16_t recv(int socket, uint8_t *buffer, uint16_t length = BUFFER_SIZE);
    uint8_t recvchar(int socket);

    // force versions of recv
    void f_recv(int socket, uint8_t *buffer, uint16_t length = BUFFER_SIZE);
    uint8_t f_recvchar(int socket);


    void send(int socket, uint8_t *buffer, uint16_t length = BUFFER_SIZE);
    void sendchar(int socket,uint8_t character);

    void close(int socket);


    void setKeyfile(KeyFile *key_file) { this->key_file = key_file;};

protected:
    KeyFile *key_file = nullptr;
};

class tcpException : public std::exception {
public:

    explicit tcpException(TcpBaseObject *object, const std::string &message) {
        this->msg_ = object->getClassName() + ": " + message;
    }

    virtual ~tcpException() throw (){}

    virtual const char* what() const throw() {
        return msg_.c_str();
    }

protected:
    std::string msg_;

};

class sendRecvException : public tcpException {
public:
    explicit sendRecvException(TcpBaseObject *object, const std::string &message) : tcpException(object, message) {}

};



#endif //STELNET_TCP_BASE_OBJECT_H
