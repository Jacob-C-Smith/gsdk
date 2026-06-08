/** !
 * dictionary tester
 * 
 * @file src/test/dict_test.c
 * 
 * @author Jacob Smith
*/

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// gsdk
/// core
#include <core/log.h>
#include <core/sync.h>
#include <core/test.h>

/// data
#include <data/dict.h>

// preprocessor macros
#define A_VALUE "A"
#define B_VALUE "B"
#define C_VALUE "C"
#define D_VALUE "D"
#define X_VALUE "X"

// function declarations
/// scenario constructors
fn_scenario_constructor construct_empty;
fn_scenario_constructor construct_empty_addA_A;
fn_scenario_constructor construct_empty_addB_B;
fn_scenario_constructor construct_empty_addC_C;
fn_scenario_constructor construct_A_popA_empty;
fn_scenario_constructor construct_A_addB_AB; 
fn_scenario_constructor construct_AB_popA_B; 
fn_scenario_constructor construct_AB_popB_A; 
fn_scenario_constructor construct_AB_addC_ABC;
fn_scenario_constructor construct_ABC_popA_BC;
fn_scenario_constructor construct_ABC_popB_AC;
fn_scenario_constructor construct_ABC_popC_AB;

/// test cases
fn_test_case test_add;
fn_test_case test_get;
fn_test_case test_pop;
fn_test_case test_size;
fn_test_case test_values;

/// result evaluators
fn_results_match get_results_match;
fn_results_match pop_results_match;
fn_results_match size_results_match;
fn_results_match values_results_match;

/// allocators
fn_allocator destruct_dict;

// data
/// values
const char *const _values   [] = { NULL };
const char *const A_values  [] = { A_VALUE, NULL };
const char *const B_values  [] = { B_VALUE, NULL };
const char *const C_values  [] = { C_VALUE, NULL };
const char *const AB_values [] = { A_VALUE, B_VALUE, NULL };
const char *const BC_values [] = { B_VALUE, C_VALUE, NULL };
const char *const AC_values [] = { A_VALUE, C_VALUE, NULL };
const char *const ABC_values[] = { A_VALUE, B_VALUE, C_VALUE, NULL };

// test
/// cases
test_case _empty_test_cases[] = 
{
    TEST_CASE ("add D"     , test_add    , D_VALUE, TEST_RESULT_ONE),
    TEST_CASE ("add (null)", test_add    , NULL   , TEST_RESULT_ZERO),
    TEST_CASE ("get X"     , test_get    , X_VALUE, TEST_RESULT_ZERO),
    TEST_CASE ("pop X"     , test_pop    , X_VALUE, TEST_RESULT_ZERO),
    TEST_MATCH("size"      , test_size   , NULL   , size_results_match),
    TEST_MATCH("values"    , test_values , NULL   , values_results_match),
};

test_case _one_element_test_cases[] = 
{
    TEST_CASE ("add D"     , test_add    , D_VALUE, TEST_RESULT_ONE),
    TEST_MATCH("get A/B/C" , test_get    , NULL   , get_results_match),
    TEST_CASE ("get X"     , test_get    , X_VALUE, TEST_RESULT_ZERO),
    TEST_MATCH("pop A/B/C" , test_pop    , NULL   , pop_results_match),
    TEST_MATCH("size"      , test_size   , NULL   , size_results_match),
    TEST_MATCH("values"    , test_values , NULL   , values_results_match),
};

test_case _two_element_test_cases[] = 
{
    TEST_CASE ("add D"     , test_add    , D_VALUE, TEST_RESULT_ONE),
    TEST_MATCH("get A/B/C" , test_get    , NULL   , get_results_match),
    TEST_MATCH("size"      , test_size   , NULL   , size_results_match),
    TEST_MATCH("values"    , test_values , NULL   , values_results_match),
};

test_case _three_element_test_cases[] = 
{
    TEST_CASE ("add D"     , test_add    , D_VALUE, TEST_RESULT_ONE),
    TEST_MATCH("get A/B/C" , test_get    , NULL   , get_results_match),
    TEST_MATCH("size"      , test_size   , NULL   , size_results_match),
    TEST_MATCH("values"    , test_values , NULL   , values_results_match),
};

