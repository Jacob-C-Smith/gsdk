/** !
 * Tester for array module
 * 
 * @file src/test/array_test.c
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
#include <data/array.h>

// preprocessor macros
#define A_ELEMENT "A"
#define B_ELEMENT "B"
#define C_ELEMENT "C"
#define D_ELEMENT "D"
#define X_ELEMENT "X"

// function declarations
/// scenario constructors
fn_scenario_constructor construct_empty;
fn_scenario_constructor construct_empty_addA_A;
fn_scenario_constructor construct_empty_addB_B;
fn_scenario_constructor construct_empty_addC_C;
fn_scenario_constructor construct_A_addB_AB;
fn_scenario_constructor construct_ABC_remove1_AC;
fn_scenario_constructor construct_B_addA_BA;
fn_scenario_constructor construct_B_addC_BC;
fn_scenario_constructor construct_C_addA_CA;
fn_scenario_constructor construct_C_addB_CB;
fn_scenario_constructor construct_AB_addC_ABC;
fn_scenario_constructor construct_AC_addB_ACB;
fn_scenario_constructor construct_BA_addC_BAC;
fn_scenario_constructor construct_BC_addA_BCA;
fn_scenario_constructor construct_CA_addB_CAB;
fn_scenario_constructor construct_CB_addA_CBA;
fn_scenario_constructor construct_empty_fromelementsA_A;
fn_scenario_constructor construct_empty_fromelementsAB_AB;
fn_scenario_constructor construct_empty_fromelementsABC_ABC;
fn_scenario_constructor construct_empty_fromargumentsA_A;
fn_scenario_constructor construct_empty_fromargumentsAB_AB;
fn_scenario_constructor construct_empty_fromargumentsABC_ABC;
fn_scenario_constructor construct_A_remove0_empty;
fn_scenario_constructor construct_AB_remove1_A;
fn_scenario_constructor construct_ABC_remove2_AB;

/// test cases
fn_test_case test_add;
fn_test_case test_remove;
fn_test_case test_index;
fn_test_case test_size;
fn_test_case test_is_empty;
fn_test_case test_get;
fn_test_case test_slice;

/// result evaluators
fn_results_match remove_results_match;
fn_results_match index_results_match;
fn_results_match size_results_match;
fn_results_match is_empty_results_match;
fn_results_match get_results_match;
fn_results_match slice_results_match;

/// allocators
fn_allocator destruct_array;

// data
/// values
void *_contents   [] = { NULL };
void *A_elements  [] = { A_ELEMENT, NULL };
void *B_elements  [] = { B_ELEMENT, NULL };
void *C_elements  [] = { C_ELEMENT, NULL };
void *AB_elements [] = { A_ELEMENT, B_ELEMENT, NULL };
void *AC_elements [] = { A_ELEMENT, C_ELEMENT, NULL };
void *BA_elements [] = { B_ELEMENT, A_ELEMENT, NULL };
void *BC_elements [] = { B_ELEMENT, C_ELEMENT, NULL };
void *CA_elements [] = { C_ELEMENT, A_ELEMENT, NULL };
void *CB_elements [] = { C_ELEMENT, B_ELEMENT, NULL };
void *ABC_elements[] = { A_ELEMENT, B_ELEMENT, C_ELEMENT, NULL };
void *ACB_elements[] = { A_ELEMENT, C_ELEMENT, B_ELEMENT, NULL };
void *BAC_elements[] = { B_ELEMENT, A_ELEMENT, C_ELEMENT, NULL };
void *BCA_elements[] = { B_ELEMENT, C_ELEMENT, A_ELEMENT, NULL };
void *CAB_elements[] = { C_ELEMENT, A_ELEMENT, B_ELEMENT, NULL };
void *CBA_elements[] = { C_ELEMENT, B_ELEMENT, A_ELEMENT, NULL };

// test
/// cases
test_case _empty_test_cases[] = 
{
    TEST_CASE ("add A"    , test_add     , A_ELEMENT, TEST_RESULT_ONE),
    TEST_CASE ("add B"    , test_add     , B_ELEMENT, TEST_RESULT_ONE),
    TEST_CASE ("index 0"  , test_index   , (void *)0, TEST_RESULT_ZERO),
    TEST_CASE ("remove 0" , test_remove  , (void *)0, TEST_RESULT_ZERO),
    TEST_MATCH("size"     , test_size    , NULL     , size_results_match),
    TEST_MATCH("is empty" , test_is_empty, NULL     , is_empty_results_match),
};

test_case _one_element_test_cases[] = 
{
    TEST_CASE ("add D"    , test_add     , D_ELEMENT                 , TEST_RESULT_ONE),
    TEST_MATCH("index 0"  , test_index   , (void *)0                 , index_results_match),
    TEST_CASE ("index 1"  , test_index   , (void *)1                 , TEST_RESULT_ZERO),
    TEST_MATCH("remove 0" , test_remove  , (void *)0                 , remove_results_match),
    TEST_CASE ("remove 1" , test_remove  , (void *)1                 , TEST_RESULT_ZERO),
    TEST_MATCH("size"     , test_size    , NULL                      , size_results_match),
    TEST_MATCH("is empty" , test_is_empty, NULL                      , is_empty_results_match),
    TEST_MATCH("get"      , test_get     , NULL                      , get_results_match),
    TEST_MATCH("slice 0:0", test_slice   , (void *)0x0000000000000000, slice_results_match),
};

test_case _two_element_test_cases[] = 
{
    TEST_CASE ("add D"    , test_add     , D_ELEMENT                 , TEST_RESULT_ONE),
    TEST_MATCH("index 0"  , test_index   , (void *)0                 , index_results_match),
    TEST_MATCH("index 1"  , test_index   , (void *)1                 , index_results_match),
    TEST_CASE ("index 2"  , test_index   , (void *)2                 , TEST_RESULT_ZERO),
    TEST_MATCH("remove 0" , test_remove  , (void *)0                 , remove_results_match),
    TEST_MATCH("remove 1" , test_remove  , (void *)1                 , remove_results_match),
    TEST_CASE ("remove 2" , test_remove  , (void *)2                 , TEST_RESULT_ZERO),
    TEST_MATCH("size"     , test_size    , NULL                      , size_results_match),
    TEST_MATCH("is empty" , test_is_empty, NULL                      , is_empty_results_match),
    TEST_MATCH("get"      , test_get     , NULL                      , get_results_match),
    TEST_MATCH("slice 0:0", test_slice   , (void *)0x0000000000000000, slice_results_match),
    TEST_MATCH("slice 1:1", test_slice   , (void *)0x0000000100000001, slice_results_match),
    TEST_MATCH("slice 0:1", test_slice   , (void *)0x0000000000000001, slice_results_match),
};

test_case _three_element_test_cases[] = 
{
    TEST_CASE ("add D"    , test_add     , D_ELEMENT                 , TEST_RESULT_ONE),
    TEST_MATCH("index 0"  , test_index   , (void *)0                 , index_results_match),
    TEST_MATCH("index 1"  , test_index   , (void *)1                 , index_results_match),
    TEST_MATCH("index 2"  , test_index   , (void *)2                 , index_results_match),
    TEST_CASE ("index 3"  , test_index   , (void *)3                 , TEST_RESULT_ZERO),
    TEST_MATCH("remove 0" , test_remove  , (void *)0                 , remove_results_match),
    TEST_MATCH("remove 1" , test_remove  , (void *)1                 , remove_results_match),
    TEST_MATCH("remove 2" , test_remove  , (void *)2                 , remove_results_match),
    TEST_CASE ("remove 3" , test_remove  , (void *)3                 , TEST_RESULT_ZERO),
    TEST_MATCH("size"     , test_size    , NULL                      , size_results_match),
    TEST_MATCH("is empty" , test_is_empty, NULL                      , is_empty_results_match),
    TEST_MATCH("get"      , test_get     , NULL                      , get_results_match),
    TEST_MATCH("slice 0:0", test_slice   , (void *)0x0000000000000000, slice_results_match),
    TEST_MATCH("slice 1:1", test_slice   , (void *)0x0000000100000001, slice_results_match),
    TEST_MATCH("slice 2:2", test_slice   , (void *)0x0000000200000002, slice_results_match),
    TEST_MATCH("slice 0:1", test_slice   , (void *)0x0000000000000001, slice_results_match),
    TEST_MATCH("slice 1:2", test_slice   , (void *)0x0000000100000002, slice_results_match),
    TEST_MATCH("slice 0:2", test_slice   , (void *)0x0000000000000002, slice_results_match),
};

/// scenarios
test_scenario _scenarios[] = 
{
    TEST_SCENARIO("empty"                       , _contents   , _empty_test_cases        , construct_empty                    , destruct_array),
    TEST_SCENARIO("empty_addA_A"                , A_elements  , _one_element_test_cases  , construct_empty_addA_A             , destruct_array),
    TEST_SCENARIO("empty_fromelementsA_A"       , A_elements  , _one_element_test_cases  , construct_empty_fromelementsA_A    , destruct_array),
    TEST_SCENARIO("empty_fromargumentsA_A"      , A_elements  , _one_element_test_cases  , construct_empty_fromargumentsA_A   , destruct_array),
    TEST_SCENARIO("empty_addB_B"                , B_elements  , _one_element_test_cases  , construct_empty_addB_B             , destruct_array),
    TEST_SCENARIO("empty_addC_C"                , C_elements  , _one_element_test_cases  , construct_empty_addC_C             , destruct_array),
    TEST_SCENARIO("A_addB_AB"                   , AB_elements , _two_element_test_cases  , construct_A_addB_AB                , destruct_array),
    TEST_SCENARIO("empty_fromelementsAB_AB"     , AB_elements , _two_element_test_cases  , construct_empty_fromelementsAB_AB  , destruct_array),
    TEST_SCENARIO("empty_fromargumentsAB_AB"    , AB_elements , _two_element_test_cases  , construct_empty_fromargumentsAB_AB , destruct_array),
    TEST_SCENARIO("ABC_remove1_AC"              , AC_elements , _two_element_test_cases  , construct_ABC_remove1_AC           , destruct_array),
    TEST_SCENARIO("B_addA_BA"                   , BA_elements , _two_element_test_cases  , construct_B_addA_BA                , destruct_array),
    TEST_SCENARIO("B_addC_BC"                   , BC_elements , _two_element_test_cases  , construct_B_addC_BC                , destruct_array),
    TEST_SCENARIO("C_addA_CA"                   , CA_elements , _two_element_test_cases  , construct_C_addA_CA                , destruct_array),
    TEST_SCENARIO("C_addB_CB"                   , CB_elements , _two_element_test_cases  , construct_C_addB_CB                , destruct_array),
    TEST_SCENARIO("AB_addC_ABC"                 , ABC_elements, _three_element_test_cases, construct_AB_addC_ABC              , destruct_array),
    TEST_SCENARIO("empty_fromelementsABC_ABC"   , ABC_elements, _three_element_test_cases, construct_empty_fromelementsABC_ABC, destruct_array),
    TEST_SCENARIO("empty_fromargumentsABC_ABC"  , ABC_elements, _three_element_test_cases, construct_empty_fromargumentsABC_ABC, destruct_array),
    TEST_SCENARIO("AC_addB_ACB"                 , ACB_elements, _three_element_test_cases, construct_AC_addB_ACB              , destruct_array),
    TEST_SCENARIO("BA_addC_BAC"                 , BAC_elements, _three_element_test_cases, construct_BA_addC_BAC              , destruct_array),
    TEST_SCENARIO("BC_addA_BCA"                 , BCA_elements, _three_element_test_cases, construct_BC_addA_BCA              , destruct_array),
    TEST_SCENARIO("CA_addB_CAB"                 , CAB_elements, _three_element_test_cases, construct_CA_addB_CAB              , destruct_array),
    TEST_SCENARIO("CB_addA_CBA"                 , CBA_elements, _three_element_test_cases, construct_CB_addA_CBA              , destruct_array),
};

/// suites
test_suite _suite = TEST_SUITE("array", _scenarios);

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
    return array_construct((array **)pp_result, 4);
}

int construct_empty_addB_B ( void **pp_result ) 
{ 

    // [] -> add(B) -> [B]
    construct_empty(pp_result);
    
    // done
    return array_add(*((array **)pp_result), B_ELEMENT); 
}

int construct_empty_addC_C ( void **pp_result ) 
{ 

    // [] -> add(C) -> [C]
    construct_empty(pp_result);
    
    // done
    return array_add(*((array **)pp_result), C_ELEMENT); 
}

int construct_ABC_remove1_AC ( void **pp_result ) 
{ 

    // [A, B, C] -> remove(1) -> [A, C]
    construct_AB_addC_ABC(pp_result);
    
    // done
    return array_remove(*((array **)pp_result), 1, NULL); 
}

int construct_B_addA_BA ( void **pp_result ) 
{ 

    // [B] -> add(A) -> [B, A]
    construct_empty_addB_B(pp_result);
    
    // done
    return array_add(*((array **)pp_result), A_ELEMENT); 
}

int construct_B_addC_BC ( void **pp_result ) 
{ 

    // [B] -> add(C) -> [B, C]
    construct_empty_addB_B(pp_result);
    
    // done
    return array_add(*((array **)pp_result), C_ELEMENT); 
}

int construct_C_addA_CA ( void **pp_result ) 
{ 

    // [C] -> add(A) -> [C, A]
    construct_empty_addC_C(pp_result);
    
    // done
    return array_add(*((array **)pp_result), A_ELEMENT); 
}

int construct_C_addB_CB ( void **pp_result ) 
{ 

    // [C] -> add(B) -> [C, B]
    construct_empty_addC_C(pp_result);
    
    // done
    return array_add(*((array **)pp_result), B_ELEMENT); 
}

int construct_AC_addB_ACB ( void **pp_result ) 
{ 

    // [AC] -> add(B) -> [A, C, B]
    construct_ABC_remove1_AC(pp_result);
    
    // done
    return array_add(*((array **)pp_result), B_ELEMENT); 
}

int construct_BA_addC_BAC ( void **pp_result ) 
{ 

    // [BA] -> add(C) -> [B, A, C]
    construct_B_addA_BA(pp_result);
    
    // done
    return array_add(*((array **)pp_result), C_ELEMENT); 
}

int construct_BC_addA_BCA ( void **pp_result ) 
{ 

    // [BC] -> add(A) -> [B, C, A]
    construct_B_addC_BC(pp_result);
    
    // done
    return array_add(*((array **)pp_result), A_ELEMENT); 
}

int construct_CA_addB_CAB ( void **pp_result ) 
{ 

    // [CA] -> add(B) -> [C, A, B]
    construct_C_addA_CA(pp_result);
    
    // done
    return array_add(*((array **)pp_result), B_ELEMENT); 
}

int construct_CB_addA_CBA ( void **pp_result ) 
{ 

    // [CB] -> add(A) -> [C, B, A]
    construct_C_addB_CB(pp_result);
    
    // done
    return array_add(*((array **)pp_result), A_ELEMENT); 
}

int construct_empty_addA_A ( void **pp_result ) 
{ 

    // []
    construct_empty(pp_result);

    // [] -> add(A) -> [A]
    return array_add(*((array **)pp_result), A_ELEMENT); 
}

int construct_A_addB_AB ( void **pp_result ) 
{ 

    // [A]
    construct_empty_addA_A(pp_result);

    // [A] -> add(B) -> [A, B]
    return array_add(*((array **)pp_result), B_ELEMENT); 
}

int construct_AB_addC_ABC ( void **pp_result ) 
{ 

    // [A, B]
    construct_A_addB_AB(pp_result);

    // [A, B] -> add(C) -> [A, B, C]
    return array_add(*((array **)pp_result), C_ELEMENT); 
}

int construct_empty_fromelementsA_A ( void **pp_result ) 
{ 

    // ... -> from_elements(A) -> [A]
    return array_from_elements((array **)pp_result, (void **)A_elements, 1);
}

int construct_empty_fromelementsAB_AB ( void **pp_result ) 
{ 

    // ... -> from_elements(A, B) -> [A, B]
    return array_from_elements((array **)pp_result, (void **)AB_elements, 2);
}

int construct_empty_fromelementsABC_ABC ( void **pp_result ) 
{ 

    // ... -> from_elements(A, B, C) -> [A, B, C]
    return array_from_elements((array **)pp_result, (void **)ABC_elements, 3);
}

int construct_empty_fromargumentsA_A ( void **pp_result ) 
{ 

    // ... -> from_arguments(A) -> [A]
    return array_from_arguments((array **)pp_result, 1, 1, A_ELEMENT);
}

int construct_empty_fromargumentsAB_AB ( void **pp_result ) 
{ 

    // ... -> from_arguments(A, B) -> [A, B]
    return array_from_arguments((array **)pp_result, 2, 2, A_ELEMENT, B_ELEMENT);
}

int construct_empty_fromargumentsABC_ABC ( void **pp_result ) 
{ 

    // ... -> from_arguments(A, B, C) -> [A, B, C]
    return array_from_arguments((array **)pp_result, 3, 3, A_ELEMENT, B_ELEMENT, C_ELEMENT);
}

int construct_A_remove0_empty ( void **pp_result ) 
{ 

    // [A]
    construct_empty_addA_A(pp_result);

    // [A] -> remove(0) -> []
    return array_remove(*((array **)pp_result), 0, NULL); 
}

int construct_AB_remove1_A ( void **pp_result ) 
{ 

    // [A, B]
    construct_A_addB_AB(pp_result);

    // [A, B] -> remove(1) -> [A]
    return array_remove(*((array **)pp_result), 1, NULL); 
}

int construct_ABC_remove2_AB ( void **pp_result ) 
{ 

    // [A, B, C]
    construct_AB_addC_ABC(pp_result);

    // [A, B, C] -> remove(2) -> [A, B]
    return array_remove(*((array **)pp_result), 2, NULL); 
}

void *test_add ( test_case *p_test_case, void *p_subject ) 
{ 

    // initialized data
    array *p_array = (array *)p_subject;
    
    // success
    return (void *)(size_t)array_add(p_array, p_test_case->p_parameters); 
}

void *test_remove ( test_case *p_test_case, void *p_subject ) 
{ 

    // initialized data
    array  *p_array = (array *)p_subject;
    signed  index   = (signed)(size_t)p_test_case->p_parameters;

    // test
    if ( 0 == array_remove(p_array, index, &p_test_case->p_out) ) return NULL;

    // success
    return (void *)1;
}

void *test_index ( test_case *p_test_case, void *p_subject ) 
{ 

    // initialized data
    array  *p_array = (array *)p_subject;
    signed  index   = (signed)(size_t)p_test_case->p_parameters;
    
    // test
    if ( 0 == array_index(p_array, index, &p_test_case->p_out) ) return NULL;

    // success
    return (void *)1;
}

void *test_size ( test_case *p_test_case, void *p_subject ) 
{ 

    // unused
    (void) p_test_case;

    // initialized data
    array *p_array = (array *)p_subject;

    // success
    return (void *)array_size(p_array);
}

void *test_is_empty ( test_case *p_test_case, void *p_subject ) 
{ 

    // unused
    (void) p_test_case;

    // initialized data
    array *p_array = (array *)p_subject;

    // success
    return (void *)(size_t)array_is_empty(p_array);
}

void *test_get ( test_case *p_test_case, void *p_subject ) 
{ 

    // initialized data
    array   *p_array     = (array *)p_subject;
    size_t   count       = 0;
    void   **pp_elements = NULL;

    // store the size
    count = array_size(p_array);
    if ( count == 0 ) return NULL;

    // allocate
    pp_elements = default_allocator(NULL, (count + 1) * sizeof(void *));

    // test
    if ( 0 == array_get(p_array, pp_elements, &count) ) 
    {

        // release the elements
        pp_elements = default_allocator(pp_elements, 0);

        // error
        return NULL;
    }

    // store the result
    p_test_case->p_out = pp_elements;

    // success
    return (void *)1;
}

void *test_slice ( test_case *p_test_case, void *p_subject ) 
{ 

    // initialized data
    array   *p_array     = (array *)p_subject;
    size_t   bounds      = (size_t)p_test_case->p_parameters;
    signed   lower       = (signed)(bounds >> 32);
    signed   upper       = (signed)(bounds & 0xFFFFFFFF);
    void   **pp_elements = NULL;

    // fast fail
    if ( upper < lower ) return NULL;

    // allocate
    pp_elements = default_allocator(NULL, (upper - lower + 1) * sizeof(void *));

    // test
    if ( 0 == array_slice(p_array, pp_elements, lower, upper) ) 
    {

        // release the elements
        pp_elements = default_allocator(pp_elements, 0);

        // error
        return NULL;
    }

    // store the result
    p_test_case->p_out = pp_elements;

    // success
    return (void *)1;
}

bool remove_results_match ( test_scenario *p_scenario, test_case *p_case, void *p_subject, void *p_result )
{
    
    // unused
    (void) p_subject;
    (void) p_result;

    // initialized data
    void   **pp_elements = p_scenario->p_data;
    signed   index       = (signed)(size_t)p_case->p_parameters;

    // success
    return p_case->p_out == pp_elements[index];
}

bool index_results_match ( test_scenario *p_scenario, test_case *p_case, void *p_subject, void *p_result )
{
    
    // unused
    (void) p_subject;
    (void) p_result;

    // initialized data
    void   **pp_elements = p_scenario->p_data;
    signed   index       = (signed)(size_t)p_case->p_parameters;

    // done
    return p_case->p_out == pp_elements[index];
}

bool size_results_match ( test_scenario *p_scenario, test_case *p_case, void *p_subject, void *p_result )
{
    
    // unused
    (void) p_case;
    (void) p_subject;

    // initialized data
    void   **pp_elements = p_scenario->p_data;
    size_t   count       = 0;

    // count
    while ( pp_elements[count] ) count++;

    // done
    return (size_t)p_result == count;
}

bool is_empty_results_match ( test_scenario *p_scenario, test_case *p_case, void *p_subject, void *p_result )
{
    
    // unused
    (void) p_scenario;
    (void) p_case;
    (void) p_subject;

    // initialized data
    void **pp_elements = p_scenario->p_data;
    bool   empty       = ( pp_elements[0] == NULL );

    // done
    return p_result == (void *)(size_t)empty;
}

bool get_results_match ( test_scenario *p_scenario, test_case *p_case, void *p_subject, void *p_result )
{
    
    // unused
    (void) p_subject;
    (void) p_result;

    // initialized data
    void **pp_expected = p_scenario->p_data;
    void **pp_actual   = p_case->p_out;
    bool   match       = true;

    // error check
    if ( NULL == pp_actual ) return false;

    // iterate through each element
    for (size_t i = 0; pp_expected[i]; i++)

        // test
        if ( pp_expected[i] != pp_actual[i] ) match = false;

    // release the elements
    pp_actual = default_allocator(pp_actual, 0);

    // done
    return match;
}

bool slice_results_match ( test_scenario *p_scenario, test_case *p_case, void *p_subject, void *p_result )
{
    
    // unused
    (void) p_subject;
    (void) p_result;

    // initialized data
    void   **pp_expected = p_scenario->p_data;
    void   **pp_actual   = p_case->p_out;
    size_t   bounds      = (size_t)p_case->p_parameters;
    signed   lower       = (signed)(bounds >> 32);
    signed   upper       = (signed)(bounds & 0xFFFFFFFF);
    bool     match       = true;

    // error check
    if ( NULL == pp_actual ) return false;

    // iterate through each element
    for (signed i = 0; i <= upper - lower; i++)

        // test
        if ( pp_expected[lower + i] != pp_actual[i] ) match = false;

    // release the elements
    pp_actual = default_allocator(pp_actual, 0);

    // done
    return match;
}

void *destruct_array ( void *p_pointer, unsigned long long size )
{

    // unused
    (void) size;

    // initialized data
    array *p_array = (array *)p_pointer;

    // release the array
    array_destroy(&p_array, NULL);

    // success
    return NULL;
}
