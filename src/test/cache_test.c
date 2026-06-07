/** !
 * Tester for cache module
 * 
 * @file src/test/cache_test.c
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
#include <data/cache.h>

// preprocessor macros
#define A_ELEMENT "A"
#define B_ELEMENT "B"
#define C_ELEMENT "C"
#define D_ELEMENT "D"
#define X_ELEMENT "X"

// function declarations
/// scenario constructors
fn_scenario_constructor construct_empty;
fn_scenario_constructor construct_A;
fn_scenario_constructor construct_BA;
fn_scenario_constructor construct_CBA;
fn_scenario_constructor construct_ACB;
fn_scenario_constructor construct_DCB;
fn_scenario_constructor construct_CA;

/// test cases
fn_test_case test_insert;
fn_test_case test_find;
fn_test_case test_remove;
fn_test_case test_size;

/// result evaluators
fn_results_match find_results_match;
fn_results_match remove_results_match;
fn_results_match size_results_match;

/// allocators
fn_allocator destruct_cache;

/// helpers
int string_equality(const void *a, const void *b);

// data
/// values
const char *A_elements[]   = { "A", NULL };
const char *BA_elements[]  = { "B", "A", NULL };
const char *CBA_elements[] = { "C", "B", "A", NULL };
const char *ACB_elements[] = { "A", "C", "B", NULL };
const char *DCB_elements[] = { "D", "C", "B", NULL };
const char *CA_elements[]  = { "C", "A", NULL };
const char *_contents[]    = { NULL };

// test
/// cases
test_case _empty_test_cases[] = 
{
    TEST_CASE ("insert A" , test_insert, A_ELEMENT, TEST_RESULT_ONE),
    TEST_CASE ("find X"   , test_find  , X_ELEMENT, TEST_RESULT_ZERO),
    TEST_CASE ("remove X" , test_remove, X_ELEMENT, TEST_RESULT_ZERO),
    TEST_MATCH("size"     , test_size  , NULL     , size_results_match),
};

test_case _one_element_test_cases[] = 
{
    TEST_CASE ("insert B" , test_insert, B_ELEMENT, TEST_RESULT_ONE),
    TEST_MATCH("find A"   , test_find  , A_ELEMENT, find_results_match),
    TEST_CASE ("find X"   , test_find  , X_ELEMENT, TEST_RESULT_ZERO),
    TEST_MATCH("size"     , test_size  , NULL     , size_results_match),
};

test_case _two_element_test_cases[] = 
{
    TEST_CASE ("insert C" , test_insert, C_ELEMENT, TEST_RESULT_ONE),
    TEST_MATCH("find A"   , test_find  , A_ELEMENT, find_results_match),
    TEST_MATCH("find B"   , test_find  , B_ELEMENT, find_results_match),
    TEST_MATCH("size"     , test_size  , NULL     , size_results_match),
};

test_case _full_test_cases[] = 
{
    TEST_CASE ("insert D (evict)", test_insert, D_ELEMENT, TEST_RESULT_ONE),
    TEST_MATCH("find A"          , test_find  , A_ELEMENT, find_results_match),
    TEST_MATCH("find B"          , test_find  , B_ELEMENT, find_results_match),
    TEST_MATCH("find C"          , test_find  , C_ELEMENT, find_results_match),
    TEST_MATCH("size"            , test_size  , NULL     , size_results_match),
};

test_case _eviction_test_cases[] = 
{
    TEST_CASE ("find A (miss)", test_find  , A_ELEMENT, TEST_RESULT_ZERO),
    TEST_MATCH("find D"       , test_find  , D_ELEMENT, find_results_match),
    TEST_MATCH("size"         , test_size  , NULL     , size_results_match),
};

test_case _lru_test_cases[] = 
{
    TEST_MATCH("find B"   , test_find  , B_ELEMENT, find_results_match),
    TEST_MATCH("find C"   , test_find  , C_ELEMENT, find_results_match),
    TEST_MATCH("find A"   , test_find  , A_ELEMENT, find_results_match),
    TEST_MATCH("size"     , test_size  , NULL     , size_results_match),
};

test_case _remove_test_cases[] = 
{
    TEST_MATCH("remove B" , test_remove, B_ELEMENT, remove_results_match),
    TEST_CASE ("remove X" , test_remove, X_ELEMENT, TEST_RESULT_ZERO),
    TEST_MATCH("size"     , test_size  , NULL     , size_results_match),
};

/// scenarios
test_scenario _scenarios[] = 
{
    TEST_SCENARIO("empty"    , _contents   , _empty_test_cases      , construct_empty, destruct_cache),
    TEST_SCENARIO("A"        , A_elements  , _one_element_test_cases, construct_A    , destruct_cache),
    TEST_SCENARIO("BA"       , BA_elements , _two_element_test_cases, construct_BA   , destruct_cache),
    TEST_SCENARIO("CBA"      , CBA_elements, _full_test_cases       , construct_CBA  , destruct_cache),
    TEST_SCENARIO("DCB"      , DCB_elements, _eviction_test_cases   , construct_DCB  , destruct_cache),
    TEST_SCENARIO("ACB"      , ACB_elements, _lru_test_cases        , construct_ACB  , destruct_cache),
    TEST_SCENARIO("remove"   , CBA_elements, _remove_test_cases     , construct_CBA  , destruct_cache),
};

/// suites
test_suite _suite = TEST_SUITE("cache", _scenarios);

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

// helpers
int string_equality(const void *a, const void *b)
{
    if ( a == NULL || b == NULL ) return a == b;

    return ( 0 == strcmp((const char *)a, (const char *)b) );
}

// constructors
int construct_empty ( void **pp_result ) 
{ 
    return cache_construct((cache **)pp_result, 3, string_equality, NULL, NULL);
}

int construct_A ( void **pp_result ) 
{ 

    // [ ] 
    construct_empty(pp_result);

    // [ ] -> insert(A) -> [ A ]
    return cache_insert(*((cache **)pp_result), A_ELEMENT, NULL);
}

int construct_BA ( void **pp_result ) 
{ 

    // [ A ] 
    construct_A(pp_result);

    // [ A ] -> insert(B) -> [ B, A ]
    return cache_insert(*((cache **)pp_result), B_ELEMENT, NULL);
}

int construct_CBA ( void **pp_result ) 
{ 

    // [ B, A ] 
    construct_BA(pp_result);

    // [ B, A ] -> insert(C) -> [ C, B, A ]
    return cache_insert(*((cache **)pp_result), C_ELEMENT, NULL);
}

int construct_ACB ( void **pp_result ) 
{ 

    // [ C, B, A ] 
    construct_CBA(pp_result);

    // [ C, B, A ] -> find(A) -> [ A, C, B ]
    return cache_find(*((cache **)pp_result), A_ELEMENT, NULL);
}

int construct_DCB ( void **pp_result ) 
{ 

    // [ C, B, A ] 
    construct_CBA(pp_result);

    // [ C, B, A ] -> insert(D) -> [ D, C, B ] 
    return cache_insert(*((cache **)pp_result), D_ELEMENT, NULL);
}

int construct_CA ( void **pp_result ) 
{ 

    // [ C, B, A ] 
    construct_CBA(pp_result);

    // [ C, B, A ] -> remove(B) -> [ C, A ]
    return cache_remove(*((cache **)pp_result), B_ELEMENT, NULL);
}

void *test_insert ( test_case *p_test_case, void *p_subject ) 
{ 

    // insert
    return (void *)(size_t)cache_insert((cache *)p_subject, p_test_case->p_parameters, NULL); 
}

void *test_find ( test_case *p_test_case, void *p_subject ) 
{ 

    // test
    if ( 0 == cache_find((cache *)p_subject, p_test_case->p_parameters, &p_test_case->p_out) ) return NULL;
    
    // success
    return (void *)1;
}

void *test_remove ( test_case *p_test_case, void *p_subject ) 
{ 

    // test
    if ( 0 == cache_remove((cache *)p_subject, p_test_case->p_parameters, &p_test_case->p_out) ) return NULL;

    // success
    return (void *)1;
}

void *test_size ( test_case *p_test_case, void *p_subject ) 
{ 

    // unused
    (void) p_test_case;

    // done
    return (void *)cache_size((cache *)p_subject);
}

bool find_results_match ( test_scenario *p_scenario, test_case *p_case, void *p_subject, void *p_result )
{

    // unused
    (void) p_scenario;
    (void) p_subject;
    (void) p_result;

    // fast exit
    if ( NULL == p_case->p_out ) return false;

    // done
    return ( 0 == strcmp((const char *)p_case->p_out, (const char *)p_case->p_parameters) );
}

bool remove_results_match ( test_scenario *p_scenario, test_case *p_case, void *p_subject, void *p_result )
{

    // unused
    (void) p_scenario;
    (void) p_subject;
    (void) p_result;

    // fast exit
    if ( NULL == p_case->p_out ) return false;

    // done
    return ( 0 == strcmp((const char *)p_case->p_out, (const char *)p_case->p_parameters) );
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

void *destruct_cache ( void *p_pointer, unsigned long long size )
{

    // unused
    (void) size;

    // initialized data
    cache *p_cache = (cache *)p_pointer;

    // destroy the cache
    cache_destroy(&p_cache);

    // success
    return NULL;
}
