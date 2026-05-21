/** ! 
 * Certificate info
 * 
 * @file src/utilities/certificate/certificate_info.c
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
const char *p_filename = NULL;

// entry point
int main ( int argc, const char *argv[] )
{
    
    // initialized data
    stream      *p_stream      = NULL;
    certificate *p_certificate = NULL;

    // parse command line arguments
    parse_command_line_arguments(argc, argv);

    // open the file
    if ( 0 == stream_from_path(&p_stream, p_filename) ) goto failed_to_open_file;

    // unpack the certificate
    certificate_unpack(&p_certificate, p_stream);

    // close the file
    stream_destroy(&p_stream);

    // print the certificate
    certificate_print(p_certificate);

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
                log_error("Error: Failed to open file \"%s\" for writing!\n", p_filename);

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
    printf("Usage: %s certificate.cer\n", argv0);

    // done
    return;
}

void parse_command_line_arguments ( int argc, const char *argv[] )
{

    // error check
    if ( argc != 2 ) goto invalid_arguments;

    // set the certificate path
    p_filename = argv[1];

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
