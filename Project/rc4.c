#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function for initializing permutation
void rc4_init_state(unsigned char *state, unsigned char *key, int key_length) {
    int i, j = 0;
    unsigned char temp;

    for (i = 0; i < 256; i++) {
        state[i] = (unsigned char)i;
    }

    //Permutation with key
    for (i = 0; i < 256; i++) {
        j = (j + state[i] + key[i % key_length]) % 256;
        temp = state[i];
        state[i] = state[j];
        state[j] = temp;
    }
}

// Function for Encryption/decription
void rc4_encrypt_decrypt(FILE *input, FILE *output, unsigned char *key, int key_length) {
    unsigned char state[256];
    int i = 0, j = 0, t;
    unsigned char byte;
    unsigned char key_byte;

    //initializing permutation state
    rc4_init_state(state, key, key_length);

    // Encryption/Decryption
    while (fread(&byte, 1, 1, input) == 1) {
        i = (i + 1) % 256;
        j = (j + state[i]) % 256;

        unsigned char temp = state[i];
        state[i] = state[j];
        state[j] = temp;

        t = (state[i] + state[j]) % 256;
        key_byte = state[t];

        // XOR key for encryption/decryption
        byte ^= key_byte;

        fwrite(&byte, 1, 1, output);
    }
}

