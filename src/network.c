// my wrapper for the network workload
#include "common.h"

inline void client_init(
                        int type, 
                        int *sock_p, 
                        struct sockaddr_in *address,  
                        char const* ip_address,
                        char const* port
                    ){
    // create a socket
	if ((*sock_p = socket(AF_INET, type, 0)) < 0) {
		printf("\n Socket creation error \n");
		exit(1);
	}

	// URL2IP_binary 
	address->sin_family = AF_INET;
	address->sin_port = htons(atoi(port));
    struct hostent *host = gethostbyname(ip_address);
    if (host == NULL){
		printf("\nInvalid Address not supported \n");
		exit(0);
    }
    address->sin_addr = *((struct in_addr *)(host->h_addr));
}

inline void TCP_connect(
                        int *sock_p, 
                        struct sockaddr_in *address
                    ){
	if (
        connect(
                *sock_p, 
                (struct sockaddr *)address, 
                sizeof(*address)
            )
        < 0
       ) {
		printf("\nTCP Connection Failed \n");
		exit(0);
	}
}

inline void server_init(
                        int type, 
                        int *sock_p, 
                        struct sockaddr_in *address,  
                        char const* port
                    ){
    int opt = 1;

    // Creating TCP/UDP socket
	if ((*sock_p = socket(AF_INET, type, 0)) < 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	if (setsockopt(*sock_p, SOL_SOCKET, 
            SO_REUSEADDR, &opt, sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address->sin_family = AF_INET;
	address->sin_addr.s_addr = INADDR_ANY; // any incoming client will be served
	address->sin_port = htons(atoi(port));

	// BIND
	if (
        bind(
            *sock_p, 
            (struct sockaddr *)address,
            sizeof(*address)
        ) 
        < 0
       ) {
	    perror("bind failed");
		exit(EXIT_FAILURE);
	}
    // LISTEN
    printf("Server (Purdec) is listening on port %s\n", port);
    if (listen(*sock_p, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
}

inline int TCP_accept_with_server_fd(
                        int *sock_p, 
                        struct sockaddr_in *address
                    ){
    // return the server_file_descriptor
    int ret_fd, addrlen = sizeof(*address);
    if (
        (
            ret_fd
              = 
            accept(
                *sock_p, 
                (struct sockaddr *)address, 
                (socklen_t *)&addrlen
            )
        ) 
        < 0
       ){
        perror("accept");
        exit(EXIT_FAILURE);
    }
    
    return ret_fd;
}