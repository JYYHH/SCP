#include "common.h"

inline void *server_handling(void *params){
    // recv msg
    int msg_length;
    char *recv_buffer = (char *)malloc(MAX_BYTES);

    msg_length = read(*((int *) params), recv_buffer, MAX_BYTES);
    printf("%d\n", (int)recv_buffer[0]);

    // release the memory
    free(params);
    free(recv_buffer);
}

int main(int argc, char const* argv[]){
    /*
        1. Initialization
    */
    char *file_name = NULL, *port_ = argv[1];
    int is_local = 0, option;

    while((option = getopt(argc, argv, "l")) > 0){
        switch(option) {
            case 'l':
                is_local = 1;
                file_name = optarg;
                break;
            default:
                printf("Unknown option %c\n", option);
                exit(1);
        }
    }
    
    /*
        2. Read from the source
    */
    int fd_unified = 0;

    if (is_local){
        // ...
    }
    else{
        // initial version, just one time accept a client
            // (TODO-JHY): add multithreading version..
        int sock = 0;
        struct sockaddr_in address;
        server_init(SOCK_STREAM, &sock, &address, port_);

        while(fd_unified = TCP_accept_with_server_fd(&sock, &address)){
            // pass the file discriptor to the sub-thread
            int *ptr = (int *)malloc(4);
            *ptr = fd_unified;
            (*server_handling)((void *)ptr);

            break; // only one time serve in present
        }
    }

    return 0;
}