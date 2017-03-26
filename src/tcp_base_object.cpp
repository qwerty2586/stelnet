#include "tcp_base_object.h"

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

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



