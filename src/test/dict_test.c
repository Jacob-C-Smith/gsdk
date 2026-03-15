/** !
 * Dictionary tester
 * 
 * @file src/test/dict_test.c
 * 
 * @author Jacob Smith
*/

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// gsdk
/// core
#include <core/log.h>
#include <core/sync.h>
#include <core/hash.h>
#include <core/pack.h>
#include <core/interfaces.h>

/// data
#include <data/dict.h>

// type definitions
typedef int (fn_dict_constructor)(dict **);

// keys
const char *const A_value   = "A",
           *const B_value   = "B",
           *const C_value   = "C",
           *const D_value   = "D",
           *const X_value   = "X";

// expected results
const void *const _values   [] = { NULL };
const void *const A_values  [] = { "A", NULL };
const void *const B_values  [] = { "B", NULL };
const void *const C_values  [] = { "C", NULL };
const void *const AB_values [] = { "A", "B", NULL };
const void *const BC_values [] = { "B", "C", NULL };
const void *const AC_values [] = { "A", "C", NULL };
const void *const ABC_values[] = { "A", "B", "C", NULL };

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
int print_time_pretty   ( double seconds );
int run_tests           ( void );
int print_final_summary ( void );
int print_test          ( const char  *scenario_name, const char *test_name, bool passed );

bool test_add    ( fn_dict_constructor *dict_constructor, const void *const value, result_t expected );
bool test_get    ( fn_dict_constructor *dict_constructor, char *key, const void *const expected_value, result_t expected );
bool test_size   ( fn_dict_constructor *dict_constructor, size_t size, result_t  expected );
bool test_values ( fn_dict_constructor *dict_constructor, const void *const *expected_values, result_t expected );
bool test_pop    ( fn_dict_constructor *dict_constructor, char *key, const void *const expected_value, result_t expected );

int test_empty_dict         ( fn_dict_constructor *dict_constructor, char *name );
int test_one_element_dict   ( fn_dict_constructor *dict_constructor, char *name, const void *const *values );
int test_two_element_dict   ( fn_dict_constructor *dict_constructor, char *name, const void *const *values );
int test_three_element_dict ( fn_dict_constructor *dict_constructor, char *name, const void *const *values );

fn_dict_constructor construct_empty;
fn_dict_constructor construct_empty_addA_A;
fn_dict_constructor construct_empty_addB_B;
fn_dict_constructor construct_empty_addC_C;
fn_dict_constructor construct_A_popA_empty;
fn_dict_constructor construct_A_addB_AB; 
fn_dict_constructor construct_AB_popA_B; 
fn_dict_constructor construct_AB_popB_A; 
fn_dict_constructor construct_AB_addC_ABC;
fn_dict_constructor construct_ABC_popA_BC;
fn_dict_constructor construct_ABC_popB_AC;
fn_dict_constructor construct_ABC_popC_AB;
fn_dict_constructor construct_empty_fromkeysABC_ABC;
fn_dict_constructor construct_empty_fromkeysAB_AB;
fn_dict_constructor construct_empty_fromkeysA_A;

// entry point
int main ( int argc, const char* argv[] )
{

    // suppress warnings
    (void) argc;
    (void) argv;    

    // initialized data
    timestamp t0 = 0,
              t1 = 0;

    // formatting
    printf(
        "╭─────────────╮\n"\
        "│ dict tester │\n"\
        "╰─────────────╯\n\n");

    // start
    t0 = timer_high_precision();

    // run tests
    run_tests();

    // stop
    t1 = timer_high_precision();

    // report the time it took to run the tests
    log_info("dict took ");
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

    // days
    while ( _seconds > 86400.0 ) { days++;_seconds-=286400.0; };

    // hours
    while ( _seconds > 3600.0 ) { hours++;_seconds-=3600.0; };

    // minutes
    while ( _seconds > 60.0 ) { minutes++;_seconds-=60.0; };

    // seconds
    while ( _seconds > 1.0 ) { __seconds++;_seconds-=1.0; };

    // milliseconds
    while ( _seconds > 0.001 ) { milliseconds++;_seconds-=0.001; };

    // microseconds        
    while ( _seconds > 0.000001 ) { microseconds++;_seconds-=0.000001; };

    // print days
    if ( days ) log_info("%d D, ", days);
    
    // print hours
    if ( hours ) log_info("%d h, ", hours);

    // print minutes
    if ( minutes ) log_info("%d m, ", minutes);

    // print seconds
    if ( __seconds ) log_info("%d s, ", __seconds);
    
    // print milliseconds
    if ( milliseconds ) log_info("%d ms, ", milliseconds);
    
    // print microseconds
    if ( microseconds ) log_info("%d us", microseconds);
    
    // success
    return 1;
}

