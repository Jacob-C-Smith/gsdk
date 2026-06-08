/** ! 
 * circular buffer tester
 * 
 * @file src/test/circular_buffer_test.c
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
#include <data/circular_buffer.h>

// preprocessor macros
#define A_ELEMENT (void *)0x1
#define B_ELEMENT (void *)0x2
#define C_ELEMENT (void *)0x3
#define D_ELEMENT (void *)0x4

// function declarations
/// scenario constructors
fn_scenario_constructor construct_empty;
fn_scenario_constructor construct_A;
fn_scenario_constructor construct_AB;
fn_scenario_constructor construct_ABC;
fn_scenario_constructor construct_A_pop_empty;
fn_scenario_constructor construct_ABC_pop_BC;
fn_scenario_constructor construct_ABC_pop_pushD_BCD;

/// test cases
fn_test_case test_push;
fn_test_case test_pop;
fn_test_case test_peek;
fn_test_case test_empty;
fn_test_case test_full;
fn_test_case test_size;

/// result evaluators
fn_results_match pop_results_match;
fn_results_match peek_results_match;
fn_results_match size_results_match;
fn_results_match empty_results_match;
fn_results_match full_results_match;

/// allocators
fn_allocator destruct_circular_buffer;

// data
/// values
void *A_contents[]   = { A_ELEMENT, NULL };
void *AB_contents[]  = { A_ELEMENT, B_ELEMENT, NULL };
void *ABC_contents[] = { A_ELEMENT, B_ELEMENT, C_ELEMENT, NULL };
void *BC_contents[]  = { B_ELEMENT, C_ELEMENT, NULL };
void *BCD_contents[] = { B_ELEMENT, C_ELEMENT, D_ELEMENT, NULL };
void *_contents[]    = { NULL };

// test
/// cases
test_case _empty_test_cases[] = 
{
    TEST_CASE ("push A" , test_push , A_ELEMENT, TEST_RESULT_ONE),
    TEST_CASE ("pop"    , test_pop  , (void *)1, TEST_RESULT_ZERO),
    TEST_CASE ("peek"   , test_peek , NULL     , TEST_RESULT_ZERO),
    TEST_MATCH("empty"  , test_empty, NULL     , empty_results_match),
    TEST_MATCH("full"   , test_full , NULL     , full_results_match),
    TEST_MATCH("size"   , test_size , NULL     , size_results_match),
};

test_case _one_element_test_cases[] = 
{
    TEST_CASE ("push B" , test_push , B_ELEMENT, TEST_RESULT_ONE),
    TEST_MATCH("pop"    , test_pop  , (void *)1, pop_results_match),
    TEST_MATCH("peek"   , test_peek , NULL     , peek_results_match),
    TEST_MATCH("empty"  , test_empty, NULL     , empty_results_match),
    TEST_MATCH("full"   , test_full , NULL     , full_results_match),
    TEST_MATCH("size"   , test_size , NULL     , size_results_match),
};

test_case _two_element_test_cases[] = 
{
    TEST_CASE ("push C" , test_push , C_ELEMENT, TEST_RESULT_ONE),
    TEST_MATCH("pop"    , test_pop  , (void *)1, pop_results_match),
    TEST_MATCH("pop(2)" , test_pop  , (void *)2, pop_results_match),
    TEST_MATCH("peek"   , test_peek , NULL     , peek_results_match),
    TEST_MATCH("empty"  , test_empty, NULL     , empty_results_match),
    TEST_MATCH("full"   , test_full , NULL     , full_results_match),
    TEST_MATCH("size"   , test_size , NULL     , size_results_match),
};

test_case _three_element_test_cases[] = 
{
    TEST_CASE ("push D" , test_push , D_ELEMENT, TEST_RESULT_ONE), 
    TEST_MATCH("pop"    , test_pop  , (void *)1, pop_results_match),
    TEST_MATCH("pop(2)" , test_pop  , (void *)2, pop_results_match),
    TEST_MATCH("pop(3)" , test_pop  , (void *)3, pop_results_match),
    TEST_MATCH("peek"   , test_peek , NULL     , peek_results_match),
    TEST_MATCH("empty"  , test_empty, NULL     , empty_results_match),
    TEST_MATCH("full"   , test_full , NULL     , full_results_match),
    TEST_MATCH("size"   , test_size , NULL     , size_results_match),
};

/// scenarios
test_scenario _scenarios[] = 
{
    TEST_SCENARIO("empty"          , _contents   , _empty_test_cases        , construct_empty                , destruct_circular_buffer),
    TEST_SCENARIO("A"              , A_contents  , _one_element_test_cases  , construct_A                    , destruct_circular_buffer),
    TEST_SCENARIO("AB"             , AB_contents , _two_element_test_cases  , construct_AB                   , destruct_circular_buffer),
    TEST_SCENARIO("ABC"            , ABC_contents, _three_element_test_cases, construct_ABC                  , destruct_circular_buffer),
    TEST_SCENARIO("A_pop_empty"    , _contents   , _empty_test_cases        , construct_A_pop_empty          , destruct_circular_buffer),
    TEST_SCENARIO("BC"             , BC_contents , _two_element_test_cases  , construct_ABC_pop_BC           , destruct_circular_buffer),
    TEST_SCENARIO("BCD"            , BCD_contents, _three_element_test_cases, construct_ABC_pop_pushD_BCD    , destruct_circular_buffer),
};

/// suites
test_suite _suite = TEST_SUITE("circular buffer", _scenarios);

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

    // ... -> [ ]
    return circular_buffer_construct((circular_buffer **)pp_result, 3);
}

int construct_A ( void **pp_result ) 
{ 

    // [ ]
    construct_empty(pp_result);
    
    // [ ] -> push(A) -> [ A ]
    return circular_buffer_push(*((circular_buffer **)pp_result), A_ELEMENT); 
}

int construct_AB ( void **pp_result ) 
{ 

    // [ A ]
    construct_A(pp_result);
    
    // [ A, B ] -> push() -> [ A, B ]
    return circular_buffer_push(*((circular_buffer **)pp_result), B_ELEMENT); 
}

int construct_ABC ( void **pp_result ) 
{ 

    // [ A, B ]
    construct_AB(pp_result);
    
    // [ A, B ] -> push(C) -> [ A, B, C ]
    return circular_buffer_push(*((circular_buffer **)pp_result), C_ELEMENT); 
}

int construct_A_pop_empty ( void **pp_result ) 
{ 

    // [ A ]
    construct_A(pp_result);
    
    // [ A ] -> pop() -> [ ]
    return circular_buffer_pop(*((circular_buffer **)pp_result), NULL); 
}

int construct_ABC_pop_BC ( void **pp_result ) 
{ 

    // [ A, B, C ]
    construct_ABC(pp_result);
    
    // [ A, B, C ] -> pop() -> [ B, C ]
    return circular_buffer_pop(*((circular_buffer **)pp_result), NULL); 
}

int construct_ABC_pop_pushD_BCD ( void **pp_result ) 
{ 

    // [ A, B, C ]
    construct_ABC_pop_BC(pp_result);
    
    // [ A, B, C ] -> push(D) -> [ B, C, D ]
    return circular_buffer_push(*((circular_buffer **)pp_result), D_ELEMENT); 
}

void *test_push ( test_case *p_test_case, void *p_subject ) 
{ 

    // done
    return (void *)(size_t)circular_buffer_push((circular_buffer *)p_subject, p_test_case->p_parameters); 
}

void *test_pop ( test_case *p_test_case, void *p_subject ) 
{ 

    // initialized data
    circular_buffer *p_circular_buffer = (circular_buffer *)p_subject;
    size_t           pops              = (size_t)p_test_case->p_parameters;
    void            *p_res             = NULL;

    // pops
    for (size_t i = 0; i < pops; i++)

        // pop
        if ( 0 == circular_buffer_pop(p_circular_buffer, &p_res) ) return NULL;

    // store the result
    p_test_case->p_out = p_res;

    // success
    return (void *)1;
}

void *test_peek ( test_case *p_test_case, void *p_subject ) 
{ 

    // peek
    if ( 0 == circular_buffer_peek((circular_buffer *)p_subject, &p_test_case->p_out) ) return NULL;

    // success
    return (void *)1;
}

void *test_empty ( test_case *p_test_case, void *p_subject ) 
{ 

    // unused
    (void) p_test_case;

    // done
    return (void *)(size_t)circular_buffer_empty((circular_buffer *)p_subject);
}

void *test_full ( test_case *p_test_case, void *p_subject ) 
{ 

    // unused
    (void) p_test_case;

    // done
    return (void *)(size_t)circular_buffer_full((circular_buffer *)p_subject);
}

void *test_size ( test_case *p_test_case, void *p_subject ) 
{ 

    // unused
    (void) p_test_case;

    // done
    return (void *)circular_buffer_size((circular_buffer *)p_subject);
}

bool pop_results_match ( test_scenario *p_scenario, test_case *p_case, void *p_subject, void *p_result )
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

bool peek_results_match ( test_scenario *p_scenario, test_case *p_case, void *p_subject, void *p_result )
{

    // unused
    (void) p_subject;
    (void) p_result;

    // initialized data
    void **pp_contents = p_scenario->p_data;

    // done
    return p_case->p_out == pp_contents[0];
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

bool full_results_match ( test_scenario *p_scenario, test_case *p_case, void *p_subject, void *p_result )
{

    // unused
    (void) p_case;
    (void) p_subject;

    // initialized data
    void   **pp_contents = p_scenario->p_data;
    size_t   count       = 0;
    bool     full        = false;

    // count
    while ( pp_contents[count] ) count++;

    // full?
    full = ( count == 3 );

    // done
    return p_result == (void *)(size_t)full;
}

void *destruct_circular_buffer ( void *p_pointer, unsigned long long size )
{

    // unused
    (void) size;
    
    // initialized data
    circular_buffer *p_circular_buffer = (circular_buffer *)p_pointer;

    // destroy the circular buffer
    circular_buffer_destroy(&p_circular_buffer, NULL);

    // success
    return NULL;
}
