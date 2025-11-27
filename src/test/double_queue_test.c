/** !
 * Double queue tester
 *
 * @file double_queue_test.c
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
#include <data/double_queue.h>

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
void  *ABC_contents [] = { (void *)0x1, (void *)0x2, (void *)0x3,  (void *)0x0 };
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

bool test_front        ( int (*double_queue_constructor)(double_queue **), void *expected_value  , result_t expected );
bool test_rear         ( int (*double_queue_constructor)(double_queue **), void *expected_value  , result_t expected );
bool test_front_add    ( int (*double_queue_constructor)(double_queue **), void *value           , result_t expected );
bool test_rear_add     ( int (*double_queue_constructor)(double_queue **), void *value           , result_t expected );
bool test_front_remove ( int (*double_queue_constructor)(double_queue **), void *expected_value  , size_t   num_removes, result_t expected );
bool test_rear_remove  ( int (*double_queue_constructor)(double_queue **), void *expected_value  , size_t   num_removes, result_t expected );
bool test_empty        ( int (*double_queue_constructor)(double_queue **), result_t expected );

int test_empty_double_queue         ( int (*double_queue_constructor)(double_queue **), char *name );
int test_one_element_double_queue   ( int (*double_queue_constructor)(double_queue **), char *name, void **elements );
int test_two_element_double_queue   ( int (*double_queue_constructor)(double_queue **), char *name, void **elements );
int test_three_element_double_queue ( int (*double_queue_constructor)(double_queue **), char *name, void **elements );

int construct_empty                 ( double_queue **pp_double_queue );
int construct_empty_front_addA_A    ( double_queue **pp_double_queue );
int construct_empty_rear_addA_A     ( double_queue **pp_double_queue );
int construct_empty_rear_addB_B     ( double_queue **pp_double_queue );
int construct_A_front_remove_empty  ( double_queue **pp_double_queue ); 
int construct_A_rear_remove_empty   ( double_queue **pp_double_queue ); 
int construct_A_front_addB_BA       ( double_queue **pp_double_queue ); 
int construct_A_rear_addB_AB        ( double_queue **pp_double_queue ); 
int construct_AB_front_remove_B     ( double_queue **pp_double_queue );
int construct_AB_rear_remove_A      ( double_queue **pp_double_queue );
int construct_BA_front_remove_A     ( double_queue **pp_double_queue );
int construct_BA_rear_remove_B      ( double_queue **pp_double_queue );
int construct_AB_rear_addC_ABC      ( double_queue **pp_double_queue );
int construct_BA_front_addC_CBA     ( double_queue **pp_double_queue );

// entry point
int main ( int argc, const char* argv[] )
{

    // unused
	(void) argc;
	(void) argv;

    // initialized data
    timestamp t0 = 0,
              t1 = 0;

    // Formatting
    printf(
        "╭─────────────────────╮\n"
        "│ double queue tester │\n"
        "╰─────────────────────╯\n\n"
    );
    
    // Start
    t0 = timer_high_precision();

    // Run tests
    run_tests();

    // Stop
    t1 = timer_high_precision();

    // Report the time it took to run the tests
    log_info("double queue took ");
    print_time_pretty ( (double)(t1-t0)/(double)timer_seconds_divisor() );
    log_info(" to test\n");

    // exit
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
    
    // success
    return 1;
}

int run_tests ( void )
{

    // ... -> []
    test_empty_double_queue(construct_empty, "empty");

    // [] -> front_add(A) -> [A]
    test_one_element_double_queue(construct_empty_front_addA_A, "empty_front_addA_A", (void **) A_contents);

    // [] -> rear_add(A) -> [A]
    test_one_element_double_queue(construct_empty_rear_addA_A, "empty_rear_addA_A", (void **) A_contents);

    // [] -> rear_add(B) -> [B]
    test_one_element_double_queue(construct_empty_rear_addB_B, "empty_rear_addB_B", (void **) B_contents);

    // [A] -> front_remove() -> []
    test_empty_double_queue(construct_A_front_remove_empty, "A_front_remove_empty");

    // [A] -> rear_remove() -> []
    test_empty_double_queue(construct_A_rear_remove_empty, "A_rear_remove_empty");

    // [A] -> front_add(B) -> [B,A]
    test_two_element_double_queue(construct_A_front_addB_BA, "A_front_addB_BA", (void **) BA_contents);
    
    // [A] -> rear_add(B) -> [A,B]
    test_two_element_double_queue(construct_A_rear_addB_AB, "A_rear_addB_AB", (void **) AB_contents);

    // [A,B] -> front_remove() -> [B]
    test_one_element_double_queue(construct_AB_front_remove_B, "AB_front_remove_B", (void **) B_contents);

    // [A,B] -> rear_remove() -> [A]
    test_one_element_double_queue(construct_AB_rear_remove_A, "AB_rear_remove_A", (void **) A_contents);

    // [B,A] -> front_remove() -> [A]
    test_one_element_double_queue(construct_BA_front_remove_A, "BA_front_remove_A", (void **) A_contents);

    // [B,A] -> rear_remove() -> [B]
    test_one_element_double_queue(construct_BA_rear_remove_B, "BA_rear_remove_B", (void **) B_contents);

    // [A,B] -> rear_add(C) -> [A,B,C]
    test_three_element_double_queue(construct_AB_rear_addC_ABC, "AB_rear_addC_ABC", (void **) ABC_contents);
    
    // [B,A] -> front_add(C) -> [C,B,A]
    test_three_element_double_queue(construct_BA_front_addC_CBA, "BA_front_addC_CBA", (void **) CBA_contents);

    // success
    return 1;
}

int construct_empty ( double_queue **pp_double_queue )
{
    double_queue_construct(pp_double_queue);
    return 1;
}

int construct_empty_front_addA_A ( double_queue **pp_double_queue )
{
    construct_empty(pp_double_queue);
    double_queue_front_add(*pp_double_queue, A_element);
    return 1;
}

int construct_empty_rear_addA_A ( double_queue **pp_double_queue )
{
    construct_empty(pp_double_queue);
    double_queue_rear_add(*pp_double_queue, A_element);
    return 1;
}

int construct_empty_rear_addB_B ( double_queue **pp_double_queue )
{
    construct_empty(pp_double_queue);
    double_queue_rear_add(*pp_double_queue, B_element);
    return 1;
}

int construct_A_front_remove_empty ( double_queue **pp_double_queue )
{
    construct_empty_front_addA_A(pp_double_queue);
    double_queue_front_remove(*pp_double_queue, (void **)0);
    return 1;
}

int construct_A_rear_remove_empty ( double_queue **pp_double_queue )
{
    construct_empty_front_addA_A(pp_double_queue);
    double_queue_rear_remove(*pp_double_queue, (void **)0);
    return 1;
}

int construct_A_front_addB_BA ( double_queue **pp_double_queue )
{
    construct_empty_front_addA_A(pp_double_queue);
    double_queue_front_add(*pp_double_queue, B_element);
    return 1;
}

int construct_A_rear_addB_AB ( double_queue **pp_double_queue )
{
    construct_empty_front_addA_A(pp_double_queue);
    double_queue_rear_add(*pp_double_queue, B_element);
    return 1;
}

int construct_AB_front_remove_B ( double_queue **pp_double_queue )
{
    construct_A_rear_addB_AB(pp_double_queue);
    double_queue_front_remove(*pp_double_queue, (void **)0);
    return 1;
}

int construct_AB_rear_remove_A ( double_queue **pp_double_queue )
{
    construct_A_rear_addB_AB(pp_double_queue);
    double_queue_rear_remove(*pp_double_queue, (void **)0);
    return 1;
}

int construct_BA_front_remove_A ( double_queue **pp_double_queue )
{
    construct_A_front_addB_BA(pp_double_queue);
    double_queue_front_remove(*pp_double_queue, (void **)0);
    return 1;
}

int construct_BA_rear_remove_B ( double_queue **pp_double_queue )
{
    construct_A_front_addB_BA(pp_double_queue);
    double_queue_rear_remove(*pp_double_queue, (void **)0);
    return 1;
}

int construct_AB_rear_addC_ABC ( double_queue **pp_double_queue )
{
    construct_A_rear_addB_AB(pp_double_queue);
    double_queue_rear_add(*pp_double_queue, C_element);
    return 1;
}

int construct_BA_front_addC_CBA ( double_queue **pp_double_queue )
{
    construct_A_front_addB_BA(pp_double_queue);
    double_queue_front_add(*pp_double_queue, C_element);
    return 1;
}

int test_empty_double_queue ( int (*double_queue_constructor)(double_queue **pp_double_queue), char *name )
{
    log_scenario("%s\n", name);

    print_test(name, "double_queue_front"       , test_front(double_queue_constructor, (void *)0, zero) );
    print_test(name, "double_queue_rear"        , test_rear(double_queue_constructor, (void *)0, zero) );
    print_test(name, "double_queue_front_add"   , test_front_add(double_queue_constructor, A_element, one) );
    print_test(name, "double_queue_rear_add"    , test_rear_add(double_queue_constructor, A_element, one) );
    print_test(name, "double_queue_front_remove", test_front_remove(double_queue_constructor, (void *)0, 1, Underflow) );
    print_test(name, "double_queue_rear_remove" , test_rear_remove(double_queue_constructor, (void *)0, 1, Underflow) );
    print_test(name, "double_queue_empty"       , test_empty(double_queue_constructor, True) );

    print_final_summary();

    return 1;
}

int test_one_element_double_queue ( int (*double_queue_constructor)(double_queue **), char *name, void **elements )
{
    log_scenario("%s\n", name);

    print_test(name, "double_queue_front"       , test_front(double_queue_constructor, elements[0], match) );
    print_test(name, "double_queue_rear"        , test_rear(double_queue_constructor, elements[0], match) );
    print_test(name, "double_queue_front_add"   , test_front_add(double_queue_constructor, D_element, one) );
    print_test(name, "double_queue_rear_add"    , test_rear_add(double_queue_constructor, D_element, one) );
    print_test(name, "double_queue_front_remove", test_front_remove(double_queue_constructor, elements[0], 1, match) );
    print_test(name, "double_queue_rear_remove" , test_rear_remove(double_queue_constructor, elements[0], 1, match) );
    print_test(name, "double_queue_empty"       , test_empty(double_queue_constructor, False) );

    print_final_summary();

    return 1;
}

int test_two_element_double_queue ( int (*double_queue_constructor)(double_queue **), char *name, void **elements )
{
    log_scenario("%s\n", name);

    print_test(name, "double_queue_front"       , test_front(double_queue_constructor, elements[0], match) );
    print_test(name, "double_queue_rear"        , test_rear(double_queue_constructor, elements[1], match) );
    print_test(name, "double_queue_front_add"   , test_front_add(double_queue_constructor, D_element, one) );
    print_test(name, "double_queue_rear_add"    , test_rear_add(double_queue_constructor, D_element, one) );
    
    print_test(name, "double_queue_front_remove", test_front_remove(double_queue_constructor, elements[0], 1, match) );
    print_test(name, "double_queue_rear_remove" , test_rear_remove(double_queue_constructor, elements[1], 1, match) );

    print_test(name, "double_queue_empty"       , test_empty(double_queue_constructor, False) );

    print_final_summary();

    return 1;
}

int test_three_element_double_queue ( int (*double_queue_constructor)(double_queue **), char *name, void **elements )
{
    log_scenario("%s\n", name);

    print_test(name, "double_queue_front"       , test_front(double_queue_constructor, elements[0], match) );
    print_test(name, "double_queue_rear"        , test_rear(double_queue_constructor, elements[2], match) );
    print_test(name, "double_queue_front_add"   , test_front_add(double_queue_constructor, D_element, one) );
    print_test(name, "double_queue_rear_add"    , test_rear_add(double_queue_constructor, D_element, one) );
    
    print_test(name, "double_queue_front_remove", test_front_remove(double_queue_constructor, elements[0], 1, match) );
    print_test(name, "double_queue_rear_remove" , test_rear_remove(double_queue_constructor, elements[2], 1, match) );

    print_test(name, "double_queue_empty"       , test_empty(double_queue_constructor, False) );

    print_final_summary();

    return 1;
}

int print_test ( const char *scenario_name, const char *test_name, bool passed )
{
    if ( passed ) 
        log_pass("%s %s\n", scenario_name, test_name);
    else
        log_fail("%s %s\n", scenario_name, test_name);

    if (passed)
        ephemeral_passes++;
    else
        ephemeral_fails++;

    ephemeral_tests++;

    return 1;
}

int print_final_summary ( void )
{
    total_tests  += ephemeral_tests;
    total_passes += ephemeral_passes;
    total_fails  += ephemeral_fails;

    log_info("\nTests: %d, Passed: %d, Failed: %d (%%%.3f)\n",  ephemeral_tests, ephemeral_passes, ephemeral_fails, ((float)ephemeral_passes/(float)ephemeral_tests*100.f));
    log_info("Total: %d, Passed: %d, Failed: %d (%%%.3f)\n\n",  total_tests, total_passes, total_fails, ((float)total_passes/(float)total_tests*100.f));

    ephemeral_tests  = 0;
    ephemeral_passes = 0;
    ephemeral_fails  = 0;

    return 1;
}

bool test_front ( int (*double_queue_constructor)(double_queue **), void *expected_value, result_t expected )
{
    result_t      result       = 0;
    double_queue *p_double_queue = 0;
    void         *result_value = 0;

    double_queue_constructor(&p_double_queue);

    result = (result_t) double_queue_front(p_double_queue, &result_value);

    if (result == zero)
        goto exit;
    else if (result_value == expected_value)
        result = match;
    else
        result = one;

    exit:
    double_queue_destroy(&p_double_queue);
    return (result == expected);
}

bool test_rear ( int (*double_queue_constructor)(double_queue **), void *expected_value, result_t expected )
{
    result_t      result       = 0;
    double_queue *p_double_queue = 0;
    void         *result_value = 0;

    double_queue_constructor(&p_double_queue);

    result = (result_t) double_queue_rear(p_double_queue, &result_value);

    if (result == zero)
        goto exit;
    else if (result_value == expected_value)
        result = match;
    else
        result = zero;

    exit:
    double_queue_destroy(&p_double_queue);
    return (result == expected);
}

bool test_front_add ( int (*double_queue_constructor)(double_queue **), void *value, result_t expected )
{
    result_t      result = 0;
    double_queue *p_double_queue = 0;

    double_queue_constructor(&p_double_queue);

    result = (result_t) double_queue_front_add(p_double_queue, value);

    double_queue_destroy(&p_double_queue);
    return (result == expected);
}

bool test_rear_add ( int (*double_queue_constructor)(double_queue **), void *value, result_t expected )
{
    result_t      result = 0;
    double_queue *p_double_queue = 0;

    double_queue_constructor(&p_double_queue);

    result = (result_t) double_queue_rear_add(p_double_queue, value);

    double_queue_destroy(&p_double_queue);
    return (result == expected);
}

bool test_front_remove ( int (*double_queue_constructor)(double_queue **), void *expected_value, size_t num_removes, result_t expected )
{
    result_t      result = 0;
    double_queue *p_double_queue = 0;
    void         *result_value = 0;

    double_queue_constructor(&p_double_queue);
    
    for (size_t i = 0; i < num_removes; i++)
        result = (result_t) double_queue_front_remove(p_double_queue, &result_value);
    
    if (result == Underflow)
        goto exit;
    else if (result_value == expected_value)
        result = match;
    else
        result = zero;

    exit:
    double_queue_destroy(&p_double_queue);
    return (result == expected);
}

bool test_rear_remove ( int (*double_queue_constructor)(double_queue **), void *expected_value, size_t num_removes, result_t expected )
{
    result_t      result = 0;
    double_queue *p_double_queue = 0;
    void         *result_value = 0;

    double_queue_constructor(&p_double_queue);
    
    for (size_t i = 0; i < num_removes; i++)
        result = (result_t) double_queue_rear_remove(p_double_queue, &result_value);
    
    if (result == Underflow)
        goto exit;
    else if (result_value == expected_value)
        result = match;
    else
        result = zero;

    exit:
    double_queue_destroy(&p_double_queue);
    return (result == expected);
}

bool test_empty ( int (*double_queue_constructor)(double_queue **), result_t expected )
{
    result_t      result = 0;
    double_queue *p_double_queue = 0;

    double_queue_constructor(&p_double_queue);

    result = double_queue_empty(p_double_queue);

    double_queue_destroy(&p_double_queue);
    return (result == expected);
}
