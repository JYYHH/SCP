#include "common.h"

char *key, *password;
int key_len, pass_len;

inline void init_key(){
    key = (char *)malloc(32);
    password = (char *)malloc(MAX_BYTES);
    printf("Password: ");
    scanf("%s", password);
    key_len = 32;
    pass_len = strlen(password);

    static const char *salt = "please_dont_delete_the_code_carelessly_again_thanks_to_myself";
    int salt_len = strlen(salt);

    gcry_kdf_derive(
        password, pass_len,
        GCRY_KDF_PBKDF2,                    // KDF algorithm
        GCRY_MD_SHA256,                     // Hash function
        salt, salt_len,
        3000,                               // Iteration round
        key_len, key
    );

    // printf("Key = %x\n", *((int *)key));
}

