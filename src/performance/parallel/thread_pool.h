/** !
 * Thread pool interface
 * 
 * @file src/performance/parallel/thread_pool.h
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
#include <core/log.h>
#include <core/sync.h>

/// performance
#include <performance/parallel.h>
#include <performance/thread.h>

// structure declarations
struct thread_pool_s;

// type definitions
typedef struct thread_pool_s thread_pool;

// function declarations
/// constructors
/** !
 * Construct a thread pool
 * 
 * @param pp_thread_pool  result
 * @param thread_quantity the quantity of threads
 * 
 * @return 1 on success, 0 on error
 */
int thread_pool_construct ( thread_pool **pp_thread_pool, size_t thread_quantity );

/// executors
/** !
 * Execute a job on a thread pool
 * 
 * @param p_thread_pool     the thread pool
 * @param pfn_parallel_task pointer to job function
 * @param p_parameter       the parameter of the parallel task
 * 
 * @return 1 on success, 0 on error
 */
int thread_pool_execute ( thread_pool *p_thread_pool, fn_parallel_task *pfn_parallel_task, void *p_parameter );

/// accessors
/** !
 * Test if the thread pool is idle
 * 
 * @param p_thread_pool the thread pool
 * 
 * @return 1 on success, 0 on error
 */
bool thread_pool_is_idle ( thread_pool *p_thread_pool );

/// blockers
/** !
 * Block until a thread pool finishes it's active jobs
 * 
 * @param p_thread_pool the thread pool
 * 
 * @return 1 on success, 0 on error
 */
int thread_pool_wait_idle ( thread_pool *p_thread_pool );

/// destructors
/** !
 * Destroy a thread pool
 * 
 * @param pp_thread_pool pointer to thread pool pointer
 * 
 * @return 1 on success, 0 on error
*/
int thread_pool_destroy ( thread_pool **pp_thread_pool );
