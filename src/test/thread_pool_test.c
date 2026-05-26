/** !
 * thread pool tester
 * 
 * @file src/test/thread_pool_test.c
 *
 * @author Jacob Smith
*/

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// gsdk
/// core
#include <core/log.h>
#include <core/sync.h>
#include <core/test.h>

/// performance
#include <performance/thread_pool.h>

// structure definitions

struct producer_param_s 
{
    thread_pool *p_pool;
    size_t      *p_counter;
};

// type definitions
typedef struct producer_param_s producer_param;

// function declarations
/// scenario constructors
fn_scenario_constructor construct_pool_1;
fn_scenario_constructor construct_pool_4;
fn_scenario_constructor construct_pool_64;

/// test cases
fn_test_case test_thread_pool_execute_single;
fn_test_case test_thread_pool_execute_multiple;
fn_test_case test_thread_pool_is_idle;
fn_test_case test_thread_pool_wait_idle;
fn_test_case test_thread_pool_stress;
fn_test_case test_thread_pool_reuse;
fn_test_case test_thread_pool_wait_idle_empty;
fn_test_case test_thread_pool_null_args;
fn_test_case test_thread_pool_invalid_quantity;
fn_test_case test_thread_pool_shutdown_busy;
fn_test_case test_thread_pool_multiple_producers;
fn_test_case test_thread_pool_max_threads;

/// allocators
fn_allocator destruct_thread_pool;

// data
mutex counter_lock;
mutex block_lock;

// Helper tasks
/** !
 * Thread-safe increment of a counter
 * 
 * @param p_counter pointer to counter
 * 
 * @return void
 */
void increment_counter ( size_t *p_counter )
{

    // argument check
    if ( NULL == p_counter ) return;

    // lock
    mutex_lock(&counter_lock);

    // increment
    (*p_counter)++;

    // unlock
    mutex_unlock(&counter_lock);

    // done
    return;
}

/** !
 * Simple task that increments a counter
 * 
 * @param p_param pointer to counter
 * 
 * @return 1
 */
void *task_simple ( void *p_param )
{

    // increment the counter
    increment_counter((size_t *)p_param);

    // success
    return (void *)1;
}

/** !
 * Task that blocks on a global mutex then increments a counter
 * 
 * @param p_param pointer to size_t counter
 * 
 * @return 1
 */
void *task_blocked ( void *p_param )
{

    // lock
    mutex_lock(&block_lock);

    // increment
    increment_counter((size_t *)p_param);

    // unlock
    mutex_unlock(&block_lock);

    // success
    return (void *)1;
}

// test
/// cases
test_case _thread_pool_test_cases[] = 
{
    TEST_CASE("execute single"    , test_thread_pool_execute_single    , NULL, TEST_RESULT_ONE),
    TEST_CASE("execute multiple"  , test_thread_pool_execute_multiple  , NULL, TEST_RESULT_ONE),
    TEST_CASE("check idle"        , test_thread_pool_is_idle           , NULL, TEST_RESULT_ONE),
    TEST_CASE("wait idle"         , test_thread_pool_wait_idle         , NULL, TEST_RESULT_ONE),
    TEST_CASE("stress test"       , test_thread_pool_stress            , NULL, TEST_RESULT_ONE),
    TEST_CASE("reuse threads"     , test_thread_pool_reuse             , NULL, TEST_RESULT_ONE),
    TEST_CASE("wait idle empty"   , test_thread_pool_wait_idle_empty   , NULL, TEST_RESULT_ONE),
    TEST_CASE("null arguments"    , test_thread_pool_null_args         , NULL, TEST_RESULT_ONE),
    TEST_CASE("invalid quantity"  , test_thread_pool_invalid_quantity  , NULL, TEST_RESULT_ONE),
    TEST_CASE("shutdown busy"     , test_thread_pool_shutdown_busy     , NULL, TEST_RESULT_ONE),
    TEST_CASE("multiple producers", test_thread_pool_multiple_producers, NULL, TEST_RESULT_ONE),
};

test_case _thread_pool_max_test_case[] = 
{
    TEST_CASE("max threads", test_thread_pool_max_threads, NULL, TEST_RESULT_ONE),
};

/// scenarios
test_scenario _scenarios[] = 
{
    TEST_SCENARIO("1 thread"  , NULL, _thread_pool_test_cases   , construct_pool_1 , destruct_thread_pool),
    TEST_SCENARIO("4 threads" , NULL, _thread_pool_test_cases   , construct_pool_4 , destruct_thread_pool),
    TEST_SCENARIO("64 threads", NULL, _thread_pool_max_test_case, construct_pool_64, destruct_thread_pool),
};

/// suites
test_suite _suite = TEST_SUITE("thread pool", _scenarios);

