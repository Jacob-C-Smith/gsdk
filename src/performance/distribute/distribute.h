/** !
 * High level abstractions for distributed computing
 *
 * @file distribute/distribute.h
 * 
 * @author Jacob Smith
 */

// header guard
#pragma once

// header files
#include <distribute/connection.h>
#include <json/json.h>
#include <sync/sync.h>
#include <parallel/parallel.h>

// platform dependent macros

// set dict reallocator
#ifdef DICT_REALLOC
    #undef DICT_REALLOC
    #define DICT_REALLOC(p, sz) realloc(p, sz)
#endif

// set array reallocator
#ifdef ARRAY_REALLOC
    #undef ARRAY_REALLOC
    #define ARRAY_REALLOC(p, sz) realloc(p, sz)
#endif

// set json reallocator
#ifdef JSON_REALLOC
    #undef JSON_REALLOC
    #define JSON_REALLOC(p, sz) realloc(p, sz)
#endif

// set parallel reallocator
#ifdef PARALLEL_REALLOC
    #undef PARALLEL_REALLOC
    #define PARALLEL_REALLOC(p, sz) realloc(p, sz)
#endif

// memory management macro
#ifndef DISTRIBUTE_REALLOC
    #define DISTRIBUTE_REALLOC(p, sz) realloc(p,sz)
#endif

// function declarations
/** !
 * Distributed coin flip
 * 
 * @param p_connection
 * 
 * @return 1 on success, 0 on error
 */
// int distribute_coin_flip ( const connection *const p_connection );
