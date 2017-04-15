#include "tcp_base_object.h"

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <algorithm>
#include <unistd.h>

#define BUFFER_SIZE 6000



void TcpBaseObject::bind(int socket, int port, bool localhost) {
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    if (localhost) {
        addr.sin_addr.s_addr = PF_LOCAL;// chceme jenom na localhostu poslouchat
    } else {
        addr.sin_addr.s_addr = INADDR_ANY;// chceme  poslouchat vsude
    }
    addr.sin_port = htons((uint16_t) port);
    if (::bind(socket, (struct sockaddr *) &addr, sizeof(addr)) < 0)
        throw tcpException(this, "bind error");

}

void TcpBaseObject::listen(int socket) {
    if (::listen(socket, 5) < 0)
        throw tcpException(this, "listen error");

}

void TcpBaseObject::set_reuse(int socket) {
    int enable_flag = 1;
    if (::setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &enable_flag, sizeof(int)) < 0)
        throw tcpException(this, "set reuse error");
}

int TcpBaseObject::socket() {
    int s = ::socket(AF_INET, SOCK_STREAM, 0);
    if (s<0) throw tcpException(this, "socket creation error");
    return s;
}

int TcpBaseObject::accept(int socket) {
    struct sockaddr_in addr;
    socklen_t addr_len;
    int new_socket = ::accept(socket, (struct sockaddr *) &addr,&addr_len);
    return new_socket;
}

void TcpBaseObject::connect(int socket, const std::string &address, int port) {
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons((u_short) port);
    addr.sin_addr.s_addr = inet_addr(address.c_str());

    if (::connect(socket, (struct sockaddr*)&addr, sizeof(addr)) < 0)
        throw tcpException(this, "cant connect error");


}

void TcpBaseObject::send(int socket, const std::string &message) {
    if (::send(socket,message.c_str(),message.size(),0) < message.size())
        throw tcpException(this,"cant send");
}

std::string TcpBaseObject::recv(int socket) {
    char buff[BUFFER_SIZE];
    ssize_t len = ::recv(socket, buff, BUFFER_SIZE - 1, 0);
    if (len<1) throw tcpException(this,"cant rcv");
    std::string r(buff,(unsigned long)len);
    return r;
}

std::string TcpBaseObject::recv(int socket, int length) {
    char buff[BUFFER_SIZE];
    ssize_t len = ::recv(socket, buff, length, 0);
    if (len<1) throw tcpException(this,"cant rcv");
    std::string r(buff,(unsigned long)len);
    return r;
}

std::vector<int> TcpBaseObject::select(std::vector<int> &socket_group, int ms_timeout) {
    std::vector<int> r;
    if (socket_group.size() < 1)
        return r;
    int max_socket = -1;
    fd_set rfds;

    // timeval ma hodnoty sekundy a microsekundy
    timeval tv;
    tv.tv_sec = ms_timeout / 1000;
    tv.tv_usec = (ms_timeout % 1000) * 1000;

    // nastavime skupinu
    FD_ZERO(&rfds);
    for (int socket : socket_group) {
        max_socket = std::max(max_socket,socket);
        FD_SET(socket, &rfds);
    }
    int count  = ::select(max_socket+1,&rfds, nullptr, nullptr, &tv); // prej musi bejt +1

    if (count == -1)
        throw tcpException(this,"cant select");
    for (int socket : socket_group) {
        if (FD_ISSET(socket, &rfds))
            r.push_back(socket);
    }
    return r;
}

void TcpBaseObject::close(int socket) {
    ::close(socket);

}

void TcpBaseObject::remove_socket(std::vector<int> &socket_group, int socket) {
    socket_group.erase
            (std::remove(socket_group.begin(),socket_group.end(),socket),socket_group.end());

}

void TcpBaseObject::add_socket(std::vector<int> &socket_group, int socket) {
    socket_group.push_back(socket);
}

int16_t TcpBaseObject::recv(int socket, char *buffer, uint16_t length) {
    ssize_t len = ::recv(socket, buffer, length, 0);
    if (len<1) throw tcpException(this,"cant rcv");
    return (int16_t) len;
}

uint8_t TcpBaseObject::recvchar(int socket) {
    char x;
    ssize_t len = ::recv(socket, &x, 1, 0);
    if (len<1) throw tcpException(this,"cant rcv");
    return (uint8_t) x;
}

void TcpBaseObject::send(int socket, char *buffer, uint16_t length) {
    if (::send(socket,buffer,length,0) < length)
        throw tcpException(this,"cant send");

}

void TcpBaseObject::sendchar(int socket, uint8_t character) {
    if (::send(socket,&character,1,0) < 1)
        throw tcpException(this,"cant send");

}

