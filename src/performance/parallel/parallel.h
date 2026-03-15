/** !
 * Parallel interface
 * 
 * @file src/performance/parallel/parallel.h
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

// gsdk
/// core
#include <core/sync.h>
#include <core/log.h>

/// data
#include <data/dict.h>
#include <data/array.h>

/// reflection
#include <reflection/json.h>

// structure declarations
struct parallel_task_s;
struct parallel_thread_s;
struct thread_pool_s;
struct schedule_s;

// type definitions
typedef struct   parallel_task_s parallel_task;
typedef struct   parallel_thread_s parallel_thread;
typedef struct   thread_pool_s thread_pool;
typedef struct   schedule_s schedule;
typedef void   *(fn_parallel_task)(void *p_parameter);

/// initializers
/** !
 * This gets called at runtime before main. 
 * 
 * @param void
 * 
 * @return void
 */
void parallel_init ( void ) __attribute__((constructor));

/// register
/** !
 * Register a named task
 * 
 * @param p_name   the name of the task
 * @param pfn_task pointer to task function
 * 
 * @sa parallel_schedule_unregister_task
 * 
 * @return 1 on success, 0 on error
 */
int parallel_register_task ( const char *const p_name, fn_parallel_task *pfn_task );

/// unregister
/** !
 * Find a named task in the task registery
 * 
 * @param p_name    the name of the task
 * @param ppfn_task result
 * 
 * @return 1 on success, 0 on error
 */
int parallel_find_task ( const char *const p_name, fn_parallel_task **ppfn_task );

/// cleanup
/** !
 * This gets called at runtime after main
 * 
 * @param void
 * 
 * @return void
 */
void parallel_exit ( void ) __attribute__((destructor));
