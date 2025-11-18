/** !
 * High level abstraction of a thread
 * 
 * @file thread.c
 *
 * @author Jacob Smith
 */

// parallel
#include <performance/thread.h>

int parallel_thread_create ( parallel_thread **pp_parallel_thread )
{
    
    // argument check
    if ( pp_parallel_thread == (void *) 0 ) goto no_parallel_thread;

    // initialized data
    parallel_thread *p_parallel_thread = default_allocator(0, sizeof(parallel_thread));

    // error check
    if ( p_parallel_thread == (void *) 0 ) goto no_mem;

    // Zero set the struct
    memset(p_parallel_thread, 0, sizeof(parallel_thread));

    // return a pointer to the caller
    *pp_parallel_thread = p_parallel_thread;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_parallel_thread:
                #ifndef NDEBUG
                    log_error("[parallel] [thread] Null pointer provided for parameter \"pp_parallel_thread\" in call to function \"%s\"\n", __FUNCTION__);
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

int parallel_thread_start ( parallel_thread **pp_parallel_thread, fn_parallel_task *pfn_task, void *p_parameter )
{

    // argument check
    if ( pp_parallel_thread == (void *) 0 ) goto no_parallel_thread;
    if ( pfn_task           == (void *) 0 ) goto no_function_pointer;

    // initialized data
    parallel_thread *p_parallel_thread = (void *) 0;

    // allocate a parallel thread
    if ( parallel_thread_create(&p_parallel_thread) == 0 ) goto failed_to_allocate_parallel_thread;

    // Platform dependent implementation
    #ifdef _WIN64

        // TODO:
        //

    #else
        // Create a pthread
        if ( pthread_create(&p_parallel_thread->platform_dependent_thread, NULL, pfn_task, p_parameter) != 0 ) goto failed_to_create_pthread;
    #endif

    // return a pointer to the caller
    *pp_parallel_thread = p_parallel_thread;

    // success
    return 1;

        // error
        return 0;

    // error handling
    {

        // argument errors
        {
            no_parallel_thread:
                #ifndef NDEBUG
                    log_error("[parallel] [thread] Null pointer provided for parameter \"pp_parallel_thread\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            no_function_pointer:
                #ifndef NDEBUG
                    log_error("[parallel] [thread] Null pointer provided for parameter \"pfn_function_pointer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Parallel errors
        {
            failed_to_allocate_parallel_thread:
                #ifndef NDEBUG
                    log_error("[parallel] [thread] Failed to allocate thread in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // error
                return 0;
        }

        // pthread errors
        {
            failed_to_create_pthread:
                #ifndef NDEBUG
                    log_error("[parallel] [thread] Call to \"pthread_create\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int parallel_thread_cancel ( parallel_thread *p_parallel_thread )
{

    // argument check
    if ( p_parallel_thread == (void *) 0 ) goto no_parallel_thread;

    // Platform dependent implementation
    #ifdef _WIN64

        // TODO:
        //

    #else
        pthread_cancel(p_parallel_thread->platform_dependent_thread);
    #endif

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_parallel_thread:
                #ifndef NDEBUG
                    log_error("[parallel] [thread] Null pointer provided for parameter \"pp_parallel_thread\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int parallel_thread_join ( parallel_thread **pp_parallel_thread )
{

    // argument check
    if ( pp_parallel_thread == (void *) 0 ) goto no_parallel_thread;

    // initialized data
    parallel_thread *p_parallel_thread = *pp_parallel_thread;

    // No more pointer for caller
    *pp_parallel_thread = (void *) 0;

    // Destory the parallel thread
    parallel_thread_destory(&p_parallel_thread);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_parallel_thread:
                #ifndef NDEBUG
                    log_error("[parallel] [thread] Null pointer provided for parameter \"pp_parallel_thread\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int parallel_thread_destory ( parallel_thread **pp_parallel_thread )
{

    // argument check
    if ( pp_parallel_thread == (void *) 0 ) goto no_parallel_thread;

    // initialized data
    parallel_thread *p_parallel_thread = *pp_parallel_thread;

    // No more pointer for caller
    *pp_parallel_thread = (void *) 0;

    // Wait for the thread to finish executing
    if ( pthread_join(p_parallel_thread->platform_dependent_thread, NULL) != 0 ) goto failed_to_join_pthread;
        
    // Free the parallel thread struct
    p_parallel_thread = default_allocator(p_parallel_thread, 0);

    // success
    return 1;

    // TODO
    failed_to_join_pthread:

        // error
        return 0;

    // error handling
    {

        // argument errors
        {
            no_parallel_thread:
                #ifndef NDEBUG
                    log_error("[parallel] [thread] Null pointer provided for parameter \"pp_parallel_thread\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}
