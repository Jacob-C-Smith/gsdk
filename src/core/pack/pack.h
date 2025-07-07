/** !
 * Include header for pack library
 * 
 * @file pack/pack.h 
 * 
 * @author Jacob Smith
 */

// header guard
#pragma once

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <limits.h>

// function declarations
/// pack
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

/// unpack
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
