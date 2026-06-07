/** !
 * Example program for bitmap module
 * 
 * @file src/examples/bitmap_example.c
 * 
 * @author Jacob Smith
 */

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// gsdk
/// core
#include <core/log.h>
#include <core/sync.h>
#include <core/hash.h>
#include <core/pack.h>
#include <core/stream.h>
#include <core/interfaces.h>

/// data
#include <data/bitmap.h>

// forward declarations
/// logs
int checkpoint ( bitmap *p_bitmap, const char *p_event );

/// bit
fn_fori bit_print;

/// hashes
hash64 h1 = 0,
       h2 = 0;

/// working bitmap
bitmap *p_bitmap = NULL;

// entry point
int main ( int argc, const char* argv[] )
{

    // unused
    (void) argc;
    (void) argv;

    // seed the random number generator
    srand((unsigned) time(NULL));

    // #0 - start
    checkpoint(p_bitmap, "start");

    // #1 - construct 
    {

        // construct the bitmap
        bitmap_construct(&p_bitmap, 64);

        // checkpoint
        checkpoint(p_bitmap, "after construction");
    }

    // #2 - set / clear some bits
    {

        // set some bits
        for (size_t i = 0; i < 64; i = i + 3)
            bitmap_set(p_bitmap, i);
            
        // clear some bits
        for (size_t i = 0; i < 64; i = i + 4)
            bitmap_clear(p_bitmap, i);

        // checkpoint
        checkpoint(p_bitmap, "after setting / clearing bits");
    }

    // #3 - test a random bit
    {

        // initialized data
        int r = rand() % 64;
        int result = -1;

        // test the r'th bit
        result = bitmap_test(p_bitmap, r);

        // print the r'th bit
        printf("bit %d = %d\n", r, result);

        // checkpoint
        checkpoint(p_bitmap, "after testing bit");
    }

    // #4 - to binary
    {
        
        // initialized data
        stream *p_stream = NULL;
        
        // construct a stream
        stream_from_path(&p_stream, "resources/reflection/bitmap.bin");

        // reflect the bitmap to a buffer
        bitmap_pack(p_stream, p_bitmap);
        
        // close the file
        stream_destroy(&p_stream);

        // checkpoint
        checkpoint(p_bitmap, "after serialize");
    }

    // #5 - hash 1
    {

        // compute the hash
        h1 = bitmap_hash(p_bitmap, NULL);

        // print the hash
        printf("hash 1 -> 0x%llx\n", h1);

        // checkpoint
        checkpoint(p_bitmap, "after hash 1");
    }

    // #6 - invert bits
    {

        // invert all bits
        for (size_t i = 0; i < 64; i++)
            ( (bitmap_test(p_bitmap, i) ) ? bitmap_clear : bitmap_set)(p_bitmap, i);

        // checkpoint
        checkpoint(p_bitmap, "after flip");
    }

    // #7 - destroy
    {
        
        // destroy the bitmap
        bitmap_destroy(&p_bitmap);

        // checkpoint
        checkpoint(p_bitmap, "after destroy");
    }

    // #8 - from binary
    {
        
        // initialized data
        stream *p_stream = NULL;
        
        // construct a stream
        stream_from_path(&p_stream, "resources/reflection/bitmap.bin");
        
        // reflect a bitmap from the buffer
        bitmap_unpack(&p_bitmap, p_stream);

        // close the file
        stream_destroy(&p_stream);

        // checkpoint
        checkpoint(p_bitmap, "after parse");
    }

    // #9 - hash 2
    {

        // compute the hash
        h2 = bitmap_hash(p_bitmap, NULL);

        // print the hash
        printf("hash 2 -> 0x%llx\n", h2);

        // error check
        if ( h1 != h2 )

            // abort
            log_error("Error: hash 1 != hash 2\n"), exit(EXIT_FAILURE);

        // checkpoint
        checkpoint(p_bitmap, "after hash 2");
    }

    // #10 - destroy
    {

        // destroy the bitmap
        bitmap_destroy(&p_bitmap);

        // checkpoint
        checkpoint(p_bitmap, "after destroy");
    }

    // #11 - end
    checkpoint(p_bitmap, "end");

    // success
    return EXIT_SUCCESS;
}

int checkpoint ( bitmap *p_bitmap, const char *p_event )
{

    // static data
    static int step = 0;

    // print the bitmap
    if ( NULL == p_bitmap )
    {
        log_info("#%d - Bitmap %s: ", step, p_event);
        printf("NULL\n");
    }
    else
    {

        // initialized data
        size_t i = 0;

        // logs
        log_info("#%d - Bitmap %s:\n", step, p_event);
        putchar('<');

        // iterate through the bitmap
        for ( iterator it = bitmap_iterator(p_bitmap); !it.done(&it); it.next(&it) )
        {
            bit_print(it.item(&it), i);
            i++;
        }

        // formatting
        putchar('>');
        putchar('\n');
    }

    // increment counter
    step++;

    // success
    return 1;
}

void bit_print ( void *p_value, int i )
{

    // unused
    (void) i;

    // print the element
    printf("%d", p_value ? 1 : 0);

    // done
    return;
}