int run_tests ( void )
{

    // ... -> []
    test_empty_dict(construct_empty, "empty");

    // [] -> add(A) -> [A]
    test_one_element_dict(construct_empty_addA_A, "empty_addA_A", A_values);

    // [] -> add(B) -> [B]
    test_one_element_dict(construct_empty_addB_B, "empty_addB_B", B_values);

    // [] -> add(C) -> [C]
    test_one_element_dict(construct_empty_addC_C, "empty_addC_C", C_values);

    // [A] -> pop(A) -> []
    test_empty_dict(construct_A_popA_empty, "A_popA_empty");

    // [A] -> add(B) -> [A,B]
    test_two_element_dict(construct_A_addB_AB, "A_addB_AB", AB_values);
    
    // [A,B] -> pop(A) -> [B]
    test_one_element_dict(construct_AB_popA_B, "AB_popA_B", B_values);

    // [A,B] -> pop(B) -> [A]
    test_one_element_dict(construct_AB_popB_A, "AB_popB_A", A_values);

    // [A,B] -> add(C) -> [A,B,C]
    test_three_element_dict(construct_AB_addC_ABC, "AB_addC_ABC", ABC_values);
    
    // [A,B,C] -> pop(A) -> [B,C]
    test_two_element_dict(construct_ABC_popA_BC, "ABC_popA_BC", BC_values);

    // [A,B,C] -> pop(B) -> [A,C]
    test_two_element_dict(construct_ABC_popB_AC, "ABC_popB_AC", AC_values);

    // [A,B,C] -> pop(C) -> [A,B]
    test_two_element_dict(construct_ABC_popC_AB, "ABC_popC_AB", AB_values);

    // success
    return 1;
}

int construct_empty ( dict **pp_dict )
{

    // construct a dict
    dict_construct(pp_dict, 1, NULL, NULL, NULL);

    // dict = []
    return 1;
}

int construct_empty_addA_A(dict **pp_dict)
{

    // Construct a [] dict
    construct_empty(pp_dict);

    // add(A)
    dict_add(*pp_dict, A_value);
    
    // dict = [A]
    // success
    return 1;
}

int construct_empty_addB_B(dict **pp_dict)
{

    // Construct a [] dict
    construct_empty(pp_dict);

    // add(B)
    dict_add(*pp_dict, B_value);
    
    // dict = [B]
    // success
    return 1;
}

int construct_empty_addC_C(dict **pp_dict)
{

    // Construct a [] dict
    construct_empty(pp_dict);

    // add(C)
    dict_add(*pp_dict, C_value);
    
    // dict = [C]
    // success
    return 1;
}

int construct_A_popA_empty(dict **pp_dict)
{

    // Construct a [A] dict
    construct_empty_addA_A(pp_dict);

    // pop(A)
    dict_pop(*pp_dict, "A", 0);
    
    // dict = []
    // success
    return 1;
}

int construct_A_addB_AB( dict **pp_dict )
{

    // Construct an [A] dict
    construct_empty_addA_A(pp_dict);

    // add(B)
    dict_add(*pp_dict, B_value);
    
    // dict = [A, B]
    // success
    return 1;
}

int construct_AB_popA_B ( dict **pp_dict )
{
    // Construct an [A, B] dict
    construct_A_addB_AB(pp_dict);

    // pop(A)
    dict_pop(*pp_dict, "A", 0);
    
    // dict = [B]
    // success
    return 1;
}

