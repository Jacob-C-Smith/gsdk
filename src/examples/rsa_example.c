/** ! 
 * RSA example
 * 
 * @file src/examples/rsa_example.c
 * 
 * @author Jacob Smith
 */

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>

// gsdk
/// core
#include <core/log.h>
#include <core/rsa.h>

// preprocessor definitions
#define RED "\033[91m"
#define BLUE "\033[94m"
#define RESET "\033[0m"

// forward declarations
/// logs
int checkpoint ( const char *p_event );

// data
/// working keys
public_key  *p_public_key  = NULL;
private_key *p_private_key = NULL;

/// plain, encrypted, decrypted
char x[256] = { 's', 'e', 'c', 'r', 'e', 't', '\0' },
     y[256] = { 0 },
     z[256] = { 0 };

/// large file
char _hegel_logic_x[922886] = { 0 };
char _hegel_logic_y[922886] = { 0 };
char _hegel_logic_z[922886] = { 0 };
hash64 h1 = 0,
       h2 = 0;

// entry point
int main ( int argc, const char *argv[] )
{

    // unused 
    (void) argc;
    (void) argv;
    
    // #1 - start
    checkpoint("start");

    // #2 - generate a keypair
    {

        // construct the keypair
        key_pair_construct(&p_public_key, &p_private_key);

        // checkpoint
        checkpoint("generating a key pair");

        // print the public and private keys
        print_public_key(p_public_key), print_private_key(p_private_key),
        putchar('\n');
        
    }

    // #3 - load a keypair
    {

        // construct a key pair
        key_pair_from_files
        (
            &p_public_key,               // result
            &p_private_key,              // result
            "resources/core/public.key", // path to public key
            "resources/core/private.key" // path to private key
        );

        // checkpoint
        checkpoint("loading a key pair");

        // print the public and private keys
        print_public_key(p_public_key), print_private_key(p_private_key);
        putchar('\n');

    }

    // #4 - block encrypt
    {

        // encrypt test value x into ciphertext y
        rsa_encrypt(&x, &y, p_public_key);

        // checkpoint
        checkpoint("block encryption");

        // input
        printf(" inp = " RED), print_n_bit_int(*(i2048*)x);

        // encrypted text
        putchar('\n'), 
        printf(RESET " enc = " BLUE), print_n_bit_int(*(i2048*)y),
        putchar('\n'), putchar('\n');
    }

    // #5 - block decrypt
    {

        // decrypt ciphertext y into plaintext z
        rsa_decrypt(&y, &z, p_public_key, p_private_key);
        
        // checkpoint
        checkpoint("block decryption");

        // decrypted text
        printf(RESET " enc = " BLUE), print_n_bit_int(*(i2048*)y),
        putchar('\n'), 
        printf(RESET " dec = " RED), print_n_bit_int(*(i2048*)z),
        putchar('\n'), putchar('\n');
    }

    // #6 - done
    checkpoint("done");

    // success
    return EXIT_SUCCESS;
}

int checkpoint ( const char *p_event )
{
    
    // static data
    static int step = 0;
    
    // print the event
    log_info("#%d - %s\n", step, p_event),
    
    // increment counter
    step++;
    
    // success
    return 1;
}
