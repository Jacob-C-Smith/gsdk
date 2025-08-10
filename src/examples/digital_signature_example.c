/** ! 
 * Digital Signature Algorithm Example
 * 
 * @file dsa_example.c
 * 
 * @author Jacob Smith
 */

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// core
#include <core/log.h>
#include <core/digital_signature.h>

// forward declarations
/// logs
int checkpoint ( const char *p_event );

// data
public_key  *p_public_key = NULL;
private_key *p_private_key = NULL;
void        *p_signature = NULL;
const char  *message = "The quick brown fox jumps over the lazy dog.";

// entry point
int main ( int argc, const char *argv[] )
{

    // unused
    (void) argc;
    (void) argv;

    // #0 - start
    checkpoint("start");
    
    // #1 - generate a keypair
    {
        
        // construct the key pair
        if ( 0 == key_pair_construct(&p_public_key, &p_private_key) ) goto failed_to_construct_key_pair;

        // checkpoint
        checkpoint("generated 2048 bit RSA key pair");
    }

    // #2 - sign the data
    {

        // sign the message
        if ( 0 == digital_signature_sign(
            p_private_key,
            p_public_key, 

            message, 
            strlen(message),

            &p_signature
        ) ) goto failed_to_sign_message;

        // checkpoint
        checkpoint("message signed");

    }

    // #3 - verify the signature
    {

        // initialized data
        int result = 0;

        // log the message
        log_info("message: \"%s\"\n", message);

        // verify the signature
        result = digital_signature_verify(
            p_public_key, 
            message, 
            strlen(message), 
            p_signature
        );

        // log the result
        if ( 1 == result )
            log_info("signature is valid.\n");
        else 
            log_error("signature is invalid.\n");

        // checkpoint
        checkpoint("signature verified");
    }
    
    // #4 - tamper with the data
    {


        // initialized data
        int result = 0;
        char tampered_message[100];

        // copy the message
        strncpy(tampered_message, message, sizeof(tampered_message));
        
        // change the message
        tampered_message[0] = 't';

        // log the tampered message
        log_info("message: \"%s\"\n", tampered_message);

        // verify the signature
        result = digital_signature_verify(
            p_public_key, 
            tampered_message, 
            strlen(tampered_message), 
            p_signature
        );

        // log the result
        if ( 1 == result )
            log_info("signature is valid.\n");
        else 
            log_error("signature is invalid.\n");

        // checkpoint
        checkpoint("tampered message verified");
    }

    // #5 - cleanup
    {
        
        // release the signature
        p_signature = default_allocator(p_signature, 0);

        // release the keys
        p_public_key  = default_allocator(p_public_key, 0);
        p_private_key = default_allocator(p_private_key, 0);

        // checkpoint
        checkpoint("after destroy");
    }

    // success
    return EXIT_SUCCESS;

    // error handling
    {
        
        // rsa errors
        {
            failed_to_construct_key_pair:
                #ifndef NDEBUG
                    log_error("Failed to construct key pair\n");
                #endif

                // error
                return EXIT_FAILURE;
        }

        // dsa errors
        {
            failed_to_sign_message:
                #ifndef NDEBUG
                    log_error("Failed to sign message\n");
                #endif

                // release the signature if allocated
                p_signature = default_allocator(p_signature, 0);

                // error
                return EXIT_FAILURE;
        }
    }
}

int checkpoint ( const char *p_event )
{
    
    // static data
    static int step = 0;
    
    // print the digital signature
    log_info("#%d - %s\n", step, p_event),
    
    // increment counter
    step++;
    
    // success
    return 1;
}
  
