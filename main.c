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
#include <core/interfaces.h>
#include <core/log.h>
#include <core/pack.h>
#include <core/stream.h>

/// data
#include <data/array.h>
#include <data/bitmap.h>
#include <data/tuple.h>

/** !
 * Pack an array of bitmaps to a stream
 * 
 * @param p_stream the stream
 * @param p_array  the array to pack
 * 
 * @return bytes written on success, 0 on error
 */
int pack_array_of_bitmaps ( stream *p_stream, array *p_array )
{

    // done
    return array_pack(p_stream, p_array, (fn_pack *) bitmap_pack);
}

// entry point
int main ( int argc, const char *argv[] )
{
 
    // unused
    (void) argc, (void) argv;

    // initialized data
    stream *p_stream  = NULL;
    tuple  *p_tuple   = NULL;
    array  *_p_a[3]   = { 0 };
    bitmap *_p_b[9]   = { 0 };
    
    // construct a stream
    stream_from_path(&p_stream, "resources/reflection/tuple_of_arrays_of_bitmaps.bin");

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
    tuple_pack(p_stream, p_tuple, (fn_pack *) pack_array_of_bitmaps);

    // destroy the stream
    stream_destroy(&p_stream);
    
    // log
    printf(
        "The data structure has been written to "\
        "\"resources/reflection/tuple_of_arrays_of_bitmaps.bin\""
        "\n"
    );

    // success
    return EXIT_SUCCESS;
}
