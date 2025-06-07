/** !
 * Include header for bitmap library
 * 
 * @file data/bitmap.h 
 * 
 * @author Jacob Smith
 */

// Include guard
#pragma once

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

// sync submodule
#include <core/sync.h>

// Forward declarations
struct bitmap_s;

// type definitions
typedef struct bitmap_s bitmap;

// Function declarations
/** !
 * Allocate a bitmap
 * 
 * @param pp_bitmap result
 * @param bits      the quantity of bits in the bitmap
 * 
 * @return pointer to bitmap if successful else null pointer
 */
int bitmap_construct ( bitmap **pp_bitmap, size_t bits );

/** !
 * Set the i'th bit of a bitmap
 * 
 * @param p_bitmap the bitmap
 * @param index    the index of the bit to set
 * 
 * @return void 
 */
void bitmap_set ( bitmap *p_bitmap, size_t i );

/** !
 * Clear the i'th bit of a bitmap
 * 
 * @param p_bitmap the bitmap
 * @param index    the index of the bit to clear
 * 
 * @return void 
 */
void bitmap_clear ( bitmap *p_bitmap, size_t i );

/** !
 * Get the i'th bit of a bitmap
 * 
 * @param p_bitmap the bitmap
 * @param index    the index of the bit to set
 * 
 * @return void 
 */
int bitmap_test ( bitmap *p_bitmap, size_t i );

/** !
 * Release a bitmap
 * 
 * @param pp_bitmap pointer to bitmap pointer
 * 
 * @return void
 */
void bitmap_destroy ( bitmap **pp_bitmap );
