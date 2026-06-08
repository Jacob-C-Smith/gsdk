/** !
 * double queue tester
 *
 * @file src/test/double_queue_test.c
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
#include <data/double_queue.h>

// preprocessor macros
#define A_ELEMENT (void *)0x1
#define B_ELEMENT (void *)0x2
#define C_ELEMENT (void *)0x3
#define D_ELEMENT (void *)0x4

// function declarations
/// scenario constructors
fn_scenario_constructor construct_empty;
fn_scenario_constructor construct_empty_front_addA_A;
fn_scenario_constructor construct_empty_rear_addA_A;
fn_scenario_constructor construct_empty_rear_addB_B;
fn_scenario_constructor construct_A_front_remove_empty; 
fn_scenario_constructor construct_A_rear_remove_empty; 
fn_scenario_constructor construct_A_front_addB_BA; 
fn_scenario_constructor construct_A_rear_addB_AB; 
fn_scenario_constructor construct_AB_front_remove_B;
fn_scenario_constructor construct_AB_rear_remove_A;
fn_scenario_constructor construct_BA_front_remove_A;
fn_scenario_constructor construct_BA_rear_remove_B;
fn_scenario_constructor construct_AB_rear_addC_ABC;
fn_scenario_constructor construct_BA_front_addC_CBA;

/// test cases
fn_test_case test_front_add;
fn_test_case test_rear_add;
fn_test_case test_front_remove;
fn_test_case test_rear_remove;
fn_test_case test_front;
fn_test_case test_rear;
fn_test_case test_empty;
fn_test_case test_size;

/// result evaluators
fn_results_match front_results_match;
fn_results_match rear_results_match;
fn_results_match front_remove_results_match;
fn_results_match rear_remove_results_match;
fn_results_match empty_results_match;
fn_results_match size_results_match;

/// allocators
fn_allocator destruct_double_queue;

// data
/// values
void *A_contents[]   = { A_ELEMENT, NULL };
void *B_contents[]   = { B_ELEMENT, NULL };
void *AB_contents[]  = { A_ELEMENT, B_ELEMENT, NULL };
void *BA_contents[]  = { B_ELEMENT, A_ELEMENT, NULL };
void *ABC_contents[] = { A_ELEMENT, B_ELEMENT, C_ELEMENT, NULL };
void *CBA_contents[] = { C_ELEMENT, B_ELEMENT, A_ELEMENT, NULL };
void *_contents[]    = { NULL };

// test
/// cases
test_case _empty_test_cases[] = 
{
    TEST_CASE ("front add A" , test_front_add   , A_ELEMENT, TEST_RESULT_ONE),
    TEST_CASE ("rear add A"  , test_rear_add    , A_ELEMENT, TEST_RESULT_ONE),
    TEST_CASE ("front remove", test_front_remove, (void *)1, TEST_RESULT_ZERO),
    TEST_CASE ("rear remove" , test_rear_remove , (void *)1, TEST_RESULT_ZERO),
    TEST_CASE ("front"       , test_front       , NULL     , TEST_RESULT_ZERO),
    TEST_CASE ("rear"        , test_rear        , NULL     , TEST_RESULT_ZERO),
    TEST_MATCH("empty"       , test_empty       , NULL     , empty_results_match),
    TEST_MATCH("size"        , test_size        , NULL     , size_results_match),
};

test_case _one_element_test_cases[] = 
{
    TEST_CASE ("front add D" , test_front_add   , D_ELEMENT, TEST_RESULT_ONE),
    TEST_CASE ("rear add D"  , test_rear_add    , D_ELEMENT, TEST_RESULT_ONE),
    TEST_MATCH("front remove", test_front_remove, (void *)1, front_remove_results_match),
    TEST_MATCH("rear remove" , test_rear_remove , (void *)1, rear_remove_results_match),
    TEST_MATCH("front"       , test_front       , NULL     , front_results_match),
    TEST_MATCH("rear"        , test_rear        , NULL     , rear_results_match),
    TEST_MATCH("empty"       , test_empty       , NULL     , empty_results_match),
    TEST_MATCH("size"        , test_size        , NULL     , size_results_match),
};

test_case _two_element_test_cases[] = 
{
    TEST_CASE ("front add D" , test_front_add   , D_ELEMENT, TEST_RESULT_ONE),
    TEST_CASE ("rear add D"  , test_rear_add    , D_ELEMENT, TEST_RESULT_ONE),
    TEST_MATCH("front remove", test_front_remove, (void *)1, front_remove_results_match),
    TEST_MATCH("rear remove" , test_rear_remove , (void *)1, rear_remove_results_match),
    TEST_MATCH("front"       , test_front       , NULL     , front_results_match),
    TEST_MATCH("rear"        , test_rear        , NULL     , rear_results_match),
    TEST_MATCH("empty"       , test_empty       , NULL     , empty_results_match),
    TEST_MATCH("size"        , test_size        , NULL     , size_results_match),
};

test_case _three_element_test_cases[] = 
{
    TEST_CASE ("front add D" , test_front_add   , D_ELEMENT, TEST_RESULT_ONE),
    TEST_CASE ("rear add D"  , test_rear_add    , D_ELEMENT, TEST_RESULT_ONE),
    TEST_MATCH("front remove", test_front_remove, (void *)1, front_remove_results_match),
    TEST_MATCH("rear remove" , test_rear_remove , (void *)1, rear_remove_results_match),
    TEST_MATCH("front"       , test_front       , NULL     , front_results_match),
    TEST_MATCH("rear"        , test_rear        , NULL     , rear_results_match),
    TEST_MATCH("empty"       , test_empty       , NULL     , empty_results_match),
    TEST_MATCH("size"        , test_size        , NULL     , size_results_match),
};

/// scenarios
test_scenario _scenarios[] = 
{
    TEST_SCENARIO("empty"               , _contents   , _empty_test_cases        , construct_empty               , destruct_double_queue),
    TEST_SCENARIO("empty_front_addA_A"  , A_contents  , _one_element_test_cases  , construct_empty_front_addA_A  , destruct_double_queue),
    TEST_SCENARIO("empty_rear_addA_A"   , A_contents  , _one_element_test_cases  , construct_empty_rear_addA_A   , destruct_double_queue),
    TEST_SCENARIO("empty_rear_addB_B"   , B_contents  , _one_element_test_cases  , construct_empty_rear_addB_B   , destruct_double_queue),
    TEST_SCENARIO("A_front_remove_empty", _contents   , _empty_test_cases        , construct_A_front_remove_empty, destruct_double_queue),
    TEST_SCENARIO("A_rear_remove_empty" , _contents   , _empty_test_cases        , construct_A_rear_remove_empty , destruct_double_queue),
    TEST_SCENARIO("A_front_addB_BA"     , BA_contents , _two_element_test_cases  , construct_A_front_addB_BA     , destruct_double_queue),
    TEST_SCENARIO("A_rear_addB_AB"      , AB_contents , _two_element_test_cases  , construct_A_rear_addB_AB      , destruct_double_queue),
    TEST_SCENARIO("AB_front_remove_B"   , B_contents  , _one_element_test_cases  , construct_AB_front_remove_B   , destruct_double_queue),
    TEST_SCENARIO("AB_rear_remove_A"    , A_contents  , _one_element_test_cases  , construct_AB_rear_remove_A    , destruct_double_queue),
    TEST_SCENARIO("BA_front_remove_A"   , A_contents  , _one_element_test_cases  , construct_BA_front_remove_A   , destruct_double_queue),
    TEST_SCENARIO("BA_rear_remove_B"    , B_contents  , _one_element_test_cases  , construct_BA_rear_remove_B    , destruct_double_queue),
    TEST_SCENARIO("AB_rear_addC_ABC"    , ABC_contents, _three_element_test_cases, construct_AB_rear_addC_ABC    , destruct_double_queue),
    TEST_SCENARIO("BA_front_addC_CBA"   , CBA_contents, _three_element_test_cases, construct_BA_front_addC_CBA   , destruct_double_queue),
};

/// suites
test_suite _suite = TEST_SUITE("double queue", _scenarios);

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

    // ... -> [ ]
    return double_queue_construct((double_queue **)pp_result);
}

int construct_empty_front_addA_A ( void **pp_result ) 
{

    // [ ]
    construct_empty(pp_result);

    // [ ] -> front_add(A) -> [ A ]
    return double_queue_front_add(*((double_queue **)pp_result), A_ELEMENT); 
}

int construct_empty_rear_addA_A ( void **pp_result ) 
{

    // [ ]
    construct_empty(pp_result);

    // [ ] -> rear_add(A) -> [ A ]
    return double_queue_rear_add(*((double_queue **)pp_result), A_ELEMENT); 
}

int construct_empty_rear_addB_B ( void **pp_result ) 
{

    // [ ]
    construct_empty(pp_result);

    // [ ] -> rear_add(B) -> [ B ]
    return double_queue_rear_add(*((double_queue **)pp_result), B_ELEMENT); 
}

int construct_A_front_remove_empty ( void **pp_result ) 
{

    // [ A ]
    construct_empty_front_addA_A(pp_result);

    // [ A ] -> front_remove() -> [ ]
    return double_queue_front_remove(*((double_queue **)pp_result), NULL); 
}

int construct_A_rear_remove_empty ( void **pp_result ) 
{

    // [ A ]
    construct_empty_front_addA_A(pp_result);

    // [ A ] -> rear_remove() -> [ ]
    return double_queue_rear_remove(*((double_queue **)pp_result), NULL); 
}

int construct_A_front_addB_BA ( void **pp_result ) 
{

    // [ A ]
    construct_empty_front_addA_A(pp_result);

    // [ A ] -> front_add(B) -> [ B, A ]
    return double_queue_front_add(*((double_queue **)pp_result), B_ELEMENT); 
}

int construct_A_rear_addB_AB ( void **pp_result ) 
{

    // [ A ]
    construct_empty_front_addA_A(pp_result);

    // [ A ] -> rear_add(B) -> [ A, B ]
    return double_queue_rear_add(*((double_queue **)pp_result), B_ELEMENT); 
}

int construct_AB_front_remove_B ( void **pp_result ) 
{

    // [ A, B ]
    construct_A_rear_addB_AB(pp_result);

    // [ A, B ] -> front_remove() -> [ B ]
    return double_queue_front_remove(*((double_queue **)pp_result), NULL); 
}

int construct_AB_rear_remove_A ( void **pp_result ) 
{

    // [ A, B ]
    construct_A_rear_addB_AB(pp_result);

    // [ A, B ] -> rear_remove() -> [ A ]
    return double_queue_rear_remove(*((double_queue **)pp_result), NULL); 
}

int construct_BA_front_remove_A ( void **pp_result ) 
{

    // [ B, A ]
    construct_A_front_addB_BA(pp_result);

    // [ B, A ] -> front_remove() -> [ A ]
    return double_queue_front_remove(*((double_queue **)pp_result), NULL); 
}

int construct_BA_rear_remove_B ( void **pp_result ) 
{

    // [ B, A ]
    construct_A_front_addB_BA(pp_result);

    // [ B, A ] -> rear_remove() -> [ B ]
    return double_queue_rear_remove(*((double_queue **)pp_result), NULL); 
}

int construct_AB_rear_addC_ABC ( void **pp_result ) 
{

    // [ A, B ]
    construct_A_rear_addB_AB(pp_result);

    // [ A, B ] -> rear_add(C) -> [ A, B, C ]
    return double_queue_rear_add(*((double_queue **)pp_result), C_ELEMENT); 
}

int construct_BA_front_addC_CBA ( void **pp_result ) 
{

    // [ B, A ]
    construct_A_front_addB_BA(pp_result);

    // [ B, A ] -> front_add(C) -> [ C, B, A ]
    return double_queue_front_add(*((double_queue **)pp_result), C_ELEMENT); 
}

void *test_front_add ( test_case *p_test_case, void *p_subject ) 
{

    // done
    return (void *)(size_t)double_queue_front_add((double_queue *)p_subject, p_test_case->p_parameters); 
}

void *test_rear_add ( test_case *p_test_case, void *p_subject ) 
{

    // done
    return (void *)(size_t)double_queue_rear_add((double_queue *)p_subject, p_test_case->p_parameters); 
}

void *test_front_remove ( test_case *p_test_case, void *p_subject ) 
{

    // initialized data
    double_queue *p_double_queue = (double_queue *)p_subject;
    size_t        removes        = (size_t)p_test_case->p_parameters;
    void         *p_res          = NULL;

    // removes
    for (size_t i = 0; i < removes; i++)

        // remove
        if ( 0 == double_queue_front_remove(p_double_queue, &p_res) ) return NULL;
    
    // store the result
    p_test_case->p_out = p_res;

    // success
    return (void *)1;
}

void *test_rear_remove ( test_case *p_test_case, void *p_subject ) 
{

    // initialized data
    double_queue *p_double_queue = (double_queue *)p_subject;
    size_t        removes        = (size_t)p_test_case->p_parameters;
    void         *p_res          = NULL;

    // removes
    for (size_t i = 0; i < removes; i++)

        // remove
        if ( 0 == double_queue_rear_remove(p_double_queue, &p_res) ) return NULL;
    
    // store the result
    p_test_case->p_out = p_res;

    // success
    return (void *)1;
}

void *test_front ( test_case *p_test_case, void *p_subject ) 
{

    // test
    if ( 0 == double_queue_front((double_queue *)p_subject, &p_test_case->p_out) ) return NULL;

    // success
    return (void *)1;
}

void *test_rear ( test_case *p_test_case, void *p_subject ) 
{

    // test
    if ( 0 == double_queue_rear((double_queue *)p_subject, &p_test_case->p_out) ) return NULL;

    // success
    return (void *)1;
}

void *test_empty ( test_case *p_test_case, void *p_subject ) 
{

    // unused
    (void) p_test_case;

    // done
    return (void *)(size_t)double_queue_empty((double_queue *)p_subject);
}

void *test_size ( test_case *p_test_case, void *p_subject ) 
{

    // unused
    (void) p_test_case;

    // done
    return (void *)double_queue_size((double_queue *)p_subject);
}

bool front_results_match ( test_scenario *p_scenario, test_case *p_case, void *p_subject, void *p_result )
{

    // unused
    (void) p_subject;
    (void) p_result;

    // initialized data
    void **pp_contents = p_scenario->p_data;

    // done
    return p_case->p_out == pp_contents[0];
}

bool rear_results_match ( test_scenario *p_scenario, test_case *p_case, void *p_subject, void *p_result )
{

    // unused
    (void) p_subject;
    (void) p_result;

    // initialized data
    void   **pp_contents = p_scenario->p_data;
    size_t   count       = 0;

    // count
    while ( pp_contents[count] ) count++;

    // done
    return p_case->p_out == pp_contents[count - 1];
}

bool front_remove_results_match ( test_scenario *p_scenario, test_case *p_case, void *p_subject, void *p_result )
{

    // unused
    (void) p_subject;
    (void) p_result;

    // initialized data
    void   **pp_contents = p_scenario->p_data;
    size_t   index       = (size_t)p_case->p_parameters - 1;

    // done
    return p_case->p_out == pp_contents[index];
}

bool rear_remove_results_match ( test_scenario *p_scenario, test_case *p_case, void *p_subject, void *p_result )
{

    // unused
    (void) p_subject;
    (void) p_result;

    // initialized data
    void   **pp_contents = p_scenario->p_data;
    size_t   count       = 0;
    size_t   index       = 0;

    // count
    while ( pp_contents[count] ) count++;

    // store the index
    index = count - (size_t)p_case->p_parameters;

    // done
    return p_case->p_out == pp_contents[index];
}

bool size_results_match ( test_scenario *p_scenario, test_case *p_case, void *p_subject, void *p_result )
{

    // unused
    (void) p_case;
    (void) p_subject;

    // initialized data
    void   **pp_contents = p_scenario->p_data;
    size_t   count       = 0;

    // count
    while ( pp_contents[count] ) count++;

    // done
    return (size_t)p_result == count;
}

bool empty_results_match ( test_scenario *p_scenario, test_case *p_case, void *p_subject, void *p_result )
{

    // unused
    (void) p_case; 
    (void) p_subject;

    // initialized data
    void **pp_contents = p_scenario->p_data;
    bool   empty       = ( pp_contents[0] == NULL );

    // done
    return p_result == (void *)(size_t)empty;
}

void *destruct_double_queue ( void *p_pointer, unsigned long long size )
{

    // unused
    (void) size;

    // initialized data
    double_queue *p_double_queue = (double_queue *)p_pointer;
    
    // release the double queue
    double_queue_destroy(&p_double_queue, NULL);

    // success
    return NULL;
}
