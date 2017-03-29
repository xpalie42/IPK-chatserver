/*
 * Created by xpalie00.
 * Module handling accepting clients on serverside.
 */

#ifndef FTREST_CONNECTIONLISTENER_H
#define FTREST_CONNECTIONLISTENER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

class ConnectionListener {
private:
    uint16_t port_number;
    int sock_fd;
    socklen_t clilen;
    struct sockaddr_in cli_addr;

public:
    ConnectionListener(uint16_t port);
    int acceptConnection();
    ~ConnectionListener();
    void closeFd();
    void start();
};


#endif //FTREST_CONNECTIONLISTENER_H
