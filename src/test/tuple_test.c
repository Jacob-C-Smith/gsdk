/** !
 * Tuple tester
 * 
 * @file src/test/tuple_test.c
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
#include <data/tuple.h>

// preprocessor macros
#define A_ELEMENT "A"
#define B_ELEMENT "B"
#define C_ELEMENT "C"
#define D_ELEMENT "D"

// function declarations
/// scenario constructors
fn_scenario_constructor construct_empty;
fn_scenario_constructor construct_A;
fn_scenario_constructor construct_AB;
fn_scenario_constructor construct_ABC;

/// test cases
fn_test_case test_size;
fn_test_case test_index;
fn_test_case test_slice;
fn_test_case test_is_empty;

/// result evaluators
fn_results_match index_results_match;
fn_results_match slice_results_match;
fn_results_match size_results_match;
fn_results_match is_empty_results_match;

/// allocators
fn_allocator destruct_tuple;

// data
/// values
void *A_elements[]   = { A_ELEMENT, NULL };
void *AB_elements[]  = { A_ELEMENT, B_ELEMENT, NULL };
void *ABC_elements[] = { A_ELEMENT, B_ELEMENT, C_ELEMENT, NULL };
void *_contents[]    = { NULL };

// test
/// cases
test_case _empty_test_cases[] = 
{
    TEST_CASE ("index 0", test_index   , (void *)0, TEST_RESULT_ZERO),
    TEST_MATCH("size"   , test_size    , NULL     , size_results_match),
    TEST_MATCH("empty"  , test_is_empty, NULL     , is_empty_results_match),
};

test_case _one_element_test_cases[] = 
{
    TEST_MATCH("index 0", test_index   , (void *)0, index_results_match),
    TEST_CASE ("index 1", test_index   , (void *)1, TEST_RESULT_ZERO),
    TEST_MATCH("size"   , test_size    , NULL     , size_results_match),
    TEST_MATCH("empty"  , test_is_empty, NULL     , is_empty_results_match),
    TEST_MATCH("slice 0:0", test_slice , (void *)0x0000000000000000, slice_results_match),
};

test_case _two_element_test_cases[] = 
{
    TEST_MATCH("index 0", test_index   , (void *)0, index_results_match),
    TEST_MATCH("index 1", test_index   , (void *)1, index_results_match),
    TEST_CASE ("index 2", test_index   , (void *)2, TEST_RESULT_ZERO),
    TEST_MATCH("size"   , test_size    , NULL     , size_results_match),
    TEST_MATCH("empty"  , test_is_empty, NULL     , is_empty_results_match),
    TEST_MATCH("slice 0:1", test_slice , (void *)0x0000000000000001, slice_results_match),
};

test_case _three_element_test_cases[] = 
{
    TEST_MATCH("index 0", test_index   , (void *)0, index_results_match),
    TEST_MATCH("index 1", test_index   , (void *)1, index_results_match),
    TEST_MATCH("index 2", test_index   , (void *)2, index_results_match),
    TEST_CASE ("index 3", test_index   , (void *)3, TEST_RESULT_ZERO),
    TEST_MATCH("index -1", test_index  , (void *)-1, index_results_match),
    TEST_MATCH("index -2", test_index  , (void *)-2, index_results_match),
    TEST_MATCH("index -3", test_index  , (void *)-3, index_results_match),
    TEST_CASE ("index -4", test_index  , (void *)-4, TEST_RESULT_ZERO),
    TEST_MATCH("size"   , test_size    , NULL     , size_results_match),
    TEST_MATCH("empty"  , test_is_empty, NULL     , is_empty_results_match),
    TEST_MATCH("slice 0:2", test_slice , (void *)0x0000000000000002, slice_results_match),
    TEST_MATCH("slice 1:2", test_slice , (void *)0x0000000100000002, slice_results_match),
};

/// scenarios
test_scenario _scenarios[] = 
{
    TEST_SCENARIO("empty", _contents   , _empty_test_cases        , construct_empty, destruct_tuple),
    TEST_SCENARIO("A"    , A_elements  , _one_element_test_cases  , construct_A    , destruct_tuple),
    TEST_SCENARIO("AB"   , AB_elements , _two_element_test_cases  , construct_AB   , destruct_tuple),
    TEST_SCENARIO("ABC"  , ABC_elements, _three_element_test_cases, construct_ABC  , destruct_tuple),
};

/// suites
test_suite _suite = TEST_SUITE("tuple", _scenarios);

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

// constructors
int construct_empty ( void **pp_result ) 
{ 

    // done
    return tuple_from_elements((tuple **)pp_result, _contents, 0);
}

int construct_A ( void **pp_result ) 
{

    // done
    return tuple_from_elements((tuple **)pp_result, (void **)A_elements, 1);
}

int construct_AB ( void **pp_result ) 
{

    // done
    return tuple_from_elements((tuple **)pp_result, (void **)AB_elements, 2);
}

int construct_ABC ( void **pp_result ) 
{

    // done
    return tuple_from_elements((tuple **)pp_result, (void **)ABC_elements, 3);
}

void *test_size ( test_case *p_test_case, void *p_subject ) 
{ 

    // unused
    (void) p_test_case;

    // done
    return (void *)tuple_size((tuple *)p_subject);
}

void *test_index ( test_case *p_test_case, void *p_subject ) 
{ 

    // initialized data
    signed long long index = (signed long long)(size_t)p_test_case->p_parameters;

    // test
    if ( 0 == tuple_index((tuple *)p_subject, index, &p_test_case->p_out) ) return NULL;

    // success
    return (void *)1;
}

void *test_slice ( test_case *p_test_case, void *p_subject ) 
{ 

    // initialized data
    size_t             bounds      = (size_t)p_test_case->p_parameters;
    signed long long   lower       = (signed long long)(bounds >> 32);
    signed long long   upper       = (signed long long)(bounds & 0xFFFFFFFF);
    void             **pp_elements = NULL;

    // test
    if ( upper < lower ) return NULL;

    // allocate memory for results
    pp_elements = default_allocator(NULL, (upper - lower + 1) * sizeof(void *));

    // test
    if ( 0 == tuple_slice((tuple *)p_subject, (const void **)pp_elements, lower, upper) ) 
    {

        // release elements
        pp_elements = default_allocator(pp_elements, 0);

        // error
        return (void *)0;
    }

    // store elements
    p_test_case->p_out = pp_elements;

    // success
    return (void *)1;
}

void *test_is_empty ( test_case *p_test_case, void *p_subject ) 
{ 

    // unused
    (void) p_test_case;

    // done
    return (void *)(size_t)tuple_is_empty((tuple *)p_subject);
}

bool index_results_match ( test_scenario *p_scenario, test_case *p_case, void *p_subject, void *p_result )
{

    // unused
    (void) p_subject;
    (void) p_result;

    // initialized data
    void             **pp_elements = p_scenario->p_data;
    signed long long   index       = (signed long long)(size_t)p_case->p_parameters;
    size_t             count       = 0;

    // count elements
    while ( pp_elements[count] ) count++;

    // positive index
    if ( index >= 0 )
        return p_case->p_out == pp_elements[index];

    // negative index
    return p_case->p_out == pp_elements[count + index];
}

bool slice_results_match ( test_scenario *p_scenario, test_case *p_case, void *p_subject, void *p_result )
{

    // unused
    (void) p_subject;
    (void) p_result;
    
    // initialized data
    void             **pp_expected = p_scenario->p_data;
    void             **pp_actual   = (void **)p_case->p_out;
    size_t             bounds      = (size_t)p_case->p_parameters;
    signed long long   lower       = (signed long long)(bounds >> 32);
    signed long long   upper       = (signed long long)(bounds & 0xFFFFFFFF);
    bool               match       = true;

    // test
    if ( NULL == pp_actual ) return false;

    // iterate through each element
    for (signed long long i = 0; i <= upper - lower; i++)

        // test
        if ( pp_expected[lower + i] != pp_actual[i] ) match = false;

    // release elements
    pp_actual = default_allocator(pp_actual, 0);

    // done
    return match;
}

bool size_results_match ( test_scenario *p_scenario, test_case *p_case, void *p_subject, void *p_result )
{

    // unused
    (void) p_case;
    (void) p_subject;

    // initialized data
    void   **pp_elements = (void **)p_scenario->p_data;
    size_t   count       = 0;

    // count
    while ( pp_elements[count] ) count++;

    // done
    return (size_t)p_result == count;
}

bool is_empty_results_match ( test_scenario *p_scenario, test_case *p_case, void *p_subject, void *p_result )
{

    // unused
    (void) p_case; (void) p_subject;

    // initialized data
    void **pp_elements = (void **)p_scenario->p_data;
    bool   empty       = ( pp_elements[0] == NULL );

    // done
    return p_result == (void *)(size_t)empty;
}

void *destruct_tuple ( void *p_pointer, unsigned long long size )
{

    // unused
    (void) size;

    // initialized data
    tuple *p_tuple = (tuple *)p_pointer;

    // release the tuple
    tuple_destroy(&p_tuple, NULL);

    // success
    return NULL;
}
