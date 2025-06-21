/** !
 * Include header for bitmap library
 * 
 * @file data/bitmap.h 
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

// core
#include <core/log.h>
#include <core/sync.h>
#include <core/pack.h>

// forward declarations
struct bitmap_s;

// type definitions
typedef struct bitmap_s bitmap;

// function declarations
// constructors
/** !
 * Allocate a bitmap
 * 
 * @param pp_bitmap result
 * @param bits      the quantity of bits in the bitmap
 * 
 * @return pointer to bitmap if successful else null pointer
 */
int bitmap_construct ( bitmap **pp_bitmap, size_t bits );

// accessors
/** !
 * Get the i'th bit of a bitmap
 * 
 * @param p_bitmap the bitmap
 * @param index    the index of the bit to set
 * 
 * @return 1 on success, 0 on error
 */
int bitmap_test ( bitmap *p_bitmap, size_t i );

// mutators
/** !
 * Set the i'th bit of a bitmap
 * 
 * @param p_bitmap the bitmap
 * @param index    the index of the bit to set
 * 
 * @return 1 on success, 0 on error
 */
int bitmap_set ( bitmap *p_bitmap, size_t i );

/** !
 * Clear the i'th bit of a bitmap
 * 
 * @param p_bitmap the bitmap
 * @param index    the index of the bit to clear
 * 
 * @return 1 on success, 0 on error
 */
int bitmap_clear ( bitmap *p_bitmap, size_t i );

// print
/** !
 * Print a bitmap
 * 
 * @param p_bitmap the bitmap
 * 
 * return 1 on success, 0 on error
 */
int bitmap_print ( bitmap *p_bitmap );

// reflection
/** !
 * Pack a bitmap into a buffer
 * 
 * @param p_buffer     the buffer
 * @param p_bitmap     the bitmap
 * @param pfn_elemenet pointer to pack function IF not null ELSE default
 * 
 * @return 1 on success, 0 on error
 */
int bitmap_pack ( void *p_buffer, bitmap *p_bitmap );

/** !
 * Unpack a buffer into a bitmap
 * 
 * @param pp_bitmap      result
 * @param p_buffer     the buffer
 * @param pfn_elemenet pointer to unpack function IF not null ELSE default
 * 
 * @return 1 on success, 0 on error
 */
int bitmap_unpack ( bitmap **pp_bitmap, void *p_buffer );

// destructors
/** !
 * Release a bitmap
 * 
 * @param pp_bitmap pointer to bitmap pointer
 * 
 * @return void
 */
int bitmap_destroy ( bitmap **pp_bitmap );
