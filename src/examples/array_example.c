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

// forward declarations
int lower_case_string ( const void *const p_value, size_t i );
int print_string ( const void *const p_value, size_t i );

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

// entry point
int main ( int argc, const char* argv[] )
{

    // Supress compiler warnings
    (void) argc;
    (void) argv;

    // initialized data
    array *p_array           = (void *) 0;
    char  *slice_of_array[]  = { 0, 0, (void *)0 };
    char  *buf[1024] = { 0 };
    FILE  *p_f = fopen("resources/reflection/array.bin", "wb");
    size_t len = 0;

    // Construct an array
    array_from_arguments(&p_array, 4, 2, "Red", "Yellow");

    // Add some elements
    array_add(p_array, "Green");
    array_add(p_array, "Blue");

    // Print the arrays' elements
    printf("Constructed array: \n"),
    array_foreach_i(p_array, (fn_array_foreach_i *)lower_case_string);

    // Get a slice of the array
    array_slice(p_array, (void**)slice_of_array, 0, 1);

    // Formatting
    printf("\nSlice [1..2]\n");

    // Print the array slice
    for (int i = 0; i < 1; i++)
        printf("[%d] : %s\n", i, slice_of_array[i]);

    // Reflect the array to a buffer
    len = array_pack(buf, p_array, pack_string),
    
    // Write the buffer to a file
    fwrite(buf, len, 1, p_f),
    memset(buf, 0, sizeof(buf)),

    // Close the file
    fclose(p_f),

    // Destroy the array
    array_destroy(&p_array),

    // Read a buffer from a file
    p_f = freopen("resources/reflection/array.bin", "rb", p_f),
    fread(buf, len, 1, p_f),
    
    // Reflect an array from the buffer
    array_unpack(&p_array, buf, unpack_string),

    // Print the arrays' elements
    printf("\nReflected array: \n"),
    array_foreach_i(p_array, (fn_array_foreach_i *)print_string),

    // Destroy the array
    array_destroy(&p_array);

    // success
    return EXIT_SUCCESS;
}

// Print the arrays' elements
int lower_case_string ( const void *const p_value, size_t i )
{

    // Unused
    (void) i;
    
    // initialized data
    char _item[16] = { 0 };
    size_t len = strlen(p_value);
    
    strncpy(_item, p_value, len);

    for (size_t j = 0; j < len; j++)
        _item[j] |= 0x20;

    // Print the element
    printf("%s\n", (const char *const)&_item);
    
    // success
    return 1;
}

int print_string ( const void *const p_value, size_t i )
{

    // Unused
    (void) i;
    
    // Print the element
    printf("%s\n", (char *)p_value);
    
    // success
    return 1;
}