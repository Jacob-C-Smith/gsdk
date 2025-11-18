/** !
 * High level abstraction of a thread pool
 * 
 * @file thread_pool.c
 *
 * @author Jacob Smith
 */

// header
#include <performance/thread_pool.h>

// preprocessor definitions
#define PARALLEL_THREAD_POOL_NAME_LENGTH        (63 + 1)
#define PARALLEL_THREAD_POOL_THREAD_NAME_LENGTH (63 + 1)
#define PARALLEL_THREAD_POOL_TASK_NAME_LENGTH   (63 + 1)
#define PARALLEL_THREAD_POOL_MAX_THREADS        64
#define PARALLEL_THREAD_POOL_MAX_TASKS          256

// forward declarations
struct thread_pool_thread_s;
struct thread_pool_work_parameter_s;

// type definitions
typedef struct thread_pool_thread_s         thread_pool_thread;
typedef struct thread_pool_work_parameter_s thread_pool_work_parameter;

// structure definitions
struct thread_pool_thread_s
{
    bool              running;
    monitor           _montior;
    void             *ret;
    void             *p_parameter;
    fn_parallel_task *pfn_parallel_task;
    parallel_thread  *p_parallel_thread;

};

struct thread_pool_work_parameter_s
{
    thread_pool        *p_thread_pool;
    thread_pool_thread  _thread;
};

struct thread_pool_s
{
    monitor _montior;
    mutex _lock;
    size_t thread_quantity;
    size_t running_threads;
    thread_pool_work_parameter _threads[];
};

// function declarations
/** !
 * Allocate memory for a scheudle thread
 * 
 * @param pp_thread_pool_thread return
 * 
 * @sa parallel_thread_pool_destroy
 * 
 * @return 1 on success, 0 on error
*/
int parallel_thread_pool_thread_create ( thread_pool_thread **pp_thread_pool_thread, size_t task_quantity );

/** !
 * Construct a named thread from a json value
 * 
 * @param pp_thread return
 * @param name      the name
 * @param value     the json value
 * 
 * @return 1 on success, 0 on error
 */
int parallel_thread_pool_thread_load_as_json_value ( thread_pool_thread **const pp_thread, const char *const name, const json_value *const p_value );

/** !
 * Worker thread loop
 * 
 * @param p_parameter who am I?
 * 
 * @return ret
 */
void *parallel_thread_pool_work ( thread_pool_work_parameter *p_parameter );

/** !
 * Main thread loop
 * 
 * @param p_parameter who am I?
 * 
 * @return ret
 */
void *parallel_thread_pool_main_work ( thread_pool_work_parameter *p_parameter );

// TODO: Document
int thread_pool_thread_destroy ( thread_pool_thread **pp_thread );

/** !
 * Start a worker thread
 * 
 * @param p_parameter 
 * 
 * @return 
 */
void *thread_pool_work ( thread_pool_work_parameter *p_parameter );

// function definitions
int thread_pool_create ( thread_pool **const pp_thread_pool )
{

    // argument check
    if ( pp_thread_pool == (void *) 0 ) goto no_thread_pool;

    // initialized data
    thread_pool *p_thread_pool = (void *) 0;

    // allocate memory for the thread_pool
    p_thread_pool = default_allocator(0, sizeof(thread_pool));

    // error check
    if ( p_thread_pool == (void *) 0 ) goto no_mem;

    // Zero set memory
    memset(p_thread_pool, 0, sizeof(thread_pool));

    // return a pointer to the caller
    *pp_thread_pool = p_thread_pool;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_thread_pool: 
                #ifndef NDEBUG
                    log_error("[parallel] [thread pool] Null pointer provided for parameter \"pp_thread_pool\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
        
        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // error
                return 0;
        }
    }
}

