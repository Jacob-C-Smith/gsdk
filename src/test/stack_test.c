/** !
 * stack library tester
 * 
 * @file stack_tester.c
 *
 * @author Jacob Smith
*/

// Include
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// core
#include <core/log.h>
#include <core/sync.h>

// data
#include <data/stack.h>

// Possible values
void *A_value = (void *) 0x0000000000000001,
     *B_value = (void *) 0x0000000000000002,
     *C_value = (void *) 0x0000000000000003,
     *X_value = (void *) 0xFFFFFFFFFFFFFFFF;

// Possible keys
char *A_key   = "A",
     *B_key   = "B",
     *C_key   = "C",
     *X_key   = "X";

// Expected results
char  *_keys     [] = { 0x0 };
char  *A_keys    [] = { "A", 0x0 };
char  *AB_keys   [] = { "A", "B", 0x0 };
char  *ABC_keys  [] = { "A", "B", "C", 0x0 };

// Test results
enum result_e {
    zero,
    one,
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

int print_time_pretty ( double seconds );

bool test_push ( int (*stack_constructor)(stack **), char  *value         , result_t expected );
bool test_peek ( int (*stack_constructor)(stack **), char  *expected_value, result_t expected );
bool test_pop  ( int (*stack_constructor)(stack **), char  *expected_value, size_t   pops,    result_t expected );

int test_empty_stack         ( int (*stack_constructor)(stack **), char *name );
int test_one_element_stack   ( int (*stack_constructor)(stack **), char *name, char **keys );
int test_two_element_stack   ( int (*stack_constructor)(stack **), char *name, char **keys );
int test_three_element_stack ( int (*stack_constructor)(stack **), char *name, char **keys );

int construct_empty         ( stack **pp_stack );
int construct_empty_pushA_A ( stack **pp_stack );
int construct_A_pop_empty   ( stack **pp_stack );
int construct_A_pushB_BA    ( stack **pp_stack );
int construct_AB_pop_A      ( stack **pp_stack );
int construct_AB_pushC_ABC  ( stack **pp_stack );
int construct_ABC_pop_AB    ( stack **pp_stack );

// entry point
int main ( int argc, const char* argv[] )
{

    // initialized data
    timestamp t0 = 0,
              t1 = 0;

    // unused
    (void) argc;
    (void) argv;

    // Formatting
    printf(
        "╭──────────────╮\n"\
        "│ stack tester │\n"\
        "╰──────────────╯\n\n"
    );

    // Start
    t0 = timer_high_precision();

    // Run tests
    run_tests();

    // Stop
    t1 = timer_high_precision();

    // Report the time it took to run the tests
    log_info("stack took ");
    print_time_pretty ( (double)(t1-t0)/(double)timer_seconds_divisor() );
    log_info(" to test\n");

    // Flush stdio
    fflush(stdout);

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
        log_info("%zd D, ", days);
    
    // Print hours
    if ( hours )
        log_info("%zd h, ", hours);

    // Print minutes
    if ( minutes )
        log_info("%zd m, ", minutes);

    // Print seconds
    if ( __seconds )
        log_info("%zd s, ", __seconds);
    
    // Print milliseconds
    if ( milliseconds )
        log_info("%zd ms, ", milliseconds);
    
    // Print microseconds
    if ( microseconds )
        log_info("%zd us", microseconds);
    
    // success
    return 1;
}

int run_tests ( void )
{

    // stack notation: bottom is left, top is right.

    // ... -> [ _, _, _ ]
    test_empty_stack(construct_empty, "empty");

    // [ _, _, _ ] -> push(A) -> [ A, _, _ ]
    test_one_element_stack(construct_empty_pushA_A, "empty_pushA_A", (char **)A_keys);

    // [ A, _, _ ] -> pop() -> [ _, _, _ ]
    test_empty_stack(construct_A_pop_empty, "A_pop_empty");

    // [ A, _, _ ] -> push(B) -> [ A, B, _ ]
    test_two_element_stack(construct_A_pushB_BA, "A_pushB_AB", (char **)AB_keys);

    // [ A, B, _ ] -> pop() -> [ A, _, _ ]
    test_one_element_stack(construct_AB_pop_A, "AB_pop_A", (char **)A_keys);

    // [ A, B, _ ] -> push(C) -> [ A, B, C ]
    test_three_element_stack(construct_AB_pushC_ABC, "AB_pushC_ABC", (char **)ABC_keys);

    // [ A, B, C ] -> pop() -> [ A, B, _ ]
    test_two_element_stack(construct_ABC_pop_AB, "ABC_pop_AB", (char **)AB_keys);

    // success
    return 1;
}

int construct_empty ( stack **pp_stack )
{
    // Construct a stack
    stack_construct(pp_stack, 3);

    // stack = [ _, _, _ ]
    return 1;
}

int construct_A_pop_empty ( stack **pp_stack )
{

    // Construct a [ A, _, _ ] stack
    construct_AB_pop_A(pp_stack);

    // pop ()
    stack_pop(*pp_stack, 0);

    // stack = [ _, _, _ ]
    // success
    return 1;

}

int construct_empty_pushA_A ( stack **pp_stack )
{

    // Construct a [ _, _, _ ] stack
    construct_empty(pp_stack);

    // push (A)
    stack_push(*pp_stack, A_key);

    // stack = [ A, _, _ ]
    // success
    return 1;
}

int construct_A_pushB_BA ( stack **pp_stack )
{

    // Construct a [ A, _, _ ] stack
    construct_empty_pushA_A(pp_stack);

    // push (B)
    stack_push(*pp_stack, B_key);

    // stack = [ A, B, _ ]
    // success
    return 1;
}

int construct_AB_pop_A ( stack **pp_stack )
{
    // Construct a [ A, B, _ ] stack
    construct_ABC_pop_AB(pp_stack);

    // pop ()
    stack_pop(*pp_stack, 0);

    // stack = [ A, _, _ ]
    // success
    return 1;
}

int construct_AB_pushC_ABC ( stack **pp_stack )
{

    // Construct a [ A, B, _ ] stack
    construct_A_pushB_BA(pp_stack);

    // push (C)
    stack_push(*pp_stack, C_key);

    // stack = [ A, B, C ]
    // success
    return 1;
}

int construct_ABC_pop_AB ( stack **pp_stack )
{

    // Construct a [ A, B, C ] stack
    construct_AB_pushC_ABC(pp_stack);

    // pop ()
    stack_pop(*pp_stack, 0);

    // stack = [ A, B, _ ]
    // success
    return 1;
}

int test_empty_stack ( int (*stack_constructor)(stack **pp_stack), char *name )
{

    // Print the name of the scenario
    log_scenario("%s\n", name);

    print_test(name, "stack_push_A", test_push(stack_constructor, A_key, one) );
    print_test(name, "stack_pop"   , test_pop (stack_constructor, (void *)0, 1, zero) );
    print_test(name, "stack_peek"  , test_peek(stack_constructor, (void *)0, zero) );

    print_final_summary();

    // success
    return 1;
}

int test_one_element_stack ( int (*stack_constructor)(stack **), char *name, char **keys )
{

    // Print the name of the scenario
    log_scenario("%s\n", name);

    print_test(name, "stack_push_B" , test_push(stack_constructor, B_key, one) );
    print_test(name, "stack_pop"    , test_pop(stack_constructor, keys[0], 1, match) );
    print_test(name, "stack_pop_pop", test_pop(stack_constructor, (void *)0, 2, zero) );
    print_test(name, "stack_peek"   , test_peek(stack_constructor, keys[0], match) );

    print_final_summary();

    // success
    return 1;
}

int test_two_element_stack ( int (*stack_constructor)(stack **), char *name, char **keys )
{

    // Print the name of the scenario
    log_scenario("%s\n", name);

    print_test(name, "stack_push_C"     , test_push(stack_constructor, C_key, one) );
    print_test(name, "stack_pop"        , test_pop(stack_constructor, keys[1], 1, match) );
    print_test(name, "stack_pop_pop"    , test_pop(stack_constructor, keys[0], 2, match) );
    print_test(name, "stack_pop_pop_pop", test_pop(stack_constructor, (void *)0, 3, zero) );
    print_test(name, "stack_peek"       , test_peek(stack_constructor, keys[1], match) );

    print_final_summary();

    // success
    return 1;
}

int test_three_element_stack ( int (*stack_constructor)(stack **), char *name, char **keys )
{

    // Print the name of the scenario
    log_scenario("%s\n", name);

    print_test(name, "stack_push_X"         , test_push(stack_constructor, X_key, zero) );
    print_test(name, "stack_pop"            , test_pop(stack_constructor, keys[2], 1, match) );
    print_test(name, "stack_pop_pop"        , test_pop(stack_constructor, keys[1], 2, match) );
    print_test(name, "stack_pop_pop_pop"    , test_pop(stack_constructor, keys[0], 3, match) );
    print_test(name, "stack_pop_pop_pop_pop", test_pop(stack_constructor, (void *)0, 4, zero) );
    print_test(name, "stack_peek"           , test_peek(stack_constructor, keys[2], match) );

    print_final_summary();

    // success
    return 1;
}

int print_test ( const char *scenario_name, const char *test_name, bool passed )
{ 

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

    ephemeral_tests  = 0;
    ephemeral_passes = 0;
    ephemeral_fails  = 0;

    // success
    return 1;
}

bool test_push ( int (*stack_constructor)(stack **), char *value, result_t expected )
{

    // initialized data
    result_t  result  = 0;
    stack    *p_stack = 0;

    // Build the stack
    stack_constructor(&p_stack);

    result = (result_t) stack_push(p_stack, value);

    // Free the stack
    stack_destroy(&p_stack);

    // Return result
    return (result == expected);
}

bool test_peek ( int (*stack_constructor)(stack **), char *expected_value, result_t expected )
{

    // argument check
    //if ( expected_value == (void *) 0 ) return false;

    // initialized data
    result_t    result       = 0;
    stack      *p_stack      = 0;
    const void *result_value = 0;

    // Build the stack
    stack_constructor(&p_stack);

    // Peek the stack
    result = (result_t) stack_peek(p_stack, &result_value);

    if (result == zero)
        goto done;
    else if ( strcmp(result_value, expected_value) == 0 )
        result = match;

    done:
    // Free the stack
    stack_destroy(&p_stack);

    // Return result
    return (result == expected);
}

bool test_pop ( int (*stack_constructor)(stack **), char *expected_value, size_t pops, result_t expected )
{

    // argument check
    //if ( expected_value == (void *) 0 ) return false;

    // initialized data
    result_t    result       = 0;
    stack      *p_stack      = 0;
    const void *result_value = 0;

    // Build the stack
    stack_constructor(&p_stack);

    // Pop N times
    for (size_t i = 0; i < pops; i++)
    {
        result = (result_t) stack_pop(p_stack, &result_value);

        if ( result == zero ) goto done;
    }

    if ( result == zero )
        goto done;
    else if ( strcmp(result_value, expected_value) == 0 )
        result = match;

    done:
    // Free the stack
    stack_destroy(&p_stack);

    // Return result
    return (result == expected);
}
