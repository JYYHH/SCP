#include "common.h"

inline void *server_handling(void *params){
    // recv msg
    int msg_length;
    char *recv_buffer = (char *)malloc(MAX_BYTES);
    struct pthread_params *ptr = params;

    while(msg_length = read(ptr->in_fd, recv_buffer, MAX_BYTES))
        write(ptr->out_fd, recv_buffer, msg_length);

    // release the memory
    close(ptr->in_fd);
    close(ptr->out_fd);
    free(ptr);
    free(recv_buffer);
}

inline int open_target(char *file_name){
    // protocol: file name through network is without extra suffix like ".pur"
    if (access(file_name, F_OK) == 0){
        // already exists
        printf("File already exists, abort\n");
        return -1;
    }
    return open(file_name, O_WRONLY | O_CREAT, 0644);
}

int main(int argc, char const* argv[]){
    /*
        1. Initialization
    */
    char *file_name = (char *)malloc(MAX_BYTES), *port_ = argv[1], tmp = 0;
    int is_local = 0, option, Len;

    while((option = getopt(argc, argv, "l:")) > 0){
        switch(option) {
            case 'l':
                is_local = 1;
                Len = strlen(optarg);
                memcpy(file_name, optarg, Len);
                file_name[Len] = '\0';
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
        struct pthread_params *ptr 
            = (struct pthread_params *)
                malloc(
                    sizeof(
                        struct pthread_params
                    )
                );
        ptr->in_fd = open(file_name, O_RDONLY, 0644);
        file_name[Len - 4] = '\0';
        ptr->out_fd = open_target(file_name);
        if (ptr->out_fd >= 0 && ptr->in_fd >= 0)
            (*server_handling)((void *)ptr);
        else if (ptr->in_fd < 0){
            printf("Input file does not exist, abort\n");
        }
    }
    else{
        // initial version, just one time accept a client
            // (TODO-JHY): add multithreading version..
        int sock = 0;
        struct sockaddr_in address;
        server_init(SOCK_STREAM, &sock, &address, port_);

        while(fd_unified = TCP_accept_with_server_fd(&sock, &address)){
            int rmt_len = read(fd_unified, file_name, MAX_BYTES);
            file_name[rmt_len] = '\0';
            printf("%s\n", file_name);

            // pass the file discriptor to the sub-thread
            struct pthread_params *ptr 
                = (struct pthread_params *)
                    malloc(
                        sizeof(
                            struct pthread_params
                        )
                    );
            ptr->in_fd = fd_unified;
            ptr->out_fd = open_target(file_name);
            tmp = (ptr->out_fd < 0);
            write(fd_unified, &tmp, 1);

            if (tmp){
                close(fd_unified);
                // continue;
                break;
            }

            (*server_handling)((void *)ptr);

            break; // only one time serve in present
        }
    }

    free(file_name); // clean up

    return 0;
}