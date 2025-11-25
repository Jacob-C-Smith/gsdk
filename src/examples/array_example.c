/** !
 * Example program for array module
 * 
 * @file src/examples/array_example.c
 * 
 * @author Jacob Smith
 */

// standard library
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// core
#include <core/log.h>
#include <core/sync.h>
#include <core/hash.h>
#include <core/pack.h>

// data
#include <data/array.h>

// enumeration definitions
enum color_e
{
    RED            = 0,
    ORANGE         = 1,
    YELLOW         = 2,
    GREEN          = 3,
    BLUE           = 4,
    PURPLE         = 5,
    COLOR_QUANTITY = 6
};

// forward declarations
/// logs
int checkpoint ( array *p_array, const char *p_event );

/// string
void    string_print ( void *p_value, int i );
int     string_compare ( const void *const p_a, const void *const p_b );
void   *string_upper_case ( void *p_value );
void   *string_lower_case ( void *p_value );
hash64  string_hash ( const void *const string, unsigned long long unused );
int     string_pack ( void *p_buffer, const void *const p_value );
int     string_unpack ( void *const p_value, void *p_buffer );
 
// data
/// immutable color strings
const char *_p_colors[COLOR_QUANTITY] =
{
    [RED]    = "Red",
    [ORANGE] = "Orange",
    [YELLOW] = "Yellow",
    [GREEN]  = "Green",
    [BLUE]   = "Blue",
    [PURPLE] = "Purple"
};

/// file for reflection
FILE *p_f = NULL;

/// hashes
hash64 h1 = 0,
       h2 = 0;

/// working array
array *p_array = NULL;
size_t file_len = 0;

// entry point
int main ( int argc, const char* argv[] )
{

    // unused
    (void) argc;
    (void) argv;

    // #0 - start
    checkpoint(p_array, "start");
    
    // #1 - initial
    {

        // construct the array
        array_construct(&p_array, 8);

        // checkpoint
        checkpoint(p_array, "after construction");
    }

    // #2 - add 
    {

        // add some colors
        for (enum color_e _color = RED; _color < GREEN; _color++)
            array_add(p_array, (void *)_p_colors[_color]);

        // checkpoint
        checkpoint(p_array,"after adding < Red, Orange, Yellow >");
    }

    // #3 - map lower case
    {

        // convert the array elements to lower case
        array_map(p_array, string_lower_case, 0);

        // checkpoint
        checkpoint(p_array, "after lower case map");
    }

    // #4 - add more
    {
        
        // add some colors
        for (enum color_e _color = GREEN; _color < COLOR_QUANTITY; _color++)
            array_add(p_array, (void *)_p_colors[_color]);

        // checkpoint
        checkpoint(p_array,"after adding < Green, Blue, Purple >");
    }

    // #5 - remove some
    {
        
        // remove some colors
        for (int i = 1; i < COLOR_QUANTITY; i += 3)
            array_remove(p_array, i, 0);

        // print the arrays' elements
        checkpoint(p_array,"after removing < orange, Purple >");
    }

    // #6 - slice 
    {

        // initialized data
        char *slice_of_array[] = { 0, 0, NULL };

        // take a slice
        array_slice(p_array, (void**)slice_of_array, 1, 2);

        // formatting
        printf("\nSlice [1..2]\n");

        // print the array slice
        for ( int i = 0; i < 2; i++ )
            printf("[%d] : %s\n", i, slice_of_array[i]);
        
        // formatting
        putchar('\n');

        // checkpoint
        checkpoint(p_array, "after slice");
    }

    // #7 - to binary
    {

        // initialized data
        char buf[1024] = { 0 };
        
        // Open a file for writing
        p_f = fopen("resources/reflection/array.bin", "wb");

        // reflect the array to a buffer
        file_len = array_pack(buf, p_array, string_pack),
        
        // write the buffer to a file
        fwrite(buf, file_len, 1, p_f),

        // close the file
        fclose(p_f);

        // checkpoint
        checkpoint(p_array, "after serialize");
    }

    // #8 - hash 1
    {

        // initialized data
        h1 = array_hash(p_array, (fn_hash64 *)string_hash);

        // print the hash
        printf("hash 1 -> 0x%llx\n", h1);

        // checkpoint
        checkpoint(p_array, "after hash 1");
    }

    // #9 - map upper case
    {

        // convert the array elements to upper case
        array_map(p_array, string_upper_case, NULL);

        // checkpoint
        checkpoint(p_array, "after upper case map");
    }

    // #10 - destroy
    {

        // destroy the array
        array_destroy(&p_array, NULL);

        // checkpoint
        checkpoint(p_array, "after destroy");
    }

    // #11 - from binary
    {
        
        // initialized data
        char buf[1024] = { 0 };
        
        // read a buffer from a file
        p_f = fopen("resources/reflection/array.bin", "rb"),
        fread(buf, sizeof(char), file_len, p_f),
        
        // reflect an array from the buffer
        array_unpack(&p_array, buf, string_unpack),

        // checkpoint
        checkpoint(p_array, "after parse");
    }

    // #8 - hash 2
    {

        // hash the array
        h2 = array_hash(p_array, (fn_hash64 *)string_hash);

        // print the hash
        printf("hash 2 -> 0x%llx\n", h2);

        // error check
        if ( h1 != h2 ) 

            // abort
            log_error("Error: hash 1 != hash 2\n"), exit(EXIT_FAILURE);

        // checkpoint
        checkpoint(p_array, "after hash 2");
    }

    // #12 - sort
    {

        // sort the array
        array_sort(p_array, string_compare);

        // checkpoint
        checkpoint(p_array, "after sort");
    }

    // #13 - destroy
    {

        // destroy the array
        array_destroy(&p_array, NULL);

        // checkpoint
        checkpoint(p_array, "after destroy");
    }

    // #14 - end
    checkpoint(p_array, "end");
    
    // success
    return EXIT_SUCCESS;
}

