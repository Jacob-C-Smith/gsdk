/** ! 
 * Circular buffer tester
 * 
 * @file circular_buffer_test.c
 * 
 * @author Jacob Smith
 */

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// core
#include <core/log.h>
#include <core/sync.h>

// data
#include <data/circular_buffer.h>

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
void  *C_contents   [] = { (void *)0x3, (void *)0x0 };
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

// forward declarations
int run_tests           ( void );
int print_final_summary ( void );
int print_test          ( const char  *scenario_name, const char *test_name, bool passed );
int print_time_pretty   ( double seconds );

bool test_empty   ( int (*circular_buffer_constructor)(circular_buffer **), result_t expected );
bool test_full    ( int (*circular_buffer_constructor)(circular_buffer **), result_t expected );
bool test_push    ( int (*circular_buffer_constructor)(circular_buffer **), void *value           , result_t expected );
bool test_peek    ( int (*circular_buffer_constructor)(circular_buffer **), void *expected_value  , result_t expected );
bool test_pop     ( int (*circular_buffer_constructor)(circular_buffer **), void **expected_values, result_t expected );

int test_empty_circular_buffer         ( int (*circular_buffer_constructor)(circular_buffer **), char *name );
int test_one_element_circular_buffer   ( int (*circular_buffer_constructor)(circular_buffer **), char *name, void **elements );
int test_two_element_circular_buffer   ( int (*circular_buffer_constructor)(circular_buffer **), char *name, void **elements );
int test_three_element_circular_buffer ( int (*circular_buffer_constructor)(circular_buffer **), char *name, void **elements );

int construct_empty            ( circular_buffer **pp_circular_buffer );

int construct_empty_pushA_A    ( circular_buffer **pp_circular_buffer );
int construct_empty_pushB_B    ( circular_buffer **pp_circular_buffer );
int construct_empty_pushC_C    ( circular_buffer **pp_circular_buffer );

int construct_A_pop_empty      ( circular_buffer **pp_circular_buffer );
int construct_B_pop_empty      ( circular_buffer **pp_circular_buffer );
int construct_C_pop_empty      ( circular_buffer **pp_circular_buffer );

int construct_A_pushB_AB       ( circular_buffer **pp_circular_buffer );
int construct_A_pushC_AC       ( circular_buffer **pp_circular_buffer );
int construct_B_pushA_BA       ( circular_buffer **pp_circular_buffer );
int construct_B_pushC_BC       ( circular_buffer **pp_circular_buffer );
int construct_C_pushA_CA       ( circular_buffer **pp_circular_buffer );
int construct_C_pushB_CB       ( circular_buffer **pp_circular_buffer );

int construct_AB_pop_A ( circular_buffer **pp_circular_buffer );
int construct_AC_pop_A ( circular_buffer **pp_circular_buffer );
int construct_BA_pop_B ( circular_buffer **pp_circular_buffer );
int construct_BC_pop_B ( circular_buffer **pp_circular_buffer );
int construct_CA_pop_C ( circular_buffer **pp_circular_buffer );
int construct_CB_pop_C ( circular_buffer **pp_circular_buffer );

int construct_empty_enqueueB_B ( circular_buffer **pp_circular_buffer );
int construct_A_dequeue_empty  ( circular_buffer **pp_circular_buffer ); 
int construct_B_dequeue_empty  ( circular_buffer **pp_circular_buffer ); 
int construct_A_enqueueB_BA    ( circular_buffer **pp_circular_buffer ); 
int construct_B_enqueueA_AB    ( circular_buffer **pp_circular_buffer ); 
int construct_AB_dequeue_A     ( circular_buffer **pp_circular_buffer ); 
int construct_BA_dequeue_B     ( circular_buffer **pp_circular_buffer );
int construct_AB_enqueueC_CAB  ( circular_buffer **pp_circular_buffer );
int construct_BA_enqueueC_CBA  ( circular_buffer **pp_circular_buffer );
int construct_CAB_dequeue_CA   ( circular_buffer **pp_circular_buffer );
int construct_CBA_dequeue_CB   ( circular_buffer **pp_circular_buffer );

