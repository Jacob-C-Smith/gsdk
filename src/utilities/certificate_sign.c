/** ! 
 * Certificate signer
 * 
 * @file src/utilities/certificate_sign.c
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
#include <core/sha.h>
#include <core/ed25519.h>
#include <core/certificate.h>

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
    FILE                *p_f           = NULL;
    ed25519_public_key   public_key    = { 0 };
    ed25519_private_key  private_key   = { 0 };
    certificate         *p_certificate = NULL;
    char                 _buffer[160]  = { 0 };
    size_t               len           = 0;

    // parse command line arguments
    parse_command_line_arguments(argc, argv);

    // load a key pair from file
    {

        // initialized data
        char _buffer[64] = { 0 };

        // open the key file
        p_f = fopen(p_key_file, "rb");
        if ( NULL == p_f ) goto failed_to_open_file;

        // read the key pair
        fread(_buffer, 1, sizeof(_buffer), p_f);

        // unpack the key pair
        ed25519_key_pair_unpack(&public_key, &private_key, _buffer);

        // close the file
        fclose(p_f);
    }

    // load the certificate
    {

        // open the file
        p_f = fopen(p_output_filename, "rb");
        if ( NULL == p_f ) goto failed_to_open_file;

        // read the certificate
        len = fread(_buffer, 1, sizeof(_buffer), p_f);

        // unpack the certificate
        certificate_unpack(&p_certificate, _buffer);

        // close the file
        fclose(p_f);
    }

    // sign the certificate
    certificate_sign(p_certificate, &public_key, &private_key);

    // print the certificate
    certificate_print(p_certificate);

    // open the file for writing
    p_f = fopen(p_output_filename, "wb");
    if ( NULL == p_f ) goto failed_to_open_file;

    // pack the certificate
    len = certificate_pack(_buffer, p_certificate);

    // write the certificate to file
    fwrite(_buffer, 1, len, p_f);

    // close the file
    fclose(p_f);

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
