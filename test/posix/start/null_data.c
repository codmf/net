#include "tests.h"

int main() {
    setup();

    Socket * sock = malloc(sizeof(Socket));
    *sock = (Socket) {.data = NULL};
    assert(netStart(sock) == ENULL_POINTER && "Did not receive the expected value for null data");
    free(sock);

    cleanup();
}
