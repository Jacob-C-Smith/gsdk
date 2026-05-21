/** ! 
 * RSA public/private key generator
 * 
 * @file src/utilities/crypto/rsa_keygen.c
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
#include <core/stream.h>

/// crypto
#include <crypto/rsa.h>

/// data
#include <data/dict.h>

/// reflection
#include <reflection/base64.h>
#include <reflection/json.h>

// entry point
int main ( int argc, const char *argv[] )
{
    
    // unused
    (void) argc;
    (void) argv;

    // initialized data
    public_key  *p_public_key  = NULL;
    private_key *p_private_key = NULL;
    int          result        = 0;

    // construct a key pair
    result = key_pair_construct
    (
        &p_public_key, // pointer to public key pointer
        &p_private_key // pointer to private key pointer
    );

    // error check
    if ( 0 == result ) goto failed_to_create_key_pair;

    // print the public and private keys
    print_public_key(p_public_key),
    print_private_key(p_private_key);

    // write the keys to files
    {

        // initialized data
        stream *p_public_key_stream  = NULL;
        stream *p_private_key_stream = NULL;

        // open the files
        if ( 0 == stream_from_path(&p_public_key_stream, "resources/core/public.key") ) goto failed_to_open_public_key;
        if ( 0 == stream_from_path(&p_private_key_stream, "resources/core/private.key") ) goto failed_to_open_private_key;
        
        // pack the keys
        if ( 0 == public_key_pack(p_public_key_stream, p_public_key) ) goto failed_to_serialize_public_key;
        if ( 0 == private_key_pack(p_private_key_stream, p_private_key) ) goto failed_to_serialize_private_key;
        
        // clean up
        stream_destroy(&p_public_key_stream);
        stream_destroy(&p_private_key_stream);
    } 

    // success
    return EXIT_SUCCESS;

    // error handling
    {

        // rsa errors
        {
            failed_to_create_key_pair:

                // log the error
                log_error("Error: Failed to create key pair!\n");

                // error
                return EXIT_FAILURE;
                
            failed_to_open_public_key:

                // log the error
                log_error("Error: Failed to open public key file!\n");

                // error
                return EXIT_FAILURE;
                
            failed_to_open_private_key:

                // log the error
                log_error("Error: Failed to open private key file!\n");

                // error
                return EXIT_FAILURE;

            failed_to_serialize_public_key:

                // log the error
                log_error("Error: Failed to serialize public key!\n");

                // error
                return EXIT_FAILURE;

            failed_to_serialize_private_key:

                // log the error
                log_error("Error: Failed to serialize private key!\n");

                // error
                return EXIT_FAILURE;
        }
    }
}
