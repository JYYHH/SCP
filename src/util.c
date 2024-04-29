#include "common.h"

inline void full_transfer(int in_fd, int out_fd, char *buffer, int type_){
    // type_ == 0: encrypt / 1: decrypt
    int msg_length;
    if (type_){
        // decrypt
        while(msg_length = read(in_fd, buffer, MAX_BYTES)){
            // first check integrity
            printf("%d\n", msg_length);
            int ret = check_integrity((unsigned char *)buffer, &msg_length);
            if (!ret){
                printf("Encounter the integrity issues...\n");
            }
            decrypt((unsigned char *)buffer, msg_length);
            unpadding_((unsigned char *)buffer, &msg_length);
            write(out_fd, buffer, msg_length);
        }
    }
    else{
        // encrypt
        while(msg_length = read(in_fd, buffer, MAX_BYTES - 48)){ // padding 16 + HMAC 32
            padding_((unsigned char *)buffer, &msg_length);
            encrypt((unsigned char *)buffer, msg_length);
            append_HMAC((unsigned char *)buffer, &msg_length);
            printf("%d\n", msg_length);
            write(out_fd, buffer, msg_length);
        }
    }
}