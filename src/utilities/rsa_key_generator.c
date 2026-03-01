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

// data
#include <data/dict.h>

// reflection
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
        FILE          *p_public_key_f            = fopen("resources/core/public.key"   , "wb"),
                      *p_private_key_f           = fopen("resources/core/private.key"  , "wb"),
                      *p_keypair_f               = fopen("resources/core/key_pair.json", "w");
        size_t         public_key_length         = 0,
                       private_key_length        = 0;
        unsigned char  _public_key_buffer [1024] = { 0 },
                       _private_key_buffer[1024] = { 0 };

        // error check
        if ( NULL == p_public_key_f  ) goto failed_to_open_public_key;
        if ( NULL == p_private_key_f ) goto failed_to_open_private_key;
        if ( NULL == p_keypair_f     ) goto failed_to_open_keypair;
        
        // pack the keys
        public_key_length  = public_key_pack(_public_key_buffer, p_public_key),
        private_key_length = private_key_pack(_private_key_buffer, p_private_key);
        
        // write the public and private keys to files
        {
            // error check
            if ( 0 == public_key_length  ) goto failed_to_serialize_public_key;
            if ( 0 == private_key_length ) goto failed_to_serialize_private_key;

            // write the keys
            fwrite(_public_key_buffer, public_key_length, 1, p_public_key_f),
            fwrite(_private_key_buffer, private_key_length, 1, p_private_key_f);

            // clean up
            fclose(p_public_key_f),
            fclose(p_private_key_f);
        }

        // write the keypair json file
        {

            // Initialized data
            char _public_key_base64 [2048] = { 0 },
                 _private_key_base64[2048] = { 0 };
            json_value _value       = { .type = JSON_VALUE_OBJECT };
            json_value _public_key  = { .type = JSON_VALUE_STRING, .string = _public_key_base64 };
            json_value _private_key = { .type = JSON_VALUE_STRING, .string = _private_key_base64 };

            // construct a dictionary
            if ( 0 == dict_construct(&_value.object, 4, 0) ) goto failed_to_construct_dictionary;

            // encode the public key
            if ( 0 == base64_encode(_public_key_base64, _public_key_buffer, public_key_length) ) goto failed_to_encode_base64;
            
            // encode the private key
            if ( 0 == base64_encode(_private_key_base64, _private_key_buffer, private_key_length) ) goto failed_to_encode_base64;

            // add the keys to the dictionary
            dict_add(_value.object, "public", &_public_key),
            dict_add(_value.object, "private", &_private_key);

            // write the value to the file
            json_value_fprint(&_value, p_keypair_f);

            // clean up
            dict_destroy(&_value.object, NULL),
            fclose(p_keypair_f);
        }
    } 

    // success
    return EXIT_SUCCESS;

    // error handling
    {

        // base64 errors
        {
            failed_to_encode_base64:

                // Error
                return EXIT_FAILURE;
        }

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
                
            failed_to_open_keypair:

                // log the error
                log_error("Error: Failed to open keypair file!\n");

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
    
        // dict errors
        {
            failed_to_construct_dictionary:

                // Error
                return EXIT_FAILURE;
        }
    }
}
