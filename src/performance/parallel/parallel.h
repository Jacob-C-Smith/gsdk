/** !
 * High level abstractions for parallelized computing
 * 
 * @file parallel/parallel.h
 *
 * @author Jacob Smith
 */

// Include guard
#pragma once

// standard library
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// core
#include <core/sync.h>
#include <core/log.h>

// data
#include <data/dict.h>
#include <data/array.h>

// reflection
#include <reflection/json.h>

// Platform dependent macros

// Set the reallocator for the dict submodule
#ifdef DICT_REALLOC
    #undef DICT_REALLOC
    #define DICT_REALLOC(p, sz) realloc(p, sz)
#endif

// Set the reallocator for the array submodule
#ifdef ARRAY_REALLOC
    #undef ARRAY_REALLOC
    #define ARRAY_REALLOC(p, sz) realloc(p, sz)
#endif

// Set the reallocator for the json submodule
#ifdef JSON_REALLOC
    #undef JSON_REALLOC
    #define JSON_REALLOC(p, sz) realloc(p, sz)
#endif

// Memory management macro
#ifndef PARALLEL_REALLOC
    #define PARALLEL_REALLOC(p, sz) realloc(p,sz)
    #define PARALLEL_FREE(p) free(p)
#endif

// Forward declarations
struct parallel_thread_s;
struct thread_pool_s;
struct schedule_s;

// type definitions
typedef struct   parallel_thread_s parallel_thread;
typedef struct   thread_pool_s thread_pool;
typedef struct   schedule_s schedule;
typedef void   *(fn_parallel_task)(void *p_parameter);

// Initializers
/** !
 * This gets called at runtime before main. 
 * 
 * @param void
 * 
 * @return void
 */
void parallel_init ( void ) __attribute__((constructor));

/** !
 * Register a task with the scheduler
 * 
 * @param name     the name of the task
 * @param pfn_task pointer to task function
 * 
 * @sa parallel_schedule_unregister_task
 * 
 * @return 1 on success, 0 on error
 */
int parallel_register_task ( const char *const name, fn_parallel_task *pfn_parallel_task );
int parallel_find_task ( const char *const name, fn_parallel_task **p_pfn_parallel_task );

// Cleanup
/** !
 * This gets called at runtime after main
 * 
 * @param void
 * 
 * @return void
 */
void parallel_exit ( void ) __attribute__((destructor));
