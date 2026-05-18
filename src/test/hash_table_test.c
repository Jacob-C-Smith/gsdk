/** !
 * hash table tester
 * 
 * @file src/test/hash_table_test.c
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
#include <data/hash_table.h>

// preprocessor macros
#define A_KEY "A"
#define B_KEY "B"
#define C_KEY "C"
#define D_KEY "D"

// function declarations
/// scenario constructors
fn_scenario_constructor construct_empty;
fn_scenario_constructor construct_A_removeA_empty;
fn_scenario_constructor construct_B_removeB_empty;
fn_scenario_constructor construct_C_removeC_empty;

fn_scenario_constructor construct_empty_insertA_A;
fn_scenario_constructor construct_AB_removeB_A;
fn_scenario_constructor construct_AC_removeC_A;
fn_scenario_constructor construct_empty_insertB_B;
fn_scenario_constructor construct_AB_removeA_B;
fn_scenario_constructor construct_BC_removeC_B;
fn_scenario_constructor construct_empty_insertC_C;
fn_scenario_constructor construct_AC_removeA_C;
fn_scenario_constructor construct_BC_removeB_C;

fn_scenario_constructor construct_A_insertB_AB;
fn_scenario_constructor construct_A_insertC_AC;
fn_scenario_constructor construct_B_insertA_AB;
fn_scenario_constructor construct_B_insertC_BC;
fn_scenario_constructor construct_C_insertA_AC;
fn_scenario_constructor construct_C_insertB_BC;
fn_scenario_constructor construct_ABC_removeC_AB;
fn_scenario_constructor construct_ABC_removeB_AC;
fn_scenario_constructor construct_ABC_removeA_BC;

fn_scenario_constructor construct_AB_insertC_ABC;
fn_scenario_constructor construct_AC_insertB_ABC;
fn_scenario_constructor construct_BC_insertA_ABC;

/// test cases
fn_test_case test_insert;
fn_test_case test_search;
fn_test_case test_remove;
fn_test_case test_is_empty;
fn_test_case test_size;

/// result evaluators
fn_results_match search_results_match;
fn_results_match remove_results_match;
fn_results_match is_empty_results_match;
fn_results_match size_results_match;

/// allocators
fn_allocator destruct_hash_table;

// data
/// values
char  *_keys     [] = { NULL };
char  *A_keys    [] = { A_KEY, NULL };
char  *B_keys    [] = { B_KEY, NULL };
char  *C_keys    [] = { C_KEY, NULL };
char  *AB_keys   [] = { A_KEY, B_KEY, NULL };
char  *AC_keys   [] = { A_KEY, C_KEY, NULL };
char  *BC_keys   [] = { B_KEY, C_KEY, NULL };
char  *ABC_keys  [] = { A_KEY, B_KEY, C_KEY, NULL };

// test
/// cases
test_case _empty_test_cases[] = 
{
    TEST_CASE ("insert A", test_insert  , A_KEY, TEST_RESULT_ONE),
    TEST_CASE ("insert B", test_insert  , B_KEY, TEST_RESULT_ONE),
    TEST_CASE ("insert C", test_insert  , C_KEY, TEST_RESULT_ONE),
    TEST_CASE ("remove A", test_remove  , A_KEY, TEST_RESULT_ZERO),
    TEST_CASE ("remove B", test_remove  , B_KEY, TEST_RESULT_ZERO),
    TEST_CASE ("remove C", test_remove  , C_KEY, TEST_RESULT_ZERO),
    TEST_CASE ("search A", test_search  , A_KEY, TEST_RESULT_ZERO),
    TEST_CASE ("search B", test_search  , B_KEY, TEST_RESULT_ZERO),
    TEST_CASE ("search C", test_search  , C_KEY, TEST_RESULT_ZERO),
    TEST_MATCH("is empty", test_is_empty, NULL , is_empty_results_match),
    TEST_MATCH("size"    , test_size    , NULL , size_results_match),
};

test_case _one_element_test_cases[] = 
{
    TEST_CASE ("insert A", test_insert  , A_KEY, TEST_RESULT_ONE),
    TEST_CASE ("insert B", test_insert  , B_KEY, TEST_RESULT_ONE),
    TEST_CASE ("insert C", test_insert  , C_KEY, TEST_RESULT_ONE),
    TEST_MATCH("remove A", test_remove  , A_KEY, remove_results_match),
    TEST_MATCH("remove B", test_remove  , B_KEY, remove_results_match),
    TEST_MATCH("remove C", test_remove  , C_KEY, remove_results_match),
    TEST_MATCH("search A", test_search  , A_KEY, search_results_match),
    TEST_MATCH("search B", test_search  , B_KEY, search_results_match),
    TEST_MATCH("search C", test_search  , C_KEY, search_results_match),
    TEST_MATCH("is empty", test_is_empty, NULL , is_empty_results_match),
    TEST_MATCH("size"    , test_size    , NULL , size_results_match),
};

test_case _two_element_test_cases[] = 
{
    TEST_CASE ("insert A", test_insert  , A_KEY, TEST_RESULT_ONE),
    TEST_CASE ("insert B", test_insert  , B_KEY, TEST_RESULT_ONE),
    TEST_CASE ("insert C", test_insert  , C_KEY, TEST_RESULT_ONE),
    TEST_MATCH("remove A", test_remove  , A_KEY, remove_results_match),
    TEST_MATCH("remove B", test_remove  , B_KEY, remove_results_match),
    TEST_MATCH("remove C", test_remove  , C_KEY, remove_results_match),
    TEST_MATCH("search A", test_search  , A_KEY, search_results_match),
    TEST_MATCH("search B", test_search  , B_KEY, search_results_match),
    TEST_MATCH("search C", test_search  , C_KEY, search_results_match),
    TEST_MATCH("is empty", test_is_empty, NULL , is_empty_results_match),
    TEST_MATCH("size"    , test_size    , NULL , size_results_match),
};

test_case _three_element_test_cases[] = 
{
    TEST_CASE ("insert A", test_insert  , A_KEY, TEST_RESULT_ONE),
    TEST_CASE ("insert B", test_insert  , B_KEY, TEST_RESULT_ONE),
    TEST_CASE ("insert C", test_insert  , C_KEY, TEST_RESULT_ONE),
    TEST_MATCH("remove A", test_remove  , A_KEY, remove_results_match),
    TEST_MATCH("remove B", test_remove  , B_KEY, remove_results_match),
    TEST_MATCH("remove C", test_remove  , C_KEY, remove_results_match),
    TEST_MATCH("search A", test_search  , A_KEY, search_results_match),
    TEST_MATCH("search B", test_search  , B_KEY, search_results_match),
    TEST_MATCH("search C", test_search  , C_KEY, search_results_match),
    TEST_MATCH("is empty", test_is_empty, NULL , is_empty_results_match),
    TEST_MATCH("size"    , test_size    , NULL , size_results_match),
};

/// scenarios
test_scenario _scenarios[] = 
{
    TEST_SCENARIO("empty"          , _keys   , _empty_test_cases        , construct_empty          , destruct_hash_table),
    TEST_SCENARIO("A_removeA_empty", _keys   , _empty_test_cases        , construct_A_removeA_empty, destruct_hash_table),
    TEST_SCENARIO("B_removeB_empty", _keys   , _empty_test_cases        , construct_B_removeB_empty, destruct_hash_table),
    TEST_SCENARIO("C_removeC_empty", _keys   , _empty_test_cases        , construct_C_removeC_empty, destruct_hash_table),

    TEST_SCENARIO("empty_insertA_A", A_keys  , _one_element_test_cases  , construct_empty_insertA_A, destruct_hash_table),
    TEST_SCENARIO("empty_insertB_B", B_keys  , _one_element_test_cases  , construct_empty_insertB_B, destruct_hash_table),
    TEST_SCENARIO("empty_insertC_C", C_keys  , _one_element_test_cases  , construct_empty_insertC_C, destruct_hash_table),
    TEST_SCENARIO("AB_removeB_A"   , A_keys  , _one_element_test_cases  , construct_AB_removeB_A   , destruct_hash_table), 
    TEST_SCENARIO("AC_removeC_A"   , A_keys  , _one_element_test_cases  , construct_AC_removeC_A   , destruct_hash_table), 
    TEST_SCENARIO("AB_removeA_B"   , B_keys  , _one_element_test_cases  , construct_AB_removeA_B   , destruct_hash_table), 
    TEST_SCENARIO("BC_removeC_B"   , B_keys  , _one_element_test_cases  , construct_BC_removeC_B   , destruct_hash_table), 
    TEST_SCENARIO("AC_removeA_C"   , C_keys  , _one_element_test_cases  , construct_AC_removeA_C   , destruct_hash_table), 
    TEST_SCENARIO("BC_removeB_C"   , C_keys  , _one_element_test_cases  , construct_BC_removeB_C   , destruct_hash_table), 

    TEST_SCENARIO("A_insertB_AB"   , AB_keys , _two_element_test_cases  , construct_A_insertB_AB   , destruct_hash_table),
    TEST_SCENARIO("B_insertA_AB"   , AB_keys , _two_element_test_cases  , construct_B_insertA_AB   , destruct_hash_table),
    TEST_SCENARIO("A_insertC_AC"   , AC_keys , _two_element_test_cases  , construct_A_insertC_AC   , destruct_hash_table),
    TEST_SCENARIO("C_insertA_AC"   , AC_keys , _two_element_test_cases  , construct_C_insertA_AC   , destruct_hash_table),
    TEST_SCENARIO("B_insertC_BC"   , BC_keys , _two_element_test_cases  , construct_B_insertC_BC   , destruct_hash_table),
    TEST_SCENARIO("C_insertB_BC"   , BC_keys , _two_element_test_cases  , construct_C_insertB_BC   , destruct_hash_table),
    TEST_SCENARIO("ABC_removeC_AB" , AB_keys , _two_element_test_cases  , construct_ABC_removeC_AB , destruct_hash_table),
    TEST_SCENARIO("ABC_removeB_AC" , AC_keys , _two_element_test_cases  , construct_ABC_removeB_AC , destruct_hash_table),
    TEST_SCENARIO("ABC_removeA_BC" , BC_keys , _two_element_test_cases  , construct_ABC_removeA_BC , destruct_hash_table),

    TEST_SCENARIO("AB_insertC_ABC" , ABC_keys, _three_element_test_cases, construct_AB_insertC_ABC , destruct_hash_table),
    TEST_SCENARIO("AC_insertB_ABC" , ABC_keys, _three_element_test_cases, construct_AC_insertB_ABC , destruct_hash_table),
    TEST_SCENARIO("BC_insertA_ABC" , ABC_keys, _three_element_test_cases, construct_BC_insertA_ABC , destruct_hash_table),
};

/// suites
test_suite _suite = TEST_SUITE("hash table", _scenarios);

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
    return hash_table_construct((hash_table **)pp_result, 3, LINEAR_PROBE, (fn_comparator *) strcmp, NULL, NULL);
}

int construct_empty_insertA_A ( void **pp_result ) 
{ 

    // [ ]
    construct_empty(pp_result);

    // [ ] -> insert(A) -> [ A ]
    return hash_table_insert(*((hash_table **)pp_result), A_KEY); 
}

int construct_AB_removeB_A ( void **pp_result )
{

    // [ A, B ]
    construct_A_insertB_AB(pp_result);

    // [ A, B ] -> remove(B) -> [ A ]
    return hash_table_remove(*((hash_table **)pp_result), B_KEY, NULL); 
}

int construct_AC_removeC_A ( void **pp_result )
{

    // [ A, C ]
    construct_A_insertC_AC(pp_result);

    // [ A, C ] -> remove(C) -> [ A ]
    return hash_table_remove(*((hash_table **)pp_result), C_KEY, NULL); 
}

int construct_A_removeA_empty ( void **pp_result ) 
{ 

    // [ A ]
    construct_empty_insertA_A(pp_result);

    // [ A ] -> remove(A) -> [ ]
    return hash_table_remove(*((hash_table **)pp_result), A_KEY, NULL); 
}

int construct_empty_insertB_B ( void **pp_result ) 
{ 

    // [ ]
    construct_A_removeA_empty(pp_result);

    // [ ] -> insert(B) -> [ B ]
    return hash_table_insert(*((hash_table **)pp_result), B_KEY); 
}

int construct_AB_removeA_B ( void **pp_result )
{

    // [ A, B ]
    construct_B_insertA_AB(pp_result);

    // [ A, B ] -> remove(A) -> [ B ]
    return hash_table_remove(*((hash_table **)pp_result), A_KEY, NULL); 
}

int construct_BC_removeC_B ( void **pp_result )
{

    // [ B, C ]
    construct_B_insertC_BC(pp_result);

    // [ B, C ] -> remove(C) -> [ B ]
    return hash_table_remove(*((hash_table **)pp_result), C_KEY, NULL); 
}

int construct_B_removeB_empty ( void **pp_result ) 
{ 

    // [ B ]
    construct_empty_insertB_B(pp_result);

    // [ B ] -> remove(B) -> [ ]
    return hash_table_remove(*((hash_table **)pp_result), B_KEY, NULL); 
}

int construct_empty_insertC_C ( void **pp_result ) 
{ 

    // [ ]
    construct_B_removeB_empty(pp_result);

    // [ ] -> insert(C) -> [ C ]
    return hash_table_insert(*((hash_table **)pp_result), C_KEY); 
}

int construct_AC_removeA_C ( void **pp_result )
{

    // [ A, C ]
    construct_C_insertA_AC(pp_result);

    // [ A, C ] -> remove(A) -> [ C ]
    return hash_table_remove(*((hash_table **)pp_result), A_KEY, NULL); 
}

int construct_BC_removeB_C ( void **pp_result )
{

    // [ B, C ]
    construct_C_insertB_BC(pp_result);

    // [ B, C ] -> remove(B) -> [ B ]
    return hash_table_remove(*((hash_table **)pp_result), B_KEY, NULL); 
}

int construct_C_removeC_empty ( void **pp_result ) 
{ 

    // [ C ]
    construct_empty_insertC_C(pp_result);

    // [ C ] -> remove(C) -> [ ]
    return hash_table_remove(*((hash_table **)pp_result), C_KEY, NULL); 
}

int construct_A_insertB_AB ( void **pp_result ) 
{ 

    // [ A ]
    construct_empty_insertA_A(pp_result);

    // [ A ] -> insert(B) -> [ A, B ]
    return hash_table_insert(*((hash_table **)pp_result), B_KEY); 
}

int construct_A_insertC_AC ( void **pp_result ) 
{ 

    // [ A ]
    construct_empty_insertA_A(pp_result);

    // [ A ] -> insert(C) -> [ A, C ]
    return hash_table_insert(*((hash_table **)pp_result), C_KEY); 
}

int construct_B_insertA_AB ( void **pp_result ) 
{ 

    // [ B ]
    construct_empty_insertB_B(pp_result);

    // [ B ] -> insert(A) -> [ A, B ]
    return hash_table_insert(*((hash_table **)pp_result), A_KEY); 
}

int construct_B_insertC_BC ( void **pp_result ) 
{ 

    // [ B ]
    construct_empty_insertB_B(pp_result);

    // [ B ] -> insert(C) -> [ B, C ]
    return hash_table_insert(*((hash_table **)pp_result), C_KEY); 
}

int construct_C_insertA_AC ( void **pp_result ) 
{ 

    // [ C ]
    construct_empty_insertC_C(pp_result);

    // [ C ] -> insert(A) -> [ A, C ]
    return hash_table_insert(*((hash_table **)pp_result), A_KEY); 
}

int construct_C_insertB_BC ( void **pp_result ) 
{ 

    // [ C ]
    construct_empty_insertC_C(pp_result);

    // [ C ] -> insert(B) -> [ B, C ]
    return hash_table_insert(*((hash_table **)pp_result), B_KEY); 
}

int construct_ABC_removeC_AB ( void **pp_result ) 
{ 

    // [ A, B, C ]
    construct_AB_insertC_ABC(pp_result);

    // [ A, B, C ] -> remove(C) -> [ A, B ]
    return hash_table_remove(*((hash_table **)pp_result), C_KEY, NULL); 
}

int construct_ABC_removeB_AC ( void **pp_result ) 
{ 

    // [ A, B, C ]
    construct_AC_insertB_ABC(pp_result);

    // [ A, B, C ] -> remove(B) -> [ A, C ]
    return hash_table_remove(*((hash_table **)pp_result), B_KEY, NULL); 
}

int construct_ABC_removeA_BC ( void **pp_result ) 
{ 

    // [ A, B, C ]
    construct_BC_insertA_ABC(pp_result);

    // [ A, B, C ] -> remove(A) -> [ B, C ]
    return hash_table_remove(*((hash_table **)pp_result), A_KEY, NULL); 
}

int construct_AB_insertC_ABC ( void **pp_result ) 
{ 

    // [ A, B ]
    construct_B_insertA_AB(pp_result);

    // [ A, B ] -> insert(C) -> [ A, B, C ]
    return hash_table_insert(*((hash_table **)pp_result), C_KEY); 
}

int construct_AC_insertB_ABC ( void **pp_result ) 
{ 

    // [ A, C ]
    construct_C_insertA_AC(pp_result);

    // [ A, C ] -> insert(B) -> [ A, B, C ]
    return hash_table_insert(*((hash_table **)pp_result), B_KEY); 
}

int construct_BC_insertA_ABC ( void **pp_result ) 
{ 

    // [ B, C ]
    construct_B_insertC_BC(pp_result);

    // [ B, C ] -> insert(A) -> [ A, B, C ]
    return hash_table_insert(*((hash_table **)pp_result), A_KEY); 
}

void *test_insert ( test_case *p_test_case, void *p_subject ) 
{ 

    // initialized data
    hash_table *p_hash_table = (hash_table *)p_subject;
    
    // test
    return (void *)(size_t)hash_table_insert(p_hash_table, p_test_case->p_parameters); 
}

void *test_search ( test_case *p_test_case, void *p_subject ) 
{ 

    // initialized data
    hash_table *p_hash_table = (hash_table *)p_subject;
    
    // test
    return (void *)(size_t) hash_table_search(p_hash_table, p_test_case->p_parameters, &p_test_case->p_out);
}

void *test_remove ( test_case *p_test_case, void *p_subject ) 
{ 

    // initialized data
    hash_table *p_hash_table = (hash_table *)p_subject;

    // test
    return (void *)(size_t) hash_table_remove(p_hash_table, p_test_case->p_parameters, &p_test_case->p_out);
}

void *test_is_empty ( test_case *p_test_case, void *p_subject ) 
{ 

    // unused
    (void) p_test_case;

    // initialized data
    hash_table *p_hash_table = (hash_table *)p_subject;

    // test
    return (void *)(size_t) hash_table_is_empty(p_hash_table);
}

void *test_size ( test_case *p_test_case, void *p_subject ) 
{ 

    // unused
    (void) p_test_case;

    // initialized data
    hash_table *p_hash_table = (hash_table *)p_subject;

    // test
    return (void *)(size_t) hash_table_size(p_hash_table);
}

bool search_results_match ( test_scenario *p_scenario, test_case *p_case, void *p_subject, void *p_result )
{
    
    // unused
    (void) p_subject;
    (void) p_result;

    // initialized data
    char   **pp_keys = p_scenario->p_data;
    size_t   i       = 0;
    bool     found   = false;

    // find the end
    while ( pp_keys[i] )
        found |= ( pp_keys[i] == p_case->p_parameters ), i++;

    // not found and NULL result is still a match
    if ( false == found && NULL == p_case->p_out ) return true;

    // done
    return p_case->p_parameters == p_case->p_out;
}

bool remove_results_match ( test_scenario *p_scenario, test_case *p_case, void *p_subject, void *p_result )
{
    
    // unused
    (void) p_subject;
    (void) p_result;

    // initialized data
    char   **pp_keys = p_scenario->p_data;
    size_t   i       = 0;
    bool     found   = false;

    // find the end
    while ( pp_keys[i] )
        found |= ( pp_keys[i] == p_case->p_parameters ), i++;

    // not found and NULL result is still a match
    if ( false == found && NULL == p_case->p_out ) return true;

    // done
    return p_case->p_parameters == p_case->p_out;
}

bool is_empty_results_match ( test_scenario *p_scenario, test_case *p_case, void *p_subject, void *p_result )
{
    
    // unused
    (void) p_scenario;
    (void) p_case;
    (void) p_subject;

    // initialized data
    char   **pp_keys = p_scenario->p_data;
    size_t   i       = 0;
    bool     result  = true;

    while ( pp_keys[i] )
        result = false,
        i++;

    // done
    return p_result == (void *)result;
}

bool size_results_match ( test_scenario *p_scenario, test_case *p_case, void *p_subject, void *p_result )
{
    
    // unused
    (void) p_scenario;
    (void) p_case;
    (void) p_subject;

    // initialized data
    char   **pp_keys = p_scenario->p_data;
    size_t   i       = 0;

    while ( pp_keys[i] )
        i++;

    // done
    return p_result == (void *)i;
}

void *destruct_hash_table ( void *p_pointer, unsigned long long size )
{

    // unused
    (void) size;

    // initialized data
    hash_table *p_hash_table = (hash_table *)p_pointer;

    // release the hash table
    hash_table_destroy(&p_hash_table, NULL);

    // success
    return NULL;
}
