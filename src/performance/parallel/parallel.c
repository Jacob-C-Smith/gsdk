/** !
 * High level abstractions for parallelized computing
 * 
 * @file parallel.c
 *
 * @author Jacob Smith
 */

// header
#include <performance/parallel.h>

// structure definitions
struct parallel_task_s
{
    char             *p_name;
    fn_parallel_task *pfn_parallel_task;
};

fn_key_accessor parallel_task_key_accessor;

// static data
static dict *parallel_parallel_tasks = 0;
static bool initialized = false;

void parallel_init ( void ) 
{

    // state check
    if ( initialized == true ) return;

    // construct a task registery
    dict_construct(&parallel_parallel_tasks, 256, NULL, parallel_task_key_accessor, NULL);

    // set the initialized flag
    initialized = true;

    // done
    return;
}

int parallel_register_task ( const char *const name, fn_parallel_task *pfn_parallel_task )
{

    // argument check
    if ( NULL ==              name ) goto no_name;
    if ( NULL == pfn_parallel_task ) goto no_task;

    // initialized data
    parallel_task *p_parallel_task = NULL;

    // allocate a parallel task
    p_parallel_task = default_allocator(p_parallel_task, sizeof(p_parallel_task));
    if ( NULL == p_parallel_task ) goto no_mem;

    // populate fields
    *p_parallel_task = (parallel_task)
    {
        .p_name            = strdup(name),
        .pfn_parallel_task = pfn_parallel_task
    };

    // store the task
    dict_add(parallel_parallel_tasks, p_parallel_task);

    // success
    return 1;

    // error handling
    {
        
        // argument errors
        {
            no_name:
                #ifndef NDEBUG
                    log_error("[parallel] Null pointer provided for parameter \"name\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_task:
                #ifndef NDEBUG
                    log_error("[parallel] Null pointer provided for parameter \"pfn_parallel_task\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[parallel] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int parallel_find_task ( const char *const name, fn_parallel_task **p_pfn_parallel_task )
{

    // TODO: Argument check
    fn_parallel_task *r = NULL;
    
    dict_get(parallel_parallel_tasks, name, &r);

    *p_pfn_parallel_task = r;

    // success
    return 1;
}

void *parallel_task_key_accessor ( const void *const p_value )
{

    // initialized data
    parallel_task *p_parallel_task = (parallel_task *)p_value;

    // done
    return p_parallel_task->p_name;
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