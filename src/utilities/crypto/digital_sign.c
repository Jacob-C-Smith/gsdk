/** ! 
 * Digital Signature Algorithm Example
 * 
 * @file src/utilities/crypto/digital_sign.c
 * 
 * @author Jacob Smith
 */

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// gsdk
/// core
#include <core/log.h>
#include <core/stream.h>

/// crypto
#include <crypto/digital_signature.h>

// forward declarations
/// logs
int checkpoint ( const char *p_event );

// data
public_key  *p_public_key = NULL;
private_key *p_private_key = NULL;
void        *p_signature = NULL;
const char  *message = "The quick brown fox jumps over the lazy dog.";

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
 * @param argc            the argc parameter of the entry point
 * @param argv            the argv parameter of the entry point
 * @param examples_to_run return
 * 
 * @return void on success, program abort on failure
 */
void parse_command_line_arguments ( int argc, const char *argv[] );

// data
/// paths
char   *private_key_path = NULL,
       *public_key_path  = NULL,
       *input_path       = NULL,
       *output_path      = NULL;
stream *input_stream     = NULL,
       *output_stream    = NULL;
char    _buffer[256]     = { 0 };

// entry point
int main ( int argc, const char *argv[] )
{

    // initialized data
    int result = 0;

    // parse command line arguments
    parse_command_line_arguments(argc, argv);

    // construct the key pair
    if ( 0 == key_pair_from_files (
            &p_public_key,
            &p_private_key,
            public_key_path,
            private_key_path
        )
    ) goto failed_to_load_key_pair;

    // open the input stream
    if ( input_path ) stream_from_path(&input_stream, input_path);
    else stream_from_file(&input_stream, stdin);

    // read the message
    result = stream_read(input_stream, _buffer, sizeof(_buffer));
    if ( 0 == result ) goto failed_to_read_message;

    // sign the message
    if ( 0 == digital_signature_sign(
        p_private_key,
        p_public_key, 

        _buffer, 
        result,

        &p_signature
    ) ) goto failed_to_sign_message;

    // open the output stream
    if ( output_path ) stream_from_path(&output_stream, output_path);
    else stream_from_file(&output_stream, stdout);

    // write the signature
    if ( 0 == stream_write(output_stream, p_signature, sizeof(((public_key *)0)->n)) ) goto failed_to_write_signature;

    // close the streams
    stream_destroy(&input_stream);
    stream_destroy(&output_stream);

    // release the signature
    p_signature = default_allocator(p_signature, 0);

    // release the keys
    p_public_key  = default_allocator(p_public_key, 0);
    p_private_key = default_allocator(p_private_key, 0);

    // success
    return EXIT_SUCCESS;

    // error handling
    {
        
        // rsa errors
        {
            failed_to_load_key_pair:
                #ifndef NDEBUG
                    log_error("Failed to load key pair\n");
                #endif

                // error
                return EXIT_FAILURE;
        }

        // digital signature errors
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

        // standard library errors
        {

            failed_to_write_signature:
                #ifndef NDEBUG
                    log_error("Failed to write signature\n");
                #endif

                // release the signature if allocated
                p_signature = default_allocator(p_signature, 0);

                // error
                return EXIT_FAILURE;

            failed_to_read_message:
                #ifndef NDEBUG
                    log_error("Failed to read message\n");
                #endif

                // release the signature if allocated
                p_signature = default_allocator(p_signature, 0);

                // error
                return EXIT_FAILURE;
        }
    }
}

void parse_command_line_arguments ( int argc, const char *argv[] )
{
    
    // iterate through each command line argument
    for (size_t i = 1; i < (size_t) argc; i++)
    {
        
        // public key
        if ( strcmp(argv[i], "-p") == 0 )
            public_key_path = (char *) argv[i + 1],
            i++;

        // private key
        else if ( strcmp(argv[i], "-q") == 0 )
            private_key_path = (char *) argv[i + 1],
            i++;
        
        // input
        else if ( strcmp(argv[i], "-i") == 0 )
            input_path = (char *) argv[i + 1],
            i++;

        // output
        else if ( strcmp(argv[i], "-o") == 0 )
            output_path = (char *) argv[i + 1],
            i++;

        // default
        else goto invalid_arguments;
    }

    // Check for required arguments
    if ( NULL ==  public_key_path ) goto invalid_arguments;
    if ( NULL == private_key_path ) goto invalid_arguments;

    // success
    return;

    // error handling
    {

        // argument errors
        {
            invalid_arguments:
                
                // Print a usage message to standard out
                print_usage(argv[0]);

                // Abort
                exit(EXIT_FAILURE);
        }
    }
}

void print_usage ( const char *argv0 )
{

    // argument check
    if ( argv0 == (void *) 0 ) exit(EXIT_FAILURE);

    // print a usage message to standard out
    printf
    (
        "Usage: %s "\
        "-p <public.key> "\
        "-q <private.key> "\
        "-i <input_file> "\
        "-o <output_file> \n",
        argv0
    );

    // done
    return;
}
