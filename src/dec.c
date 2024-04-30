#include "common.h"

// the main routine of server/recver
inline void *server_handling(void *params){
    // recv msg
    char *recv_buffer = (char *)malloc(MAX_BYTES);
    struct pthread_params *ptr = params;

    full_transfer(ptr->in_fd, ptr->out_fd, recv_buffer, 1);

    // release the memory
    close(ptr->in_fd);
    close(ptr->out_fd);
    free(ptr);
    free(recv_buffer);
    // close_cry();
}

// try to open a file through file_name, but return error if already exists
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
    char file_name[MAX_BYTES], *port_ = argv[1], tmp = 0;
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
        1.5: initialize the key
    */
    init_key();

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
        ptr->in_fd = open(file_name, O_RDONLY, 0644); // input fd is from local
        file_name[Len - 4] = '\0';                    // output file name should remove suffix ".pur"
        ptr->out_fd = open_target(file_name);
        if (ptr->out_fd >= 0 && ptr->in_fd >= 0)
            (*server_handling)((void *)ptr);
        else if (ptr->in_fd < 0){
            printf("Input file does not exist, abort\n");
        }
    }
    else{
        // initial some variables
        int sock = 0;
        struct sockaddr_in address;
        server_init(SOCK_STREAM, &sock, &address, port_);
        global_server_sock = sock;
        present_thread = 0;
        thread_arr = (pthread_t *)malloc(MAX_THREADS * sizeof(pthread_t));
        pthread_mutex_init(mutex, NULL);

        while(fd_unified = TCP_accept_with_server_fd(&sock, &address)){
            // read file_name from sender
            int rmt_len = read(fd_unified, file_name, MAX_BYTES);
            file_name[rmt_len] = '\0';
            printf("Recv file name: %s (from sender)\n", file_name);

            struct pthread_params *ptr 
                = (struct pthread_params *)
                    malloc(
                        sizeof(
                            struct pthread_params
                        )
                    );
            ptr->in_fd = fd_unified;  // input fd is from network
            ptr->out_fd = open_target(file_name); // file_name already without suffix ".pur"
            tmp = (ptr->out_fd < 0);
            write(fd_unified, &tmp, 1); // tell the sender whether already exist

            if (tmp){
                close(fd_unified);
                continue;
            }
            pthread_create(
                &thread_arr[present_thread ++], 
                NULL, 
                server_handling, 
                (void *)ptr
            );
        }
    }

    return 0;
}