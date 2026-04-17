/** ! 
 * Certificate constructor
 * 
 * @file src/utilities/certificate_create.c
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
const char *p_subject         = NULL;
const char *p_key_file        = NULL;
const char *p_issuer_filename = NULL;
const char *p_output_filename = NULL;
bool        is_ca              = false;

// entry point
int main ( int argc, const char *argv[] )
{
    
    // initialized data
    FILE                *p_f           = NULL;
    ed25519_public_key   public_key    = { 0 };
    ed25519_private_key  private_key   = { 0 };
    certificate         *p_certificate = NULL;
    sha256_hash          _h            = { 0 };
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
    
    // fingerprint the issuer
    if ( NULL == p_issuer_filename )
    {

        // initialized data
        sha256_state s = { 0 };

        sha256_construct(&s);
        sha256_update(&s, (const unsigned char *)p_subject, strlen(p_subject));
        sha256_final(&s, _h);
    }
    else
    {

        // initialized data
        certificate *p_issuer          = NULL;
        FILE        *p_issuer_f        = NULL;
        char         _issuer_buf[1024] = { 0 };
        const char  *p_issuer_subject  = NULL;
        sha256_state s                 = { 0 };

        // open the issuer certificate
        p_issuer_f = fopen(p_issuer_filename, "rb");
        if ( NULL == p_issuer_f ) goto failed_to_open_file;

        // read the issuer certificate
        fread(_issuer_buf, 1, sizeof(_issuer_buf), p_issuer_f);

        // construct the certificate
        certificate_unpack(&p_issuer, _issuer_buf);

        // close the file
        fclose(p_issuer_f);

        // get the issuer's subject
        certificate_get_subject(p_issuer, &p_issuer_subject);

        // fingerprint the issuer
        sha256_construct(&s);
        sha256_update(&s, (const unsigned char *)p_issuer_subject, strlen(p_issuer_subject));
        sha256_final(&s, _h);

        // destroy the issuer certificate
        certificate_destroy(&p_issuer);
    }

    // create a certificate
    certificate_construct(
        &p_certificate,
        p_subject, 
        &public_key, 
        _h, 
        time(NULL), 
        time(NULL) + 365 * 24 * 60 * 60,
        is_ca
    );

    // print the certificate
    certificate_print(p_certificate);

    // open the file
    p_f = fopen(p_output_filename, "wb");
    if ( NULL == p_f ) goto failed_to_open_file;

    // pack the certificate
    len = certificate_pack(_buffer, p_certificate);

    // write the certificate to the file
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
    printf("Usage: %s -o certificate.cer -s subject -k key.key [-i issuer.cer] [-ca]\n", argv0);

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

        // subject
        else if ( 0 == strcmp(argv[i], "-s") )

            // set the subject name
            p_subject = argv[++i];

        // key
        else if ( 0 == strcmp(argv[i], "-k") )

            // set the key file
            p_key_file = argv[++i];

        // issuer
        else if ( 0 == strcmp(argv[i], "-i") )

            // set the issuer file
            p_issuer_filename = argv[++i];

        // CA
        else if ( 0 == strcmp(argv[i], "-ca") )

            // set the CA flag
            is_ca = true;

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
