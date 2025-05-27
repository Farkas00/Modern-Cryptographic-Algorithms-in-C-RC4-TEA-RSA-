#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include the algorithm implementations
#include "rc4.h"
#include "tea.h"
#include "rsa.h"

void print_usage(const char *program_name) {
    printf("Usage:\n");
    printf("  %s <algo> -e|-d <input> -k <key> -o <output>\n", program_name);
    printf("  (Note: 'rsa' does not require a key file, but -k <key> is still required syntactically)\n");
}

int main(int argc, char *argv[]) {
    if (argc != 8) {
        print_usage(argv[0]);
        return 1;
    }

    const char *algo = argv[1];
    int encrypt = strcmp(argv[2], "-e") == 0 ? 1 :
                  strcmp(argv[2], "-d") == 0 ? 0 : -1;

    if (encrypt == -1) {
        fprintf(stderr, "Invalid mode: use -e for encryption or -d for decryption.\n");
        return 1;
    }

    const char *input = argv[3];
    const char *key_filename = argv[5];
    const char *output = argv[7];

    if (strcmp(algo, "rc4") == 0) {
        FILE *input_file = fopen(input, "rb");
        FILE *output_file = fopen(output, "wb");
        FILE *key_file = fopen(key_filename, "rb");

        if (!input_file || !output_file || !key_file) {
            fprintf(stderr, "Error opening files.\n");
            return 1;
        }

        fseek(key_file, 0, SEEK_END);
        long key_size = ftell(key_file);
        fseek(key_file, 0, SEEK_SET);
        unsigned char *key = malloc(key_size);
        if (!key) {
            fprintf(stderr, "Memory allocation error.\n");
            return 1;
        }

        fread(key, 1, key_size, key_file);
        rc4_encrypt_decrypt(input_file, output_file, key, key_size);

        free(key);
        fclose(input_file);
        fclose(output_file);
        fclose(key_file);
    }

    else if (strcmp(algo, "tea") == 0) {
        tea_process(encrypt, input, key_filename, output);
    }

    else if (strcmp(algo, "rsa") == 0) {
        rsa_process(encrypt, input, output);
    }

    else {
        fprintf(stderr, "Unsupported algorithm: %s\n", algo);
        return 1;
    }

    return 0;
}
