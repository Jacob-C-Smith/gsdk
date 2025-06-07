/** !
 * dict tester
 * 
 * @file dict_test.c
 * 
 * @author Jacob Smith
*/

// Include
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// core
#include <core/log.h>

// data
#include <data/dict.h>

// Possible values
void *A_value = (void *) 0x0000000000000001,
     *B_value = (void *) 0x0000000000000002,
     *C_value = (void *) 0x0000000000000003,
     *D_value = (void *) 0x0000000000000004,
     *X_value = (void *) 0xFFFFFFFFFFFFFFFF;

// Possible keys
char *A_key   = "A",
     *B_key   = "B",
     *C_key   = "C",
     *D_key   = "D",
     *X_key   = "X";

// Expected results
char  *_keys     [] = { 0x0 };
void  *_values   [] = { (void *) 0x0 };
char  *A_keys    [] = { "A", 0x0 };
void  *A_values  [] = { (void *) 0x1, (void *) 0x0 };
char  *B_keys    [] = { "B", 0x0 };
void  *B_values  [] = { (void *) 0x2, (void *) 0x0 };
char  *C_keys    [] = { "C", 0x0 };
void  *C_values  [] = { (void *) 0x3, (void *) 0x0 };
char  *AB_keys   [] = { "A", "B", 0x0 };
void  *AB_values [] = { (void *) 0x1, (void *) 0x2, (void *) 0x0 };
char  *BC_keys   [] = { "B", "C", 0x0 };
void  *BC_values [] = { (void *) 0x2, (void *) 0x3, (void *) 0x0 };
char  *AC_keys   [] = { "A", "C", 0x0 };
void  *AC_values [] = { (void *) 0x1, (void *) 0x3, (void *) 0x0 };
char  *ABC_keys  [] = { "A", "B", "C", 0x0 };
void  *ABC_values[] = { (void *) 0x1, (void *) 0x2, (void *) 0x3, (void *) 0x0 };

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


// Forward declarations
int print_time_pretty       ( double seconds );
int run_tests               ( void );
int print_final_summary     ( void );
int print_test              ( const char  *scenario_name, const char *test_name, bool passed );

bool test_add               ( int (*dict_constructor)(dict **), char  *key            , void     *value         , result_t expected );
bool test_get               ( int (*dict_constructor)(dict **), char  *key            , void     *expected_value, result_t expected );
bool test_key_count         ( int (*dict_constructor)(dict **), char **expected_keys  , result_t  expected );
bool test_value_count       ( int (*dict_constructor)(dict **), void **expected_values, result_t  expected );
bool test_values            ( int (*dict_constructor)(dict **), void **expected_values, result_t  expected );
bool test_keys              ( int (*dict_constructor)(dict **), char **expected_keys  , result_t  expected );
bool test_pop               ( int (*dict_constructor)(dict **), char  *key            , void     *expected_value, result_t expected );

int test_empty_dict         ( int (*dict_constructor)(dict **), char *name );
int test_one_element_dict   ( int (*dict_constructor)(dict **), char *name, char **keys, void **values );
int test_two_element_dict   ( int (*dict_constructor)(dict **), char *name, char **keys, void **values );
int test_three_element_dict ( int (*dict_constructor)(dict **), char *name, char **keys, void **values );

int construct_empty                 ( dict **pp_dict );
int construct_empty_addA_A          ( dict **pp_dict );
int construct_A_popA_empty          ( dict **pp_dict );
int construct_A_addB_AB             ( dict **pp_dict ); 
int construct_AB_popA_B             ( dict **pp_dict ); 
int construct_AB_popB_A             ( dict **pp_dict ); 
int construct_AB_addC_ABC           ( dict **pp_dict );
int construct_ABC_popA_BC           ( dict **pp_dict );
int construct_ABC_popB_AC           ( dict **pp_dict );
int construct_ABC_popC_AB           ( dict **pp_dict );
int construct_ABC_clear_empty       ( dict **pp_dict );
int construct_AB_clear_empty        ( dict **pp_dict );
int construct_A_clear_empty         ( dict **pp_dict );
int construct_empty_fromkeysABC_ABC ( dict **pp_dict );
int construct_empty_fromkeysAB_AB   ( dict **pp_dict );
int construct_empty_fromkeysA_A     ( dict **pp_dict );

