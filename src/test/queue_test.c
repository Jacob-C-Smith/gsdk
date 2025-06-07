/** !
 * Queue tester
 * 
 * @file queue_test.c
 * 
 * @author Jacob Smith
*/

// TODO: Improve documentation

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// core
#include <core/log.h>
#include <core/sync.h>

// data
#include <data/queue.h>

// Possible elements
void *A_element = (void *)0x1,
     *B_element = (void *)0x2,
     *C_element = (void *)0x3,
     *D_element = (void *)0x4,
     *X_element = (void *)0xFFFFFFFFFFFFFFFF;

// Expected results
void  *_contents    [] = { (void *)0x0 };
void  *A_contents   [] = { (void *)0x1, (void *)0x0 };
void  *B_contents   [] = { (void *)0x2, (void *)0x0 };
void  *AB_contents  [] = { (void *)0x1, (void *)0x2, (void *)0x0 };
void  *BA_contents  [] = { (void *)0x2, (void *)0x1, (void *)0x0 };
void  *BC_contents  [] = { (void *)0x2, (void *)0x3, (void *)0x0 };
void  *CA_contents  [] = { (void *)0x3, (void *)0x1, (void *)0x0 };
void  *CB_contents  [] = { (void *)0x3, (void *)0x2, (void *)0x0 };
void  *CAB_contents [] = { (void *)0x3, (void *)0x1, (void *)0x2,  (void *)0x0 };
void  *CBA_contents [] = { (void *)0x3, (void *)0x2, (void *)0x1,  (void *)0x0 };

// Test results
enum result_e {
    zero=0,
    False=0,
    Underflow=0,
    Overflow=0,
    one=1,
    True=1,
    match
};

typedef enum result_e result_t;

int total_tests      = 0,
    total_passes     = 0,
    total_fails      = 0,
    ephemeral_tests  = 0,
    ephemeral_passes = 0,
    ephemeral_fails  = 0;

// Forward declarations
int run_tests           ( void );
int print_final_summary ( void );
int print_test          ( const char  *scenario_name, const char *test_name, bool passed );
int print_time_pretty   ( double seconds );

bool test_front   ( int (*queue_constructor)(queue **), void *expected_value  , result_t expected );
bool test_rear    ( int (*queue_constructor)(queue **), void *expected_value  , result_t expected );
bool test_enqueue ( int (*queue_constructor)(queue **), void *value           , result_t expected );
bool test_dequeue ( int (*queue_constructor)(queue **), void *expected_value  , size_t   num_dequeues, result_t expected );
bool test_empty   ( int (*queue_constructor)(queue **), void **expected_values, result_t expected );

int test_empty_queue         ( int (*queue_constructor)(queue **), char *name );
int test_one_element_queue   ( int (*queue_constructor)(queue **), char *name, void **elements );
int test_two_element_queue   ( int (*queue_constructor)(queue **), char *name, void **elements );
int test_three_element_queue ( int (*queue_constructor)(queue **), char *name, void **elements );

int construct_empty            ( queue **pp_queue );
int construct_empty_enqueueA_A ( queue **pp_queue );
int construct_empty_enqueueB_B ( queue **pp_queue );
int construct_A_dequeue_empty  ( queue **pp_queue ); 
int construct_B_dequeue_empty  ( queue **pp_queue ); 
int construct_A_enqueueB_BA    ( queue **pp_queue ); 
int construct_B_enqueueA_AB    ( queue **pp_queue ); 
int construct_AB_dequeue_A     ( queue **pp_queue ); 
int construct_BA_dequeue_B     ( queue **pp_queue );
int construct_AB_enqueueC_CAB  ( queue **pp_queue );
int construct_BA_enqueueC_CBA  ( queue **pp_queue );
int construct_CAB_dequeue_CA   ( queue **pp_queue );
int construct_CBA_dequeue_CB   ( queue **pp_queue );

