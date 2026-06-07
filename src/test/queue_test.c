/** !
 * queue tester
 * 
 * @file src/test/queue_test.c
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
#include <core/test.h>

/// data
#include <data/queue.h>

// preprocessor macros
#define A_ELEMENT (void *)0x1
#define B_ELEMENT (void *)0x2
#define C_ELEMENT (void *)0x3
#define D_ELEMENT (void *)0x4

// function declarations
/// scenario constructors
fn_scenario_constructor construct_empty;
fn_scenario_constructor construct_empty_enqueueA_A;
fn_scenario_constructor construct_empty_enqueueB_B;
fn_scenario_constructor construct_A_dequeue_empty;
fn_scenario_constructor construct_B_dequeue_empty;
fn_scenario_constructor construct_A_enqueueB_BA;
fn_scenario_constructor construct_B_enqueueA_AB;
fn_scenario_constructor construct_AB_dequeue_A;
fn_scenario_constructor construct_BA_dequeue_B;
fn_scenario_constructor construct_AB_enqueueC_CAB;
fn_scenario_constructor construct_BA_enqueueC_CBA;
fn_scenario_constructor construct_CAB_dequeue_CA;
fn_scenario_constructor construct_CBA_dequeue_CB;

/// test cases
fn_test_case test_enqueue;
fn_test_case test_dequeue;
fn_test_case test_front;
fn_test_case test_rear;
fn_test_case test_empty;

/// result evaluators
fn_results_match dequeue_results_match;
fn_results_match front_results_match;
fn_results_match rear_results_match;
fn_results_match empty_results_match;

/// allocators
fn_allocator destruct_queue;

// data
/// values
void *_contents    [] = { NULL };
void *A_contents   [] = { A_ELEMENT, NULL };
void *B_contents   [] = { B_ELEMENT, NULL };
void *AB_contents  [] = { A_ELEMENT, B_ELEMENT, NULL };
void *BA_contents  [] = { B_ELEMENT, A_ELEMENT, NULL };
void *BC_contents  [] = { B_ELEMENT, C_ELEMENT, NULL };
void *CA_contents  [] = { C_ELEMENT, A_ELEMENT, NULL };
void *CB_contents  [] = { C_ELEMENT, B_ELEMENT, NULL };
void *CAB_contents [] = { C_ELEMENT, A_ELEMENT, B_ELEMENT, NULL };
void *CBA_contents [] = { C_ELEMENT, B_ELEMENT, A_ELEMENT, NULL };

// test
/// cases
test_case _empty_test_cases[] = 
{
    TEST_CASE ("enqueue A", test_enqueue, A_ELEMENT, TEST_RESULT_ONE),
    TEST_CASE ("dequeue"  , test_dequeue, (void *)1, TEST_RESULT_ZERO),
    TEST_CASE ("front"    , test_front  , NULL     , TEST_RESULT_ZERO),
    TEST_CASE ("rear"     , test_rear   , NULL     , TEST_RESULT_ZERO),
    TEST_MATCH("empty"    , test_empty  , NULL     , empty_results_match),
};

test_case _one_element_test_cases[] = 
{
    TEST_CASE ("enqueue D"  , test_enqueue, D_ELEMENT, TEST_RESULT_ONE),
    TEST_MATCH("dequeue"    , test_dequeue, (void *)1, dequeue_results_match),
    TEST_CASE ("dequeue(3)" , test_dequeue, (void *)3, TEST_RESULT_ZERO),
    TEST_MATCH("front"      , test_front  , NULL     , front_results_match),
    TEST_MATCH("rear"       , test_rear   , NULL     , rear_results_match),
    TEST_MATCH("empty"      , test_empty  , NULL     , empty_results_match),
};

test_case _two_element_test_cases[] = 
{
    TEST_CASE ("enqueue D"  , test_enqueue, D_ELEMENT, TEST_RESULT_ONE),
    TEST_MATCH("dequeue"    , test_dequeue, (void *)1, dequeue_results_match),
    TEST_MATCH("dequeue(2)" , test_dequeue, (void *)2, dequeue_results_match),
    TEST_CASE ("dequeue(3)" , test_dequeue, (void *)3, TEST_RESULT_ZERO),
    TEST_MATCH("front"      , test_front  , NULL     , front_results_match),
    TEST_MATCH("rear"       , test_rear   , NULL     , rear_results_match),
    TEST_MATCH("empty"      , test_empty  , NULL     , empty_results_match),
};

test_case _three_element_test_cases[] = 
{
    TEST_CASE ("enqueue D"  , test_enqueue, D_ELEMENT, TEST_RESULT_ONE),
    TEST_MATCH("dequeue"    , test_dequeue, (void *)1, dequeue_results_match),
    TEST_MATCH("dequeue(2)" , test_dequeue, (void *)2, dequeue_results_match),
    TEST_MATCH("dequeue(3)" , test_dequeue, (void *)3, dequeue_results_match),
    TEST_CASE ("dequeue(4)" , test_dequeue, (void *)4, TEST_RESULT_ZERO),
    TEST_MATCH("front"      , test_front  , NULL     , front_results_match),
    TEST_MATCH("rear"       , test_rear   , NULL     , rear_results_match),
    TEST_MATCH("empty"      , test_empty  , NULL     , empty_results_match),
};

/// scenarios
test_scenario _scenarios[] = 
{
    TEST_SCENARIO("empty"            , _contents   , _empty_test_cases        , construct_empty            , destruct_queue),
    TEST_SCENARIO("empty_enqueueA_A" , A_contents  , _one_element_test_cases  , construct_empty_enqueueA_A , destruct_queue),
    TEST_SCENARIO("empty_enqueueB_B" , B_contents  , _one_element_test_cases  , construct_empty_enqueueB_B , destruct_queue),
    TEST_SCENARIO("A_dequeue_empty"  , _contents   , _empty_test_cases        , construct_A_dequeue_empty  , destruct_queue),
    TEST_SCENARIO("B_dequeue_empty"  , _contents   , _empty_test_cases        , construct_B_dequeue_empty  , destruct_queue),
    TEST_SCENARIO("A_enqueueB_BA"    , BA_contents , _two_element_test_cases  , construct_A_enqueueB_BA    , destruct_queue),
    TEST_SCENARIO("B_enqueueA_AB"    , AB_contents , _two_element_test_cases  , construct_B_enqueueA_AB    , destruct_queue),
    TEST_SCENARIO("AB_dequeue_A"     , A_contents  , _one_element_test_cases  , construct_AB_dequeue_A     , destruct_queue),
    TEST_SCENARIO("BA_dequeue_B"     , B_contents  , _one_element_test_cases  , construct_BA_dequeue_B     , destruct_queue),
    TEST_SCENARIO("AB_enqueueC_CAB"  , CAB_contents, _three_element_test_cases, construct_AB_enqueueC_CAB  , destruct_queue),
    TEST_SCENARIO("BA_enqueueC_CBA"  , CBA_contents, _three_element_test_cases, construct_BA_enqueueC_CBA  , destruct_queue),
    TEST_SCENARIO("CAB_dequeue_CA"   , CA_contents , _two_element_test_cases  , construct_CAB_dequeue_CA   , destruct_queue),
    TEST_SCENARIO("CBA_dequeue_CB"   , CB_contents , _two_element_test_cases  , construct_CBA_dequeue_CB   , destruct_queue),
};

/// suites
test_suite _suite = TEST_SUITE("queue", _scenarios);

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

    // ... -> []
    return queue_construct((queue **)pp_result);
}

int construct_empty_enqueueA_A ( void **pp_result ) 
{ 

    // []
    construct_empty(pp_result);

    // [] -> enqueue(A) -> [A]
    return queue_enqueue(*((queue **)pp_result), A_ELEMENT); 
}

int construct_empty_enqueueB_B ( void **pp_result ) 
{ 

    // []
    construct_empty(pp_result);

    // [] -> enqueue(B) -> [B]
    return queue_enqueue(*((queue **)pp_result), B_ELEMENT); 
}

int construct_A_dequeue_empty ( void **pp_result ) 
{ 

    // [A]
    construct_empty_enqueueA_A(pp_result);

    // [A] -> dequeue() -> []
    return queue_dequeue(*((queue **)pp_result), NULL); 
}

int construct_B_dequeue_empty ( void **pp_result ) 
{ 

    // [B]
    construct_empty_enqueueB_B(pp_result);

    // [B] -> dequeue() -> []
    return queue_dequeue(*((queue **)pp_result), NULL); 
}

int construct_A_enqueueB_BA ( void **pp_result ) 
{ 

    // [A]
    construct_empty_enqueueA_A(pp_result);

    // [A] -> enqueue(B) -> [B, A]
    return queue_enqueue(*((queue **)pp_result), B_ELEMENT); 
}

int construct_B_enqueueA_AB ( void **pp_result ) 
{ 

    // [B]
    construct_empty_enqueueB_B(pp_result);

    // [B] -> enqueue(A) -> [A, B]
    return queue_enqueue(*((queue **)pp_result), A_ELEMENT); 
}

int construct_AB_dequeue_A ( void **pp_result )
{

    // [A, B]
    construct_B_enqueueA_AB(pp_result);

    // [A, B] -> dequeue() -> [A]
    return queue_dequeue(*((queue **)pp_result), NULL); 
}

int construct_BA_dequeue_B ( void **pp_result )
{

    // [B, A]
    construct_A_enqueueB_BA(pp_result);

    // [B, A] -> dequeue() -> [B]
    return queue_dequeue(*((queue **)pp_result), NULL); 
}

int construct_AB_enqueueC_CAB ( void **pp_result ) 
{ 

    // [A, B]
    construct_B_enqueueA_AB(pp_result);

    // [A, B] -> enqueue(C) -> [C, A, B]
    return queue_enqueue(*((queue **)pp_result), C_ELEMENT); 
}

int construct_BA_enqueueC_CBA ( void **pp_result ) 
{ 

    // [B, A]
    construct_A_enqueueB_BA(pp_result);

    // [B, A] -> enqueue(C) -> [C, B, A]
    return queue_enqueue(*((queue **)pp_result), C_ELEMENT); 
}

int construct_CAB_dequeue_CA ( void **pp_result ) 
{ 

    // [C, A, B]
    construct_AB_enqueueC_CAB(pp_result);

    // [C, A, B] -> dequeue() -> [C, A]
    return queue_dequeue(*((queue **)pp_result), NULL); 
}

int construct_CBA_dequeue_CB ( void **pp_result ) 
{ 

    // [C, B, A]
    construct_BA_enqueueC_CBA(pp_result);

    // [C, B, A] -> dequeue() -> [C, B]
    return queue_dequeue(*((queue **)pp_result), NULL); 
}

void *test_enqueue ( test_case *p_test_case, void *p_subject ) 
{ 

    // initialized data
    queue *p_queue = (queue *)p_subject;
    
    // done
    return (void *)(size_t)queue_enqueue(p_queue, p_test_case->p_parameters); 
}

void *test_dequeue ( test_case *p_test_case, void *p_subject ) 
{ 

    // initialized data
    queue  *p_queue  = (queue *)p_subject;
    size_t  dequeues = (size_t)p_test_case->p_parameters;
    void   *p_result = NULL;

    // test
    for (size_t i = 0; i < dequeues; i++)
        if ( 0 == queue_dequeue(p_queue, &p_result) ) return NULL;

    // store the result
    p_test_case->p_out = p_result;

    // done
    return (void *)1;
}

void *test_front ( test_case *p_test_case, void *p_subject ) 
{ 

    // initialized data
    queue *p_queue = (queue *)p_subject;
    
    // test
    if ( 0 == queue_front(p_queue, &p_test_case->p_out) ) return NULL;

    // done
    return (void *)1;
}

void *test_rear ( test_case *p_test_case, void *p_subject ) 
{ 

    // initialized data
    queue *p_queue = (queue *)p_subject;
    
    // test
    if ( 0 == queue_rear(p_queue, &p_test_case->p_out) ) return NULL;

    // done
    return (void *)1;
}

void *test_empty ( test_case *p_test_case, void *p_subject ) 
{ 

    // unused
    (void) p_test_case;

    // initialized data
    queue *p_queue = (queue *)p_subject;

    // done
    return (void *)(size_t) queue_empty(p_queue);
}

bool dequeue_results_match ( test_scenario *p_scenario, test_case *p_case, void *p_subject, void *p_result )
{
    
    // unused
    (void) p_subject;
    (void) p_result;

    // initialized data
    void **pp_contents = p_scenario->p_data;
    size_t dequeues    = (size_t)p_case->p_parameters;
    size_t count       = 0;

    // find the end
    while ( pp_contents[count] ) count++;

    // done
    return p_case->p_out == pp_contents[count - dequeues];
}

bool front_results_match ( test_scenario *p_scenario, test_case *p_case, void *p_subject, void *p_result )
{
    
    // unused
    (void) p_subject;
    (void) p_result;

    // initialized data
    void **pp_contents = p_scenario->p_data;
    size_t count       = 0;

    // find the end
    while ( pp_contents[count] ) count++;

    // done
    return p_case->p_out == pp_contents[count - 1];
}

bool rear_results_match ( test_scenario *p_scenario, test_case *p_case, void *p_subject, void *p_result )
{
    
    // unused
    (void) p_subject;
    (void) p_result;

    // initialized data
    void **pp_contents = p_scenario->p_data;

    // done
    return p_case->p_out == pp_contents[0];
}

bool empty_results_match ( test_scenario *p_scenario, test_case *p_case, void *p_subject, void *p_result )
{
    
    // unused
    (void) p_scenario;
    (void) p_case;
    (void) p_subject;

    // initialized data
    void **pp_contents = p_scenario->p_data;
    bool   result      = ( pp_contents[0] == NULL );

    // done
    return p_result == (void *)(size_t)result;
}

void *destruct_queue ( void *p_pointer, unsigned long long size )
{

    // unused
    (void) size;

    // initialized data
    queue *p_queue = (queue *)p_pointer;

    // release the queue
    queue_destroy(&p_queue, NULL);

    // success
    return NULL;
}
