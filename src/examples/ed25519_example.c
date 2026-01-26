/** ! 
 * Cryptographic primitives
 *
 * @file ed25519_example.c
 *
 * @author Jacob Smith
 */

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// core
#include <core/log.h>
#include <core/ed25519.h>

// preprocessor definitions
#define RED "\033[91m"
#define BLUE "\033[94m"
#define RESET "\033[0m"

void print_hex(const char *label, const unsigned char *data, size_t len) {
    printf("%s", label);
    for (size_t i = 0; i < len; i++) {
        printf("%02x", data[i]);
    }
    printf("\n");
}

// entry point
int main ( int argc, const char *argv[] )
{
    // unused 
    (void) argc;
    (void) argv;
    
    // initialized data
    ed25519_public_key  *p_public_key  = NULL;
    ed25519_private_key *p_private_key = NULL;
    ed25519_signature    signature;
    const char *message = "secret";
    
    // SHA-512 test
    unsigned char sha_out[64];
    const char *sha_in = "abc";
    sha512_state st;
    sha512_construct(&st);
    sha512_update(&st, (const unsigned char *)sha_in, 3);
    sha512_final(&st, sha_out);
    print_hex(RESET "sha512(\"abc\"): ", sha_out, 64);
    printf(" expected:      ddaf35a193617abacc417349ae20413112e6fa4e89a97ea20a9eeee64b55d39a2192992a274fc1a836ba3c23a3feebbd454d4423643ce80e2a9ac94fa54ca49f\n");

    // construct a key pair
    if (ed25519_key_pair_construct(&p_public_key, &p_private_key) == 0) {
        log_error("Failed to construct random key pair");
        return EXIT_FAILURE;
    }

    // print the public and private keys
    printf(RESET "key info:\n");
    print_hex(" public key:  " BLUE, p_public_key->data, 32);
    print_hex(RESET " private key: " RED, p_private_key->data, 32);

    // print the results
    printf(RESET "signature test:\n");

    // input
    printf(" msg = " RED "%s\n", message);

    // sign the message
    if (ed25519_sign(&signature, (const unsigned char *)message, strlen(message), p_public_key, p_private_key) == 0) {
        log_error("Failed to sign message");
        return EXIT_FAILURE;
    }

    // signature
    printf(RESET " sig = " BLUE);
    for (int i = 0; i < 64; i++) {
        printf("%02x", signature.data[i]);
    }
    printf("\n");

    // verify the signature
    int valid = ed25519_verify(&signature, (const unsigned char *)message, strlen(message), p_public_key);
    
    // result
    printf(RESET " ver = " RED "%s\n", valid ? "valid" : "invalid");
    printf(RESET "\n");

    // success
    return EXIT_SUCCESS;
}
