#include "common.h"

unsigned char *key;
char *password;
int key_len, pass_len;
gcry_cipher_hd_t handle;

inline void init_key(){
    gcry_control(GCRYCTL_SUSPEND_SECMEM_WARN);
    gcry_control(GCRYCTL_INIT_SECMEM, 16384, 0);
    gcry_control(GCRYCTL_RESUME_SECMEM_WARN);
    gcry_control(GCRYCTL_INITIALIZATION_FINISHED, 0);
    // finish gcrypt initialization

    key = (unsigned char *)malloc(32);
    password = (char *)malloc(MAX_BYTES);
    printf("Password: ");
    scanf("%s", password);
    key_len = 32;
    pass_len = strlen(password);

    static const char *salt = "please_dont_delete_the_code_carelessly_again_thanks_to_myself";

    gcry_kdf_derive(
        password, pass_len,
        GCRY_KDF_PBKDF2,                    // KDF algorithm
        GCRY_MD_SHA256,                     // Hash function
        salt, strlen(salt),
        3000,                               // Iteration round
        key_len, key
    );

    // printf("Key = %x\n", *((int *)key));
}

inline void init_cipher(){
    gcry_cipher_open(&handle, GCRY_CIPHER, GCRY_MODE, 0);
    gcry_cipher_setkey(handle, key, key_len);
}

// need padding and unpadding, since the "gcry_cipher_encrypt" and "gcry_cipher_decrypt" only support length is multiple block_sz
inline void padding_(unsigned char *buffer, int *len_pt){
    int pad_size = (16 - (*len_pt & 0xf)) & 0xf; // block_size = 16
    printf("Padding size = %d\n", pad_size);
    if (pad_size == 0)
        return;
    for (int i = 0; i < pad_size; i++, (*len_pt)++){
        buffer[*len_pt] = pad_size;
    }
}

inline void unpadding_(unsigned char *buffer, int *len_pt){
    unsigned char pad_num = buffer[(*len_pt) - 1];
    int pos = *len_pt;
    for(pos--; buffer[pos] == pad_num; pos--);

    if ((*len_pt) - (pos + 1) < pad_num){
        // no padding
        return;
    }
    memset(buffer + (*len_pt) - pad_num, 0, pad_num); // reset to 0
    *len_pt -= pad_num;
}

inline void encrypt(unsigned char *buffer, int len){
    static const unsigned char iv[16] = "1234567890123456"; 
    gcry_cipher_setiv(handle, iv, sizeof(iv));
    gcry_cipher_encrypt(
        handle, 
        buffer, 
        len, 
        NULL,
        0
    );
}

inline void decrypt(unsigned char *buffer, int len){
    static const unsigned char iv[16] = "1234567890123456"; 
    gcry_cipher_setiv(handle, iv, sizeof(iv));
    gcry_cipher_decrypt(
        handle, 
        buffer, 
        len, 
        NULL,
        0
    );
}

inline void close_cry(){
    free(key);
    free(password);
    gcry_cipher_close(handle);
}