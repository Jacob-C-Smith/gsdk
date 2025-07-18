/** !
 * Example program for bitmap module
 * 
 * @file main.c
 * 
 * @author Jacob Smith
 */

// standard library
#include <stdio.h>
#include <stdlib.h>

// Include
#include <data/bitmap.h>

// entry point
int main ( int argc, const char* argv[] )
{

    // unused
    (void) argc;
    (void) argv;

    // initialized data
    bitmap *p_bitmap = (void *) 0;
    char  *buf[1024] = { 0 };
    FILE  *p_f = fopen("resources/reflection/bitmap.bin", "wb");
    size_t len = 0;

    // construct an bitmap
    bitmap_construct(&p_bitmap, 64);

    // set some bits
    for (size_t i = 0; i < 64; i = i + 4)
        bitmap_set(p_bitmap, i);

    // clear some bits
    for (size_t i = 0; i < 64; i = i + 8)
        bitmap_clear(p_bitmap, i);

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

    // success
    return EXIT_SUCCESS;
}