/// scenarios
test_scenario _scenarios[] = 
{
    TEST_SCENARIO("empty"        , _values   , _empty_test_cases        , construct_empty         , destruct_dict),
    TEST_SCENARIO("empty_addA_A" , A_values  , _one_element_test_cases  , construct_empty_addA_A  , destruct_dict),
    TEST_SCENARIO("empty_addB_B" , B_values  , _one_element_test_cases  , construct_empty_addB_B  , destruct_dict),
    TEST_SCENARIO("empty_addC_C" , C_values  , _one_element_test_cases  , construct_empty_addC_C  , destruct_dict),
    TEST_SCENARIO("A_popA_empty" , _values   , _empty_test_cases        , construct_A_popA_empty  , destruct_dict),
    TEST_SCENARIO("A_addB_AB"    , AB_values , _two_element_test_cases  , construct_A_addB_AB     , destruct_dict),
    TEST_SCENARIO("AB_popA_B"    , B_values  , _one_element_test_cases  , construct_AB_popA_B     , destruct_dict),
    TEST_SCENARIO("AB_popB_A"    , A_values  , _one_element_test_cases  , construct_AB_popB_A     , destruct_dict),
    TEST_SCENARIO("AB_addC_ABC"  , ABC_values, _three_element_test_cases, construct_AB_addC_ABC   , destruct_dict),
    TEST_SCENARIO("ABC_popA_BC"  , BC_values , _two_element_test_cases  , construct_ABC_popA_BC   , destruct_dict),
    TEST_SCENARIO("ABC_popB_AC"  , AC_values , _two_element_test_cases  , construct_ABC_popB_AC   , destruct_dict),
    TEST_SCENARIO("ABC_popC_AB"  , AB_values , _two_element_test_cases  , construct_ABC_popC_AB   , destruct_dict),
};

/// suites
test_suite _suite = TEST_SUITE("dict", _scenarios);