int thread_pool_construct ( thread_pool **pp_thread_pool, size_t thread_quantity )
{

    // argument check
    if ( pp_thread_pool  ==                       (void *) 0 ) goto no_thread_pool;
    if ( thread_quantity ==                                0 ) goto no_thread_quantity;
    if ( thread_quantity >  PARALLEL_THREAD_POOL_MAX_THREADS ) goto too_many_threads;
    
    // initialized data
    thread_pool *p_thread_pool = (void *) 0;

    // Construct a thread pool
    if ( thread_pool_create(&p_thread_pool) == 0 ) goto failed_to_create_thread_pool;

    // Grow the allocation
    p_thread_pool = default_allocator(p_thread_pool, sizeof(thread_pool) * ( thread_quantity * sizeof(thread_pool_work_parameter) ));

    // Initialize data
    memset(p_thread_pool, 0, sizeof(thread_pool) * ( thread_quantity * sizeof(thread_pool_work_parameter) ));

    // error check
    if ( p_thread_pool == (void *) 0 ) goto no_mem;

    // store the quantity of threads
    *p_thread_pool = (thread_pool)
    {
        .thread_quantity = thread_quantity,
        ._montior = { 0 }
    };

    // Construct a monitor
    monitor_create(&p_thread_pool->_montior);

    // Construct threads
    for (size_t i = 0; i < thread_quantity; i++)
    {
    
        // store the thread pool in the parameter
        p_thread_pool->_threads[i].p_thread_pool = p_thread_pool;

        // Construct a monitor
        monitor_create(&p_thread_pool->_threads[i]._thread._montior);

        // Construct a thread
        if ( parallel_thread_start(&p_thread_pool->_threads[i]._thread.p_parallel_thread, (fn_parallel_task *)thread_pool_work, &p_thread_pool->_threads[i]) == 0 ) goto failed_to_start_thread;
    }

    // Wait for all the threads to start
    while ( p_thread_pool->running_threads != thread_quantity )
        sleep(0);
    
    // Start the worker threads
    monitor_notify_all(&p_thread_pool->_montior);

    // return a pointer to the caller
    *pp_thread_pool = p_thread_pool;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_thread_pool:
                #ifndef NDEBUG
                    log_error("[parallel] [thread pool] Null pointer provided for parameter \"pp_thread_pool\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_thread_quantity:
                #ifndef NDEBUG
                    log_error("[parallel] [thread pool] Null pointer provided for parameter \"thread_quantity\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            too_many_threads:
                #ifndef NDEBUG
                    log_error("[parallel] [thread pool] Parameter \"thread_quantity\" must be less than %d in call to function \"%s\"\n", PARALLEL_THREAD_POOL_MAX_THREADS, __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Parallel errors
        {
            failed_to_create_thread_pool:
                #ifndef NDEBUG
                    log_error("[parallel] [thread pool] Failed to create thread pool in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            failed_to_start_thread:
                #ifndef NDEBUG
                    log_error("[parallel] [thread pool] Failed to create thread in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // error
                return 0;
        }
    }
}

int thread_pool_execute ( thread_pool *p_thread_pool, fn_parallel_task *pfn_parallel_task, void *p_parameter )
{

    // argument check
    if ( p_thread_pool     == (void *) 0 ) goto no_thread_pool;
    if ( pfn_parallel_task == (void *) 0 ) goto no_parallel_task;

    // initialized data
    size_t i = 0;

    try_again:

    // lock
    mutex_lock(&p_thread_pool->_lock);

    // Defer to other threads
    sleep(0);

    // Find an idle thread
    for (i = 0; i < p_thread_pool->thread_quantity; i++)
    {   

        // If this thread is running, try the next one
        if ( p_thread_pool->_threads[i]._thread.running ) continue;

        // Found one
        goto found_thread;
    }

    // unlock
    mutex_unlock(&p_thread_pool->_lock);

    // Defer to other threads
    sleep(0);

    // Find the idle thread
    goto try_again;

    found_thread:
 
    // Set up the task
    p_thread_pool->_threads[i]._thread.pfn_parallel_task = pfn_parallel_task;
    p_thread_pool->_threads[i]._thread.p_parameter       = p_parameter;
    p_thread_pool->_threads[i]._thread.running           = true;

    sleep(0);

    // Signal the thread
    monitor_notify(&p_thread_pool->_threads[i]._thread._montior);

    // unlock
    mutex_unlock(&p_thread_pool->_lock);
    
    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_thread_pool:
                #ifndef NDEBUG
                    log_error("[parallel] [thread pool] Null pointer provided for parameter \"p_thread_pool\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_parallel_task:
                #ifndef NDEBUG
                    log_error("[parallel] [thread pool] Null pointer provided for parameter \"pfn_parallel_task\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int thread_pool_wait_idle ( thread_pool *p_thread_pool )
{

    // argument check
    if ( p_thread_pool == (void *) 0 ) goto no_thread_pool;

    // initialized data
    bool is_running = true;

    // Until the thread pool is idle
    while ( is_running )
    {

        is_running = false;

        // For all threads in the thread pool ...
        for (size_t i = 0; i < p_thread_pool->thread_quantity; i++)
        
            // ... if a thread is running set the flag ...
            is_running |= p_thread_pool->_threads[i]._thread.running;
        
        // ... defer to other threads 
        sleep(0);
    };

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_thread_pool:
                #ifndef NDEBUG
                    log_error("[parallel] [thread pool] Null pointer provided for parameter \"p_thread_pool\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

void *thread_pool_work ( thread_pool_work_parameter *p_parameter )
{

    // argument check
    if ( p_parameter == (void *) 0 ) goto no_work_parameter;

    // initialized data
    thread_pool *p_thread_pool = p_parameter->p_thread_pool;

    // lock
    mutex_lock(&p_thread_pool->_lock);

    // Increment the quantity of running threads
    p_thread_pool->running_threads++;

    // unlock
    mutex_unlock(&p_thread_pool->_lock);

    // Wait for the thread pool to start
    monitor_wait(&p_thread_pool->_montior);

    wait_for_next_task:

    p_parameter->_thread.running = false;

    // Wait for a task to be assigned
    monitor_wait(&p_parameter->_thread._montior);

    p_parameter->_thread.running = true;

    // Run the user's task
    p_parameter->_thread.ret = p_parameter->_thread.pfn_parallel_task(p_parameter->_thread.p_parameter);

    p_parameter->_thread.running = false;

    sleep(0);

    monitor_notify(&p_thread_pool->_montior);

    // Wait for the next task
    goto wait_for_next_task;    

    // success
    return (void *) 1;

    // error handling
    {

        // argument errors
        {
            no_work_parameter:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] Null pointer provided for parameter \"p_parameter\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}
