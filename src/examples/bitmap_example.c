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

// core
#include <core/log.h>
#include <core/sync.h>
#include <core/hash.h>
#include <core/pack.h>

// data
#include <data/bitmap.h>

// forward declarations
/// logs
int checkpoint ( bitmap *p_bitmap, const char *p_event );

/// bit
void bit_print ( void *p_value, int i );

/// file for reflection
FILE *p_f = NULL;

/// hashes
hash64 h1 = 0,
       h2 = 0;

/// working bitmap
bitmap *p_bitmap = NULL;
size_t file_len = 0;

// entry point
int main ( int argc, const char* argv[] )
{

    // unused
    (void) argc;
    (void) argv;

    // #0 - start
    checkpoint(p_bitmap, "start");

    // #1 - initial
    {

        // construct the bitmap
        bitmap_construct(&p_bitmap, 64);

        // checkpoint
        checkpoint(p_bitmap, "after construction");
    }

    // #2 - set / clear some bits
    {

        // set some bits
        for (size_t i = 0; i < 64; i = i + 4)
            bitmap_set(p_bitmap, i);
            
        // clear some bits
        for (size_t i = 0; i < 64; i = i + 8)
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
        char buf[1024] = { 0 };
        
        // Open a file for writing
        p_f = fopen("resources/reflection/bitmap.bin", "wb");

        // reflect the array to a buffer
        file_len = bitmap_pack(buf, p_bitmap),
        
        // write the buffer to a file
        fwrite(buf, file_len, 1, p_f),

        // close the file
        fclose(p_f);

        // checkpoint
        checkpoint(p_bitmap, "after serialize");
    }

    // #5 - hash 1
    {

        // initialized data
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
            if ( bitmap_test(p_bitmap, i) )
                bitmap_clear(p_bitmap, i);
            else
                bitmap_set(p_bitmap, i);

        // checkpoint
        checkpoint(p_bitmap, "after flip");
    }

    /*
    // initialized data
    bitmap *p_bitmap = (void *) 0;
    char  *buf[1024] = { 0 };
    FILE  *p_f = fopen("resources/reflection/bitmap.bin", "wb");
    size_t len = 0;

    // construct an bitmap
    


    // test some bits
    printf("[%02d] --> %c\n", 4 , bitmap_test(p_bitmap, 4)  ? '1' : '0'),
    printf("[%02d] --> %c\n", 63, bitmap_test(p_bitmap, 63) ? '1' : '0');
    
    // reflection
    {

        // reflect the bitmap to a buffer
        len = bitmap_pack(buf, p_bitmap);
        
        // write the buffer to a file
        fwrite(buf, len, 1, p_f),
        
        // close the file
        fclose(p_f),
        
        // read a buffer from a file
        p_f = fopen("resources/reflection/bitmap.bin", "rb"),
        memset(buf, 0, sizeof(buf)),
        fread(buf, len, 1, p_f),
        
        // reflect an bitmap from the buffer
        bitmap_unpack(&p_bitmap, buf);
    }

    // print the bitmaps' elements
    bitmap_print(p_bitmap);

    // destroy the bitmap
    bitmap_destroy(&p_bitmap);
    */

    // success
    return EXIT_SUCCESS;
}

int checkpoint ( bitmap *p_bitmap, const char *p_event )
{

    // static data
    static int step = 0;

    // print the bitmap
    if ( NULL == p_bitmap )
        log_info("#%d - Bitmap %s: ", step, p_event),
        printf("NULL\n");
    else
        log_info("#%d - Bitmap %s:\n", step, p_event),
        //bitmap_fori(p_bitmap, bit_print),
        putchar('\n');

    // increment counter
    step++;

    // success
    return 1;
}

void bit_print ( void *p_value, int i )
{

    // print the element
    printf("%d", p_value ? 1 : 0);

    // done
    return;
}