// Entry point
int main ( int argc, const char* argv[] )
{

    // Suppress warnings
    (void) argc;
    (void) argv;    

    // Initialized data
    timestamp t0 = 0,
              t1 = 0;

    // Formatting
    printf(
        "╭─────────────╮\n"\
        "│ dict tester │\n"\
        "╰─────────────╯\n\n");

    // Start
    t0 = timer_high_precision();

    // Run tests
    run_tests();

    // Stop
    t1 = timer_high_precision();

    // Report the time it took to run the tests
    log_info("dict took ");
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
    if ( days ) log_info("%d D, ", days);
    
    // Print hours
    if ( hours ) log_info("%d h, ", hours);

    // Print minutes
    if ( minutes ) log_info("%d m, ", minutes);

    // Print seconds
    if ( __seconds ) log_info("%d s, ", __seconds);
    
    // Print milliseconds
    if ( milliseconds ) log_info("%d ms, ", milliseconds);
    
    // Print microseconds
    if ( microseconds ) log_info("%d us", microseconds);
    
    // Success
    return 1;
}

int run_tests ( void )
{

    // ... -> []
    test_empty_dict(construct_empty, "empty");

    // [] -> add(A) -> [A]
    test_one_element_dict(construct_empty_addA_A, "empty_addA_A", (char **)A_keys, (void **) A_values);

    // [A] -> pop(A) -> []
    test_empty_dict(construct_A_popA_empty, "A_popA_empty");

    // [A] -> add(B) -> [A,B]
    test_two_element_dict(construct_A_addB_AB, "A_addB_AB", (char **)AB_keys, (void **) AB_values);
    
    // [A,B] -> pop(A) -> [B]
    test_one_element_dict(construct_AB_popA_B, "AB_popA_B", (char **)B_keys, (void **) B_values);

    // [A,B] -> pop(B) -> [A]
    test_one_element_dict(construct_AB_popB_A, "AB_popB_A", (char **)A_keys, (void **) A_values);

    // [A,B] -> add(C) -> [A,B,C]
    test_three_element_dict(construct_AB_addC_ABC, "AB_addC_ABC", (char **)ABC_keys, (void **) ABC_values);
    
    // [A,B,C] -> pop(A) -> [B,C]
    test_two_element_dict(construct_ABC_popA_BC, "ABC_popA_BC", (char **)BC_keys, (void **) BC_values);

    // [A,B,C] -> pop(B) -> [A,C]
    test_two_element_dict(construct_ABC_popB_AC, "ABC_popB_AC", (char **)AC_keys, (void **) AC_values);

    // [A,B,C] -> pop(C) -> [A,B]
    test_two_element_dict(construct_ABC_popC_AB, "ABC_popC_AB", (char **)AB_keys, (void **) AB_values);

    // [A,B,C] -> clear() -> []
    test_empty_dict(construct_ABC_clear_empty, "ABC_clear_empty");

    // [A,B] -> clear() -> []
    test_empty_dict(construct_AB_clear_empty, "AB_clear_empty");

    // [A] -> clear() -> []
    test_empty_dict(construct_A_clear_empty, "A_clear_empty");

    // Success
    return 1;
}

int construct_empty(dict **pp_dict)
{
    // Construct a dict
    dict_construct(pp_dict, 1, 0);

    // dict = []
    return 1;
}

int construct_empty_addA_A(dict **pp_dict)
{

    // Construct a [] dict
    construct_empty(pp_dict);

    // add(A)
    dict_add(*pp_dict, A_key, A_value);
    
    // dict = [A]
    // Success
    return 1;
}

