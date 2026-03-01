/** !
 * GSDK demo program
 * 
 * @file main.c
 * 
 * @author Jacob Smith
 * 
 * This demo packs a tuple containing 3 arrays. Each array contains 3 bitmaps,
 * for a total of 1 tuple, 3 arrays, and 9 bitmaps. This example demonstrates
 * the tremendous flexability of the pack interface.
 */

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// gsdk
/// core
#include <core/log.h>
#include <core/pack.h>

/// data
#include <data/array.h>
#include <data/bitmap.h>
#include <data/tuple.h>

/** !
 * Pack an array of bitmaps.
 * 
 * @param p_buffer result
 * @param p_array  the array to pack
 * 
 * @return bytes written on success, 0 on error
 */
int pack_array_of_bitmaps ( void *p_buffer, array *p_array )
{

    // done
    return array_pack(p_buffer, p_array, (fn_pack *) bitmap_pack);
}

// entry point
int main ( int argc, const char *argv[] )
{
 
    // unused
    (void) argc, (void) argv;

    // initialized data
    tuple  *p_tuple   = NULL;
    array  *_p_a[3]   = { 0 };
    bitmap *_p_b[9]   = { 0 };
    FILE   *p_f       = fopen("resources/reflection/tuple_of_arrays_of_bitmaps.bin", "wb");
    size_t  len       = 0;
    char    buf[4096] = { 0 };

    // construct 9 bitmaps
    for (size_t i = 0; i < 9; i++)
    {

        // construct a bitmap
        bitmap_construct(&_p_b[i], 64);

        // set different bits in different bitmaps
        for (size_t j = 0; j < 64; j = j + i + 1)
            bitmap_set(_p_b[i], j);
    }

    // construct 3 arrays
    for (size_t i = 0; i < 3; i++)
    {

        // construct an array
        array_construct(&_p_a[i], 3);

        // add 3 bitmaps to each array
        array_add(_p_a[i], _p_b[i * 3 + 0]),
        array_add(_p_a[i], _p_b[i * 3 + 1]),
        array_add(_p_a[i], _p_b[i * 3 + 2]);
    }

    // construct a tuple
    tuple_from_elements(&p_tuple, (void *const *)_p_a, 3);
    
    // pack the tuple
    len = tuple_pack(buf, p_tuple, (fn_pack *) pack_array_of_bitmaps);

    // write the buffer to a file
    fwrite(buf, len, 1, p_f);

    // close the file
    fclose(p_f);
    
    // log
    printf(
        "The data structure has been written to "\
        "\"resources/reflection/tuple_of_arrays_of_bitmaps.bin\""
        "\n"
    );

    // success
    return EXIT_SUCCESS;
}
