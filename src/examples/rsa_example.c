/** ! 
 * Cryptographic primitives
 * 
 * @file crypto_example.c
 * 
 * @author Jacob Smith
 */

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>

// core
#include <core/log.h>
#include <core/rsa.h>

// preprocessor definitions
#define RED "\033[91m"
#define BLUE "\033[94m"
#define RESET "\033[0m"

// entry point
int main ( int argc, const char *argv[] )
{

    // unused 
    (void) argc;
    (void) argv;
    
    // initialized data
    public_key  *p_public_key  = NULL;
    private_key *p_private_key = NULL;
    char x[256] = { 0 },
         y[256] = { 0 },
         z[256] = { 0 };

    memcpy(x, "secret\0\0", 8);
    
    // construct a key pair
    key_pair_from_files
    (
        &p_public_key,               // result
        &p_private_key,              // result
        "resources/core/public.key", // path to public key
        "resources/core/private.key" // path to private key
    );

    // print the public and private keys
    printf(RESET "key info:"),
    print_public_key(p_public_key),
    print_private_key(p_private_key);

    // print the results
    printf(RESET "encryption test:\n"),

    // input
    printf(" inp = " RED);
    print_n_bit_int(*(i2048*)x);

    // encrypt test value x into ciphertext y
    rsa_encrypt(&x, &y, p_public_key);

    // encrypted text
    printf(RESET "\n enc = " BLUE);
    print_n_bit_int(*(i2048*)y);

    // decrypt ciphertext y into plaintext z
    rsa_decrypt(&y, &z, p_public_key, p_private_key);
    
    // decrypted text
    printf(RESET "\n dec = " RED);
    print_n_bit_int(*(i2048*)z);
    printf(RESET "\n");

    // success
    return EXIT_SUCCESS;
}