int construct_A_popA_empty(dict **pp_dict)
{

    // Construct a [A] dict
    construct_empty_addA_A(pp_dict);

    // pop(A)
    dict_pop(*pp_dict, A_key, 0);
    
    // dict = []
    // Success
    return 1;
}

int construct_A_addB_AB( dict **pp_dict )
{

    // Construct an [A] dict
    construct_empty_addA_A(pp_dict);

    // add(B)
    dict_add(*pp_dict, B_key, B_value);
    
    // dict = [A, B]
    // Success
    return 1;
}

int construct_AB_popA_B ( dict **pp_dict )
{
    // Construct an [A, B] dict
    construct_A_addB_AB(pp_dict);

    // pop(A)
    dict_pop(*pp_dict, A_key, 0);
    
    // dict = [B]
    // Success
    return 1;
}

int construct_AB_popB_A ( dict **pp_dict )
{
    // Construct an [A, B] dict
    construct_A_addB_AB(pp_dict);

    // pop(B)
    dict_pop(*pp_dict, B_key, 0);
    
    // dict = [A]
    // Success
    return 1;
}

int construct_AB_addC_ABC ( dict **pp_dict )
{

    // Construct an [A, B] dict
    construct_A_addB_AB(pp_dict);

    // add(C)
    dict_add(*pp_dict, C_key, C_value);

    // dict = [A, B, C]
    // Success
    return 1;
}

int construct_ABC_popA_BC  ( dict **pp_dict )
{
    // Construct an [A, B, C] dict
    construct_AB_addC_ABC(pp_dict);

    // pop(A)
    dict_pop(*pp_dict, A_key, 0);

    // dict = [B, C]
    // Success
    return 1;
}

int construct_ABC_popB_AC  ( dict **pp_dict )
{
    // Construct an [A, B, C] dict
    construct_AB_addC_ABC(pp_dict);

    // pop(B)
    dict_pop(*pp_dict, B_key, 0);

    // dict = [A, C]
    // Success
    return 1;
}

int construct_ABC_popC_AB  ( dict **pp_dict )
{
    // Construct an [A, B, C] dict
    construct_AB_addC_ABC(pp_dict);

    // pop(C)
    dict_pop(*pp_dict, C_key, 0);

    // dict = [A, B]
    // Success
    return 1;
}

int construct_ABC_clear_empty ( dict **pp_dict )
{

    // Construct an [A, B, C] dict
    construct_AB_addC_ABC(pp_dict);

    // clear()
    dict_clear(*pp_dict);

    // dict = []
    // Success
    return 1;
}

int construct_AB_clear_empty ( dict **pp_dict )
{
    // Construct an [A, B] dict
    construct_A_addB_AB(pp_dict);

    // clear()
    dict_clear(*pp_dict);

    // dict = []
    // Success
    return 1;
}

int construct_A_clear_empty ( dict **pp_dict )
{
    // Construct an [A] dict
    construct_empty_addA_A(pp_dict);

    // clear()
    dict_clear(*pp_dict);

    // dict = []
    // Success
    return 1;
}

int test_empty_dict(int(*dict_constructor)(dict **pp_dict), char *name)
{

    log_info("Scenario: %s\n", name);

    print_test(name, "dict_add_A"      , test_add(dict_constructor, A_key    , A_value, one) );
    print_test(name, "dict_add_(null)" , test_add(dict_constructor, (void *)0, A_value, zero) );
    print_test(name, "dict_get_X"      , test_get(dict_constructor, X_key, X_value, zero) );    
    print_test(name, "dict_value_count", test_value_count(dict_constructor, 0, zero) );
    print_test(name, "dict_key_count"  , test_key_count(dict_constructor, 0, zero) );
    print_test(name, "dict_values"     , test_values(dict_constructor, 0, zero) );
    print_test(name, "dict_keys"       , test_keys(dict_constructor, 0, zero) );
    print_test(name, "dict_pop_X"      , test_pop(dict_constructor, X_key, X_value, zero) );
    print_test(name, "dict_pop_(null)" , test_pop(dict_constructor, (void *)0, X_value, zero) );

    print_final_summary();

    return 1;
}

