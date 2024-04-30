#include "common.h"

int present_thread;
pthread_t *thread_arr; 
int global_server_sock;
pthread_mutex_t mutex;

// for server's quit
inline void sigint_handler(int sig){
    printf("\nCaught SIGINT, AND The server will quit safely!\n");
    // wait for all sub-threads
	for(int i = 0; i < present_thread; i++)
		pthread_join(thread_arr[i], NULL);
	// clean-up
	shutdown(global_server_sock, SHUT_RDWR);
    close_cry();
    exit(0);   
}    

/*
    add for real world remote (insteed of local network) transmission.
    Although through TCP and we can also set flags of recv() to wait for as many bytes delivered,
        WE STILL CAN'T AVOID chunk_size < 1024 read by the recver (one time, except the last chunk).
        And this will hurt our crypto functionality
        SO here is the solution:
            we read until we reach the last chunk of data (sender will close the socket to notify us),
                or reach the 'max_bytes',
            and this will help us build a consistent protocol through both sides
*/
inline int read_from_network(int fd, char *buffer, int max_bytes){
    int total_bytes = 0, msg_length;
    while(total_bytes < max_bytes){
        msg_length = read(fd, buffer + total_bytes, max_bytes - total_bytes);
        if (msg_length <= 0)
            break;
        total_bytes += msg_length;
    }

    return total_bytes;
}

/*
    func for the main file transfer routine
*/
inline void full_transfer(int in_fd, int out_fd, char *buffer, int type_){
    // in_fd: fd for read
    // out_fd: fd for write
    // type_ == 0: encrypt / 1: decrypt
    int msg_length;
    if (type_){
        // decrypt
        while(msg_length = read_from_network(in_fd, buffer, MAX_BYTES)){ // may from network, but act identical as 'read' when from local
            pthread_mutex_lock(&mutex);
            printf("THREAD %d read %d bytes from network/local\n", pthread_self(), msg_length);
            // first check integrity
            int ret = check_integrity((unsigned char *)buffer, &msg_length);
            if (!ret){
                printf("Encounter an integrity issue... please check for it\n");
            }
            // decrypt
            decrypt((unsigned char *)buffer, msg_length);
            // unpad
                // TODO-JHY: later implementation, use the return value of it to delete incorrect transmission
            unpadding_((unsigned char *)buffer, &msg_length);
            printf("THREAD %d write %d bytes to local\n", pthread_self(), msg_length);
            pthread_mutex_unlock(&mutex);
            
            // in order to improve the performance, this write should not be in the sritical section
            write(out_fd, buffer, msg_length);
        }
    }
    else{
        // encrypt
        while(msg_length = read(in_fd, buffer, MAX_BYTES - 48)){ // padding 16 + HMAC 32
            printf("read %d bytes from local\n", msg_length);
            // pad
            padding_((unsigned char *)buffer, &msg_length);
            // encrypt
            encrypt((unsigned char *)buffer, msg_length);
            // add the hashing value
            append_HMAC((unsigned char *)buffer, &msg_length);
            printf("write %d bytes to network/local\n", msg_length);
            write(out_fd, buffer, msg_length);
        }
    }
}