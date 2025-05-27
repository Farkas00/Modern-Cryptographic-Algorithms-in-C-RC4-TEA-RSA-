#ifndef RC4_H
#define RC4_H

#include <stdio.h>

void rc4_encrypt_decrypt(FILE *input, FILE *output, unsigned char *key, int key_length);

#endif
