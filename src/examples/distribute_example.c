// standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// distribute
#include <distribute/distribute.h>
#include <distribute/connection.h>

// forward declarations
/** !
 * Distributed connection program
 * 
 * @param argc the argc parameter of the entry point
 * @param argv the argv parameter of the entry point
 * 
 * @return 1 on success, 0 on error
 */
int distribute_connection_example ( int argc, const char *argv[] );

/** !
 * Distributed connection program
 * 
 * @param argc the argc parameter of the entry point
 * @param argv the argv parameter of the entry point
 * 
 * @return 1 on success, 0 on error
 */
int distribute_compute_squares_example ( int argc, const char *argv[] );


// Data
const char *pp_example_names[] = { "", "", "", "", "", "", (void *) 0 };

// entry point
int main ( int argc, const char *argv[] )
{

    // Initialize parallel
    // if ( distribute_init() == 0 ) goto failed_to_initialize_distribute;

    // Parse command line arguments
    for (size_t i = 0; i < argc; i++)
    {

        // Help
        if ( strcmp(argv[i], "-h") == 0 )
        {

            // Print a usage message to standard out
            //print_usage();

            // successs
            return EXIT_SUCCESS;
        }
    }

    // Connection example
    distribute_connection_example(argc, argv);

    // success
    return EXIT_SUCCESS;

    // error handling
    {
        
        failed_to_initialize_distribute:

            // Write an error message to standard out
            printf("Failed to initialize distribute!\n");

            // error
            return EXIT_FAILURE;

        failed_to_run_thread_example:

            // Print an error message
            printf("Failed to run distribute connection example!\n");

            // error
            return EXIT_FAILURE;
    }
}

int distribute_connection_example ( int argc, const char *argv[] )
{
    
    // initialized data
    connection *p_connection = (void *) 0;
    char example_text[5000] = { 0};

    // Attempt a connection
    if ( connection_initiate_hostname(&p_connection, "localhost", 3000) == 0 ) goto failed_to_initiate_connection;

    // Recieve some data
    connection_read(p_connection, example_text, 5000);

    // Send some data
    // connection_write(p_connection, example_text, strlen(example_text));

    // TODO: Destroy the connection
    //

    // success
    return 1;

    // error handling
    {

        failed_to_initiate_connection:

            // Write an error message to standard out
            printf("Failed to initiate connection in call to function \"%s\"\n", __FUNCTION__);

            // error
            return 0;

        failed_to_join_thread:
            
            // Write an error message to standard out
            printf("Failed to join parallel thread in call to function \"%s\"\n", __FUNCTION__);

            // error
            return 0;
    }
}
