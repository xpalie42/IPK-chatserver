/*
 * Created by xpalie00.
 * Module handling accepting clients on serverside.
 */

#include <cstring>
#include <string>
#include "ConnectionListener.h"

using namespace std;

ConnectionListener::ConnectionListener(uint16_t port) {
    port_number = port;
}

int ConnectionListener::acceptConnection() {
    return accept(sock_fd, (struct sockaddr *) &cli_addr, &clilen);
}

void ConnectionListener::closeFd() {
    close(sock_fd);
}

ConnectionListener::~ConnectionListener() {
    closeFd();
}

void ConnectionListener::start() {
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0)
        throw string("Could not open socket");
    struct sockaddr_in serv_addr;
    memset((char *) &serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port_number);
    if (bind(sock_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        throw string("Could not bind.");
    listen(sock_fd, 5);
    clilen = sizeof(cli_addr);
}
