/** !
 * JSON example program
 * 
 * @file main.c
 * 
 * @author Jacob Smith
 */

// standard library
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// json
#include <reflection/json.h>

// Function declarations
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
 * @param path path to the file
 * @param buffer buffer
 * @param binary_mode "wb" IF true ELSE "w"
 * 
 * @return 1 on success, 0 on error
 */
size_t load_file ( const char *path, void *buffer, bool binary_mode );

// Entry point
int main ( int argc, const char* argv[] )
{

    // Check for valid argument
    if ( argc == 1 ) goto no_argument;

    // Iterate over command line arguments
    for (int i = 1; i < argc; i++)
    {

        // Output formatting
        printf("--- %s ---\n", argv[i]);

        // Write the contents of the file to stdout
        print_json_file(argv[i]);

        // Output formatting
        putchar('\n');
        putchar('\n');
    }

    // Success
    return EXIT_SUCCESS;
    
    // Error handling
    {
        no_argument:

            // Write a usage message
            printf("Usage: json_example file1.json [file2.json ... fileN.json]\n");
            
            // Error
            return EXIT_FAILURE;
    }
}

int print_json_file ( const char *path )
{
    
    // Argument checking 
    if ( path == 0 ) goto no_path;

    // Initialized data
    json_value  *p_value  = 0;
    size_t       file_len = load_file(path, 0, false);
    char        *file_buf = calloc(file_len + 1, sizeof(char));

    // Load the file
    if ( load_file(path, file_buf, false) == 0 ) goto failed_to_load_file;

    // Parse the JSON into a value
    if ( json_value_parse(file_buf, 0, &p_value) == 0 ) goto failed_to_parse_json;

    // Free the allocation
    file_buf = JSON_REALLOC(file_buf, 0);

    // Print the parsed contents to stdout
    json_value_print(p_value);

    // Free the JSON value
    json_value_free(p_value);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_path:
                #ifndef NDEBUG
                    log_error("[json] Null path provided to function \"%s\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // standard library errors
        {
            failed_to_load_file:
                #ifndef NDEBUG
                    log_error("[json] Failed to load file \"%s\" in call to function \"%s\"\n", path, __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // JSON Errors
        {

            failed_to_parse_json:
                #ifndef NDEBUG
                    log_error("[json] Failed to parse JSON in call to function \"%s\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

size_t load_file ( const char *path, void *buffer, bool binary_mode )
{

    // Argument checking 
    if ( path == 0 ) goto no_path;

    // Initialized data
    size_t  ret = 0;
    FILE   *f   = fopen(path, (binary_mode) ? "rb" : "r");
    
    // Check if file is valid
    if ( f == NULL ) goto invalid_file;

    // Find file size and prep for read
    fseek(f, 0, SEEK_END);
    ret = (size_t) ftell(f);
    fseek(f, 0, SEEK_SET);
    
    // Read to data
    if ( buffer ) 
        ret = fread(buffer, 1, ret, f);

    // The file is no longer needed
    fclose(f);
    
    // Success
    return ret;

    // Error handling
    {

        // Argument errors
        {
            no_path:
                #ifndef NDEBUG
                    log_error("[json] Null path provided to function \"%s\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // File errors
        {
            invalid_file:
                #ifndef NDEBUG
                    printf("[Standard library] Failed to load file \"%s\". %s\n",path, strerror(errno));
                #endif

                // Error
                return 0;
        }
    }
}
