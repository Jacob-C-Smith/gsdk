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

// performance
#include <performance/parallel.h>

// structure definitions
struct parallel_thread_s
{

    // Platform dependent struct members
    #ifdef _WIN64
        // TODO
    #else
        pthread_t platform_dependent_thread;
    #endif
};

// Constructors
/** !
 *  Allocate a parallel thread
 *
 * @param p_thread return
 * 
 * @sa parallel_thread_start
 *
 * @return 1 on success, 0 on error
 */
int parallel_thread_create ( parallel_thread **pp_parallel_thread );

// Start
/** !
 * Start a new parallel thread
 * 
 * @param pp_parallel_thread return
 * @param pfn_task           pointer to start function
 * @param p_parameter        parameter for start function
 * 
 * @sa parallel_thread_join
 * @sa parallel_thread_destroy
 * 
 * @return 1 on success, 0 on error
 */
int parallel_thread_start ( parallel_thread **pp_parallel_thread, fn_parallel_task *pfn_task, void *p_parameter );

// Cancel
/** !
 * Stop a thread
 * 
 * @param p_parallel_thread the thread
 * 
 * @return 1 on success, 0 on error
 */
int parallel_thread_cancel ( parallel_thread *p_parallel_thread );

// Stop
/** !
 *  Wait for a thread to end, then destory it
 *
 * @param pp_parallel_thread pointer to a parallel thread pointer
 *
 * @sa parallel_thread_create
 *
 * @return 1 on success, 0 on error
 */
int parallel_thread_join ( parallel_thread **pp_parallel_thread );

// Destructors
/** !
 *  Destroy a thread
 *
 * @param pp_parallel_thread pointer to a parallel thread pointer
 *
 * @sa parallel_thread_create
 *
 * @return 1 on success, 0 on error
 */
int parallel_thread_destory ( parallel_thread **pp_parallel_thread );