int test_one_element_dict ( int (*dict_constructor)(dict **), char       *name     , char **keys  , void **values )
{

    log_info("Scenario: %s\n", name);

    print_test(name, "dict_add_D"      , test_add(dict_constructor, D_key    , D_value, one) );
    print_test(name, "dict_add_(null)" , test_add(dict_constructor, (void *)0, A_value, zero) );
    
    for (size_t i = 0; keys[i]; i++)
    {
        char *test_name = DICT_REALLOC(0, (10 + 1) * sizeof(char));
        memset(test_name, 0, sizeof(10+1 * sizeof(char)));
        sprintf(test_name, "dict_get_%s", keys[i]);
        print_test(name, test_name, test_get(dict_constructor, keys[i], values[i], match) );    
        if ( DICT_REALLOC(test_name, 0) ) return 0;
    }

    print_test(name, "dict_get_X"      , test_get(dict_constructor, X_key, X_value, zero) );    
    print_test(name, "dict_value_count", test_value_count(dict_constructor, values, zero) );
    print_test(name, "dict_key_count"  , test_key_count(dict_constructor, keys, zero) );
    print_test(name, "dict_values"     , test_values(dict_constructor, values, match) );
    print_test(name, "dict_keys"       , test_keys(dict_constructor, keys, match) );

    for (size_t i = 0; keys[i]; i++)
    {
        char *test_name = DICT_REALLOC(0, (10 + 1) * sizeof(char));
        memset(test_name, 0, sizeof(10+1 * sizeof(char)));
        sprintf(test_name, "dict_pop_%s", keys[i]);
        print_test(name, test_name , test_pop(dict_constructor, keys[i], values[i], match) );
        if ( DICT_REALLOC(test_name, 0) ) return 0;
    }
    print_test(name, "dict_pop_X"      , test_pop(dict_constructor, X_key, X_value, zero) );
    print_test(name, "dict_pop_(null)" , test_pop(dict_constructor, (void *)0, X_value, zero) );

    print_final_summary();

    // Success
    return 1;
}

int test_two_element_dict ( int (*dict_constructor)(dict **), char *name, char **keys, void **values )
{

    printf("Scenario: %s\n", name);

    print_test(name, "dict_add_D"      , test_add(dict_constructor, D_key    , D_value, one) );
    print_test(name, "dict_add_(null)" , test_add(dict_constructor, (void *)0, A_value, zero) );
    
    for (size_t i = 0; keys[i]; i++)
    {
        char *test_name = DICT_REALLOC(0, ( 10 + 1 ) * sizeof(char));
        memset(test_name, 0, sizeof(10+1 * sizeof(char)));
        sprintf(test_name, "dict_get_%s", keys[i]);
        print_test(name, test_name, test_get(dict_constructor, keys[i], values[i], match) );    
        if ( DICT_REALLOC(test_name, 0) ) return 0;
    }

    print_test(name, "dict_get_X"      , test_get(dict_constructor, X_key, X_value, zero) );    
    print_test(name, "dict_value_count", test_value_count(dict_constructor, values, zero) );
    print_test(name, "dict_key_count"  , test_key_count(dict_constructor, keys, zero) );
    print_test(name, "dict_values"     , test_values(dict_constructor, values, match) );
    print_test(name, "dict_keys"       , test_keys(dict_constructor, keys, match) );

    for (size_t i = 0; keys[i]; i++)
    {
        char *test_name = DICT_REALLOC(0, ( 10 + 1 ) * sizeof(char));
        memset(test_name, 0, sizeof(10+1 * sizeof(char)));
        sprintf(test_name, "dict_pop_%s", keys[i]);
        print_test(name, test_name , test_pop(dict_constructor, keys[i], values[i], match) );
        if ( DICT_REALLOC(test_name, 0) ) return 0;
    }
    print_test(name, "dict_pop_X"      , test_pop(dict_constructor, X_key, X_value, zero) );
    print_test(name, "dict_pop_(null)" , test_pop(dict_constructor, (void *)0, X_value, zero) );

    print_final_summary();

    // Success
    return 1;
}

