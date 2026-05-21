/** ! 
 * X25519 example
 * 
 * @file src/examples/x25519_example.c
 * 
 * @author Jacob Smith
 */

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// gsdk
/// core
#include <core/log.h>
#include <core/stream.h>

/// crypto
#include <crypto/x25519.h>

// preprocessor definitions
#define RED "\033[91m"
#define BLUE "\033[94m"
#define YELLOW "\033[93m"
#define RESET "\033[0m"

// forward declarations
/// logs
int checkpoint ( const char *p_event );

// data
/// alice's keys
x25519_public_key    _alice_public_key    = { 0 };
x25519_private_key   _alice_private_key   = { 0 };
x25519_shared_secret _alice_shared_secret = { 0 };

/// bob's keys
x25519_public_key    _bob_public_key    = { 0 };
x25519_private_key   _bob_private_key   = { 0 };
x25519_shared_secret _bob_shared_secret = { 0 };

/// files for reflection
FILE   *p_f      = NULL,
       *p_g      = NULL,
       *p_h      = NULL;
size_t  file_len = 0;

// entry point
int main ( int argc, const char *argv[] )
{

    // unused
    (void) argc;
    (void) argv;

    // #0 - start
    checkpoint("start");

    // #1 - alice makes a key pair
    {
        
        // construct the key pair
        x25519_key_pair_construct(&_alice_public_key, &_alice_private_key);

        // checkpoint
        checkpoint("after alice makes a key pair");
    }

    // #2 - bob makes a key pair
    {
        
        // construct the key pair
        x25519_key_pair_construct(&_bob_public_key, &_bob_private_key);

        // checkpoint
        checkpoint("after bob makes a key pair");
    }

    // #3 - after deriving shared secrets
    {

        // alice derives the shared secret
        x25519_shared_secret_derive(&_alice_private_key, &_bob_public_key, &_alice_shared_secret);
        
        // bob derives the shared secret
        x25519_shared_secret_derive(&_bob_private_key, &_alice_public_key, &_bob_shared_secret);

        // checkpoint
        checkpoint("after deriving shared secrets");
    }

    // #4 - verify secrets match
    {
        
        // initialized data
        int result = 1;

        // compare the shared secrets
        result = memcmp(_alice_shared_secret, _bob_shared_secret, 32);

        // log the result
        if ( 0 == result )
            log_info("shared secrets match.\n");
        else 
            log_error("shared secrets do not match.\n");
    }

    // #5 - keypairs to binary
    {

        // initialized data
        stream *p_stream = NULL;
        
        // open a file for writing
        stream_from_path(&p_stream, "resources/reflection/x25519_alice.bin");

        // reflect alices public key to a buffer
        x25519_key_pair_pack(p_stream, &_alice_public_key, &_alice_private_key);
        
        // close the file
        stream_destroy(&p_stream);

        // open a file for writing
        stream_from_path(&p_stream, "resources/reflection/x25519_bob.bin");

        // reflect bobs public key to a buffer
        x25519_key_pair_pack(p_stream, &_bob_public_key, &_bob_private_key);

        // close the file
        stream_destroy(&p_stream);

        // checkpoint
        checkpoint("after serializing keypairs");
    }

    // #6 - shared secret to binary
    {

        // initialized data
        stream *p_stream = NULL;
        
        // open a file for writing
        stream_from_path(&p_stream, "resources/reflection/x25519_shared_secret.bin");
 
        // reflect the shared secret to a buffer
        x25519_shared_secret_pack(p_stream, &_alice_shared_secret);

        // close the file
        stream_destroy(&p_stream);

        // checkpoint
        checkpoint("after serializing shared secret");
    }

    // #7 - keypairs and shared secrets clear
    {
        
        // clear alice's keys
        memset(_alice_public_key, 0, sizeof(x25519_public_key)),
        memset(_alice_private_key, 0, sizeof(x25519_private_key)),

        // clear bob's keys
        memset(_bob_public_key, 0, sizeof(x25519_public_key)),
        memset(_bob_private_key, 0, sizeof(x25519_private_key)),

        // clear shared secrets
        memset(_bob_shared_secret, 0, sizeof(x25519_shared_secret)),
        memset(_alice_shared_secret, 0, sizeof(x25519_shared_secret));

        // checkpoint
        checkpoint("after clearing keypairs and shared secrets");
    }

    // #8 - keypairs and shared secret from binary
    {
        
        // initialized data
        stream *p_stream = NULL;

        // open a file for reading
        stream_from_path(&p_stream, "resources/reflection/x25519_alice.bin");

        // reflect alices key pair from the buffer
        x25519_key_pair_unpack(&_alice_public_key, &_alice_private_key, p_stream); 
        
        // close the file
        stream_destroy(&p_stream);

        // open a file for reading
        stream_from_path(&p_stream, "resources/reflection/x25519_bob.bin");

        // reflect bobs key pair from the buffer        
        x25519_key_pair_unpack(&_bob_public_key, &_bob_private_key, p_stream); 

        // close the file
        stream_destroy(&p_stream);

        // checkpoint
        checkpoint("after parsing keypairs");
    }

    // #9 - shared secret from binary
    {
        
        // initialized data
        stream *p_stream = NULL;

        // open a file for reading
        stream_from_path(&p_stream, "resources/reflection/x25519_shared_secret.bin");

        // reflect the shared secret from the buffer
        x25519_shared_secret_unpack(&_alice_shared_secret, p_stream);

        // close the file
        stream_destroy(&p_stream);

        // checkpoint
        checkpoint("after parsing alice's secret");
    }

    // #10 - derive bob's shared secret
    {

        // bob derives the shared secret
        x25519_shared_secret_derive(&_bob_private_key, &_alice_public_key, &_bob_shared_secret);

        // checkpoint
        checkpoint("after deriving bob's secret from reflected keypair");
    }

    // #11 - verify secrets match
    {
        
        // initialized data
        int result = 1;

        // compare the shared secrets
        result = memcmp(_alice_shared_secret, _bob_shared_secret, 32);

        // log the result
        if ( 0 == result )
            log_info("shared secrets match.\n");
        else 
            log_error("shared secrets do not match.\n");
    }

    // success
    return EXIT_SUCCESS;
}

int checkpoint ( const char *p_event )
{
    
    // static data
    static int step = 0;
    
    // print the event
    log_info("\n#%d - %s\n", step, p_event),

    // print alice's keys
    printf(       " - Alice - \n"),
    printf(BLUE   "PUBLIC KEY   " RESET ": "), x25519_public_key_print(&_alice_public_key),
    printf(RED    "PRIVATE KEY  " RESET ": "), x25519_private_key_print(&_alice_private_key),
    printf(YELLOW "SHARED SECRET" RESET ": "), x25519_shared_secret_print(&_alice_shared_secret);

    // formatting
    putchar('\n');

    // print bob's keys
    printf(       " - Bob - \n"),
    printf(BLUE   "PUBLIC KEY   " RESET ": "), x25519_public_key_print(&_bob_public_key),
    printf(RED    "PRIVATE KEY  " RESET ": "), x25519_private_key_print(&_bob_private_key),
    printf(YELLOW "SHARED SECRET" RESET ": "), x25519_shared_secret_print(&_bob_shared_secret);
    
    // formatting
    putchar('\n');

    // increment counter
    step++;
    
    // success
    return 1;
}