// entry point
int main ( int argc, const char *argv[] ) 
{

    // unused
    (void) argc;
    (void) argv;
     
    // run the tests
    test_suite_test(&_suite); 
    
    // done
    return (_suite.counters.total.fails == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int construct_empty ( void **pp_result ) 
{ 

    // ... -> { }
    return dict_construct((dict **)pp_result, 1, NULL, NULL, NULL);
}

int construct_empty_addA_A ( void **pp_result ) 
{ 

    // { }
    construct_empty(pp_result);

    // { } -> add(A) -> { A }
    return dict_add(*((dict **)pp_result), A_VALUE); 
}

int construct_empty_addB_B ( void **pp_result ) 
{

    // { }
    construct_empty(pp_result);
    
    // { } -> add(B) -> { B }
    return dict_add(*((dict **)pp_result), B_VALUE); 
}

int construct_empty_addC_C ( void **pp_result ) 
{

    // { }
    construct_empty(pp_result);
    
    // { } -> add(C) -> { C }
    return dict_add(*((dict **)pp_result), C_VALUE); 
}

int construct_A_popA_empty ( void **pp_result ) 
{

    // { A }
    construct_empty_addA_A(pp_result);
    
    // { A } -> pop(A) -> { }
    return dict_pop(*((dict **)pp_result), A_VALUE, NULL); 
}

int construct_A_addB_AB ( void **pp_result ) 
{

    // { A }
    construct_empty_addA_A(pp_result);
    
    // { A } -> add(B) -> { A, B }
    return dict_add(*((dict **)pp_result), B_VALUE); 
}

int construct_AB_popA_B ( void **pp_result ) 
{

    // { A, B }
    construct_A_addB_AB(pp_result);
    
    // { A, B } -> pop(A) -> { B }
    return dict_pop(*((dict **)pp_result), A_VALUE, NULL); 
}

int construct_AB_popB_A ( void **pp_result ) 
{

    // { A, B }
    construct_A_addB_AB(pp_result);
    
    // { A, B } -> pop(B) -> { A }
    return dict_pop(*((dict **)pp_result), B_VALUE, NULL); 
}

int construct_AB_addC_ABC ( void **pp_result ) 
{

    // { A, B }
    construct_A_addB_AB(pp_result);
    
    // { A, B } -> add(C) -> { A, B, C }
    return dict_add(*((dict **)pp_result), C_VALUE); 
}

int construct_ABC_popA_BC ( void **pp_result ) 
{

    // { A, B, C }
    construct_AB_addC_ABC(pp_result);
    
    // { A, B, C } -> pop(A) -> { B, C }
    return dict_pop(*((dict **)pp_result), A_VALUE, NULL); 
}

int construct_ABC_popB_AC ( void **pp_result ) 
{

    // { A, B, C }
    construct_AB_addC_ABC(pp_result);
    
    // { A, B, C } -> pop(B) -> { A, C }
    return dict_pop(*((dict **)pp_result), B_VALUE, NULL); 
}

int construct_ABC_popC_AB ( void **pp_result ) 
{

    // { A, B, C }
    construct_AB_addC_ABC(pp_result);
    
    // { A, B, C } -> pop(C) -> { A, B }
    return dict_pop(*((dict **)pp_result), C_VALUE, NULL); 
}

void *test_add ( test_case *p_test_case, void *p_subject ) 
{ 

    // done
    return (void *)(size_t)dict_add((dict *)p_subject, p_test_case->p_parameters); 
}

void *test_get ( test_case *p_test_case, void *p_subject ) 
{ 

    // initialized data
    const char *key = (const char *)p_test_case->p_parameters;
    
    // key?
    if ( key == NULL ) 
    {
        
        // initialized data
        const char **pp_keys = (const char **)p_test_case->p_data;

        // store the key
        key = pp_keys[0];
    }

    // test
    if ( 0 == dict_get((dict *)p_subject, key, &p_test_case->p_out) ) return NULL;

    // success
    return (void *)1;
}

void *test_pop ( test_case *p_test_case, void *p_subject ) 
{ 

    // initialized data
    const char *key = (const char *)p_test_case->p_parameters;

    // key?
    if ( key == NULL ) 
    {
        
        // initialized data
        const char **pp_keys = (const char **)p_test_case->p_data;
        
        // store the key
        key = pp_keys[0];
    }

    // test 
    if ( 0 == dict_pop((dict *)p_subject, key, (const void **)&p_test_case->p_out) ) return NULL;

    // success
    return (void *)1;
}

void *test_size ( test_case *p_test_case, void *p_subject ) 
{ 

    // unused
    (void) p_test_case;
    
    // initialized data
    size_t size = 0;

    // store the size
    dict_size((dict *)p_subject, &size);

    // done
    return (void *)size;
}

void *test_values ( test_case *p_test_case, void *p_subject ) 
{ 

    // initialized data
    size_t   size      = 0;
    void   **pp_values = NULL;

    // store the size
    dict_size((dict *)p_subject, &size);

    // fast exit
    if ( 0 == size ) return NULL;

    // allocate memory for results
    pp_values = default_allocator(NULL, (size + 1) * sizeof(void *));

    // store the values
    dict_values((dict *)p_subject, pp_values, size);
    p_test_case->p_out = pp_values;

    // success
    return (void *)1;
}

bool get_results_match ( test_scenario *p_scenario, test_case *p_case, void *p_subject, void *p_result )
{

    // unused
    (void) p_scenario; 
    (void) p_subject; 
    (void) p_result;

    // initialized data
    const char *expected = (const char *)p_case->p_parameters;

    // test
    if ( expected == NULL ) expected = ((const char **)p_scenario->p_data)[0];

    // done
    return p_case->p_out == expected;
}

bool pop_results_match ( test_scenario *p_scenario, test_case *p_case, void *p_subject, void *p_result )
{

    // unused
    (void) p_scenario; 
    (void) p_subject; 
    (void) p_result;

    // initialized data
    const char *expected = (const char *)p_case->p_parameters;

    // test
    if ( expected == NULL ) expected = ((const char **)p_scenario->p_data)[0];

    // done
    return p_case->p_out == expected;
}

bool size_results_match ( test_scenario *p_scenario, test_case *p_case, void *p_subject, void *p_result )
{

    // unused
    (void) p_case; 
    (void) p_subject;

    // initialized data
    const char **pp_elements = (const char **)p_scenario->p_data;
    size_t       count       = 0;

    // count
    while ( pp_elements[count] ) count++;

    // done
    return (size_t)p_result == count;
}

bool values_results_match ( test_scenario *p_scenario, test_case *p_case, void *p_subject, void *p_result )
{

    // unused
    (void) p_subject; 
    (void) p_result;

    // initialized data
    const char **pp_expected    = (const char **)p_scenario->p_data;
    void       **pp_actual      = (void **)p_case->p_out;
    size_t       expected_count = 0;
    size_t       actual_count   = 0;
    bool         match          = true;

    // count
    while ( pp_expected[expected_count] ) expected_count++;
    
    // store the size
    if ( pp_actual ) 
        dict_size((dict *)p_subject, &actual_count); 

    // test
    if ( expected_count != actual_count ) 
    {

        // release the values
        if ( pp_actual ) pp_actual = default_allocator(pp_actual, 0);

        // error
        return false;
    }

    // iterate through each value
    for (size_t i = 0; i < actual_count; i++) 
    {

        // initialized data
        bool found = false;

        // iterate through each value
        for (size_t j = 0; j < expected_count; j++) 
        {

            // match?
            if ( pp_actual[i] == pp_expected[j] ) 
            {

                // found
                found = true;

                // done
                break;
            }
        }

        // found?
        if ( !found ) 
        {

            // not found
            match = false;

            // done
            break;
        }
    }

    // release the values
    if ( pp_actual ) pp_actual = default_allocator(pp_actual, 0);

    // done
    return match;
}

void *destruct_dict ( void *p_pointer, unsigned long long size )
{

    // unused
    (void) size;

    // initialized data
    dict *p_dict = (dict *)p_pointer;

    // release the dictionary
    dict_destroy(&p_dict, NULL);

    // success
    return NULL;
}
