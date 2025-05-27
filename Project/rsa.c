#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Small primes for demonstration
#define P 61
#define Q 53
#define N (P * Q)           // N = 3233
#define PHI ((P - 1)*(Q - 1))// φ = 3120
#define E 17                // public exponent
#define D 2753              // private exponent

//computes base^ext % and returns the result
unsigned long mod_exp(unsigned long base, unsigned long exp, unsigned long mod) {
    unsigned long result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

//encrypts, each byte is read, encrypted with public key
void rsa_encrypt(FILE *in, FILE *out) {
    int c;
    while ((c = fgetc(in)) != EOF) {
    //encrypt using c^E %N
        unsigned long enc = mod_exp(c, E, N);
        fwrite(&enc, sizeof(enc), 1, out);
    }
}
//decrypts 
void rsa_decrypt(FILE *in, FILE *out) {
    unsigned long enc;
    //decrypt using enc^D mod N
    while (fread(&enc, sizeof(enc), 1, in) == 1) {
        unsigned long dec = mod_exp(enc, D, N);
        fputc((char)dec, out);
    }
}

// This function is called from main.c, opens input and output 1 to enc, 0 to dec
void rsa_process(int encrypt, const char *inputFile, const char *outputFile) {
    FILE *in = fopen(inputFile, "rb");
    if (!in) {
        perror("Input file");
        exit(1);
    }
    FILE *out = fopen(outputFile, "wb");
    if (!out) {
        perror("Output file");
        fclose(in);
        exit(1);
    }

    if (encrypt)
        rsa_encrypt(in, out);
    else
        rsa_decrypt(in, out);

    fclose(in);
    fclose(out);
}

