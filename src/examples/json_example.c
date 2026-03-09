/** !
 * Example program for json module
 * 
 * @file src/examples/json_example.c
 * 
 * @author Jacob Smith
 */

// standard library
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// gsdk
/// core
#include <core/log.h>
#include <core/sync.h>
#include <core/hash.h>
#include <core/pack.h>
#include <core/interfaces.h>

/// data
#include <data/array.h>
#include <data/dict.h>

/// reflection
#include <reflection/json.h>

// function declarations
/**!
 * Print a json_value to stdout
 * 
 * @param p_value the json_value
 * 
 * @return 1 on success, 0 on error
 */
int print_value ( json_value *p_value );

/**!
 * Parse the contents of a file as JSON, and print it to standard out
 * 
 * @param path the json_value
 * 
 * @return 1 on success, 0 on error
 */
int print_json_file ( const char *path );

/**!
 * Return the size of a file IF buffer == 0 ELSE read a file into buffer
 * 
 * @param path        path to the file
 * @param buffer      buffer
 * @param binary_mode "wb" IF true ELSE "w"
 * 
 * @return 1 on success, 0 on error
 */
size_t load_file ( const char *path, void *buffer, bool binary_mode );

// entry point
int main ( int argc, const char* argv[] )
{

    // check for valid arguments
    if ( argc == 1 ) goto no_argument;

    // iterate over command line arguments
    for (int i = 1; i < argc; i++)
    {

        // formatting
        printf("--- %s ---\n", argv[i]);

        // print the contents of the file to stdout
        print_json_file(argv[i]);

        // formatting
        putchar('\n');
        putchar('\n');
    }

    // success
    return EXIT_SUCCESS;
    
    // error handling
    {
        no_argument:

            // print a usage message
            printf("Usage: json_example file1.json [file2.json ... fileN.json]\n");
            
            // error
            return EXIT_FAILURE;
    }
}

int print_json_file ( const char *path )
{
    
    // argument check 
    if ( path == 0 ) goto no_path;

    // initialized data
    json_value  *p_value  = 0;
    size_t       file_len = load_file(path, 0, false);
    char        *file_buf = calloc(file_len + 1, sizeof(char));

    // load the file
    if ( load_file(path, file_buf, false) == 0 ) goto failed_to_load_file;

    // parse the JSON into a value
    if ( json_value_parse(file_buf, 0, &p_value) == 0 ) goto failed_to_parse_json;

    // release the allocation
    file_buf = default_allocator(file_buf, 0);

    // print the parsed contents to stdout
    json_value_print(p_value);

    // release the json value
    json_value_free(p_value, 0);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_path:
                #ifndef NDEBUG
                    log_error("[json] Null path provided to function \"%s\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            failed_to_load_file:
                #ifndef NDEBUG
                    log_error("[json] Failed to load file \"%s\" in call to function \"%s\"\n", path, __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // json errors
        {

            failed_to_parse_json:
                #ifndef NDEBUG
                    log_error("[json] Failed to parse JSON in call to function \"%s\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

size_t load_file ( const char *path, void *buffer, bool binary_mode )
{

    // argument check 
    if ( path == 0 ) goto no_path;

    // initialized data
    size_t  ret = 0;
    FILE   *f   = fopen(path, (binary_mode) ? "rb" : "r");
    
    // check if file exists
    if ( NULL == f ) goto invalid_file;

    // Find file size and prep for read
    fseek(f, 0, SEEK_END),
    ret = (size_t) ftell(f),
    fseek(f, 0, SEEK_SET);
    
    // read the file to a buffer
    if ( buffer ) 
        ret = fread(buffer, 1, ret, f);

    // close the file
    fclose(f);
    
    // success
    return ret;

    // error handling
    {

        // argument errors
        {
            no_path:
                #ifndef NDEBUG
                    log_error("[json] Null path provided to function \"%s\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // File errors
        {
            invalid_file:
                #ifndef NDEBUG
                    printf("[standard library] Failed to load file \"%s\". %s\n",path, strerror(errno));
                #endif

                // error
                return 0;
        }
    }
}
