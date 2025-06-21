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
#include <core/certificate.h>

// Preprocessor definitions
#define RED "\033[91m"
#define BLUE "\033[94m"
#define RESET "\033[0m"

int certificate_example ( int argc, const char * argv[] )
{

    // Unused
    (void) argc;
    (void) argv;

    // Initialized data
    public_key  *p_public_key  = NULL;
    private_key *p_private_key = NULL;
    char y[256] = { 0x12, 0x34, 0xcd, 0xef },
         z[256] = { 0x00, 0x00, 0x00, 0x00 };
    
    printf(RESET "enc = " BLUE "%hhx%hhx%hhx%hhx\n", y[0], y[1], y[2], y[3]);

    dec(&y, &z, p_public_key, p_private_key);

    printf(RESET "dec = " BLUE "%hhx%hhx%hhx%hhx\n", z[0], z[1], z[2], z[3]);

    // Success 
    return 1;
}

int rsa_example ( int argc, const char * argv[] )
{

    // Unused
    (void) argc;
    (void) argv;

    // Initialized data

    // Success 
    return 1;
    
}

// Entry point
int main ( int argc, const char *argv[] )
{

    // Unused 
    (void) argc;
    (void) argv;
    
    // Initialized data
    public_key  *p_public_key  = NULL;
    private_key *p_private_key = NULL;
    char x[128] = { 0 },
         y[128] = { 0 },
         z[128] = { 0 };

    memcpy(x, "secret\0\0", 8);
    
    // Construct a key pair
    key_pair_from_files
    (
        &p_public_key,                       // Pointer to public key pointer
        &p_private_key,                      // Pointer to private key pointer
        "resources/core/rsa_public_key.bin", // Path to public key
        "resources/core/rsa_private_key.bin" // Path to private key
    );

    // Print the public and private keys
    print_public_key(p_public_key),
    print_private_key(p_private_key);

    // Encrypt test value x into ciphertext y
    enc(&x, &y, p_public_key);

    // Decrypt ciphertext y into plaintext z
    dec(&y, &z, p_public_key, p_private_key);
    
    // Print the results
    printf(RESET "ENCRYPTION TEST:\n"),

    // Input
    printf(" inp = " RED);
    print_n_bit_int(*(i1024*)x);

    // Encrypted text
    printf(RESET " enc = " BLUE);
    print_n_bit_int(*(i1024*)y);

    // Decrypted text
    printf(RESET " dec = " RED);
    print_n_bit_int(*(i1024*)z);
    printf(RESET);

    // Success
    return EXIT_SUCCESS;
}

