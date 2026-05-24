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

// gsdk
/// core
#include <core/log.h>
#include <core/sync.h>
#include <core/hash.h>
#include <core/pack.h>
#include <core/stream.h>

/// data
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
fn_comparator string_compare;
fn_fori string_print;
fn_hash64 string_hash;
fn_map string_upper_case;
fn_map string_lower_case;
fn_pack string_pack;
fn_unpack string_unpack;
 
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
FILE   *p_f      = NULL;
size_t  file_len = 0;

/// hashes
hash64 h1 = 0,
       h2 = 0;

/// working array
array *p_array = NULL;

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

    // #3 - add more
    {
        
        // add some colors
        for (enum color_e _color = GREEN; _color < COLOR_QUANTITY; _color++)
            array_add(p_array, (void *)_p_colors[_color]);

        // checkpoint
        checkpoint(p_array, "after adding < Green, Blue, Purple >");
    }

    // #4 - remove some
    {
        
        // remove some colors
        for (int i = 1; i < COLOR_QUANTITY; i += 3)
            array_remove(p_array, i, 0);

        // print the arrays' elements
        checkpoint(p_array,"after removing < orange, Purple >");
    }

    // #5 - slice 
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

    // #6 - to binary
    {

        // initialized data
        stream *p_stream = NULL;
        
        // construct a stream
        stream_from_path(&p_stream, "resources/reflection/array.bin");
        
        // reflect the array to a buffer
        array_pack(p_stream, p_array, string_pack),

        // destroy the stream
        stream_destroy(&p_stream);
        
        // checkpoint
        checkpoint(p_array, "after serialize");
    }

    // #7 - hash 1
    {

        // initialized data
        h1 = array_hash(p_array, (fn_hash64 *)string_hash);

        // print the hash
        printf("hash 1 -> 0x%llx\n", h1);

        // checkpoint
        checkpoint(p_array, "after hash 1");
    }

    // #8 - map upper case
    {

        // convert the array elements to upper case
        array_map(p_array, string_upper_case, NULL);

        // checkpoint
        checkpoint(p_array, "after upper case map");
    }

    // #9 - destroy
    {

        // destroy the array
        array_destroy(&p_array, default_allocator);

        // checkpoint
        checkpoint(p_array, "after destroy");
    }

    // #10 - from binary
    {
        
        // initialized data
        stream *p_stream = NULL;
        
        // construct a stream
        stream_from_path(&p_stream, "resources/reflection/array.bin");
        
        // reflect an array from the buffer
        array_unpack(&p_array, p_stream, string_unpack),

        // close the file
        stream_destroy(&p_stream);

        // checkpoint
        checkpoint(p_array, "after parse");
    }

    // #11 - hash 2
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
        array_destroy(&p_array, default_allocator);

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
    {

        // initialized data
        size_t i = 0;

        // logs
        log_info("#%d - Array %s:\n", step, p_event);

        // iterate through the array
        for ( iterator it = array_iterator(p_array); !it.done(&it); it.next(&it) )
            string_print(it.item(&it), i),
            i++;

        // formatting
        putchar('\n');
    }

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

int string_pack ( stream *p_stream, const void *const p_value )
{

    // done
    return pack_pack(p_stream, "%s", p_value);
}

int string_unpack ( void *const p_value, stream *p_stream )
{

    // initialized data
    char       **pp_value        = (char **) p_value;
    int          result          = 0;
    char        *p_string        = NULL;
    const char   _string  [1024] = { 0 };

    // unpack the buffer
    result = pack_unpack(p_stream, "%s", &_string);

    // duplicate the string
    p_string = strdup(_string);

    // return a pointer to the caller
    *pp_value = p_string;

    // done
    return result;
}
