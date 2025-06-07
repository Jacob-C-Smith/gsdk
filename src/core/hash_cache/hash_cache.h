/** !
 * Include header for hash cache library
 * 
 * @file hash_cache/hash_cache.h 
 * 
 * @author Jacob Smith 
 */

// Include guard
#pragma once

// standard library
#include <stdio.h>
#include <stddef.h>

// sync module
#include <core/sync.h>

// log module
#include <core/log.h>

// Platform dependent macros

// Memory management macro
#ifndef HASH_CACHE_REALLOC
#define HASH_CACHE_REALLOC(p, sz) realloc(p,sz)
#endif

// Function declarations 

// Initializer
/** !
 * This gets called at runtime before main. 
 * 
 * @param void
 * 
 * @return void
 */
void hash_cache_init ( void ) __attribute__((constructor));

// Cleanup
/** !
 * This gets called at runtime after main
 * 
 * @param void
 * 
 * @return void
 */
void hash_cache_exit ( void ) __attribute__((destructor));
