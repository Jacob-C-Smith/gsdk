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

// forward declarations
void parallel_quit ( void );

// Data
static bool initialized = false;

void parallel_init ( void ) 
{

    // State check
    if ( initialized == true ) return;

    // Initialize the log library
    log_init();

    // Initialize the sync library
    sync_init();

    // Initialize the array library
    array_init();

    // Initialize the json library
    json_init();

    // Construct a dictionary for parallel tasks
    dict_construct(&parallel_parallel_tasks, 256, 0);

    // Set the initialized flag
    initialized = true;

    // Done
    return;
}

int parallel_register_task ( const char *const name, fn_parallel_task *pfn_parallel_task )
{

    // TODO: Argument check

    // Store the task
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

    // State check
    if ( initialized == false ) return;

    // Clean up the log library
    log_exit();

    // Clean up the sync library
    sync_exit();

    // Clean up the array library
    array_exit();

    // Clean up the json library
    json_exit();

    // Destroy the task registery
    dict_destroy(&parallel_parallel_tasks);

    // Clear the initialized flag
    initialized = false;

    // Done
    return;
}