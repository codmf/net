#ifndef NET_NET_H
#define NET_NET_H

#include "util.h"

NET_BEGIN_C_DECLS

#include <stdlib.h>

#ifndef DEFAULT_PORT
#define DEFAULT_PORT 0
#endif

/** The side of a connection */
enum ENetSide {
    CLIENT, /* The client side of a connection */
    SERVER /* The server side of a connection */
};

/** The type of connection to make */
enum ENetType {
    TCP, /* A Transmission Control Protocol socket */
    UDP /* A User Datagram Protocol socket */
};

/** The type of the specified address address */
enum EAddressType {
    UNSPEC, /* An unspecified address */
    IPv4, /* An IPv4 address */
    IPv6 /* An IPv6 address */
};

/** Implementation specific data to associate with a socket */
typedef struct SocketData SocketData;

/** A generic socket */
typedef struct Socket {
    int side; /* The side of the socket. Corresponds to ENetSide */
    int type; /* The type of the socket. Corresponds to ENetType */
    const char * address; /* The address to connect to or listen on */
    int addressType; /* The type of the address */
    unsigned port; /* The port to connect to or listen on */
    SocketData * data; /* Implementation specific data associated with this socket */
} Socket;

typedef struct MessageData MessageData;

typedef struct Message {
    MessageData * data;
    const char * address;
    unsigned port;
    void * buffer;
    size_t size;
} Message;

/** A collection of functions to allow implementations dynamically change how different tasks are handled */
typedef struct NetHandler {
    SocketData * (* initialize)(int side, int type); /* Initialize socket data for when creating a socket */
    int (* connect)(Socket *); /* Client. Connect to the specified server and prepare the environment to use the connection */
    int (* start)(Socket *); /* Server. Initialize a socket and prepare it for incoming connections */
    int (* loop)(Socket *); /* Server. Accept a connection and prepare the environment to use the connection */
    int (* receive)(Socket *, void *, size_t, ssize_t *); /* Receive data from the currently opened connection. Works on client and server */
    int (* send)(Socket *, const void *, size_t); /* Send data on the currently opened connection. Works on client and server */
    int (* closeConnection)(Socket *); /* Server. Close the currently opened connection and free memory relating to it */
    int (* close)(Socket **); /* Close and dispose of a socket. This should free the socket and any related data */
    void (* cleanup)(); /* Clean up the environment and free any related memory */
} NetHandler;

/** Setup the environment for networking. This basically just sets the environment to whatever
    setupPlatform spits out, so it you wanted to use your own handler, you could simply omit this
    call and set handler to your implementation. */
NET_API
void netSetup();

/** An implementation specific setup function to provide a handler for said implementation */
NET_API
NetHandler * netSetupPlatform();

/** Get the current handler */
NET_API
NET_PURE
NET_NO_THROW
NetHandler * netGetHandler();

/** Set the current handler */
NET_API
NET_NO_THROW
void netSetHandler(NetHandler *);

/** Create a socket using the specified side and type. Side refers to ENetSide and type refers to ENetType */
NET_API
NET_RETURNS_NON_NULL
Socket * netSocket(int side, int type);

/** Cleanup the environment of networking related things. This should be the last call when using networking.
    After this is called, it would need to be setup again before networking functions can be used. */
NET_API
void netCleanup();

// Functions to manipulate the ip and port of the socket

/** Set a socket's address */
NET_API
NET_NO_THROW
NET_NON_NULL(1)
void netSetAddress(NET_NO_ESCAPE Socket * NET_RESTRICT, const char *);

/** Get a socket's address */
NET_API
NET_PURE
NET_NO_THROW
NET_NON_NULL(1)
const char * netGetAddress(NET_NO_ESCAPE Socket * NET_RESTRICT);

/** Set the socket's address' type */
NET_API
NET_NO_THROW
NET_NON_NULL(1)
void netSetAddressType(NET_NO_ESCAPE Socket * NET_RESTRICT, int);

/** Get the socket's address' type */
NET_API
NET_PURE
NET_NO_THROW
NET_NON_NULL(1)
int netGetAddressType(NET_NO_ESCAPE Socket * NET_RESTRICT);

/** Set a socket's port */
NET_API
NET_NO_THROW
NET_NON_NULL(1)
void netSetPort(NET_NO_ESCAPE Socket * NET_RESTRICT, unsigned);

/** Get a socket's port */
NET_API
NET_PURE
NET_NO_THROW
NET_NON_NULL(1)
unsigned netGetPort(NET_NO_ESCAPE Socket * NET_RESTRICT);

// Functions relating to the manipulation of a socket

/** Client side only. This will connect the specified socket to whatever address and port
    were previously assigned. Once connected, data can be sent and received on this socket. */
NET_API
NET_NON_NULL(1)
int netConnect(Socket *);

/** Server side only. Starts the socket on the previously assigned address and port. Once
    started, connections can be listened for */
NET_API
NET_NON_NULL(1)
int netStart(Socket *);

/** Server side only. Listen for a connection on the socket. When one is made, return ESUCCESS
    and prepare the socket for further access to the connection. */
NET_API
NET_NON_NULL(1)
int netLoop(Socket *);

/** Recieve data from the socket. Put the data recieved in the specified port up to the specified
    count. The number of bytes actually read will be returned and 0 if the connection closed. */
NET_API
NET_NON_NULL(1, 2, 4)
int netReceive(Socket *, void *, size_t, ssize_t *);

/** Send data on the socket. Use the data in the specified buffer up to the specified count. */
NET_API
NET_NON_NULL(1, 2)
int netSend(Socket *, const void *, size_t);

/** Server side only. Close the currently open connection. */
NET_API
NET_NON_NULL(1)
int netCloseConnection(Socket *);

/** Close the socket. This should be a reference to the socket pointer, and will free and null the pointer
    so that it cannot be used later. */
NET_API
NET_NON_NULL(1)
int netClose(Socket **);

NET_END_C_DECLS

#endif
