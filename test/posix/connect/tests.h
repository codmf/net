#ifndef NET_POSIX_CONNECT_H
#define NET_POSIX_CONNECT_H

#include <net/net.h>

#include <stdlib.h>

extern NetHandler * gHandler;

extern int connectPosix(Socket *);

struct SocketData {
    int fd;
    int conn;
};

void setup() {
    gHandler = malloc(sizeof(NetHandler));
    gHandler->connect = &connectPosix;
}

void cleanup() {
    free(gHandler);
}

#endif