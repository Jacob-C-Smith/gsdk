/** ! 
 * Certificate chain verifier
 * 
 * @file src/utilities/certificate/certificate_chain_verify.c
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
const char  *p_root_filename    = NULL;
const char **pp_chain_filenames = NULL;
size_t       chain_count        = 0;

// entry point
int main ( int argc, const char *argv[] )
{
    
    // initialized data
    certificate  *p_root       = NULL;
    certificate **pp_chain     = NULL;
    stream       *p_stream     = NULL;

    // parse command line arguments
    parse_command_line_arguments(argc, argv);

    // load the root certificate
    {

        // open the file
        if ( 0 == stream_from_path(&p_stream, p_root_filename) ) goto failed_to_open_file;

        // construct the trust root
        certificate_unpack(&p_root, p_stream);

        // close the file
        stream_destroy(&p_stream);
    }

    // load the certificate chain
    pp_chain = default_allocator(NULL, chain_count * sizeof(certificate *));

    // load each certificate
    for (size_t i = 0; i < chain_count; i++)
    {
        
        // open the file
        if ( 0 == stream_from_path(&p_stream, pp_chain_filenames[i]) ) goto failed_to_open_file;

        // construct the certificate
        certificate_unpack(&pp_chain[i], p_stream);

        // close the file
        stream_destroy(&p_stream);
    }

    // verify the chain
    if ( 1 == certificate_chain_verify(pp_chain, chain_count, p_root) )
        printf("Certificate chain is valid.\n");
    else
        printf("Certificate chain is invalid.\n");

    // release the trust root
    certificate_destroy(&p_root);

    // release each certificate in the chain
    for (size_t i = 0; i < chain_count; i++)
        certificate_destroy(&pp_chain[i]);
    
    // release the certificate chain list
    pp_chain = default_allocator(pp_chain, 0);

    // release the certificate chain paths
    pp_chain_filenames = default_allocator(pp_chain_filenames, 0);

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
    printf("Usage: %s -r root.cer -c leaf.cer [intermediate.cer ...]\n", argv0);

    // done
    return;
}

void parse_command_line_arguments ( int argc, const char *argv[] )
{

    // iterate through each command line argument
    for (size_t i = 1; i < (size_t) argc; i++)
    {
        
        // root file
        if ( 0 == strcmp(argv[i], "-r") )

            // set the root file
            p_root_filename = argv[++i];

        // certificate chain
        else if ( 0 == strcmp(argv[i], "-c") )
        {
            // increment the index
            i++;

            // store the starting index
            size_t start = i;

            // count the chain files
            while ( i < (size_t) argc && argv[i][0] != '-' )
            {
                chain_count++;
                i++;
            }

            // allocate the chain filenames array
            pp_chain_filenames = default_allocator(NULL, chain_count * sizeof(char *));

            // store the chain filenames
            for (size_t j = 0; j < chain_count; j++)
                pp_chain_filenames[j] = argv[start + j];

            // backtrack the index
            i--;
        }

        // default
        else goto invalid_arguments;
    }

    // error check
    if ( NULL == p_root_filename ) goto invalid_arguments;
    if ( 0    == chain_count     ) goto invalid_arguments;
    
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
