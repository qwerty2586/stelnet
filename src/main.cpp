#include <iostream>
#include <getopt.h>
#include <stdlib.h>
#include <thread>
#include <unistd.h>
#include "client.h"
#include "server.h"
#include "log.h"
#include "key_file.h"


#define DEFAULT_LISTEN_PORT 5000;
#define DEFAULT_PORT 5000;
#define DEFAULT_TELNET_PORT 23;
#define DEFAULT_TELNETD_PORT 23;

static struct option longOpts[] = {
        {"server",       no_argument,       NULL, 's'},
        {"client",       no_argument,       NULL, 'c'},
        {"listen-port",  required_argument, NULL, 'l'},
        {"port",         required_argument, NULL, 'p'},
        {"telnet-port",  required_argument, NULL, 't'},
        {"telnetd-port", required_argument, NULL, 'd'},
        {"address",      required_argument, NULL, 'a'},
        {"help",         no_argument,       NULL, 'h'},
        {"no-log",       no_argument,       NULL, 'n'},
        {"key-file",     required_argument, NULL, 'k'},
        {NULL,           no_argument,       NULL, 0}
};

int main(int argc, char *argv[]) {

    bool client = false;
    bool server = false;
    bool log = true;
    int listenport = DEFAULT_LISTEN_PORT;
    int port = DEFAULT_PORT;
    int telnetport = DEFAULT_TELNET_PORT;
    int telnetdport = DEFAULT_TELNETD_PORT;
    std::string address = "";
    std::string key_file_path = "";
    KeyFile* keyFile = nullptr;

    int choice;
    while ((choice = getopt_long(argc, argv, "scl:p:t:d:a:hnk:", longOpts, NULL)) != -1) {
        switch (choice) {
            case 's':
                server = true;
                break;
            case 'c':
                client = true;
                break;
            case 'l':
                listenport = atoi(optarg);
                break;
            case 'p':
                port = atoi(optarg);
                break;
            case 't':
                telnetport = atoi(optarg);
                break;
            case 'd':
                telnetdport = atoi(optarg);
                break;
            case 'a':
                address = optarg;
                break;
            case 'h': // printdata help;
                break;
            case 'n':
                log = false;
                break;
            case 'k':
                key_file_path = optarg;
                break;
        }
    }

    if (key_file_path.size() == 0 ) {
        std::cout << "No keyfile specified can't continue" << std::endl;
        return 0;
    }

    keyFile = new KeyFile(key_file_path);

    logging_enabled = log;

    auto s_thread = std::thread([&]() {
        if (server) {
            Server *s = new Server();
            s->setup(listenport, telnetdport);
            if (keyFile) s->setKeyfile(keyFile);
            s->live();


        }
    });

    auto c_thread = std::thread([&]() {
        if (client) {
            Client *c = new Client();
            c->setup(telnetport, port, address);
            if (keyFile) c->setKeyfile(keyFile);
            c->live();
        }
    });

    if (server) s_thread.join();
    if (client) c_thread.join();



    return 0;
}