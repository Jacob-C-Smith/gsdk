/** ! 
 * Ed25519 key generator
 * 
 * @file src/utilities/crypto/ed25519_key_generator.c
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
const char *p_output_filename = NULL;

// entry point
int main ( int argc, const char *argv[] )
{
    
    // parse command line arguments
    parse_command_line_arguments(argc, argv);

    // initialized data
    ed25519_public_key   p_public_key  = { 0 };
    ed25519_private_key  p_private_key = { 0 };
    stream              *p_stream      = NULL;

    // construct a key pair
    if ( 0 == ed25519_key_pair_construct
    (
        &p_public_key, 
        &p_private_key 
    ) ) goto failed_to_create_key_pair;

    // open the file
    if ( 0 == stream_from_path(&p_stream, p_output_filename) ) goto failed_to_open_file;

    // pack the keypair
    ed25519_key_pair_pack(p_stream, &p_public_key, &p_private_key);

    // close the file
    stream_destroy(&p_stream);

    // success
    return EXIT_SUCCESS;

    // error handling
    {

        // ed25519 errors
        {
            failed_to_create_key_pair:

                // log the error
                log_error("Error: Failed to create key pair!\n");

                // error
                return EXIT_FAILURE;
        }

        // standard library errors
        {
            failed_to_open_file:

                // log the error
                log_error("Error: Failed to open file \"%s\" for writing!\n", p_output_filename);

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
    printf("Usage: %s -o keypair.key\n", argv0);

    // done
    return;
}

void parse_command_line_arguments ( int argc, const char *argv[] )
{

    // error check
    if ( argc != 3 ) goto invalid_arguments;

    // iterate through each command line argument
    for (size_t i = 1; i < (size_t) argc; i++)
    {
        
        // output file
        if ( 0 == strcmp(argv[i], "-o") )

            // set the output file
            p_output_filename = argv[++i];

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
