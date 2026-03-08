/** !
 * Bitmap interface
 * 
 * @file src/data/bitmap/bitmap.h 
 * 
 * @author Jacob Smith
 */

// header guard
#pragma once

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

// gsdk
/// core
#include <core/hash.h>
#include <core/interfaces.h>
#include <core/log.h>
#include <core/pack.h>
#include <core/sync.h>

// forward declarations
struct bitmap_s;

// type definitions
typedef struct bitmap_s bitmap;

// function declarations
/// constructors
/** !
 * Allocate a bitmap
 * 
 * @param pp_bitmap result
 * @param bits      the quantity of bits in the bitmap
 * 
 * @return 1 on success, 0 on error
 */
int bitmap_construct ( bitmap **pp_bitmap, size_t bits );

/// accessors
/** !
 * Get the i'th bit of a bitmap
 * 
 * @param p_bitmap the bitmap
 * @param i        the index of the bit to set
 * 
 * @return 1 IF set ELSE 0
 */
int bitmap_test ( bitmap *p_bitmap, size_t i );

/// mutators
/** !
 * Set the i'th bit of a bitmap
 * 
 * @param p_bitmap the bitmap
 * @param i        the index of the bit to set
 * 
 * @return 1 on success, 0 on error
 */
int bitmap_set ( bitmap *p_bitmap, size_t i );

/** !
 * Clear the i'th bit of a bitmap
 * 
 * @param p_bitmap the bitmap
 * @param i        the index of the bit to clear
 * 
 * @return 1 on success, 0 on error
 */
int bitmap_clear ( bitmap *p_bitmap, size_t i );

/// print
/** !
 * Print a bitmap to standard out
 * 
 * @param p_bitmap the bitmap
 * 
 * @return 1 on success, 0 on error
 */
int bitmap_print ( bitmap *p_bitmap );

/// iterators
/** !
 * Call a function on every bit in a bitmap
 *
 * @param p_bitmap the bitmap
 * @param pfn_fori pointer to fori function
 * 
 * @return 1 on success, 0 on error
 */
int bitmap_fori ( bitmap *p_bitmap, fn_fori *pfn_fori );

/** !
 * Call a function on every bit in a bitmap
 *
 * @param p_bitmap    the bitmap
 * @param pfn_foreach pointer to foreach function
 * 
 * @return 1 on success, 0 on error
 */
int bitmap_foreach ( bitmap *p_bitmap, fn_foreach *pfn_foreach );

/// reflection
/** !
 * Pack a bitmap into a buffer
 * 
 * @param p_buffer the buffer
 * @param p_bitmap the bitmap
 * 
 * @return 1 on success, 0 on error
 */
int bitmap_pack ( void *p_buffer, bitmap *p_bitmap );

/** !
 * Unpack a buffer into a bitmap
 * 
 * @param pp_bitmap result
 * @param p_buffer  the buffer
 * 
 * @return 1 on success, 0 on error
 */
int bitmap_unpack ( bitmap **pp_bitmap, void *p_buffer );

/// hash
/** !
 * Compute a 64-bit hash of a bitmap
 * 
 * @param p_bitmap    the bitmap
 * @param pfn_element pointer to hashing function
 * 
 * @return hash on success, 0 on error
 */
hash64 bitmap_hash ( bitmap *p_bitmap, fn_hash64 *pfn_hash64 );

/// destructors
/** !
 * Release a bitmap
 * 
 * @param pp_bitmap pointer to bitmap pointer
 * 
 * @return 1 on success, 0 on error
 */
int bitmap_destroy ( bitmap **pp_bitmap );
