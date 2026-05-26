/** !
 * Thread pool implementation
 * 
 * @file src/performance/parallel/thread_pool.c
 *
 * @author Jacob Smith
 */

// header file
#include <performance/thread_pool.h>

// preprocessor definitions
#define PARALLEL_THREAD_POOL_MAX_THREADS 64

// forward declarations
struct thread_pool_thread_s;
struct thread_pool_work_parameter_s;

// type definitions
typedef struct thread_pool_thread_s         thread_pool_thread;
typedef struct thread_pool_work_parameter_s thread_pool_work_parameter;

// structure definitions
struct thread_pool_thread_s
{
    size_t                       index;
    volatile bool                running;
    volatile bool                terminate;
    condition_variable           _ready;
    void                        *ret;
    void                        *p_parameter;
    fn_parallel_task * volatile  pfn_parallel_task;
    parallel_thread             *p_parallel_thread;

};

struct thread_pool_work_parameter_s
{
    thread_pool        *p_thread_pool;
    thread_pool_thread  _thread;
};

struct thread_pool_s
{
    condition_variable   _ready;
    mutex                _lock;
    size_t               thread_quantity;
    volatile size_t      running_threads;
    volatile bool        started;
    volatile bool        terminate;
    thread_pool_work_parameter _threads[];
};

// function declarations
/** !
 * Start a worker thread
 * 
 * @param p_parameter any
 * 
 * @return any
 */
void *thread_pool_work ( thread_pool_work_parameter *p_parameter );

// function definitions
int thread_pool_construct ( thread_pool **pp_thread_pool, size_t thread_quantity )
{

    // argument check
    if ( NULL                             ==  pp_thread_pool ) goto no_thread_pool;
    if ( 0                                == thread_quantity ) goto no_thread_quantity;
    if ( PARALLEL_THREAD_POOL_MAX_THREADS <  thread_quantity ) goto too_many_threads;
    
    // initialized data
    thread_pool *p_thread_pool = NULL;
    size_t       size          = sizeof(thread_pool) + (thread_quantity * sizeof(thread_pool_work_parameter));

    // allocate memory for a thread pool
    p_thread_pool = default_allocator(p_thread_pool, size);
    if ( NULL == p_thread_pool ) goto no_mem;

    // store the quantity of threads
    *p_thread_pool = (thread_pool)
    {
        .thread_quantity = thread_quantity,
        ._ready = { 0 },
        ._lock = { 0 }
    };

    // construct a condition variable
    condition_variable_create(&p_thread_pool->_ready);

    // construct a lock
    mutex_create(&p_thread_pool->_lock);

    // lock
    mutex_lock(&p_thread_pool->_lock);

    // construct threads
    for (size_t i = 0; i < thread_quantity; i++)
    {
    
        // store the thread pool in the parameter
        p_thread_pool->_threads[i].p_thread_pool = p_thread_pool;
        p_thread_pool->_threads[i]._thread.index = i;

        // construct a condition variable
        condition_variable_create(&p_thread_pool->_threads[i]._thread._ready);

        // construct a thread
        if ( 0 == parallel_thread_start(&p_thread_pool->_threads[i]._thread.p_parallel_thread, (fn_parallel_task *)thread_pool_work, &p_thread_pool->_threads[i]) )
        {

            // unlock
            mutex_unlock(&p_thread_pool->_lock);
            goto failed_to_start_thread;
        }
    }

    // wait for threads to start
    while ( p_thread_pool->running_threads != thread_quantity )
        condition_variable_wait(&p_thread_pool->_ready, &p_thread_pool->_lock);
    
    // start the worker threads
    p_thread_pool->started = true;

    // broadcast
    condition_variable_broadcast(&p_thread_pool->_ready);

    // unlock
    mutex_unlock(&p_thread_pool->_lock);

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

        // parallel errors
        {
            failed_to_start_thread:
                #ifndef NDEBUG
                    log_error("[parallel] [thread pool] Failed to create thread in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // destroy the thread pool
                thread_pool_destroy(&p_thread_pool);

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
    if ( NULL ==     p_thread_pool ) goto no_thread_pool;
    if ( NULL == pfn_parallel_task ) goto no_parallel_task;

    // lock
    mutex_lock(&p_thread_pool->_lock);

    // state check
    while ( p_thread_pool->terminate == false )
    {

        // find an idle thread
        for (size_t i = 0; i < p_thread_pool->thread_quantity; i++)
        {   

            // busy?
            if ( p_thread_pool->_threads[i]._thread.pfn_parallel_task || p_thread_pool->_threads[i]._thread.running || p_thread_pool->_threads[i]._thread.terminate ) continue;

            // prepare the task
            p_thread_pool->_threads[i]._thread.pfn_parallel_task = pfn_parallel_task;
            p_thread_pool->_threads[i]._thread.p_parameter       = p_parameter;
            
            // signal
            condition_variable_signal(&p_thread_pool->_threads[i]._thread._ready);

            // unlock
            mutex_unlock(&p_thread_pool->_lock);
            
            // success
            return 1;
        }

        // wait
        condition_variable_wait(&p_thread_pool->_ready, &p_thread_pool->_lock);
    }

    // unlock
    mutex_unlock(&p_thread_pool->_lock);

    // error
    return 0;

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
    if ( NULL == p_thread_pool ) goto no_thread_pool;

    // initialized data
    bool is_running = true;

    // lock
    mutex_lock(&p_thread_pool->_lock);

    // wait
    while ( is_running && p_thread_pool->terminate == false )
    {

        // clear the running flag
        is_running = false;

        // iterate through each thread in the pool
        for (size_t i = 0; i < p_thread_pool->thread_quantity; i++)
            
            // running?  
            is_running |= ( p_thread_pool->_threads[i]._thread.pfn_parallel_task || p_thread_pool->_threads[i]._thread.running );
        
        // wait 
        if ( is_running ) condition_variable_wait(&p_thread_pool->_ready, &p_thread_pool->_lock);
    };

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
        }
    }
}

