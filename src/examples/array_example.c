/** !
 * Example program for array module
 * 
 * @file main.c
 * 
 * @author Jacob Smith
 */

// standard library
#include <stdio.h>
#include <stdlib.h>

// Include
#include <data/array.h>

// Forward declarations
int lower_case_string ( const void *const p_value, size_t i );

// Entry point
int main ( int argc, const char* argv[] )
{

    // Supress compiler warnings
    (void) argc;
    (void) argv;

    // Initialized data
    array *p_array          = (void *) 0;
    char  *slice_of_array[] = { 0, 0, (void *)0 };
    
    // Make an array with 4 elements
    array_construct(&p_array, 4);

    // Insert some items
    array_add(p_array, "Green");
    array_add(p_array, "Blue");
    
    // Construct an array
    array_from_arguments(&p_array, 4, 2, "Red", "Yellow");

    array_add(p_array, "Green");
    array_add(p_array, "Blue");

    // Print the arrays' elements
    array_foreach_i(p_array, (fn_array_foreach_i *)lower_case_string);

    // Get a slice of the array
    array_slice(p_array, (void**)slice_of_array, 0, 1);

    // Formatting
    printf("\nSlice [1..2]\n");

    // Print the array slice
    printf("%s\n",slice_of_array[0]);
    printf("%s\n",slice_of_array[1]);
    
    // Destroy the array
    array_destroy(&p_array);
    
    // Success
    return EXIT_SUCCESS;
}

// Print the arrays' elements
int lower_case_string ( const void *const p_value, size_t i )
{

    // Supress compiler warnings
    (void) i;
    
    // Initialized data
    char _item[16] = { 0 };
    size_t len = strlen(p_value);
    
    strncpy(_item, p_value, len);

    for (size_t j = 0; j < len; j++)
        _item[j] |= 0x20;

    // Print the element
    printf("%s\n", (const char *const)&_item);
    
    // Success
    return 1;
}