// entry point
int main ( int argc, const char* argv[] )
{

    // unused
	(void) argc;
	(void) argv;

    // initialized data
    timestamp t0 = 0, t1 = 0;


    // Formatting
    printf(
        "╭────────────────────────╮\n"\
        "│ circular buffer tester │\n"\
        "╰────────────────────────╯\n\n"
    );
    
    // Start
    t0 = timer_high_precision();

    // Run tests
    run_tests();

    // Stop
    t1 = timer_high_precision();

    // Report the time it took to run the tests
    log_info("circular buffer took ");
    print_time_pretty ( (double)(t1-t0)/(double)timer_seconds_divisor() );
    log_info(" to test\n");

    // done
    return ( total_passes == total_tests ) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int print_time_pretty ( double seconds )
{

    // initialized data
    double _seconds     = seconds;
    size_t days         = 0,
           hours        = 0,
           minutes      = 0,
           __seconds    = 0,
           milliseconds = 0,
           microseconds = 0;

    // Days
    while ( _seconds > 86400.0 ) { days++;_seconds-=86400.0; };

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
    
    // success
    return 1;
}

int run_tests()
{

    // ... -> [ _, _ ]
    test_empty_circular_buffer(construct_empty, "empty");

    // [ _, _ ] -> push(A) -> [ A, _ ]
    test_one_element_circular_buffer(construct_empty_pushA_A, "empty_pushA_A", A_contents);

    // [ _, _ ] -> push(B) -> [ B, _ ]
    test_one_element_circular_buffer(construct_empty_pushB_B, "empty_pushB_B", B_contents);
    
    // [ _, _ ] -> push(C) -> [ C, _ ]
    test_one_element_circular_buffer(construct_empty_pushC_C, "empty_pushC_C", C_contents);

    // [ A, _ ] -> pop() -> [ _, _ ]
    test_empty_circular_buffer(construct_A_pop_empty, "A_pop_empty");

    // [ B, _ ] -> pop() -> [ _, _ ]
    test_empty_circular_buffer(construct_B_pop_empty, "B_pop_empty");

    // [ C, _ ] -> pop() -> [ _, _ ]
    test_empty_circular_buffer(construct_C_pop_empty, "C_pop_empty");

    // [ A, B ] -> pop() -> [ B, _ ]
    test_one_element_circular_buffer(construct_AB_pop_A, "AB_pop_A", B_contents);

    // [ A, C ] -> pop() -> [ C, _ ]
    test_one_element_circular_buffer(construct_AC_pop_A, "AC_pop_A", C_contents);

    // [ B, A ] -> pop() -> [ A, _ ]
    test_one_element_circular_buffer(construct_BA_pop_B, "BA_pop_B", A_contents);

    // [ B, C ] -> pop() -> [ C, _ ]
    test_one_element_circular_buffer(construct_BC_pop_B, "BC_pop_B", C_contents);

    // [ C, A ] -> pop() -> [ A, _ ]
    test_one_element_circular_buffer(construct_CA_pop_C, "CA_pop_C", A_contents);

    // [ C, B ] -> pop() -> [ B, _ ]
    test_one_element_circular_buffer(construct_CB_pop_C, "CB_pop_C", B_contents);

    // success
    return 1;
}

int construct_empty ( circular_buffer **pp_circular_buffer )
{

    // Construct a queue
    circular_buffer_construct(pp_circular_buffer, 3);

    // circular_buffer = [ ]
    return 1;
}

int construct_empty_pushA_A ( circular_buffer **pp_circular_buffer )
{    

    // Construct a queue
    construct_empty(pp_circular_buffer);

    // Push A 
    circular_buffer_push(*pp_circular_buffer, A_element);

    // circular_buffer = [ A, _ ]
    return 1;
}

int construct_empty_pushB_B ( circular_buffer **pp_circular_buffer )
{    

    // Construct a queue
    construct_empty(pp_circular_buffer);

    // Push B
    circular_buffer_push(*pp_circular_buffer, B_element);

    // circular_buffer = [ B, _ ]
    return 1;
}

int construct_empty_pushC_C ( circular_buffer **pp_circular_buffer )
{    

    // Construct a queue
    construct_empty(pp_circular_buffer);

    // Push C
    circular_buffer_push(*pp_circular_buffer, C_element);

    // circular_buffer = [ C, _ ]
    return 1;
}

int construct_A_pop_empty ( circular_buffer **pp_circular_buffer )
{

    // initialized data
    void *result = (void *) 0;
    
    // Construct a circular buffer
    construct_empty_pushA_A(pp_circular_buffer);

    // Pop an element
    circular_buffer_pop(*pp_circular_buffer, &result);

    // success
    return 1;

}

int construct_B_pop_empty ( circular_buffer **pp_circular_buffer )
{

    // initialized data
    void *result = (void *) 0;
    
    // Construct a circular buffer
    construct_empty_pushB_B(pp_circular_buffer);

    // Pop an element
    circular_buffer_pop(*pp_circular_buffer, &result);

    // success
    return 1;
}

int construct_C_pop_empty ( circular_buffer **pp_circular_buffer )
{

    // initialized data
    void *result = (void *) 0;
    
    // Construct a circular buffer
    construct_empty_pushC_C(pp_circular_buffer);

    // Pop an element
    circular_buffer_pop(*pp_circular_buffer, &result);

    // success
    return 1;

}

int construct_A_pushB_AB ( circular_buffer **pp_circular_buffer )
{

    construct_empty_pushA_A(pp_circular_buffer);

    circular_buffer_push(*pp_circular_buffer, B_element);

    // circular_buffer = [ A, B ]
    return 1;
}

int construct_A_pushC_AC ( circular_buffer **pp_circular_buffer )
{

    construct_empty_pushA_A(pp_circular_buffer);

    circular_buffer_push(*pp_circular_buffer, C_element);

    // circular_buffer = [ A, B ]
    return 1;
}

int construct_B_pushA_BA ( circular_buffer **pp_circular_buffer )
{

    construct_empty_pushB_B(pp_circular_buffer);

    circular_buffer_push(*pp_circular_buffer, A_element);

    // circular_buffer = [ A, B ]
    return 1;
}

int construct_B_pushC_BC ( circular_buffer **pp_circular_buffer )
{

    construct_empty_pushB_B(pp_circular_buffer);

    circular_buffer_push(*pp_circular_buffer, C_element);

    // circular_buffer = [ A, B ]
    return 1;
}

int construct_C_pushA_CA ( circular_buffer **pp_circular_buffer )
{

    construct_empty_pushC_C(pp_circular_buffer);

    circular_buffer_push(*pp_circular_buffer, A_element);

    // circular_buffer = [ A, B ]
    return 1;
}

int construct_C_pushB_CB ( circular_buffer **pp_circular_buffer )
{

    construct_empty_pushC_C(pp_circular_buffer);

    circular_buffer_push(*pp_circular_buffer, B_element);

    // circular_buffer = [ A, B ]
    return 1;
}


int construct_AB_pop_A ( circular_buffer **pp_circular_buffer )
{
    
    void *result = (void *) 0;
    
    construct_A_pushB_AB(pp_circular_buffer);

    circular_buffer_pop(*pp_circular_buffer, &result);

    return 1;
}

int construct_AC_pop_A ( circular_buffer **pp_circular_buffer )
{
    
    void *result = (void *) 0;
    
    construct_A_pushC_AC(pp_circular_buffer);

    circular_buffer_pop(*pp_circular_buffer, &result);

    return 1;
}

int construct_BA_pop_B ( circular_buffer **pp_circular_buffer )
{
    
    void *result = (void *) 0;
    
    construct_B_pushA_BA(pp_circular_buffer);

    circular_buffer_pop(*pp_circular_buffer, &result);

    return 1;
}

int construct_BC_pop_B ( circular_buffer **pp_circular_buffer )
{
    
    void *result = (void *) 0;
    
    construct_B_pushC_BC(pp_circular_buffer);

    circular_buffer_pop(*pp_circular_buffer, &result);

    return 1;
}

int construct_CA_pop_C ( circular_buffer **pp_circular_buffer )
{
    
    void *result = (void *) 0;
    
    construct_C_pushA_CA(pp_circular_buffer);

    circular_buffer_pop(*pp_circular_buffer, &result);

    return 1;
}

int construct_CB_pop_C ( circular_buffer **pp_circular_buffer )
{
    
    void *result = (void *) 0;
    
    construct_C_pushB_CB(pp_circular_buffer);

    circular_buffer_pop(*pp_circular_buffer, &result);

    return 1;
}


int test_empty_circular_buffer(int(*circular_buffer_constructor)(circular_buffer **pp_circular_buffer), char *name)
{

    log_scenario("%s\n", name);

    print_test(name, "circular_buffer_empty", test_empty(circular_buffer_constructor, True) );
    print_test(name, "circular_buffer_full" , test_full(circular_buffer_constructor, False) );
    // print_test(name, "circular_buffer_push" , test_push(circular_buffer_constructor, A_element, one) );
    // print_test(name, "circular_buffer_peek" , test_peek(circular_buffer_constructor, (void **)zero, zero) );    
    // print_test(name, "circular_buffer_pop"  , test_pop(circular_buffer_constructor, 0, True) );

    // Print the final summary
    print_final_summary();

    // success
    return 1;
}


int test_one_element_circular_buffer   ( int (*circular_buffer_constructor)(circular_buffer **), char *name, void **elements )
{
    
    log_scenario("%s\n", name);

    print_test(name, "circular_buffer_empty", test_empty(circular_buffer_constructor, False) );
    print_test(name, "circular_buffer_full" , test_full(circular_buffer_constructor, False) );
    print_test(name, "circular_buffer_peek" , test_peek(circular_buffer_constructor, elements[0], match) );    
    
    // Print the final summary
    print_final_summary();

    // success
    return 1;
}
/*
int test_two_element_circular_buffer   ( int (*queue_constructor)(queue **), char *name, void **elements )
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

    // success
    return 1;
}
*/

int print_test ( const char *scenario_name, const char *test_name, bool passed )
{

    // initialized data
    if ( passed ) log_pass("%s %s\n", scenario_name, test_name);
    else          log_fail("%s %s\n", scenario_name, test_name);

    // Increment the counters
    if (passed) ephemeral_passes++;
    else        ephemeral_fails++;

    ephemeral_tests++;

    // success
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
    
    ephemeral_tests  = 0,
    ephemeral_passes = 0,
    ephemeral_fails  = 0;

    // success
    return 1;
}


bool test_empty ( int (*circular_buffer_constructor)(circular_buffer **), result_t expected )
{

    // initialized data
    result_t         result = 0;
    circular_buffer *p_queue = 0;

    // Build the queue
    circular_buffer_constructor(&p_queue);

    // store the result
    result = circular_buffer_empty(p_queue);

    // Free the queue
    circular_buffer_destroy(&p_queue);

    // Return result
    return (result == expected);
}

bool test_full ( int (*circular_buffer_constructor)(circular_buffer **), result_t expected )
{

    // initialized data
    result_t         result = 0;
    circular_buffer *p_queue = 0;

    // Build the queue
    circular_buffer_constructor(&p_queue);

    // store the result
    result = circular_buffer_full(p_queue);

    // Free the queue
    circular_buffer_destroy(&p_queue);

    // Return result
    return (result == expected);
}


bool test_peek ( int (*circular_buffer_constructor)(circular_buffer **), void *expected_value, result_t expected )
{
    // initialized data
    result_t         result            = 0;
    circular_buffer *p_circular_buffer = 0;
    void           *result_value      = 0;

    // Build the circular buffer
    circular_buffer_constructor(&p_circular_buffer);

    // Peek the circular buffer
    result = circular_buffer_peek(p_circular_buffer, (void **)&result_value);

    // Check if the peeked value matches the expected value
    if (result == zero) {
        result = (expected == zero) ? match : zero;
    } else if (result_value == expected_value) {
        result = match;
    }

    // Free the circular buffer
    circular_buffer_destroy(&p_circular_buffer);

    // Return result
    return (result == expected);
}