int construct_AB_popB_A ( dict **pp_dict )
{
    // Construct an [A, B] dict
    construct_A_addB_AB(pp_dict);

    // pop(B)
    dict_pop(*pp_dict, "B", 0);
    
    // dict = [A]
    // success
    return 1;
}

int construct_AB_addC_ABC ( dict **pp_dict )
{

    // Construct an [A, B] dict
    construct_A_addB_AB(pp_dict);

    // add(C)
    dict_add(*pp_dict, C_value);

    // dict = [A, B, C]
    // success
    return 1;
}

int construct_ABC_popA_BC  ( dict **pp_dict )
{
    // Construct an [A, B, C] dict
    construct_AB_addC_ABC(pp_dict);

    // pop(A)
    dict_pop(*pp_dict, "A", 0);

    // dict = [B, C]
    // success
    return 1;
}

int construct_ABC_popB_AC  ( dict **pp_dict )
{
    // Construct an [A, B, C] dict
    construct_AB_addC_ABC(pp_dict);

    // pop(B)
    dict_pop(*pp_dict, "B", 0);

    // dict = [A, C]
    // success
    return 1;
}

int construct_ABC_popC_AB  ( dict **pp_dict )
{
    // Construct an [A, B, C] dict
    construct_AB_addC_ABC(pp_dict);

    // pop(C)
    dict_pop(*pp_dict, "C", 0);

    // dict = [A, B]
    // success
    return 1;
}

int test_empty_dict(fn_dict_constructor *dict_constructor, char *name)
{

    log_scenario("%s\n", name);

    print_test(name, "dict_add_D"      , test_add(dict_constructor, D_value, one) );
    print_test(name, "dict_add_(null)" , test_add(dict_constructor, NULL, zero) );
    print_test(name, "dict_get_X"      , test_get(dict_constructor, "X", X_value, zero) );    
    print_test(name, "dict_values"     , test_values(dict_constructor, 0, zero) );
    print_test(name, "dict_pop_X"      , test_pop(dict_constructor, "X", X_value, zero) );
    print_test(name, "dict_pop_(null)" , test_pop(dict_constructor, NULL, X_value, zero) );

    print_final_summary();

    return 1;
}

int test_one_element_dict ( fn_dict_constructor *dict_constructor, char *name, const void *const *values )
{

    log_scenario("%s\n", name);

    print_test(name, "dict_add_D"      , test_add(dict_constructor, D_value, one) );
    print_test(name, "dict_add_(null)" , test_add(dict_constructor, NULL, zero) );
    print_test(name, "dict_get_X"      , test_get(dict_constructor, "X", X_value, zero) );    
    print_test(name, "dict_values"     , test_values(dict_constructor, values, match) );
    print_test(name, "dict_pop_X"      , test_pop(dict_constructor, "X", X_value, zero) );
    print_test(name, "dict_pop_(null)" , test_pop(dict_constructor, NULL, X_value, zero) );

    print_final_summary();

    // success
    return 1;
}

int test_two_element_dict ( fn_dict_constructor *dict_constructor, char *name, const void *const *values )
{

    log_scenario("%s\n", name);

    print_test(name, "dict_add_D"      , test_add(dict_constructor, D_value, one) );
    print_test(name, "dict_add_(null)" , test_add(dict_constructor, NULL, zero) );
    print_test(name, "dict_get_X"      , test_get(dict_constructor, "X", X_value, zero) );    
    print_test(name, "dict_values"     , test_values(dict_constructor, values, match) );
    print_test(name, "dict_pop_X"      , test_pop(dict_constructor, "X", X_value, zero) );
    print_test(name, "dict_pop_(null)" , test_pop(dict_constructor, NULL, X_value, zero) );

    print_final_summary();

    // success
    return 1;
}

