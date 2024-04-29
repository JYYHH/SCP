#pragma once
#include <gcrypt.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <stdlib.h>
#include <netinet/in.h>

#define MAX_BYTES 1024

struct pthread_params{
	int in_fd;
	int out_fd;
};

// "network.c"
    // client (purenc) side initialization
extern inline void client_init(
                        int type, 
                        int *sock_p, 
                        struct sockaddr_in *address,  
                        char const* ip_address,
                        char const* port
                    );
    // TCP connect try from client (purenc)
extern inline void TCP_connect(
                        int *sock_p, 
                        struct sockaddr_in *address
                    );
    // server (purdec) side initialization
extern inline void server_init(
                        int type, 
                        int *sock_p, 
                        struct sockaddr_in *address,  
                        char const* port
                    );
    // server blocked waiting for the client
extern inline int TCP_accept_with_server_fd(
                        int *sock_p, 
                        struct sockaddr_in *address
                    );

// "dec.c"
extern inline void *server_handling(void *params);
extern inline int open_target(char *file_name);

// "crypto.c"
extern char *key, *password;
extern int key_len, pass_len;
extern inline void init_key();