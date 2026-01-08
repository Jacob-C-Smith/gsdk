/** ! 
 * Digital Signature Algorithm Example
 * 
 * @file src/utilities/digital_verify.c
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
 * 
 * @return void on success, program abort on failure
 */
void parse_command_line_arguments ( int argc, const char *argv[] );

// data
/// paths
char *private_key_path = NULL,
     *public_key_path  = NULL,
     *signature_path = NULL;
FILE *input_file       = NULL,
     *signature_file   = NULL;
char  _buffer[256]     = { 0 };

// entry point
int main ( int argc, const char *argv[] )
{

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

    // read the message
    size_t message_len = fread(_buffer, sizeof(char), sizeof(_buffer), input_file);
    if ( message_len == 0 ) goto failed_to_read_message;

    // allocate memory for the signature
    p_signature = default_allocator(0, sizeof(_buffer));
    if ( p_signature == NULL ) goto failed_to_allocate_signature;

    // read the signature
    if ( 0 == fread(p_signature, sizeof(char), sizeof(_buffer), signature_file) ) goto failed_to_read_signature;

    // verify the signature
    int verification_result = digital_signature_verify(
        p_public_key,
        _buffer, 
        message_len,
        p_signature
    );

    // print the result
    if (verification_result == 1)
    {
        printf("Signature is valid.\n");
    }
    else
    {
        printf("Signature is invalid.\n");
    }

    // close the files
    fclose(input_file);
    fclose(signature_file);

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
            failed_to_allocate_signature:
                #ifndef NDEBUG
                    log_error("Failed to allocate memory for signature\n");
                #endif

                // error
                return EXIT_FAILURE;
        }

        // standard library errors
        {

            failed_to_read_signature:
                #ifndef NDEBUG
                    log_error("Failed to read signature\n");
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

        // private key, needed for public key reconstruction
        else if ( strcmp(argv[i], "-q") == 0 )
            private_key_path = (char *) argv[i + 1],
            i++;
        
        // signature
        else if ( strcmp(argv[i], "-s") == 0 )
            signature_file = fopen(argv[i + 1], "rb"),
            i++;

        // input
        else if ( strcmp(argv[i], "-i") == 0 )
            input_file = ( 0 == strcmp(argv[i + 1], "-") ) ? stdin : fopen(argv[i + 1], "rb"),
            i++;

        // default
        else goto invalid_arguments;
    }

    // Check for required arguments
    if ( NULL ==  public_key_path ) goto invalid_arguments;
    if ( NULL == private_key_path ) goto invalid_arguments;
    if ( NULL == signature_file ) goto invalid_arguments;

    if ( NULL == input_file ) input_file = stdin;

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
        "Usage: %s -p <public.key> -q <private.key> -s <signature> -i <input_file>\n",
        argv0
    );

    // done
    return;
}