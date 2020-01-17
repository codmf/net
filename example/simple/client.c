#include "net/net.h"
#include "net/error.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

int main(int argc, char ** argv) {
    netSetup();

    bool run = true;
    while (run) {
        fprintf(stdout, "Enter a URL: ");

        char host[100];
        unsigned port;
        scanf("%99s %u", host, &port);

        if (strcmp(host, "close") == 0) {
            fprintf(stdout, "Finishing up\n");
            run = false;
            continue;
        }

        Socket * sock = netSocket(CLIENT, TCP);

        netSetAddress(sock, host);
        netSetPort(sock, port);

        fprintf(stdout, "Connecting to %s:%u\n", host, port);
        int ce;
        if ((ce = netConnect(sock)) != ESUCCESS) {
            fprintf(stderr, "Error connecting: %d\n", ce);

            netCleanup();
            exit(EXIT_FAILURE);
        }

        char request[] = "GET / HTTP/1.0\r\n\r\n";
        netSend(sock, request, sizeof(request)/sizeof(char));

        char * response = netReceiveText(sock);
        fprintf(stdout, "Received message:\n\n%s\n\n", response);
        free(response);

        netClose(&sock);
    }

    netCleanup();
    exit(EXIT_SUCCESS);
}
