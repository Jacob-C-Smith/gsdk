/** !
 * sync example program
 * 
 * @file main.c
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

// preprocessor definitions
#define NTH_FIBONACCI_NUMBER 1000000000

// structure definitions
struct box_s
{
    int value;
    bool occupied;
    mutex m;
    condition_variable empty, full;
};

// type definitions
typedef struct box_s box;

// forward declarations
/// logs
int checkpoint ( const char *p_event );

/// box
int box_put ( box *p_box, int  value );
int box_get ( box *p_box, int *p_value );

/// threads
void *child ( void *p_parameter );

// data
static box _box = { 0 };
size_t i = 0;

// entry point
int main ( int argc, const char *argv[] )
{

    // unused
    (void) argc;
    (void) argv;

    // #0 - start
    checkpoint("start");

    // #1 - timer example
    {

        // initialized data
        size_t    c   = 0,
                  d   = 1;
        double    sec = 0;
        timestamp t1  = 0, 
                  t2  = 0,
                  td  = 0;

        // checkpoint
        checkpoint("timer example");

        // record a timestamp
        t1 = timer_high_precision();

        // iteratively calculate fib(NTH_FIBONACCI_NUMBER) 
        for (size_t i = 0; i < NTH_FIBONACCI_NUMBER; i++)
        {

            // initialized data
            size_t e = c + d;

            // Iteration
            c = d, d = e;
        }
        
        // record another timestamp
        t2 = timer_high_precision();

        // compute the difference
        td = t2 - t1;

        // convert the difference to seconds
        sec = (double) td / (double) ( timer_seconds_divisor() );

        // print the time difference in seconds
        printf("%zu\rIt took %lf seconds to calculate fib(%d)\n", d, sec, NTH_FIBONACCI_NUMBER);
    }
    
    // #2 - mutex example
    {

        // initialized data
        mutex m = { 0 };

        // checkpoint
        checkpoint("mutex example");

        // construct a mutex
        if ( 0 == mutex_create(&m) ) return EXIT_FAILURE;

        // lock ...
        printf("\033[44m[WAIT]\033[0m Locking mutex ... \r"), fflush(stdout);
        sleep(1), mutex_lock(&m),
        printf("\033[42m[DONE]\033[0m\n");

        // ... (pretend) critical section ...
        log_error("This message was printed from a critical section\n");

        // ... unlock
        printf("\033[44m[WAIT]\033[0m Unlocking mutex ... \r"), fflush(stdout);
        sleep(1), mutex_unlock(&m),
        printf("\033[42m[DONE]\033[0m\n");

        // destroy
        (void) mutex_destroy(&m);
    }

    // #3 - semaphore example
    {

        // initialized data
        semaphore s = { 0 };

        // checkpoint
        checkpoint("semaphore example");

        // construct a semaphore
        if ( 0 == semaphore_create(&s, 1) ) return EXIT_FAILURE;

        // lock ...
        printf("\033[44m[WAIT]\033[0m Locking semaphore ... \r"), fflush(stdout);
        sleep(1), semaphore_wait(&s),
        printf("\033[42m[DONE]\033[0m\n");

        // ... (pretend) critical section ...
        log_error("This message was printed from a critical section\n");

        // ... unlock
        printf("\033[44m[WAIT]\033[0m Unlocking semaphore ... \r"), fflush(stdout);
        sleep(1), semaphore_signal(&s),
        printf("\033[42m[DONE]\033[0m\n");

        // destroy
        (void) semaphore_destroy(&s);
    }

    // #4 - condition variable example
    {

        // initialized data
        pthread_t _threads[8] = { 0 };
        int v = 0;

        // checkpoint
        checkpoint("condition variable example");
        
        // construct a mutex
        if ( 0 == mutex_create(&_box.m) ) return EXIT_FAILURE;

        // construct condition variables
        if ( 0 == condition_variable_create(&_box.empty) ) return EXIT_FAILURE;
        if ( 0 == condition_variable_create(&_box.full)  ) return EXIT_FAILURE;

        // spawn threads
        for (size_t i = 0; i < 8; i++)
            pthread_create(&_threads[i], NULL, child, (void *)i);

        // get products of boxes
        for (size_t i = 0; i < 8; i++)
            box_get(&_box, &v);

        // join threads
        for (size_t i = 0; i < 8; i++)
            pthread_join(_threads[i], NULL);

        // destroy
    }

    // #X - end
    checkpoint("end");

    // success
    return EXIT_SUCCESS;
}

void *child ( void *p_parameter )
{

    // put a number in the box
    box_put(&_box, (int)p_parameter);

    // done
    return NULL;
}

int box_put ( box *p_box, int  value )
{

    // lock
    mutex_lock(&_box.m);

    // monitor
    while ( p_box->occupied )

        // wait
        condition_variable_wait(&_box.empty, &_box.m);

    // update the value
    p_box->value = value,
    p_box->occupied = true;

    // logs
    printf("\033[41m[CRITICAL]\033[0m put(%d)\n", p_box->value), fflush(stdout);

    // signal
    condition_variable_signal(&_box.full);

    // unlock
    mutex_unlock(&_box.m);

    // success
    return 1;
}

int box_get ( box *p_box, int *p_value )
{

    // lock
    mutex_lock(&_box.m);

    // monitor
    while ( !p_box->occupied )

        // wait
        condition_variable_wait(&_box.full, &_box.m);

    // get the value
    *p_value        = p_box->value,
    p_box->value    = 0,

    // clear the occupied flag
    p_box->occupied = false;

    // logs
    printf("\033[41m[CRITICAL]\033[0m get( ) -> %d\n", *p_value), fflush(stdout);

    // signal
    condition_variable_signal(&_box.empty);

    // unlock
    mutex_unlock(&_box.m);
    
    // success
    return 1;
}

int checkpoint ( const char *p_event )
{

    // static data
    static int step = 0;

    // log the event
    log_info("#%d - %s\n", step, p_event);

    // increment counter
    step++;

    // success
    return 1;
}
