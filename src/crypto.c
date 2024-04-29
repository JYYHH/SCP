#include "common.h"

unsigned char *key;
char *password;
int key_len, pass_len;
gcry_cipher_hd_t handle, handle_HMAC;

inline void init_key(){
    gcry_control(GCRYCTL_SUSPEND_SECMEM_WARN);
    gcry_control(GCRYCTL_INIT_SECMEM, 16384, 0);
    gcry_control(GCRYCTL_RESUME_SECMEM_WARN);
    gcry_control(GCRYCTL_INITIALIZATION_FINISHED, 0);
    // finish gcrypt initialization

    /*
        1. from password build key
    */
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

    /*
        2. Initialize the CIPHER
    */
    // printf("Key = %x\n", *((int *)key));
        // Algorithm: AES256, Mode: CBC
    gcry_cipher_open(&handle, GCRY_CIPHER_AES256, GCRY_CIPHER_MODE_CBC, 0);
    gcry_cipher_setkey(handle, key, key_len);

    /*
        3. Initialize the HMAC
    */
    gcry_md_open(&handle_HMAC, GCRY_MD_SHA256, GCRY_MD_FLAG_HMAC);
    gcry_md_setkey(handle_HMAC, key, key_len);
}

// need padding and unpadding, since the "gcry_cipher_encrypt" and "gcry_cipher_decrypt" only support block_size|length
inline void padding_(unsigned char *buffer, int *len_pt){
    int pad_size = 16 - (*len_pt & 0xf); // block_size = 16
    printf("Padding size = %d\n", pad_size);
    for (int i = 0; i < pad_size; i++, (*len_pt)++){
        buffer[*len_pt] = pad_size;
    }
}

inline int unpadding_(unsigned char *buffer, int *len_pt){
    unsigned char pad_size = buffer[(*len_pt) - 1];
    printf("UNPadding size = %d\n", pad_size);
    // check for unpad..
    if (pad_size > 16)
        return 1;
    for (int i = (*len_pt) - pad_size; i < (*len_pt); i++)
        if (buffer[i] != pad_size)
            return 1;

    for (int i = 0; i < pad_size; i++, (*len_pt)--){
        buffer[(*len_pt) - 1] = 0;
    }
    return 0;
}

inline void encrypt(unsigned char *buffer, int len){
    static const unsigned char iv[16] = "1234567890123456"; 
    gcry_cipher_setiv(handle, iv, sizeof(iv));
    // operate in place
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
    // operate in place
    gcry_cipher_decrypt(
        handle, 
        buffer, 
        len, 
        NULL,
        0
    );
}

// append the hash value to the back of msg chunck
inline void append_HMAC(unsigned char *buffer, int *len_pt){ // here 16|len must stand
    unsigned char *hmac;
    gcry_md_write(handle_HMAC, buffer, *len_pt);
    hmac = gcry_md_read(handle_HMAC, GCRY_MD_SHA256);
    memcpy(buffer + (*len_pt), hmac, 32);
    (*len_pt) += 32;
}

// check the hash value, then remove it.
inline int check_integrity(unsigned char *buffer, int *len_pt){
    unsigned char *hmac;
    (*len_pt) -= 32;
    gcry_md_write(handle_HMAC, buffer, *len_pt);
    hmac = gcry_md_read(handle_HMAC, GCRY_MD_SHA256);
    return (memcmp(hmac, buffer + (*len_pt), 32) == 0);
}

// clean-up
inline void close_cry(){
    free(key);
    free(password);
    gcry_cipher_close(handle);
    gcry_md_close(handle_HMAC);
}