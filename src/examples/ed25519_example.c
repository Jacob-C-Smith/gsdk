/** ! 
 * Ed25519 example
 * 
 * @file src/examples/ed25519_example.c
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
#include <crypto/ed25519.h>

// preprocessor definitions
#define RED "\033[91m"
#define BLUE "\033[94m"
#define YELLOW "\033[93m"
#define RESET "\033[0m"

// forward declarations
/// logs
int checkpoint ( const char *p_event );

// data
/// key pair
ed25519_public_key  _public_key  = { 0 };
ed25519_private_key _private_key = { 0 };

/// signature
ed25519_signature _signature = { 0 };

/// file for reflection
FILE   *p_f      = NULL;
size_t  file_len = 0;

/// message
const char _message[] = "The quick brown fox jumps over the lazy dog.";

// entry point
int main ( int argc, const char *argv[] )
{

    // unused
    (void) argc;
    (void) argv;

    // #0 - start
    checkpoint("start");
    
    // #1 - generate a key pair
    {
        
        // construct the key pair
        if ( 0 == ed25519_key_pair_construct(
            &_public_key,
            &_private_key
        ) ) goto failed_to_construct_key_pair;

        // checkpoint
        checkpoint("key pair constructed");
    }

    // #2 - sign the data
    {

        // sign the message
        if ( 0 == ed25519_sign(
            &_signature, 
            (const unsigned char *)_message,
            sizeof(_message) / sizeof(*_message),
            &_public_key,
            &_private_key
        ) ) goto failed_to_sign_message;

        // checkpoint
        checkpoint("message signed");

        // print the message
        printf("sign(\"%s\")\n", _message);
    }

    // #3 - signature to binary
    {

        // initialized data
        stream *p_stream = NULL;
        
        // open a stream
        stream_from_path(&p_stream, "resources/reflection/ed25519_signature.bin");

        // reflect the public key to a stream
        ed25519_signature_pack(p_stream, &_signature),

        // close the stream
        stream_destroy(&p_stream);

        // checkpoint
        checkpoint("after signature serialize");
    }

    // #4 - verify the signature
    {

        // initialized data
        int result = 0;

        // verify the signature
        result = ed25519_verify(
            &_signature,
            (const unsigned char *)_message,
            sizeof(_message) / sizeof(*_message),
            &_public_key
        );

        // checkpoint
        checkpoint("signature verified");
        
        // log the tampered message
        printf("verify(\"%s\")\n", _message);

        // log the result
        if ( result )
            log_info("signature is valid.\n");
        else 
            log_error("signature is invalid.\n");

    }
    
    // #5 - tamper with the data
    {

        // initialized data
        int result = 0;
        char _tampered_message[100] = { 0 };

        // copy the message
        strncpy(_tampered_message, _message, sizeof(_message) / sizeof(*_message));
        
        // change the message
        _tampered_message[0] = 't';

        // verify the signature
        result = ed25519_verify(
            &_signature,
            (const unsigned char *)_tampered_message,
            sizeof(_message) / sizeof(*_message),
            &_public_key
        );

        // checkpoint
        checkpoint("tampered message");

        // log the tampered message
        printf("verify(\"%s\")\n", _tampered_message);

        // log the result
        if ( 1 == result )
            log_info("signature is valid.\n");
        else 
            log_error("signature is invalid.\n");
    }

    // #6 - keypair to binary
    {

        // initialized data
        stream *p_stream = NULL;
        
        // open a stream
        stream_from_path(&p_stream, "resources/reflection/ed25519.bin");

        // reflect the public key to a stream
        ed25519_key_pair_pack(p_stream, &_public_key, &_private_key),
        
        // close the stream
        stream_destroy(&p_stream);

        // checkpoint
        checkpoint("after keypair serialize");
    }

    // #7 - keypair and signature clear
    {
        
        // clear the public key
        memset(_public_key, 0, sizeof(ed25519_public_key)),

        // clear the private key
        memset(_private_key, 0, sizeof(ed25519_private_key)),

        // clear the signature
        memset(_signature, 0, sizeof(ed25519_signature));

        // checkpoint
        checkpoint("after keypair and signature clear");
    }

    // #8 - keypair from binary
    {
        
        // initialized data
        stream *p_stream = NULL;
        
        // open a stream
        stream_from_path(&p_stream, "resources/reflection/ed25519.bin");
        
        // reflect an ed25519 key pair from the buffer
        ed25519_key_pair_unpack(&_public_key, &_private_key, p_stream);

        // close the stream
        stream_destroy(&p_stream);

        // checkpoint
        checkpoint("after keypair parse");
    }

    // #9 - signature from binary
    {
        
        // initialized data
        stream *p_stream = NULL;
        
        // open a stream
        stream_from_path(&p_stream, "resources/reflection/ed25519_signature.bin");
        
        // reflect an ed25519 signature from the buffer
        ed25519_signature_unpack(&_signature, p_stream);

        // close the stream
        stream_destroy(&p_stream);

        // checkpoint
        checkpoint("after signature parse");
    }

    // #10 - verify from reflected keypair and signature
    {

        // initialized data
        int result = 0;

        // verify the signature
        result = ed25519_verify(
            &_signature,
            (const unsigned char *)_message,
            sizeof(_message) / sizeof(*_message),
            &_public_key
        );

        // log the result
        if ( 1 == result )
            log_info("signature is valid.\n");
        else 
            log_error("signature is invalid.\n");

        // checkpoint
        checkpoint("reflected signature verified");
    }
    
    // #11 - done
    checkpoint("done");

    // success
    return EXIT_SUCCESS;

    // error handling
    {

        // ed25519 errors
        {
            failed_to_construct_key_pair:
                #ifndef NDEBUG
                    log_error("Failed to construct key pair\n");
                #endif

                // error
                return EXIT_FAILURE;

            failed_to_sign_message:
                #ifndef NDEBUG
                    log_error("Failed to sign message\n");
                #endif

                // error
                return EXIT_FAILURE;
        }
    }
}

int checkpoint ( const char *p_event )
{
    
    // static data
    static int step = 0;
    
    // print the event
    log_info("\n#%d - %s\n", step, p_event),

    // print the public key
    printf(BLUE   "PUBLIC KEY  " RESET ": "), ed25519_public_key_print(&_public_key);

    // print the private key
    printf(RED    "PRIVATE KEY " RESET ": "), ed25519_private_key_print(&_private_key),

    // print the signature
    printf(YELLOW "SIGNATURE   " RESET ": "), ed25519_signature_print(&_signature),

    // increment counter
    step++;
    
    // success
    return 1;
}