int test_three_element_dict ( int (*dict_constructor)(dict **), char *name, char **keys, void **values )
{
    
    printf("Scenario: %s\n", name);

    print_test(name, "dict_add_D"      , test_add(dict_constructor, D_key    , D_value, one) );
    print_test(name, "dict_add_(null)" , test_add(dict_constructor, (void *)0, A_value, zero) );
    
    for (size_t i = 0; keys[i]; i++)
    {
        char *test_name = DICT_REALLOC(0, ( 10 + 1 ) * sizeof(char));

        memset(test_name, 0, 10+1 * sizeof(char));

        sprintf(test_name, "dict_get_%s", keys[i]);
        print_test(name, test_name, test_get(dict_constructor, keys[i], values[i], match) );    
        if ( DICT_REALLOC(test_name, 0) ) return 0;
    }

    print_test(name, "dict_get_X"      , test_get(dict_constructor, X_key, X_value, zero) );    
    print_test(name, "dict_value_count", test_value_count(dict_constructor, values, zero) );
    print_test(name, "dict_key_count"  , test_key_count(dict_constructor, keys, zero) );
    print_test(name, "dict_values"     , test_values(dict_constructor, values, match) );
    print_test(name, "dict_keys"       , test_keys(dict_constructor, keys, match) );

    for (size_t i = 0; keys[i]; i++)
    {
        char *test_name = DICT_REALLOC(0, ( 10 + 1 ) * sizeof(char));
        memset(test_name, 0, 10+1 * sizeof(char));
        sprintf(test_name, "dict_pop_%s", keys[i]);
        print_test(name, test_name , test_pop(dict_constructor, keys[i], values[i], match) );
        if ( DICT_REALLOC(test_name, 0) ) return 0;
    }
    print_test(name, "dict_pop_X"      , test_pop(dict_constructor, X_key, X_value, zero) );
    print_test(name, "dict_pop_(null)" , test_pop(dict_constructor, (void *)0, X_value, zero) );

    print_final_summary();

    // Success
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

    // Success
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

    // Success
    return 1;
}

bool test_add ( int(*dict_constructor)(dict **pp_dict), char *key, void *value, result_t expected )
{

    // Initialized data
    result_t  result = 0;
    dict     *p_dict = 0;

    // Build the dict
    dict_constructor(&p_dict);

    result = (result_t) dict_add(p_dict, key, value);

    // Free the dict
    dict_destroy(&p_dict);

    // Return result
    return (result == expected);
}

bool test_get ( int(*dict_constructor)(dict **pp_dict), char *key, void *expected_value, result_t expected )
{

    // Initialized data
    result_t    result       = 0;
    dict       *p_dict       = 0;
    const void *result_value = 0;

    // Build the dict
    dict_constructor(&p_dict);

    // Get a key 
    result_value = dict_get(p_dict, key);

    if (result_value == expected_value)
        result = match;
    else
        result = zero;

    // Free the dict
    dict_destroy(&p_dict);

    // Return result
    return (result == expected);
}

bool test_key_count ( int(*dict_constructor)(dict **pp_dict), char **expected_keys, result_t expected )
{

    // Suppress warnings
    (void) expected;

    // Initialized data
    dict      *p_dict              = 0;
    size_t     expected_keys_count = 0,
               keys_count          = 0;

    // Build the dict
    dict_constructor(&p_dict);

    // Count the keys
    if (expected_keys)
        while(expected_keys[++expected_keys_count]);

    // Get a key 
    keys_count = dict_keys(p_dict, 0);

    // Free the dict
    dict_destroy(&p_dict);

    // Return result
    return (expected_keys_count == keys_count) ? true : false;
}

