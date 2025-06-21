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
#include <stdlib.h>

// type definitions
typedef unsigned long long hash64;
typedef struct hash_table_s hash_table;

/// Allocator
typedef void *(fn_allocator) ( void *p_pointer, unsigned long long size );

/// Comparator
typedef int (fn_comparator) ( const void *p_a, const void *p_b );

/// Equality
typedef int    (fn_equality)     ( const void *const p_a, const void *const p_b );
typedef void  *(fn_key_accessor) ( const void *const p_value );

/// Hash
typedef hash64 (fn_hash64) ( const void *const k, unsigned long long l );

/// Indexed iteration
typedef void (fn_fori)         ( void *p_element, int i );
typedef void (fn_fori_context) ( void *p_element, int i, void *p_context );

/// Iteration
typedef void (fn_foreach)         ( void *p_element );
typedef void (fn_foreach_context) ( void *p_element, void *p_context );

// function declarations   
void   *default_allocator     ( void *p_pointer, unsigned long long size );
int     default_comparator    ( const void *p_a, const void *p_b );
int     default_equality      ( const void *const p_a, const void *const p_b );
hash64  default_hash          ( const void *const k, unsigned long long l );
void    *default_key_accessor ( const void *const p_value );
