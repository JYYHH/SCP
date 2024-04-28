#include "common.h"

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
    int fd_unified = 0, msg_length;
    char recv_buffer[MAX_BYTES];

    if (is_local){
        // ...
    }
    else{
        // initial version, just one time accept a client
            // (TODO-JHY): add multithreading version..
        int sock = 0;
        struct sockaddr_in address;
        server_init(SOCK_STREAM, &sock, &address, port_);
        fd_unified = TCP_accept_with_server_fd(&sock, &address);
        msg_length = read(fd_unified, recv_buffer, MAX_BYTES);
        printf("%d\n", (int)recv_buffer[0]);
    }

    return 0;
}