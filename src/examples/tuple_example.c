/** !
 * Tuple example program
 * 
 * @file main.c
 * 
 * @author Jacob Smith
 */

// standard library
#include <stdio.h>
#include <stdlib.h>

// log module
#include <core/log.h>
#include <core/sync.h>
#include <core/pack.h>

// tuple
#include <data/tuple.h>

int pack_string ( void *p_buffer, const void *const p_value )
{

    // success
    return pack_pack(p_buffer, "%s", p_value);
}

int unpack_string ( void *const p_value, void *p_buffer )
{

    // success
    return pack_unpack(p_buffer, "%s", p_value);
}

// Forward declaration
/** !
 *  Print each element in a tuple 
 * 
 * @param p_tuple the tuple
 * 
 * @return 1 on success, 0 on error
*/
int print_all_elements ( tuple *p_tuple );

// entry point
int main ( int argc, const char* argv[] )
{

    // Suppress warnings
    (void) argc;
    (void) argv;

    // initialized data
    tuple *p_tuple          = (void *) 0;
    const char *_p_slice_of_tuple[] = { 0, 0, (void *)0 };
    char  *buf[1024] = { 0 };
    FILE  *p_f = fopen("resources/reflection/tuple.bin", "wb");
    size_t len = 0;

    // Output
    log_info("Constructing tuple (\"Dogs\", \"Cats\", \"Birds\", \"Fish\")\n");

    // Make a 4 element tuple with variadic arguments
    tuple_from_arguments(&p_tuple, 4, "Dogs", "Cats", "Birds", "Fish");

    // Print the tuples' keys
    print_all_elements(p_tuple);

    // Output
    log_info("Tuple slice [1:2]\n");

    // Get elements [1:2]
    tuple_slice(p_tuple, (const void **const) &_p_slice_of_tuple, 1, 2);

    // Print the slice
    printf("%s\n",_p_slice_of_tuple[0]);
    printf("%s\n",_p_slice_of_tuple[1]);
    
    // Reflect the tuple to a buffer
    len = tuple_pack(p_tuple, buf, pack_string);
    
    // Write the buffer to a file
    fwrite(buf, len, 1, p_f),
    memset(buf, 0, sizeof(buf)),

    // Close the file
    fclose(p_f),
    log_info("\nSerializig tuple to \"resources/reflection/tuple.bin\"\n\n");

    // Destroy the tuple
    tuple_destroy(&p_tuple),
    log_info("Destroyed tuple\n\n");
    
    // Read a buffer from a file
    p_f = freopen("resources/reflection/tuple.bin", "rb", p_f),
    fread(buf, len, 1, p_f),
    
    // Reflect an tuple from the buffer
    tuple_unpack(&p_tuple, buf, unpack_string);

    // Print the tuples' elements
    log_info("Reflected tuple \"resources/reflection/tuple.bin\""),
    putchar(':'), putchar('\n'),
    print_all_elements(p_tuple);

    // success
    return EXIT_SUCCESS;
}

// Print the tuples' elements
int print_all_elements ( tuple *p_tuple )
{

    // argument check
    if ( p_tuple == 0 ) return 0;

    // initialized data
    const void **pp_elements = 0;
    size_t       size        = 0;

    // Get quantity of elements in the tuple
    size = tuple_size(p_tuple);

    // Allocate memory for tuple contents
    pp_elements = calloc(size, sizeof(void *));

    // error check
    if ( pp_elements == (void *) 0 ) return 0;

    // Get the contents of the tuple
    tuple_slice(p_tuple, pp_elements, 0, (signed long long) size);

    // Iterate over each element
    for (size_t i = 0; i < size; i++)

        // Print each key
        printf("[%lu] %s\n", i, (char *)pp_elements[i]);
    
    // Formatting
    putchar('\n');

    // Clean up
    free(pp_elements);
    
    // success
    return 1;
}