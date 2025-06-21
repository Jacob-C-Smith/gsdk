/** ! 
 * RSA public/private key generator
 * 
 * @file src/utilities/rsa_keygen.c
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

// entry point
int main ( int argc, const char *argv[] )
{

    // unused
    (void) argc;
    (void) argv;

    // initialized data
    public_key  *p_public_key  = NULL;
    private_key *p_private_key = NULL;
    
    // construct a key pair
    key_pair_from_files
    (
        &p_public_key,  // Pointer to public key pointer
        &p_private_key, // Pointer to private key pointer
        "public.key",   // Path to public key
        "private.key"   // Path to private key
    );

    // Print the public and private keys
    print_public_key(p_public_key),
    print_private_key(p_private_key);

    // success
    return EXIT_SUCCESS;
}