int test_three_element_dict ( fn_dict_constructor *dict_constructor, char *name, const void *const *values )
{
    
    log_scenario("%s\n", name);

    print_test(name, "dict_add_D"      , test_add(dict_constructor, D_value, one) );
    print_test(name, "dict_add_(null)" , test_add(dict_constructor, NULL, zero) );
    print_test(name, "dict_get_X"      , test_get(dict_constructor, "X", X_value, zero) );    
    print_test(name, "dict_values"     , test_values(dict_constructor, values, match) );
    print_test(name, "dict_pop_X"      , test_pop(dict_constructor, "X", X_value, zero) );
    print_test(name, "dict_pop_(null)" , test_pop(dict_constructor, NULL, X_value, zero) );

    print_final_summary();

    // success
    return 1;
}

int print_test ( const char *scenario_name, const char *test_name, bool passed )
{

    // Log the success/failure
    if   ( passed ) log_pass("%s %s\n", scenario_name, test_name);
    else            log_fail("%s %s\n", scenario_name, test_name);

    // Increment the counters
    if   ( passed ) ephemeral_passes++;
    else            ephemeral_fails++;

    // Increment the ephemeral test counter
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
    
    log_info("\nTests: %d, Passed: %d, Failed: %d (%%%.3f)\n",  ephemeral_tests, ephemeral_passes, ephemeral_fails, ((float)ephemeral_passes/(float)ephemeral_tests*100.f));
    log_info("Total: %d, Passed: %d, Failed: %d (%%%.3f)\n\n",  total_tests, total_passes, total_fails, ((float)total_passes/(float)total_tests*100.f));

    ephemeral_tests  = 0,
    ephemeral_passes = 0,
    ephemeral_fails  = 0;

    // success
    return 1;
}

bool test_add ( fn_dict_constructor *dict_constructor, const void *const value, result_t expected )
{

    // initialized data
    result_t  result = 0;
    dict     *p_dict = 0;

    // build the dict
    dict_constructor(&p_dict);

    // store the result
    result = (result_t) dict_add(p_dict, value);

    // release the dict
    dict_destroy(&p_dict, 0);

    // return result
    return (result == expected);
}

bool test_get ( fn_dict_constructor *dict_constructor, char *key, const void *const expected_value, result_t expected )
{

    // initialized data
    result_t    result       = 0;
    dict       *p_dict       = 0;
    void       *result_value = 0;

    // build the dict
    dict_constructor(&p_dict);

    // get the key 
    dict_get(p_dict, key, &result_value);

    result = (result_value == expected_value) ? match : zero;

    // release the dict
    dict_destroy(&p_dict, 0);

    // return result
    return (result == expected);
}

bool test_values ( fn_dict_constructor *dict_constructor, const void *const *expected_values, result_t expected )
{

    // initialized data
    result_t   result               = match;
    dict      *p_dict               = 0;
    size_t     expected_value_count = 0,
               value_count          = 0;
    void      *values[4]            = { 0 };

    // build the dict
    dict_constructor(&p_dict);

    // count the values
    if ( expected_values )
        while(expected_values[++expected_value_count]);
    else
    {
        result = zero;
        goto exit;
    }

    // store the expected values
    dict_size(p_dict, &value_count),
    dict_values(p_dict, values, value_count);

    // fast fail
    if ( expected_value_count != value_count )
    {
        result = zero; 
        goto exit;
    }

    // iterate through each actual value
    for (size_t i = 0; i < value_count; i++)
    {

        // initialized data
        bool found = false;

        // iterate through each expected value
        for (size_t j = 0; j < expected_value_count; j++)
            if ( values[i] == expected_values[j] )
                found = true;

        // catch missing values
        if ( found == false )
            result = zero; 
    }
    
    exit:

    // release the dict
    dict_destroy(&p_dict, 0);

    // return result
    return (result == expected);
}

bool test_pop ( fn_dict_constructor *dict_constructor, char *key, const void *const expected_value, result_t expected )
{

    // initialized data
    result_t    result       = 0;
    dict       *p_dict       = 0;
    const void *result_value = 0;

    // build the dict
    dict_constructor(&p_dict);

    // remove a value
    dict_pop(p_dict, key, &result_value);

    // compute the result
    result = (result_value == expected_value) ? match : zero;

    // release the dict
    dict_destroy(&p_dict, 0);

    // return result
    return (result == expected);
} 