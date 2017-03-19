#include <iostream>
#include <getopt.h>

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
        {NULL,           no_argument,       NULL, 0}
};

int main(int argc, char *argv[]) {

    bool client = false;
    bool server = false;
    int listenport = DEFAULT_LISTEN_PORT;
    int port = DEFAULT_PORT;
    int telnetport = DEFAULT_TELNET_PORT;
    int telnetdport = DEFAULT_TELNETD_PORT;
    std::string address = "";

    int choice;
    while ((choice = getopt_long(argc, argv, "scl:p:t:d:a:h", longOpts, NULL)) != -1) {
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
            case 'h': // print help;
                break;
        }
    }

    

    return 0;
}