/** !
 * High level abstractions for parallelized computing
 * 
 * @file parallel.c
 *
 * @author Jacob Smith
 */

// header
#include <performance/parallel.h>

// static data
static dict *parallel_parallel_tasks = 0;
static bool initialized = false;

void parallel_init ( void ) 
{

    // state check
    if ( initialized == true ) return;

    // construct a task registery
    dict_construct(&parallel_parallel_tasks, 256, 0);

    // set the initialized flag
    initialized = true;

    // done
    return;
}

int parallel_register_task ( const char *const name, fn_parallel_task *pfn_parallel_task )
{

    // TODO: Argument check

    // store the task
    dict_add(parallel_parallel_tasks, name, pfn_parallel_task);

    // success
    return 1;
}

int parallel_find_task ( const char *const name, fn_parallel_task **p_pfn_parallel_task )
{

    // TODO: Argument check
    fn_parallel_task *r = dict_get(parallel_parallel_tasks, name);

    *p_pfn_parallel_task = r;

    // success
    return 1;
}

void parallel_exit ( void )
{

    // state check
    if ( initialized == false ) return;

    // destroy the task registery
    dict_destroy(&parallel_parallel_tasks, NULL);

    // Clear the initialized flag
    initialized = false;

    // done
    return;
}