#include "common.h"

inline void full_transfer(int in_fd, int out_fd, char *buffer, int type_){
    // type_ == 0: encrypt / 1: decrypt
    int msg_length;
    while(msg_length = read(in_fd, buffer, MAX_BYTES)){
        if (type_){
            decrypt((unsigned char *)buffer, msg_length);
            unpadding_((unsigned char *)buffer, &msg_length);
        }
        else{
            padding_((unsigned char *)buffer, &msg_length);
            encrypt((unsigned char *)buffer, msg_length);
        }
        write(out_fd, buffer, msg_length);
    }
}