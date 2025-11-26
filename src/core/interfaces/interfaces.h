/** !
 * Interfaces used throughout the gsdk
 * 
 * @file core/interfaces.h
 * 
 * @author Jacob Smith
 */

// header guard
#pragma once

// standard library
#include <stdio.h>
#include <stdlib.h>

// type definitions
typedef unsigned long long hash64;

/// allocator
typedef void *(fn_allocator) ( void *p_pointer, unsigned long size );

/// identity
typedef int   (fn_comparator)   ( const void *p_a, const void *p_b );
typedef int   (fn_equality)     ( const void *p_a, const void *p_b );
typedef void *(fn_key_accessor) ( const void *const  p_value );

/// iterators
typedef void  (fn_fori)    ( void *p_element, int i );
typedef void  (fn_foreach) ( void *p_element );
typedef void *(fn_map)     ( void *p_element );

/// reflection
typedef int    (fn_pack)   ( void              *p_buffer, const void *const   p_value );
typedef int    (fn_unpack) ( void              *p_value , void               *p_buffer );
typedef hash64 (fn_hash64) ( const void *const  k       , unsigned long long  l );

// function declarations   
/** !
 * Realloc wrapper
 * 
 * @param p_pointer pointer to existing allocation IF not NULL ELSE allocate
 * @param size      size of allocation IF not NULL else FREE
 * 
 * @return pointer to heap memory IF (re)allocated ELSE NULL
 */
void *default_allocator ( void *p_pointer, unsigned long long size );

/** !
 * B - A
 * 
 * @param p_a A
 * @param p_b B
 * 
 * @return 0 IF A == B ELSE -1 IF A < B ELSE 1
 */
int default_comparator ( const void *p_a, const void *p_b );

/** !
 * A == B
 * 
 * @param p_a A
 * @param p_b B
 * 
 * @return 1 IF A == B ELSE 0
 */
int default_equality ( const void *const p_a, const void *const p_b );

/** !
 * Returns the value
 * 
 * @param p_value the value
 * 
 * @return the value
 */
void *default_key_accessor ( const void *const p_value );

/** !
 * Fallback hashing function 
 * 
 * @param k the input
 * @param l the length
 * 
 * @return a 64-bit hash, computed from k to (k + l)
 */
hash64 default_hash ( const void *const k, unsigned long long l );