int thread_pool_thread_destroy ( thread_pool_thread *p_thread )
{

    // argument check
    if ( NULL == p_thread ) goto no_thread;

    // release the thread
    if ( p_thread->p_parallel_thread )
        parallel_thread_join(&p_thread->p_parallel_thread);

    // release the condition variable
    condition_variable_destroy(&p_thread->_ready);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_thread:
                #ifndef NDEBUG
                    log_error("[parallel] [thread pool] Null pointer provided for parameter \"p_thread\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int thread_pool_is_idle ( thread_pool *p_thread_pool )
{

    // argument check
    if ( NULL == p_thread_pool ) goto no_thread_pool;

    // initialized data
    int idle = 1;

    // lock
    mutex_lock(&p_thread_pool->_lock);

    // iterate through each thread in the pool
    for (size_t i = 0; i < p_thread_pool->thread_quantity; i++)

        // busy?
        if ( p_thread_pool->_threads[i]._thread.pfn_parallel_task || p_thread_pool->_threads[i]._thread.running )
        {
            idle = 0;
            break;
        }

    // unlock
    mutex_unlock(&p_thread_pool->_lock);

    // success
    return idle;

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

int thread_pool_destroy ( thread_pool **pp_thread_pool )
{

    // argument check
    if ( NULL == pp_thread_pool ) goto no_thread_pool;

    // initialized data
    thread_pool *p_thread_pool = *pp_thread_pool;

    // edge case
    if ( NULL == p_thread_pool ) return 1;

    // no more pointer for caller
    *pp_thread_pool = NULL;

    // lock
    mutex_lock(&p_thread_pool->_lock);

    // set the terminate flag
    p_thread_pool->terminate = true;

    // iterate through each thread
    for (size_t i = 0; i < p_thread_pool->thread_quantity; i++)
    {

        // set the terminate flag
        p_thread_pool->_threads[i]._thread.terminate = true;

        // broadcast
        condition_variable_broadcast(&p_thread_pool->_threads[i]._thread._ready);
    }

    // broadcast
    condition_variable_broadcast(&p_thread_pool->_ready);

    // unlock
    mutex_unlock(&p_thread_pool->_lock);

    // release the threads
    for (size_t i = 0; i < p_thread_pool->thread_quantity; i++)
        thread_pool_thread_destroy(&p_thread_pool->_threads[i]._thread);

    // release the condition variable
    condition_variable_destroy(&p_thread_pool->_ready);

    // release the lock
    mutex_destroy(&p_thread_pool->_lock);

    // release the thread pool
    p_thread_pool = default_allocator(p_thread_pool, 0);

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

    // increment the quantity of running threads
    p_thread_pool->running_threads++;

    // broadcast
    condition_variable_broadcast(&p_thread_pool->_ready);

    // wait for the thread pool to start
    while ( p_thread_pool->started == false && p_thread_pool->terminate == false )
        condition_variable_wait(&p_thread_pool->_ready, &p_thread_pool->_lock);

    // work loop
    while ( true )
    {

        // set the running flag
        p_parameter->_thread.running = false;

        // broadcast
        condition_variable_broadcast(&p_thread_pool->_ready);

        // wait for a task or a termination signal
        while 
        ( 
            NULL  == p_parameter->_thread.pfn_parallel_task && 
            false == p_parameter->_thread.terminate         && 
            false == p_thread_pool->terminate 
        )

            // wait
            condition_variable_wait(&p_parameter->_thread._ready, &p_thread_pool->_lock);

        // Prioritize executing an assigned task over termination
        if ( p_parameter->_thread.pfn_parallel_task )
        {

            // initialized data
            fn_parallel_task *pfn_parallel_task = p_parameter->_thread.pfn_parallel_task;
            void             *p_task_parameter  = p_parameter->_thread.p_parameter;

            // set the running flag
            p_parameter->_thread.running = true;

            // clear the previous task
            p_parameter->_thread.pfn_parallel_task = NULL;

            // unlock
            mutex_unlock(&p_thread_pool->_lock);

            // run the user's task
            p_parameter->_thread.ret = pfn_parallel_task(p_task_parameter);

            // lock
            mutex_lock(&p_thread_pool->_lock);

            // continue
            continue;
        }

        // terminate?
        if ( p_parameter->_thread.terminate || p_thread_pool->terminate ) break;
    }

    // unlock
    mutex_unlock(&p_thread_pool->_lock);

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
