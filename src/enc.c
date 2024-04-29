#include "common.h"

int main(int argc, char const* argv[]){
    /*
        1. Initialization
    */
    char *ip_ = NULL, *port_ = NULL, *file_name = argv[1], *split = NULL, tmp = 0;
    int is_local = 0, option, in_fd = open(file_name, O_RDONLY, 0644);

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
        1.5: initialize the key
    */
    init_key();

    /*
        2. Write to the target
    */
    int fd_unified = 0;
    char send_buffer[MAX_BYTES];

    if (is_local){
        char present[MAX_BYTES];
        int len = strlen(file_name);
        memcpy(present, file_name, len);
        present[len] = '\0';
        strcat(present, ".pur");
        fd_unified = open(present, O_WRONLY | O_CREAT, 0644);
    }
    else{
        struct sockaddr_in serv_addr;
        // client side init, TCP connection
        client_init(SOCK_STREAM, &fd_unified, &serv_addr, ip_, port_);

        // TCP socket will first build the connection
        TCP_connect(&fd_unified, &serv_addr);
        write(fd_unified, file_name, strlen(file_name));
        read(fd_unified, &tmp, 1); // get the feedback
    }

        // 2.2: Unifed write 

    if (tmp == 0){
        full_transfer(in_fd, fd_unified, send_buffer, 0, 0); // the last parameter for the sender is useless, just feel free
    }
    else{
        printf("File already exists (local or remote side), abort\n");
    }

    /*
        3. end the program
    */
    close(in_fd);
    close(fd_unified);
    close_cry();

    return 0;
}