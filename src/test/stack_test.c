/** !
 * stack tester
 * 
 * @file src/test/stack_test.c
 *
 * @author Jacob Smith
*/

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// gsdk
/// core
#include <core/log.h>
#include <core/sync.h>
#include <core/test.h>

/// data
#include <data/stack.h>

// preprocessor macros
#define A_KEY "A"
#define B_KEY "B"
#define C_KEY "C"
#define X_KEY "X"

// function declarations
/// scenario constructors
fn_scenario_constructor construct_empty;
fn_scenario_constructor construct_empty_pushA_A;
fn_scenario_constructor construct_A_pop_empty;
fn_scenario_constructor construct_A_pushB_AB;
fn_scenario_constructor construct_AB_pop_A;
fn_scenario_constructor construct_AB_pushC_ABC;
fn_scenario_constructor construct_ABC_pop_AB;

/// test cases
fn_test_case test_push;
fn_test_case test_pop;
fn_test_case test_peek;
fn_test_case test_is_empty;

/// result evaluators
fn_results_match pop_results_match;
fn_results_match peek_results_match;
fn_results_match is_empty_results_match;

/// allocators
fn_allocator destruct_stack;

// data
/// values
char  *_keys     [] = { NULL };
char  *A_keys    [] = { A_KEY, NULL };
char  *AB_keys   [] = { A_KEY, B_KEY, NULL };
char  *ABC_keys  [] = { A_KEY, B_KEY, C_KEY, NULL };

// test
/// cases
test_case _empty_test_cases[] = 
{
    TEST_CASE ("push A"  , test_push    , A_KEY, TEST_RESULT_ONE),
    TEST_CASE ("pop"     , test_pop     , (void *)1, TEST_RESULT_ZERO),
    TEST_CASE ("peek"    , test_peek    , NULL , TEST_RESULT_ZERO),
    TEST_MATCH("is empty", test_is_empty, NULL , is_empty_results_match),
};

test_case _one_element_test_cases[] = 
{
    TEST_CASE ("push B"  , test_push    , B_KEY, TEST_RESULT_ONE),
    TEST_MATCH("pop"     , test_pop     , (void *)1, pop_results_match),
    TEST_CASE ("pop(2)"  , test_pop     , (void *)2, TEST_RESULT_ZERO),
    TEST_MATCH("peek"    , test_peek    , NULL , peek_results_match),
    TEST_MATCH("is empty", test_is_empty, NULL , is_empty_results_match),
};

test_case _two_element_test_cases[] = 
{
    TEST_CASE ("push C"  , test_push    , C_KEY, TEST_RESULT_ONE),
    TEST_MATCH("pop"     , test_pop     , (void *)1, pop_results_match),
    TEST_MATCH("pop(2)"  , test_pop     , (void *)2, pop_results_match),
    TEST_CASE ("pop(3)"  , test_pop     , (void *)3, TEST_RESULT_ZERO),
    TEST_MATCH("peek"    , test_peek    , NULL , peek_results_match),
    TEST_MATCH("is empty", test_is_empty, NULL , is_empty_results_match),
};

test_case _three_element_test_cases[] = 
{
    TEST_CASE ("push X"  , test_push    , X_KEY, TEST_RESULT_ZERO),
    TEST_MATCH("pop"     , test_pop     , (void *)1, pop_results_match),
    TEST_MATCH("pop(2)"  , test_pop     , (void *)2, pop_results_match),
    TEST_MATCH("pop(3)"  , test_pop     , (void *)3, pop_results_match),
    TEST_CASE ("pop(4)"  , test_pop     , (void *)4, TEST_RESULT_ZERO),
    TEST_MATCH("peek"    , test_peek    , NULL , peek_results_match),
    TEST_MATCH("is empty", test_is_empty, NULL , is_empty_results_match),
};

/// scenarios
test_scenario _scenarios[] = 
{
    TEST_SCENARIO("empty"          , _keys   , _empty_test_cases        , construct_empty          , destruct_stack),
    TEST_SCENARIO("empty_pushA_A"  , A_keys  , _one_element_test_cases  , construct_empty_pushA_A  , destruct_stack),
    TEST_SCENARIO("A_pop_empty"    , _keys   , _empty_test_cases        , construct_A_pop_empty    , destruct_stack),
    TEST_SCENARIO("A_pushB_AB"     , AB_keys , _two_element_test_cases  , construct_A_pushB_AB     , destruct_stack),
    TEST_SCENARIO("AB_pop_A"       , A_keys  , _one_element_test_cases  , construct_AB_pop_A       , destruct_stack),
    TEST_SCENARIO("AB_pushC_ABC"   , ABC_keys, _three_element_test_cases, construct_AB_pushC_ABC   , destruct_stack),
    TEST_SCENARIO("ABC_pop_AB"     , AB_keys , _two_element_test_cases  , construct_ABC_pop_AB     , destruct_stack),
};

