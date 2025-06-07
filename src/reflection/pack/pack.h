/** !
 * Include header for pack library
 * 
 * @file pack/pack.h 
 * 
 * @author Jacob Smith
 */

// Include guard
#pragma once

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <limits.h>

// Platform dependent macros

// type definitions
typedef int (fn_pack)   ( void *p_buffer, const void *const p_value );
typedef int (fn_unpack) ( void *p_value, void *p_buffer );
typedef struct 
{
    fn_pack   *pfn_pack;
    fn_unpack *pfn_unpack;
} pack_pair;

// Initializer
/** !
 * This gets called at runtime before main. 
 * 
 * @param void
 * 
 * @return void
 */
void pack_init ( void ) __attribute__((constructor));

// Register
/** !
 * Register a pack/unpack pair
 * 
 * @param p_name     the name of the data type
 * @param _pack_pair a pack function, and an unpack function
 * 
 * @return 1 on success, 0 on error
 */
int pack_register ( const char *const p_name, pack_pair _pack_pair );

// Unregister
/** !
 * Unregister a pack/unpack pair
 * 
 * @param p_name the name of the data type
 * 
 * @return 1 on success, 0 on error
 */
int pack_unregister ( const char *const p_name );

// Pack
/** !
 * Pack a registered data type into a buffer
 * 
 * @param type     the name of the data type
 * @param p_buffer the buffer
 * @param p_value  the value
 * 
 * @return bytes written on success, 0 on error
 */
size_t pack ( char *type, void *p_buffer, void *p_value );

/** !
 * Pack data into a buffer
 * 
 * @param p_buffer the buffer
 * @param format   format string
 * @param ...      variadic arguments
 * 
 * @return bytes written on success, 0 on error
 */
size_t pack_pack ( void *p_buffer, const char *restrict format, ... );

// Unpack
/** !
 * Unpack a registered data type from a buffer
 * 
 * @param type     the name of the data type
 * @param p_value  the value
 * @param p_buffer the buffer
 * 
 * @return bytes written on success, 0 on error
 */
size_t unpack ( char *type, void *p_value, void *p_buffer );

/** !
 * Unpack data from a buffer
 * 
 * @param p_buffer the buffer
 * @param format   format string
 * @param ...      variadic arguments
 * 
 * @return quantity of matched fields on success, 0 on error
 */
size_t pack_unpack ( void *p_buffer, const char *restrict format, ... );

// Cleanup
/** !
 * This gets called at runtime after main
 * 
 * @param void
 * 
 * @return void
 */
void pack_exit ( void ) __attribute__((destructor));
