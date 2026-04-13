/** ! 
 * Certificate verifier
 * 
 * @file src/utilities/certificate_verify.c
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
const char *p_filename        = NULL;
const char *p_issuer_filename = NULL;

// entry point
int main ( int argc, const char *argv[] )
{
    
    // initialized data
    FILE        *p_f           = NULL;
    certificate *p_certificate = NULL;
    certificate *p_issuer      = NULL;
    char         _buffer[160]  = { 0 };
    size_t       len           = 0;

    // parse command line arguments
    parse_command_line_arguments(argc, argv);

    // load the certificate
    {

        // open the file
        p_f = fopen(p_filename, "rb");
        if ( NULL == p_f ) goto failed_to_open_file;

        // read the certificate
        fread(_buffer, 1, sizeof(_buffer), p_f);

        // unpack the certificate
        certificate_unpack(&p_certificate, _buffer);

        // close the file
        fclose(p_f);
    }

    // load the issuer certificate if provided
    if ( p_issuer_filename )
    {

        // initialized data
        char _issuer_buf[160] = { 0 };

        // open the issuer certificate
        p_f = fopen(p_issuer_filename, "rb");
        if ( NULL == p_f ) goto failed_to_open_file;

        // read the issuer certificate
        fread(_issuer_buf, 1, sizeof(_issuer_buf), p_f);

        // construct the issuer certificate
        certificate_unpack(&p_issuer, _issuer_buf);

        // close the file
        fclose(p_f);
    }

    // print the certificate
    certificate_print(p_certificate);

    // verify the certificate
    if ( 1 == certificate_verify(p_certificate, p_issuer) ) 
        printf("Certificate is valid.\n");
    else
        printf("Certificate is invalid.\n");

    // destroy the certificate
    certificate_destroy(&p_certificate);

    // destroy the issuer certificate
    if ( p_issuer ) 
        certificate_destroy(&p_issuer);

    // success
    return EXIT_SUCCESS;

    // error handling
    {

        // standard library errors
        {
            failed_to_open_file:

                // log the error
                log_error("Error: Failed to open file!\n");

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
    printf("Usage: %s -o certificate.cer [-i issuer.cer]\n", argv0);

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
            p_filename = argv[++i];

        // issuer
        else if ( 0 == strcmp(argv[i], "-i") )

            // set the issuer file
            p_issuer_filename = argv[++i];

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