// entry point
int main ( int argc, const char *argv[] ) 
{

    // unused
    (void) argc;
    (void) argv;

    // construct locks
    mutex_create(&counter_lock),
    mutex_create(&block_lock);
     
    // run the tests
    test_suite_test(&_suite); 
    
    // done
    return (_suite.counters.total.fails == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int construct_pool_1 ( void **pp_result ) 
{ 

    // construct a thread pool
    return thread_pool_construct((thread_pool **)pp_result, 1);
}

int construct_pool_4 ( void **pp_result ) 
{ 

    // construct a thread pool
    return thread_pool_construct((thread_pool **)pp_result, 4);
}

int construct_pool_64 ( void **pp_result ) 
{ 

    // construct a thread pool
    return thread_pool_construct((thread_pool **)pp_result, 64);
}

void *destruct_thread_pool ( void *p_pointer, unsigned long long size )
{

    // unused
    (void) size;

    // initialized data
    thread_pool *p_thread_pool = (thread_pool *)p_pointer;

    // release the thread pool
    if ( p_thread_pool ) 
        thread_pool_destroy(&p_thread_pool);

    // success
    return NULL;
}

void *test_thread_pool_execute_single ( test_case *p_test_case, void *p_subject ) 
{ 

    // unused
    (void) p_test_case;

    // initialized data
    thread_pool *p_pool  = (thread_pool *)p_subject;
    size_t       counter = 0;

    // execute a task
    if ( thread_pool_execute(p_pool, task_simple, &counter) == 0 ) return NULL;

    // wait idle
    thread_pool_wait_idle(p_pool);

    // verify counter
    if ( counter != 1 ) return NULL;

    // success
    return (void *)1;
}

void *test_thread_pool_execute_multiple ( test_case *p_test_case, void *p_subject ) 
{ 

    // unused
    (void) p_test_case;

    // initialized data
    thread_pool *p_pool       = (thread_pool *)p_subject;
    size_t       counters[16] = { 0 };

    // execute tasks
    for (size_t i = 0; i < 16; i++)
        if ( thread_pool_execute(p_pool, task_simple, &counters[i]) == 0 ) return NULL;

    // wait idle
    thread_pool_wait_idle(p_pool);

    // verify counters
    for (size_t i = 0; i < 16; i++)
        if ( counters[i] != 1 ) return NULL;

    // success
    return (void *)1;
}

void *test_thread_pool_is_idle ( test_case *p_test_case, void *p_subject ) 
{ 

    // unused
    (void) p_test_case;

    // initialized data
    thread_pool *p_pool  = (thread_pool *)p_subject;
    size_t       counter = 0;

    // starts idle?
    if ( 1 != thread_pool_is_idle(p_pool) ) return NULL;

    // Lock the block_lock to ensure the next task blocks
    mutex_lock(&block_lock);

    // execute a task
    if ( 0 == thread_pool_execute(p_pool, task_blocked, &counter) ) 
    {

        // unlock
        mutex_unlock(&block_lock);

        // error
        return NULL;
    }

    // not idle?
    if ( 0 != thread_pool_is_idle(p_pool) ) 
    {

        // unlock
        mutex_unlock(&block_lock);

        // error
        return NULL;
    }

    // unlock
    mutex_unlock(&block_lock);

    // wait idle
    thread_pool_wait_idle(p_pool);

    // idle?
    if ( thread_pool_is_idle(p_pool) != 1 ) return NULL;

    // success
    return (void *)1;
}

void *test_thread_pool_wait_idle ( test_case *p_test_case, void *p_subject ) 
{ 

    // unused
    (void) p_test_case;

    // initialized data
    thread_pool *p_pool  = (thread_pool *)p_subject;
    size_t       counter = 0;

    // lock
    mutex_lock(&block_lock);

    // execute a task
    if ( 0 == thread_pool_execute(p_pool, task_blocked, &counter) ) 
    {

        // unlock
        mutex_unlock(&block_lock);

        // error
        return NULL;
    }

    // unlock
    mutex_unlock(&block_lock);

    // wait idle
    if ( 0 == thread_pool_wait_idle(p_pool) ) return NULL;

    // counter?
    if ( 1 != counter ) return NULL;

    // idle?
    if ( thread_pool_is_idle(p_pool) != 1 ) return NULL;

    // success
    return (void *)1;
}

void *test_thread_pool_stress ( test_case *p_test_case, void *p_subject ) 
{ 

    // unused
    (void) p_test_case;

    // initialized data
    thread_pool *p_pool  = (thread_pool *)p_subject;
    size_t       counter = 0;

    // execute many tasks
    for (size_t i = 0; i < 1000; i++)
        if ( 0 == thread_pool_execute(p_pool, task_simple, &counter) ) return NULL;

    // wait for idle
    thread_pool_wait_idle(p_pool);

    // verify counter
    if ( 1000 != counter ) return NULL;

    // success
    return (void *)1;
}

void *test_thread_pool_reuse ( test_case *p_test_case, void *p_subject ) 
{ 

    // unused
    (void) p_test_case;

    // initialized data
    thread_pool *p_pool  = (thread_pool *)p_subject;
    size_t       counter = 0;

    // execute and wait
    for (size_t i = 0; i < 5; i++)
    {

        // execute tasks
        for (size_t j = 0; j < 8; j++)
            if ( 0 == thread_pool_execute(p_pool, task_simple, &counter) ) return NULL;

        // wait idle
        thread_pool_wait_idle(p_pool);
    }

    // verify counter
    if ( 40 != counter ) return NULL;

    // success
    return (void *)1;
}

void *test_thread_pool_wait_idle_empty ( test_case *p_test_case, void *p_subject ) 
{ 

    // unused
    (void) p_test_case;

    // initialized data
    thread_pool *p_pool = (thread_pool *)p_subject;

    // block until idle (should return immediately)
    if ( 0 == thread_pool_wait_idle(p_pool) ) return NULL;

    // success
    return (void *)1;
}

void *test_thread_pool_null_args ( test_case *p_test_case, void *p_subject ) 
{ 

    // unused
    (void) p_test_case;

    // initialized data
    thread_pool *p_pool = (thread_pool *)p_subject;

    // test NULL pool
    if ( thread_pool_execute(NULL, task_simple, NULL) ) return NULL;

    // test NULL task
    if ( thread_pool_execute(p_pool, NULL, NULL) ) return NULL;

    // test NULL wait_idle
    if ( thread_pool_wait_idle(NULL) ) return NULL;

    // test NULL is_idle
    if ( false != thread_pool_is_idle(NULL) ) return NULL;

    // test NULL destroy
    if ( thread_pool_destroy(NULL) ) return NULL;

    // success
    return (void *)1;
}

void *test_thread_pool_invalid_quantity ( test_case *p_test_case, void *p_subject ) 
{ 

    // unused
    (void) p_test_case;
    (void) p_subject;

    // initialized data
    thread_pool *p_pool = NULL;

    // test 0 threads
    if ( thread_pool_construct(&p_pool, 0) ) return NULL;

    // test too many threads (assuming PARALLEL_THREAD_POOL_MAX_THREADS is 64)
    if ( thread_pool_construct(&p_pool, 65) ) return NULL;

    // success
    return (void *)1;
}

void *test_thread_pool_shutdown_busy ( test_case *p_test_case, void *p_subject ) 
{

    // unused
    (void) p_test_case;
    (void) p_subject;

    // initialized data
    thread_pool *p_local_pool = NULL;
    size_t       counter      = 0;

    // construct thread pool
    if ( 0 == thread_pool_construct(&p_local_pool, 4) ) return NULL;
    
    // lock
    mutex_lock(&block_lock);

    // execute a task
    if ( 0 == thread_pool_execute(p_local_pool, task_blocked, &counter) ) 
    {

        // unlock
        mutex_unlock(&block_lock);

        // release the thread pool
        thread_pool_destroy(&p_local_pool);

        // error
        return NULL;
    }

    // unlock
    mutex_unlock(&block_lock);

    // release the thread pool while the task is still running.
    if ( 0 == thread_pool_destroy(&p_local_pool) ) return NULL;

    // task completed?
    return ( counter == 1 ) ? (void *)1 : NULL;
}

void *producer_thread ( void *p_param )
{

    // initialized data
    producer_param *pp = (producer_param *)p_param;

    // execute tasks
    for (size_t i = 0; i < 100; i++)
        thread_pool_execute(pp->p_pool, task_simple, pp->p_counter);

    // done
    return NULL;
}

void *test_thread_pool_multiple_producers ( test_case *p_test_case, void *p_subject ) 
{

    // unused
    (void) p_test_case;

    // initialized data
    thread_pool     *p_pool       = (thread_pool *)p_subject;
    size_t           counter      = 0;
    parallel_thread *producers[4] = { 0 };
    producer_param   param        = { p_pool, &counter };

    // construct 4 producers
    for (size_t i = 0; i < 4; i++)
        parallel_thread_start(&producers[i], (fn_parallel_task *)producer_thread, &param);

    // join producers
    for (size_t i = 0; i < 4; i++)
        parallel_thread_join(&producers[i]);

    // wait idle
    thread_pool_wait_idle(p_pool);

    // all processed? 
    return ( counter == 400 ) ? (void *)1 : NULL;
}

void *test_thread_pool_max_threads ( test_case *p_test_case, void *p_subject ) 
{

    // unused
    (void) p_test_case;

    // initialized data
    thread_pool *p_pool  = (thread_pool *)p_subject;
    size_t       counter = 0;

    // execute many tasks
    for (size_t i = 0; i < 128; i++)
        thread_pool_execute(p_pool, task_simple, &counter);

    // wait
    thread_pool_wait_idle(p_pool);

    // done
    return ( counter == 128 ) ? (void *)1 : NULL;
}