// Entry point
int main ( int argc, const char* argv[] )
{

    // Supress compiler warnings
    (void) argc;
    (void) argv;

    // Initialized data
    timestamp t0 = 0,
              t1 = 0;

    // Formatting
    printf(
        "╭──────────────╮\n"\
        "│ queue tester │\n"\
        "╰──────────────╯\n\n"
    );
    
    // Start
    t0 = timer_high_precision();

    // Run tests
    run_tests();

    // Stop
    t1 = timer_high_precision();

    // Report the time it took to run the tests
    log_info("queue took ");
    print_time_pretty ( (double)(t1-t0)/(double)timer_seconds_divisor() );
    log_info(" to test\n");

    // Exit
    return ( total_passes == total_tests ) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int print_time_pretty ( double seconds )
{

    // Initialized data
    double _seconds     = seconds;
    size_t days         = 0,
           hours        = 0,
           minutes      = 0,
           __seconds    = 0,
           milliseconds = 0,
           microseconds = 0;

    // Days
    while ( _seconds > 86400.0 ) { days++;_seconds-=286400.0; };

    // Hours
    while ( _seconds > 3600.0 ) { hours++;_seconds-=3600.0; };

    // Minutes
    while ( _seconds > 60.0 ) { minutes++;_seconds-=60.0; };

    // Seconds
    while ( _seconds > 1.0 ) { __seconds++;_seconds-=1.0; };

    // milliseconds
    while ( _seconds > 0.001 ) { milliseconds++;_seconds-=0.001; };

    // Microseconds        
    while ( _seconds > 0.000001 ) { microseconds++;_seconds-=0.000001; };

    // Print days
    if ( days ) 
        log_info("%d D, ", days);
    
    // Print hours
    if ( hours )
        log_info("%d h, ", hours);

    // Print minutes
    if ( minutes )
        log_info("%d m, ", minutes);

    // Print seconds
    if ( __seconds )
        log_info("%d s, ", __seconds);
    
    // Print milliseconds
    if ( milliseconds )
        log_info("%d ms, ", milliseconds);
    
    // Print microseconds
    if ( microseconds )
        log_info("%d us", microseconds);
    
    // Success
    return 1;
}

int run_tests ( void )
{

    // ... -> []
    test_empty_queue(construct_empty, "empty");

    // [] -> enqueue(A) -> [A]
    test_one_element_queue(construct_empty_enqueueA_A, "empty_enqueueA_A", (void **) A_contents);

    // [] -> enqueue(B) -> [B]
    test_one_element_queue(construct_empty_enqueueB_B, "empty_enqueueB_B", (void **) B_contents);

    // [A] -> dequeue() -> []
    test_empty_queue(construct_A_dequeue_empty, "A_dequeue_empty");

    // [B] -> dequeue() -> []
    test_empty_queue(construct_B_dequeue_empty, "B_dequeue_empty");

    // [A] -> enqueue(B) -> [B,A]
    test_two_element_queue(construct_A_enqueueB_BA, "A_enqueueB_BA", (void **) BA_contents);
    
    // [B] -> enqueue(A) -> [A,B]
    test_two_element_queue(construct_B_enqueueA_AB, "B_enqueueA_AB", (void **) AB_contents);

    // [A,B] -> dequeue() -> [A]
    test_one_element_queue(construct_AB_dequeue_A, "construct_AB_dequeue_A", (void **) A_contents);

    // [B,A] -> dequeue() -> [B]
    test_one_element_queue(construct_BA_dequeue_B, "construct_BA_dequeue_B", (void **) B_contents);

    // [A,B] -> enqueue(C) -> [C,A,B]
    test_three_element_queue(construct_AB_enqueueC_CAB, "AB_enqueueC_CAB", (void **) CAB_contents);
    
    // [A,B] -> enqueue(C) -> [C,B,A]
    test_three_element_queue(construct_BA_enqueueC_CBA, "BA_enqueueC_CBA", (void **) CBA_contents);
    
    // [C,A,B] -> dequeue() -> [C,A]
    test_two_element_queue(construct_CAB_dequeue_CA, "CAB_dequeue_CA", (void **) CA_contents);

    // [C,B,A] -> dequeue() -> [C,B]
    test_two_element_queue(construct_CBA_dequeue_CB, "CBA_dequeue_CB", (void **)CB_contents);

    // Success
    return 1;
}

int construct_empty ( queue **pp_queue )
{

    // Construct a queue
    queue_construct(pp_queue);

    // queue = []
    return 1;
}

int construct_empty_enqueueA_A(queue **pp_queue)
{

    // Construct a [] queue
    construct_empty(pp_queue);

    // enqueue(A)
    queue_enqueue(*pp_queue, A_element);

    // queue = [A]
    // Success
    return 1;
}

int construct_empty_enqueueB_B(queue **pp_queue)
{

    // Construct a [] queue
    construct_empty(pp_queue);

    // enqueue(B)
    queue_enqueue(*pp_queue, B_element);

    // queue = [B]
    // Success
    return 1;
}

int construct_A_dequeue_empty(queue **pp_queue)
{

    // Construct a [A] queue
    construct_empty_enqueueA_A(pp_queue);

    // dequeue()
    queue_dequeue(*pp_queue, (void **)0);

    // queue = []
    // Success
    return 1;
}

int construct_B_dequeue_empty ( queue **pp_queue )
{
    // Construct a [B] queue
    construct_empty_enqueueB_B(pp_queue);

    // dequeue()
    queue_dequeue(*pp_queue, (void **)0);

    // queue = []
    // Success
    return 1;
}

int construct_A_enqueueB_BA ( queue **pp_queue )
{
    // Construct a [A] queue
    construct_empty_enqueueA_A(pp_queue);

    // enqueue(B)
    queue_enqueue(*pp_queue, B_element);

    // queue = [B,A]
    // Success
    return 1;
}

int construct_B_enqueueA_AB ( queue **pp_queue )
{
    // Construct a [B] queue
    construct_empty_enqueueB_B(pp_queue);

    // enqueue(A)
    queue_enqueue(*pp_queue, A_element);
    
    // queue = [A,B]
    // Success
    return 1;
}

int construct_AB_dequeue_A ( queue **pp_queue )
{
    // Construct a [A,B] queue
    construct_B_enqueueA_AB(pp_queue);

    // dequeue()
    queue_dequeue(*pp_queue, (void **)0);

    // queue = [A]
    // Success
    return 1;
}

int construct_BA_dequeue_B ( queue **pp_queue )
{
    // Construct a [B,A] queue
    construct_A_enqueueB_BA(pp_queue);

    // dequeue()
    queue_dequeue(*pp_queue, (void **)0);

    // queue = [B]
    // Success
    return 1;
}

int construct_AB_enqueueC_CAB ( queue **pp_queue )
{
    // Construct a [] queue
    construct_B_enqueueA_AB(pp_queue);

    // enqueue(C)
    queue_enqueue(*pp_queue, C_element);

    // queue = [C,A,B]
    // Success
    return 1;
}

int construct_BA_enqueueC_CBA ( queue **pp_queue )
{
    // Construct a [B,A] queue
    construct_A_enqueueB_BA(pp_queue);

    // enqueue(C)
    queue_enqueue(*pp_queue, C_element);

    // queue = [C,B,A]
    // Success
    return 1;
}

int construct_CAB_dequeue_CA ( queue **pp_queue )
{
    // Construct a [C,A,B] queue
    construct_AB_enqueueC_CAB(pp_queue);

    // dequeue()
    queue_dequeue(*pp_queue, (void **)0);

    // queue = [C,A]
    // Success
    return 1;
}

int construct_CBA_dequeue_CB ( queue **pp_queue )
{
    // Construct a [C,B,A] queue
    construct_BA_enqueueC_CBA(pp_queue);

    // dequeue()
    queue_dequeue(*pp_queue, (void **)0);

    // queue = [C,B]
    // Success
    return 1;
}

int test_empty_queue(int(*queue_constructor)(queue **pp_queue), char *name)
{

    log_info("Scenario: %s\n", name);

    print_test(name, "queue_front"  , test_front(queue_constructor, (void *)0, zero) );
    print_test(name, "queue_rear"   , test_rear(queue_constructor, (void *)0, zero) );
    print_test(name, "queue_enqueue", test_enqueue(queue_constructor, A_element, one) );
    print_test(name, "queue_dequeue", test_dequeue(queue_constructor, (void **)zero, 1, Underflow) );    
    print_test(name, "queue_empty"  , test_empty(queue_constructor, 0, True) );

    print_final_summary();

    return 1;
}

int test_one_element_queue   ( int (*queue_constructor)(queue **), char *name, void **elements )
{

    log_info("Scenario: %s\n", name);

    print_test(name, "queue_front"    , test_front(queue_constructor, elements[0], match) );
    print_test(name, "queue_rear"     , test_rear(queue_constructor, elements[0], match) );
    print_test(name, "queue_enqueue"  , test_enqueue(queue_constructor, D_element, one) );
    print_test(name, "queue_dequeue_0", test_dequeue(queue_constructor, elements[0], 1, match) );
    print_test(name, "queue_dequeue_3", test_dequeue(queue_constructor, 0, 3, Underflow) );
    print_test(name, "queue_empty"    , test_empty(queue_constructor, 0, False) );

    print_final_summary();

    // Success
    return 1;
}

int test_two_element_queue   ( int (*queue_constructor)(queue **), char *name, void **elements )
{

    log_info("Scenario: %s\n", name);

    print_test(name, "queue_front"  , test_front(queue_constructor, elements[1], match) );
    print_test(name, "queue_rear"   , test_rear(queue_constructor, elements[0], match) );
    print_test(name, "queue_enqueue", test_enqueue(queue_constructor, D_element, one) );
    print_test(name, "queue_dequeue_0", test_dequeue(queue_constructor, elements[0], 2, match) );
    print_test(name, "queue_dequeue_1", test_dequeue(queue_constructor, elements[1], 1, match) );
    // Force an underflow
    print_test(name, "queue_dequeue_3", test_dequeue(queue_constructor, 0, 3, Underflow) );

    print_test(name, "queue_empty"  , test_empty(queue_constructor, 0, False) );

    print_final_summary();

    // Success
    return 1;
}

int test_three_element_queue   ( int (*queue_constructor)(queue **), char *name, void **elements )
{

    log_info("Scenario: %s\n", name);

    print_test(name, "queue_front"  , test_front(queue_constructor, elements[2], match) );
    print_test(name, "queue_rear"   , test_rear(queue_constructor, elements[0], match) );
    print_test(name, "queue_enqueue", test_enqueue(queue_constructor, D_element, one) );
   
    for (size_t i = 0; elements[i]; i++)
    {
        char *test_name = calloc(15+1, sizeof(char));
        sprintf(test_name, "queue_dequeue_%zu", i);
        print_test(name, test_name , test_dequeue(queue_constructor, elements[2-i], i+1, match) );
        free(test_name);
    }
    
    // Force an underflow
    print_test(name, "queue_dequeue_3", test_dequeue(queue_constructor, 0, 3, Underflow) );

    print_test(name, "queue_empty"  , test_empty(queue_constructor, 0, False) );

    print_final_summary();

    // Success
    return 1;
}

int print_test ( const char *scenario_name, const char *test_name, bool passed )
{

    // Initialized data
    if ( passed )
        log_pass("%s %s\n", scenario_name, test_name);
    else
        log_fail("%s %s\n", scenario_name, test_name);


    // Increment the counters
    {
        if (passed)
        {
            ephemeral_passes++;
        }
        else
        {
            ephemeral_fails++;
        }

        ephemeral_tests++;
    }

    // Success
    return 1;
}

int print_final_summary ( void )
{

    // Accumulate
    total_tests  += ephemeral_tests,
    total_passes += ephemeral_passes,
    total_fails  += ephemeral_fails;

    // Print
    log_info("\nTests: %d, Passed: %d, Failed: %d (%%%.3f)\n",  ephemeral_tests, ephemeral_passes, ephemeral_fails, ((float)ephemeral_passes/(float)ephemeral_tests*100.f));
    log_info("Total: %d, Passed: %d, Failed: %d (%%%.3f)\n\n",  total_tests, total_passes, total_fails, ((float)total_passes/(float)total_tests*100.f));
    
    ephemeral_tests  = 0;
    ephemeral_passes = 0;
    ephemeral_fails  = 0;

    // Success
    return 1;
}

bool test_front ( int (*queue_constructor)(queue **), void *expected_value, result_t expected )
{

    // Initialized data
    result_t  result       = 0;
    queue    *p_queue      = 0;
    void     *result_value = 0;

    // Build the queue
    queue_constructor(&p_queue);

    // Get the front
    result = (result_t) queue_front(p_queue, &result_value);

    // Check the result
    if (result == zero)
    {
        goto exit;
    }
    else if (result_value == expected_value)
    {
        result = match; // Match implies queue_front reutrned 1
    }
    else
    {
        result = one;
    }

    exit:

    // Free the queue
    queue_destroy(&p_queue);

    // Return result
    return (result == expected);
}

bool test_rear ( int (*queue_constructor)(queue **), void *expected_value, result_t expected )
{
    
    // Initialized data
    result_t  result       = 0;
    queue    *p_queue      = 0;
    void     *result_value = 0;

    // Build the queue
    queue_constructor(&p_queue);

    // Get the rear
    result = (result_t) queue_rear(p_queue, &result_value);

    // Check the result
    if (result == zero)
    {
        goto exit;
    }
    else if (result_value == expected_value)
    {
        result = match; // Match implies queue_rear reutrned 1
    }
    else
    {
        result = zero;
    }

    exit:

    // Free the queue
    queue_destroy(&p_queue);

    // Return result
    return (result == expected);
}

bool test_enqueue ( int (*queue_constructor)(queue **), void *value, result_t  expected )
{

    // Initialized data
    result_t  result = 0;
    queue    *p_queue = 0;

    // Build the queue
    queue_constructor(&p_queue);

    result = (result_t) queue_enqueue(p_queue, value);

    // Free the queue
    queue_destroy(&p_queue);

    // Return result
    return (result == expected);
}

bool test_dequeue ( int (*queue_constructor)(queue **), void *expected_value  , size_t   num_dequeues, result_t expected )
{

    // Initialized data
    result_t  result       = 0;
    queue    *p_queue      = 0;
    void     *result_value = 0;

    // Build the queue
    queue_constructor(&p_queue);
    
    for (size_t i = 0; i < num_dequeues; i++)
        result = (result_t) queue_dequeue(p_queue, &result_value);
    
    
    // Check the result
    if (result == Underflow)
        goto exit;
    else if (result_value == expected_value)
        result = match;
    else
        result = zero;

    exit:
    // Free the queue
    queue_destroy(&p_queue);

    // Return result
    return (result == expected);
}

bool test_empty ( int (*queue_constructor)(queue **), void **expected_values, result_t  expected )
{

    // Initialized data
    result_t  result = 0;
    queue    *p_queue = 0;

    // Unused
    (void) expected_values;

    // Build the queue
    queue_constructor(&p_queue);

    result = queue_empty(p_queue);

    // Free the queue
    queue_destroy(&p_queue);

    // Return result
    return (result == expected);
}
