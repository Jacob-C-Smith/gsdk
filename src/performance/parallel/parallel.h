/** !
 * High level abstractions for parallelized computing
 * 
 * @file parallel/parallel.h
 *
 * @author Jacob Smith
 */

// header guard
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

// forward declarations
struct parallel_thread_s;
struct thread_pool_s;
struct schedule_s;

// type definitions
typedef struct   parallel_thread_s parallel_thread;
typedef struct   thread_pool_s thread_pool;
typedef struct   schedule_s schedule;
typedef void   *(fn_parallel_task)(void *p_parameter);

// initializers
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

// cleanup
/** !
 * This gets called at runtime after main
 * 
 * @param void
 * 
 * @return void
 */
void parallel_exit ( void ) __attribute__((destructor));
