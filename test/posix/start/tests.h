#ifndef NET_POSIX_START_H
#define NET_POSIX_START_H

#include <net/net.h>
#include <net/error.h>

#include <stdlib.h>
#include <assert.h>

extern NetHandler * gHandler;

extern int startPosix(Socket *);

struct SocketData {
    int fd;
    int conn;
};

void setup() {
    gHandler = malloc(sizeof(NetHandler));
    gHandler->start = &startPosix;
}

void cleanup() {
    free(gHandler);
}

#endif
