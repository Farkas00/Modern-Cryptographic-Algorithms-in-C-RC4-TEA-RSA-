#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define DELTA 0x9e3779b9
//number of encryption/dec rounds
#define ROUNDS 32
//TEA works on 64bit, 2x32=8bytes
#define BLOCK_SIZE 8
//encrypts 64bit block 
//v pointer to 2 element array, 64bit block
//k pointer to 4 element array 128bit key
void tea_encrypt(uint32_t* v, uint32_t* k) {
    uint32_t v0 = v[0], v1 = v[1], sum = 0;
    for (int i = 0; i < ROUNDS; i++) {
        sum += DELTA;
        v0 += ((v1 << 4) + k[0]) ^ (v1 + sum) ^ ((v1 >> 5) + k[1]);
        v1 += ((v0 << 4) + k[2]) ^ (v0 + sum) ^ ((v0 >> 5) + k[3]);
    }
    v[0] = v0; v[1] = v1;
}

//decrypts 64bit block
void tea_decrypt(uint32_t* v, uint32_t* k) {
    uint32_t v0 = v[0], v1 = v[1], sum = DELTA * ROUNDS;
    for (int i = 0; i < ROUNDS; i++) {
        v1 -= ((v0 << 4) + k[2]) ^ (v0 + sum) ^ ((v0 >> 5) + k[3]);
        v0 -= ((v1 << 4) + k[0]) ^ (v1 + sum) ^ ((v1 >> 5) + k[1]);
        sum -= DELTA;
    }
    v[0] = v0; v[1] = v1;
}


//hanles file input/outpup
//1 to enc, 2 to dec

void tea_process(int encrypt, const char* inputFile, const char* keyFile, const char* outputFile) {
//open input file
    FILE *in = fopen(inputFile, "rb");
    if (!in) {
        perror("Input file");
        exit(1);
    }

//open key
    FILE *keyf = fopen(keyFile, "rb");
    if (!keyf) {
        perror("Key file");
        fclose(in);
        exit(1);
    }
//open output file
    FILE *out = fopen(outputFile, "wb");
    if (!out) {
        perror("Output file");
        fclose(in); fclose(keyf);
        exit(1);
    }

//read 128 bit key
    uint32_t key[4];
    if (fread(key, sizeof(uint32_t), 4, keyf) != 4) {
        fprintf(stderr, "Invalid key file (must be 16 bytes).\n");
        fclose(in); fclose(out); fclose(keyf);
        exit(1);
    }

//get file size
    if (encrypt) {
        // Get input size
        fseek(in, 0, SEEK_END);
        uint64_t filesize = ftell(in);
        fseek(in, 0, SEEK_SET);

        // Write original file size first
        fwrite(&filesize, sizeof(uint64_t), 1, out);

        unsigned char buffer[BLOCK_SIZE];
        size_t read;
//read and encrypt blobk by blobk 8bytes
        while ((read = fread(buffer, 1, BLOCK_SIZE, in)) > 0) {
            if (read < BLOCK_SIZE) {
                for (size_t i = read; i < BLOCK_SIZE; i++) {
                    buffer[i] = 0;  // padding with zeroes
                }
            }

            uint32_t* block = (uint32_t*)buffer;
            tea_encrypt(block, key);
            fwrite(buffer, 1, BLOCK_SIZE, out);
        }
    } else {
        // Read original file size
        uint64_t original_size;
        if (fread(&original_size, sizeof(uint64_t), 1, in) != 1) {
            fprintf(stderr, "Invalid encrypted file.\n");
            fclose(in); fclose(out); fclose(keyf);
            exit(1);
        }

        unsigned char buffer[BLOCK_SIZE];
        size_t total_written = 0;
//read and decrypt blobk by block
        while (fread(buffer, 1, BLOCK_SIZE, in) == BLOCK_SIZE) {
            uint32_t* block = (uint32_t*)buffer;
            tea_decrypt(block, key);

            // Write only up to original size
            size_t to_write = BLOCK_SIZE;
            if (total_written + BLOCK_SIZE > original_size) {
                to_write = original_size - total_written;
            }

            fwrite(buffer, 1, to_write, out);
            total_written += to_write;
        }
    }

    fclose(in);
    fclose(out);
    fclose(keyf);
}

