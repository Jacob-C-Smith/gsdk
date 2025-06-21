// standard library
#include <stdio.h>
#include <stdlib.h>

// dict submodule
#include <dict/dict.h>

// path submodule
#include <path/path.h>

void fn_path ( const char *full_path, path_type type, size_t i )
{
    printf("%c : %i : %s\n", (type == PATH_TYPE_DIRECTORY) ? 'D' : 'F', i, full_path);
}

// entry point
int main ( int argc, const char *argv[] )
{

    // initialized data
    path       *p_path_file        = 0;
    path       *p_path_directory   = 0;
    size_t      size_in_bytes      = 0;
    size_t      directory_contents = 0;
    const char *p_path_name        = 0;

    // Open a path
    if ( path_open(&p_path_file, "./") == 0 ) goto failed_to_open_path;
    
    // Get the contents of the path
    path_directory_foreach_i(p_path_file, &fn_path);

    // Close the path
    (void)path_close(&p_path_file);

    // success
    return EXIT_SUCCESS;

    failed_to_get_file_size:
    failed_to_open_path:
    failed_to_navigate:
    failed_to_create_file:
    failed_to_create_directory:

    // error
    return EXIT_FAILURE;

    usage_message:
        printf("Usage: path_example /path/to/directory_for_path_example");

        // error
        return EXIT_FAILURE;
}