bool test_value_count ( int(*dict_constructor)(dict **pp_dict), void **expected_values, result_t expected )
{

    // Suppress warnings
    (void) expected;

    // Initialized data
    dict      *p_dict               = 0;
    size_t     expected_value_count = 0,
               value_count          = 0;

    // Build the dict
    dict_constructor(&p_dict);

    // Count the values
    if ( expected_values)
        while(expected_values[++expected_value_count]);

    // Get a key 
    value_count = dict_values(p_dict, 0);

    // Free the dict
    dict_destroy(&p_dict);

    // Return result
    return (expected_value_count == value_count) ? true : false;
}

bool test_keys ( int(*dict_constructor)(dict **pp_dict), char **expected_keys, result_t expected )
{
    // Initialized data
    result_t     result             = match;
    dict        *p_dict             = 0;
    const char **keys               = 0;
    size_t       expected_key_count = 0,
                 key_count          = 0;

    // Build the dict
    dict_constructor(&p_dict);

    // Count the key
    if ( expected_keys)
    {
        while(expected_keys[++expected_key_count]);
    }
    else
    {
        result = zero;
    }

    // Get the key
    key_count = dict_keys(p_dict, 0);
    keys      = DICT_REALLOC(0, ( key_count + 1 ) * sizeof(char *));
    dict_keys(p_dict, keys);

    if ( expected_key_count != key_count )
    {
        result = zero; 
        goto exit;
    }

    for (size_t i = 0; i < key_count; i++)
    {
        bool found = false;

        for (size_t j = 0; j < expected_key_count; j++)
        {
            if ( strcmp(keys[i], expected_keys[j]) == 0 )
                found = true;
        }

        if (found == false)
            result = zero; 
    }
    
    exit:

    // Free the keys
    if ( DICT_REALLOC(keys, 0) ) return false;

    // Free the dict
    dict_destroy(&p_dict);

    // Return result
    return (result == expected);    
}

bool test_values ( int(*dict_constructor)(dict **pp_dict), void **expected_values, result_t expected )
{

    // Initialized data
    result_t   result               = match;
    dict      *p_dict               = 0;
    void     **values               = 0;
    size_t     expected_value_count = 0,
               value_count          = 0;

    // Build the dict
    dict_constructor(&p_dict);

    // Count the values
    if ( expected_values)
    {
        while(expected_values[++expected_value_count]);
    }
    else
    {
        result = zero;
        goto exit;
    }

    // Get the values
    value_count = dict_values(p_dict, 0);
    values      = DICT_REALLOC(0, (value_count + 1) * sizeof(char *));
    dict_values(p_dict, values);

    if ( expected_value_count != value_count )
    {
        result = zero; 
        goto exit2;
    }

    for (size_t i = 0; i < value_count; i++)
    {
        bool found = false;

        for (size_t j = 0; j < expected_value_count; j++)
        {
            if ( values[i] == expected_values[j] )
                found = true;
        }

        if (found == false)
            result = zero; 
    }
    
    exit2:

    // Free the values
    if ( DICT_REALLOC(values, 0) ) return false;

    exit:
    // Free the dict
    dict_destroy(&p_dict);

    // Return result
    return (result == expected);
}

bool test_pop ( int (*dict_constructor)(dict **), char *key , void *expected_value, result_t expected )
{

    // Initialized data
    result_t    result       = 0;
    dict       *p_dict       = 0;
    const void *result_value = 0;

    // Build the dict
    dict_constructor(&p_dict);

    // Pop a property
    dict_pop(p_dict, key, &result_value);

    if (result_value == expected_value)
        result = match;
    else
        result = zero;

    // Free the dict
    dict_destroy(&p_dict);

    // Return result
    return (result == expected);
} 