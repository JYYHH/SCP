#include "common.h"

int main(int argc, char const* argv[]){
    /*
        1. Initialization
    */
    char *ip_ = NULL, *port_ = NULL, *file_name = argv[1], *split = NULL;
    int is_local = 0, option;

    while((option = getopt(argc, argv, "d:l")) > 0){
        switch(option) {
            case 'd':
                split = optarg;
                for(; *split != ':'; split++);
                *(split++) = '\0';
                ip_ = optarg, port_ = split;
                break;
            case 'l':
                is_local = 1;
                break;
            default:
                printf("Unknown option %c\n", option);
                exit(1);
        }
    }
    if (is_local && ip_){
        printf("Want both local and remote, conflict..\n");
        exit(1);
    }

    /*
        2. Write to the target
    */
    int fd_unified = 0, msg_length;
    char send_buffer[MAX_BYTES];

    if (is_local){
        // ...
    }
    else{
        // talk to the remote server (purdec) directly

        struct sockaddr_in serv_addr;
        // client side init, TCP connection
        client_init(SOCK_STREAM, &fd_unified, &serv_addr, ip_, port_);

        // TCP socket will first build the connection
        TCP_connect(&fd_unified, &serv_addr);
    }

        // 2.2: Unifed write 
    msg_length = 1;
    send_buffer[0] = 66;
    send(fd_unified, send_buffer, msg_length, 0);

    return 0;
}