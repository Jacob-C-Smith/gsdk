/** !
 *  Node example program
 * 
 * @file main.c
 * 
 * @author Jacob Smith
 */

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// node module
#include <node/node.h>

// Function declarations
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
int main ( int argc, const char *argv[] )
{

    // Unused
    (void) argc;
    (void) argv;

    // Initialized data
    node_graph *p_node_graph = (void *) 0;
    json_value *p_value = (void *) 0;
    char _file[4096] = { 0 };

    // Load the file
    if ( load_file("resources/deferred.json", &_file, false) == 0 ) goto failed_to_load_file;

    // Parse the json text 
    if ( json_value_parse((char *)&_file, 0, &p_value) == 0 ) goto failed_to_parse_json;

    // Construct a node graph
    if ( node_graph_construct(&p_node_graph, p_value) == 0 ) goto failed_to_construct_graph;

    // Print the node graph to standard out
    node_graph_print(p_node_graph);
    
    // Success
    return EXIT_SUCCESS;

    // Error handling
    {

        failed_to_load_file:
            #ifndef NDEBUG
                log_error("Error: Failed to load file!\n");
            #endif

            // Error
            return 0;

        failed_to_parse_json:
            #ifndef NDEBUG
                log_error("Error: Failed to parse json text!\n");
            #endif

            // Error
            return 0;

        failed_to_construct_graph:
            #ifndef NDEBUG
                log_error("Error: Failed to construct graph!\n");
            #endif

            // Error
            return 0;
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
                    log_error("[node] Null path provided to function \"%s\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // File errors
        {
            invalid_file:
                #ifndef NDEBUG
                    log_error("[Standard library] Failed to load file \"%s\". %s\n",path, strerror(errno));
                #endif

                // Error
                return 0;
        }
    }
}
