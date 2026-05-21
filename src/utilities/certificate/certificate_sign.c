/** ! 
 * Certificate signer
 * 
 * @file src/utilities/certificate/certificate_sign.c
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
#include <crypto/sha.h>
#include <crypto/ed25519.h>
#include <crypto/certificate.h>

// forward declarations
/** !
 * Print a usage message to standard out
 * 
 * @param argv0 the name of the program
 * 
 * @return void
 */
void print_usage ( const char *argv0 );

/** !
 * Parse command line arguments
 * 
 * @param argc the argc parameter of the entry point
 * @param argv the argv parameter of the entry point
 * 
 * @return void on success, program abort on failure
 */
void parse_command_line_arguments ( int argc, const char *argv[] );

// data
const char *p_key_file        = NULL;
const char *p_output_filename = NULL;

// entry point
int main ( int argc, const char *argv[] )
{
    
    // initialized data
    stream              *p_stream      = NULL;
    ed25519_public_key   public_key    = { 0 };
    ed25519_private_key  private_key   = { 0 };
    certificate         *p_certificate = NULL;

    // parse command line arguments
    parse_command_line_arguments(argc, argv);

    // load a key pair from file
    {

        // open the key file
        if ( 0 == stream_from_path(&p_stream, p_key_file) ) goto failed_to_open_file;

        // unpack the key pair
        ed25519_key_pair_unpack(&public_key, &private_key, p_stream);

        // close the file
        stream_destroy(&p_stream);
    }

    // load the certificate
    {

        // open the file
        if ( 0 == stream_from_path(&p_stream, p_output_filename) ) goto failed_to_open_file;

        // unpack the certificate
        certificate_unpack(&p_certificate, p_stream);

        // close the file
        stream_destroy(&p_stream);
    }

    // sign the certificate
    certificate_sign(p_certificate, &public_key, &private_key);

    // print the certificate
    certificate_print(p_certificate);

    // open the file for writing
    if ( 0 == stream_from_path(&p_stream, p_output_filename) ) goto failed_to_open_file;

    // pack the certificate
    certificate_pack(p_stream, p_certificate);

    // close the file
    stream_destroy(&p_stream);

    // destroy the certificate
    certificate_destroy(&p_certificate);

    // success
    return EXIT_SUCCESS;

    // error handling
    {

        // standard library errors
        {
            failed_to_open_file:

                // log the error
                log_error("Error: Failed to open file \"%s\"!\n", p_output_filename);

                // error
                return EXIT_FAILURE;
        }
    }
}

void print_usage ( const char *argv0 )
{

    // argument check
    if ( NULL == argv0 ) exit(EXIT_FAILURE);

    // print a usage message to standard out
    printf("Usage: %s -o certificate.cer -k key.key\n", argv0);

    // done
    return;
}

void parse_command_line_arguments ( int argc, const char *argv[] )
{

    // iterate through each command line argument
    for (size_t i = 1; i < (size_t) argc; i++)
    {
        
        // output file
        if ( 0 == strcmp(argv[i], "-o") )

            // set the output file
            p_output_filename = argv[++i];

        // key
        else if ( 0 == strcmp(argv[i], "-k") )

            // set the key file
            p_key_file = argv[++i];

        // default
        else goto invalid_arguments;
    }
    
    // success
    return;

    // error handling
    {

        // argument errors
        {
            invalid_arguments:
                
                // print a usage message to standard out
                print_usage(argv[0]);

                // abort
                exit(EXIT_FAILURE);
        }
    }
}
