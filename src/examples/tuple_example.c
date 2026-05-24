/** !
 * Example program for tuple module
 * 
 * @file src/examples/tuple_example.c
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
#include <core/stream.h>

// data
#include <data/tuple.h>

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
int checkpoint ( tuple *p_tuple, const char *p_event );

/// string
fn_fori   string_print;
fn_hash64 string_hash;
fn_pack   string_pack;
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
FILE *p_f = NULL;

/// hashes
hash64 h1 = 0,
       h2 = 0;

/// working tuple
tuple *p_tuple = NULL;
size_t file_len = 0;

// entry point
int main ( int argc, const char* argv[] )
{

    // unused
    (void) argc;
    (void) argv;

    // #0 - start
    checkpoint(p_tuple, "start");
    
    // #1 - initial
    {

        // construct the tuple
        tuple_from_arguments(&p_tuple, 6,
            (void *)_p_colors[RED],
            (void *)_p_colors[ORANGE],
            (void *)_p_colors[YELLOW],
            (void *)_p_colors[GREEN],
            (void *)_p_colors[BLUE],
            (void *)_p_colors[PURPLE]
        );

        // checkpoint
        checkpoint(p_tuple, "after construction");
    }

    // #2 - slice 
    {

        // initialized data
        char *slice_of_tuple[] = { 0, 0, NULL };

        // take a slice
        tuple_slice(p_tuple, (const void **const)slice_of_tuple, 1, 2);

        // formatting
        printf("\nSlice [1..2]\n");

        // print the tuple slice
        for ( int i = 0; i < 2; i++ )
            printf("[%d] : %s\n", i, slice_of_tuple[i]);
        
        // formatting
        putchar('\n');

        // checkpoint
        checkpoint(p_tuple, "after slice");
    }

    // #3 - to binary
    {

        // initialized data
        stream *p_stream = NULL;
        
        // Open a file for writing
        stream_from_path(&p_stream, "resources/reflection/tuple.bin");

        // reflect the tuple to a buffer
        tuple_pack(p_stream, p_tuple, string_pack);
        
        // close the file
        stream_destroy(&p_stream);

        // checkpoint
        checkpoint(p_tuple, "after serialize");
    }

    // #4 - hash 1
    {

        // initialized data
        h1 = tuple_hash(p_tuple, string_hash);

        // print the hash
        printf("hash 1 -> 0x%llx\n", h1);

        // checkpoint
        checkpoint(p_tuple, "after hash 1");
    }

    // #5 - destroy
    {

        // destroy the tuple
        tuple_destroy(&p_tuple, NULL);

        // checkpoint
        checkpoint(p_tuple, "after destroy");
    }

    // #6 - from binary
    {
        
        // initialized data
        stream *p_stream = NULL;
        
        // read a buffer from a file
        stream_from_path(&p_stream, "resources/reflection/tuple.bin");
        
        // reflect an tuple from the buffer
        tuple_unpack(&p_tuple, p_stream, string_unpack);
        
        // close the file
        stream_destroy(&p_stream);

        // checkpoint
        checkpoint(p_tuple, "after parse");
    }

    // #7 - hash
    {

        // initialized data
        h2 = tuple_hash(p_tuple, string_hash);

        // print the hash
        printf("hash 2 -> 0x%llx\n", h2);

        // error check
        if ( h1 != h2 ) log_error("Error: hash 1 != hash 2\n");

        // checkpoint
        checkpoint(p_tuple, "after hash 2");
    }

    // #8 - destroy
    {

        // destroy the tuple
        tuple_destroy(&p_tuple, default_allocator);

        // checkpoint
        checkpoint(p_tuple, "after destroy");
    }

    // #9 - end
    checkpoint(p_tuple, "end");
    
    // success
    return EXIT_SUCCESS;
}

int checkpoint ( tuple *p_tuple, const char *p_event )
{

    // static data
    static int step = 0;

    // print the tuple
    if ( NULL == p_tuple )
        log_info("#%d - Tuple %s: ", step, p_event),
        printf("NULL\n");
    else
    {

        // initialized data
        size_t i = 0;

        // logs
        log_info("#%d - Tuple %s:\n", step, p_event);

        // iterate through the tuple
        for ( iterator it = tuple_iterator(p_tuple); !it.done(&it); it.next(&it) )
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

void string_print ( void *p_value, int i )
{
    
    // print the element
    printf("[%d] - %s\n", i, (char *)p_value);
    
    // done
    return;
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
