/** ! 
 * Secure Socket Server
 * 
 * @file src/utilities/secure_socket_server.c
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

    // initialized data
    //

    // construct a secure socket
    //

    // error check
    //

    // start a secure connection
    {

    }

    // transmit a file confidentiality
    {

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

        // crypto errors
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