/// suites
test_suite _suite = TEST_SUITE("stack", _scenarios);

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
    return stack_construct((stack **)pp_result, 3);
}

int construct_empty_pushA_A ( void **pp_result ) 
{ 

    // [ ]
    construct_empty(pp_result);

    // [ ] -> push(A) -> [ A ]
    return stack_push(*((stack **)pp_result), A_KEY); 
}

int construct_A_pop_empty ( void **pp_result ) 
{ 

    // [ A ]
    construct_empty_pushA_A(pp_result);

    // [ A ] -> pop() -> [ ]
    return stack_pop(*((stack **)pp_result), NULL); 
}

int construct_A_pushB_AB ( void **pp_result ) 
{ 

    // [ A ]
    construct_empty_pushA_A(pp_result);

    // [ A ] -> push(B) -> [ A, B ]
    return stack_push(*((stack **)pp_result), B_KEY); 
}

int construct_AB_pop_A ( void **pp_result )
{

    // [ A, B ]
    construct_A_pushB_AB(pp_result);

    // [ A, B ] -> pop() -> [ A ]
    return stack_pop(*((stack **)pp_result), NULL); 
}

int construct_AB_pushC_ABC ( void **pp_result ) 
{ 

    // [ A, B ]
    construct_A_pushB_AB(pp_result);

    // [ A, B ] -> push(C) -> [ A, B, C ]
    return stack_push(*((stack **)pp_result), C_KEY); 
}

int construct_ABC_pop_AB ( void **pp_result ) 
{ 

    // [ A, B, C ]
    construct_AB_pushC_ABC(pp_result);

    // [ A, B, C ] -> pop() -> [ A, B ]
    return stack_pop(*((stack **)pp_result), NULL); 
}

void *test_push ( test_case *p_test_case, void *p_subject ) 
{ 

    // initialized data
    stack *p_stack = (stack *)p_subject;
    
    // test
    return (void *)(size_t)stack_push(p_stack, p_test_case->p_parameters); 
}

void *test_pop ( test_case *p_test_case, void *p_subject ) 
{ 

    // initialized data
    stack  *p_stack = (stack *)p_subject;
    size_t  pops    = (size_t)p_test_case->p_parameters;
    void   *p_res   = NULL;

    // test
    for (size_t i = 0; i < pops; i++)
        if ( 0 == stack_pop(p_stack, &p_res) ) return NULL;

    // store the result
    p_test_case->p_out = p_res;

    // success
    return (void *)1;
}

void *test_peek ( test_case *p_test_case, void *p_subject ) 
{ 

    // initialized data
    stack *p_stack = (stack *)p_subject;
    
    // test
    if ( 0 == stack_peek(p_stack, &p_test_case->p_out) ) return NULL;

    // success
    return (void *)1;
}

void *test_is_empty ( test_case *p_test_case, void *p_subject ) 
{ 

    // unused
    (void) p_test_case;

    // initialized data
    stack *p_stack = (stack *)p_subject;

    // done
    return (void *)(size_t) stack_is_empty(p_stack);
}

bool pop_results_match ( test_scenario *p_scenario, test_case *p_case, void *p_subject, void *p_result )
{
    
    // unused
    (void) p_subject;
    (void) p_result;

    // initialized data
    char   **pp_keys = p_scenario->p_data;
    size_t   pops    = (size_t)p_case->p_parameters;
    size_t   count   = 0;

    // find the end
    while ( pp_keys[count] )
        count++;

    // done
    return p_case->p_out == pp_keys[count - pops];
}

bool peek_results_match ( test_scenario *p_scenario, test_case *p_case, void *p_subject, void *p_result )
{
    
    // unused
    (void) p_subject;
    (void) p_result;

    // initialized data
    char   **pp_keys = p_scenario->p_data;
    size_t   count   = 0;

    // find the end
    while ( pp_keys[count] )
        count++;

    // done
    return p_case->p_out == pp_keys[count - 1];
}

bool is_empty_results_match ( test_scenario *p_scenario, test_case *p_case, void *p_subject, void *p_result )
{
    
    // unused
    (void) p_scenario;
    (void) p_case;
    (void) p_subject;

    // initialized data
    char **pp_keys = p_scenario->p_data;
    bool   result  = ( pp_keys[0] == NULL );

    // done
    return p_result == (void *)(size_t)result;
}

void *destruct_stack ( void *p_pointer, unsigned long long size )
{

    // unused
    (void) size;

    // initialized data
    stack *p_stack = (stack *)p_pointer;

    // release the stack
    stack_destroy(&p_stack, NULL);

    // success
    return NULL;
}
