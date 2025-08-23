/** !
 * Example program for tuple module
 * 
 * @file main.c
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
void    string_print ( void *p_value, int i );
int     string_compare ( const void *const p_a, const void *const p_b );
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
        tuple_slice(p_tuple, (void**)slice_of_tuple, 1, 2);

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
        char buf[1024] = { 0 };
        
        // Open a file for writing
        p_f = fopen("resources/reflection/tuple.bin", "wb");

        // reflect the tuple to a buffer
        file_len = tuple_pack(buf, p_tuple, string_pack),
        
        // write the buffer to a file
        fwrite(buf, file_len, 1, p_f),

        // close the file
        fclose(p_f);

        // checkpoint
        checkpoint(p_tuple, "after serialize");
    }

    // #4 - hash 1
    {

        // initialized data
        h1 = tuple_hash(p_tuple, (fn_hash64 *)string_hash);

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
        char buf[1024] = { 0 };
        
        // read a buffer from a file
        p_f = fopen("resources/reflection/tuple.bin", "rb"),
        fread(buf, file_len, 1, p_f),
        
        // reflect an tuple from the buffer
        tuple_unpack(&p_tuple, buf, string_unpack),

        // checkpoint
        checkpoint(p_tuple, "after parse");
    }

    // #7 - hash
    {

        // initialized data
        h2 = tuple_hash(p_tuple, (fn_hash64 *)string_hash);

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
        tuple_destroy(&p_tuple, NULL);

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
        log_info("#%d - Tuple %s:\n", step, p_event),
        tuple_fori(p_tuple, string_print),
        putchar('\n');

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
    return ;
}

int string_compare ( const void *const p_a, const void *const p_b )
{
    char *a = *(char **)p_a,
         *b = *(char **)p_b;

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