int checkpoint ( array *p_array, const char *p_event )
{

    // static data
    static int step = 0;

    // print the array
    if ( NULL == p_array )
        log_info("#%d - Array %s: ", step, p_event),
        printf("NULL\n");
    else
        log_info("#%d - Array %s:\n", step, p_event),
        array_fori(p_array, string_print),
        putchar('\n');

    // increment counter
    step++;

    // success
    return 1;
}

void *string_upper_case ( void *p_value )
{

    // initialized data
    char *p_string = strdup((char *)p_value),
         *p_result = p_string;
         
    // iterate upto null terminator
    while (*p_string)
    {

        // initialized data
        char c = *p_string;

        // convert offending characters
        if ( islower(c) ) c = toupper(c);
        
        // store the upper case character
        *p_string++ = c;
    }

    // success
    return p_result;
}

void *string_lower_case ( void *p_value )
{

    // initialized data
    char *p_string = strdup((char *)p_value),
         *p_result = p_string;
         
    // iterate upto null terminator
    while (*p_string)
    {

        // initialized data
        char c = *p_string;

        // convert offending characters
        if ( isupper(c) ) c = tolower(c);
        
        // store the lower case character
        *p_string++ = c;
    }

    // success
    return p_result;
}

void string_print ( void *p_value, int i )
{
    
    // print the element
    printf("[%d] - %s\n", i, (char *)p_value);
    
    // done
    return;
}

int string_compare ( const void *const p_a, const void *const p_b )
{
    
    // initialized data
    char *a = *(char **)p_a,
         *b = *(char **)p_b;

    // done
    return strcmp(a, b);
}

hash64 string_hash ( const void *const string, unsigned long long unused )
{

    // unused
    (void)unused;

    // done
    return hash_crc64(string, strlen(string));
}

int string_pack ( void *p_buffer, const void *const p_value )
{

    // done
    return pack_pack(p_buffer, "%s", p_value);
}

int string_unpack ( void *const p_value, void *p_buffer )
{

    // done
    return pack_unpack(p_buffer, "%s", p_value);
}
