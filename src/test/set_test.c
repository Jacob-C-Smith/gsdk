/** !
 * set tester
 * 
 * @file src/test/set_test.c
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
#include <data/set.h>

// preprocessor macros
#define A_VALUE "A"
#define B_VALUE "B"
#define C_VALUE "C"
#define D_VALUE "D"
#define X_VALUE "X"

// function declarations
/// scenario constructors
fn_scenario_constructor construct_empty;
fn_scenario_constructor construct_A_removeA_empty;
fn_scenario_constructor construct_B_removeB_empty;
fn_scenario_constructor construct_C_removeC_empty;
fn_scenario_constructor construct_empty_addA_A;
fn_scenario_constructor construct_empty_addB_B;
fn_scenario_constructor construct_empty_addC_C;
fn_scenario_constructor construct_AB_removeB_A;
fn_scenario_constructor construct_AB_removeA_B;
fn_scenario_constructor construct_AC_removeC_A;
fn_scenario_constructor construct_AC_removeA_C;
fn_scenario_constructor construct_BC_removeB_C;
fn_scenario_constructor construct_BC_removeC_B;
fn_scenario_constructor construct_A_addB_AB; 
fn_scenario_constructor construct_A_addC_AC; 
fn_scenario_constructor construct_B_addA_AB; 
fn_scenario_constructor construct_B_addC_BC; 
fn_scenario_constructor construct_C_addB_BC; 
fn_scenario_constructor construct_C_addA_AC; 
fn_scenario_constructor construct_ABC_removeC_AB;
fn_scenario_constructor construct_ABC_removeB_AC;
fn_scenario_constructor construct_ABC_removeA_BC;
fn_scenario_constructor construct_AB_addC_ABC;
fn_scenario_constructor construct_AC_addB_ABC;
fn_scenario_constructor construct_BC_addA_ABC;

/// test cases
fn_test_case test_add;
fn_test_case test_remove;
fn_test_case test_count;
fn_test_case test_union;
fn_test_case test_intersection;
fn_test_case test_difference;
fn_test_case test_issubset;
fn_test_case test_issuperset;

/// result evaluators
fn_results_match remove_results_match;
fn_results_match count_results_match;
fn_results_match set_union_results_match;
fn_results_match set_intersection_results_match;
fn_results_match set_difference_results_match;
fn_results_match set_subset_results_match;
fn_results_match set_superset_results_match;

/// allocators
fn_allocator destruct_set;

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
    TEST_CASE ("add A"        , test_add         , A_VALUE   , TEST_RESULT_ONE),
    TEST_CASE ("add B"        , test_add         , B_VALUE   , TEST_RESULT_ONE),
    TEST_CASE ("add C"        , test_add         , C_VALUE   , TEST_RESULT_ONE),
    TEST_CASE ("remove A"     , test_remove      , A_VALUE   , TEST_RESULT_ZERO),
    TEST_CASE ("remove B"     , test_remove      , B_VALUE   , TEST_RESULT_ZERO),
    TEST_CASE ("remove C"     , test_remove      , C_VALUE   , TEST_RESULT_ZERO),
    TEST_MATCH("count"        , test_count       , NULL      , count_results_match),

    TEST_MATCH("∪ { A }"      , test_union       , A_values  , set_union_results_match),
    TEST_MATCH("∪ { B }"      , test_union       , B_values  , set_union_results_match),
    TEST_MATCH("∪ { C }"      , test_union       , C_values  , set_union_results_match),
    TEST_MATCH("∪ { A, B }"   , test_union       , AB_values , set_union_results_match),
    TEST_MATCH("∪ { A, C }"   , test_union       , AC_values , set_union_results_match),
    TEST_MATCH("∪ { B, C }"   , test_union       , BC_values , set_union_results_match),
    TEST_MATCH("∪ { A, B, C }", test_union       , ABC_values, set_union_results_match),

    TEST_MATCH("∩ { A }"      , test_intersection, A_values  , set_intersection_results_match),
    TEST_MATCH("∩ { B }"      , test_intersection, B_values  , set_intersection_results_match),
    TEST_MATCH("∩ { C }"      , test_intersection, C_values  , set_intersection_results_match),
    TEST_MATCH("∩ { A, B }"   , test_intersection, AB_values , set_intersection_results_match),
    TEST_MATCH("∩ { A, C }"   , test_intersection, AC_values , set_intersection_results_match),
    TEST_MATCH("∩ { B, C }"   , test_intersection, BC_values , set_intersection_results_match),
    TEST_MATCH("∩ { A, B, C }", test_intersection, ABC_values, set_intersection_results_match),

    TEST_MATCH("Δ { A }"      , test_difference  , A_values  , set_difference_results_match),
    TEST_MATCH("Δ { B }"      , test_difference  , B_values  , set_difference_results_match),
    TEST_MATCH("Δ { C }"      , test_difference  , C_values  , set_difference_results_match),
    TEST_MATCH("Δ { A, B }"   , test_difference  , AB_values , set_difference_results_match),
    TEST_MATCH("Δ { A, C }"   , test_difference  , AC_values , set_difference_results_match),
    TEST_MATCH("Δ { B, C }"   , test_difference  , BC_values , set_difference_results_match),
    TEST_MATCH("Δ { A, B, C }", test_difference  , ABC_values, set_difference_results_match),

    TEST_MATCH("⊂ { A }"      , test_issubset    , A_values  , set_subset_results_match),
    TEST_MATCH("⊂ { B }"      , test_issubset    , B_values  , set_subset_results_match),
    TEST_MATCH("⊂ { C }"      , test_issubset    , C_values  , set_subset_results_match),
    TEST_MATCH("⊂ { A, B }"   , test_issubset    , AB_values , set_subset_results_match),
    TEST_MATCH("⊂ { A, C }"   , test_issubset    , AC_values , set_subset_results_match),
    TEST_MATCH("⊂ { B, C }"   , test_issubset    , BC_values , set_subset_results_match),
    TEST_MATCH("⊂ { A, B, C }", test_issubset    , ABC_values, set_subset_results_match),

    TEST_MATCH("⊃ { A }"      , test_issuperset  , A_values  , set_superset_results_match),
    TEST_MATCH("⊃ { B }"      , test_issuperset  , B_values  , set_superset_results_match),
    TEST_MATCH("⊃ { C }"      , test_issuperset  , C_values  , set_superset_results_match),
    TEST_MATCH("⊃ { A, B }"   , test_issuperset  , AB_values , set_superset_results_match),
    TEST_MATCH("⊃ { A, C }"   , test_issuperset  , AC_values , set_superset_results_match),
    TEST_MATCH("⊃ { B, C }"   , test_issuperset  , BC_values , set_superset_results_match),
    TEST_MATCH("⊃ { A, B, C }", test_issuperset  , ABC_values, set_superset_results_match),
};

test_case _one_element_test_cases[] = 
{
    TEST_CASE ("add A"        , test_add         , A_VALUE   , TEST_RESULT_ONE),
    TEST_CASE ("add B"        , test_add         , B_VALUE   , TEST_RESULT_ONE),
    TEST_CASE ("add C"        , test_add         , C_VALUE   , TEST_RESULT_ONE),
    TEST_MATCH("remove A"     , test_remove      , A_VALUE   , remove_results_match),
    TEST_MATCH("remove B"     , test_remove      , B_VALUE   , remove_results_match),
    TEST_MATCH("remove C"     , test_remove      , C_VALUE   , remove_results_match),
    TEST_MATCH("count"        , test_count       , NULL      , count_results_match),

    TEST_MATCH("∪ { A }"      , test_union       , A_values  , set_union_results_match),
    TEST_MATCH("∪ { B }"      , test_union       , B_values  , set_union_results_match),
    TEST_MATCH("∪ { C }"      , test_union       , C_values  , set_union_results_match),
    TEST_MATCH("∪ { A, B }"   , test_union       , AB_values , set_union_results_match),
    TEST_MATCH("∪ { A, C }"   , test_union       , AC_values , set_union_results_match),
    TEST_MATCH("∪ { B, C }"   , test_union       , BC_values , set_union_results_match),
    TEST_MATCH("∪ { A, B, C }", test_union       , ABC_values, set_union_results_match),

    TEST_MATCH("∩ { A }"      , test_intersection, A_values  , set_intersection_results_match),
    TEST_MATCH("∩ { B }"      , test_intersection, B_values  , set_intersection_results_match),
    TEST_MATCH("∩ { C }"      , test_intersection, C_values  , set_intersection_results_match),
    TEST_MATCH("∩ { A, B }"   , test_intersection, AB_values , set_intersection_results_match),
    TEST_MATCH("∩ { A, C }"   , test_intersection, AC_values , set_intersection_results_match),
    TEST_MATCH("∩ { B, C }"   , test_intersection, BC_values , set_intersection_results_match),
    TEST_MATCH("∩ { A, B, C }", test_intersection, ABC_values, set_intersection_results_match),

    TEST_MATCH("Δ { A }"      , test_difference  , A_values  , set_difference_results_match),
    TEST_MATCH("Δ { B }"      , test_difference  , B_values  , set_difference_results_match),
    TEST_MATCH("Δ { C }"      , test_difference  , C_values  , set_difference_results_match),
    TEST_MATCH("Δ { A, B }"   , test_difference  , AB_values , set_difference_results_match),
    TEST_MATCH("Δ { A, C }"   , test_difference  , AC_values , set_difference_results_match),
    TEST_MATCH("Δ { B, C }"   , test_difference  , BC_values , set_difference_results_match),
    TEST_MATCH("Δ { A, B, C }", test_difference  , ABC_values, set_difference_results_match),

    TEST_MATCH("⊂ { A }"      , test_issubset    , A_values  , set_subset_results_match),
    TEST_MATCH("⊂ { B }"      , test_issubset    , B_values  , set_subset_results_match),
    TEST_MATCH("⊂ { C }"      , test_issubset    , C_values  , set_subset_results_match),
    TEST_MATCH("⊂ { A, B }"   , test_issubset    , AB_values , set_subset_results_match),
    TEST_MATCH("⊂ { A, C }"   , test_issubset    , AC_values , set_subset_results_match),
    TEST_MATCH("⊂ { B, C }"   , test_issubset    , BC_values , set_subset_results_match),
    TEST_MATCH("⊂ { A, B, C }", test_issubset    , ABC_values, set_subset_results_match),

    TEST_MATCH("⊃ { A }"      , test_issuperset  , A_values  , set_superset_results_match),
    TEST_MATCH("⊃ { B }"      , test_issuperset  , B_values  , set_superset_results_match),
    TEST_MATCH("⊃ { C }"      , test_issuperset  , C_values  , set_superset_results_match),
    TEST_MATCH("⊃ { A, B }"   , test_issuperset  , AB_values , set_superset_results_match),
    TEST_MATCH("⊃ { A, C }"   , test_issuperset  , AC_values , set_superset_results_match),
    TEST_MATCH("⊃ { B, C }"   , test_issuperset  , BC_values , set_superset_results_match),
    TEST_MATCH("⊃ { A, B, C }", test_issuperset  , ABC_values, set_superset_results_match),
};

test_case _two_element_test_cases[] = 
{
    TEST_CASE ("add A"        , test_add         , A_VALUE   , TEST_RESULT_ONE),
    TEST_CASE ("add B"        , test_add         , B_VALUE   , TEST_RESULT_ONE),
    TEST_CASE ("add C"        , test_add         , C_VALUE   , TEST_RESULT_ONE),
    TEST_MATCH("remove A"     , test_remove      , A_VALUE   , remove_results_match),
    TEST_MATCH("remove B"     , test_remove      , B_VALUE   , remove_results_match),
    TEST_MATCH("remove C"     , test_remove      , C_VALUE   , remove_results_match),
    TEST_MATCH("count"        , test_count       , NULL      , count_results_match),

    TEST_MATCH("∪ { A }"      , test_union       , A_values  , set_union_results_match),
    TEST_MATCH("∪ { B }"      , test_union       , B_values  , set_union_results_match),
    TEST_MATCH("∪ { C }"      , test_union       , C_values  , set_union_results_match),
    TEST_MATCH("∪ { A, B }"   , test_union       , AB_values , set_union_results_match),
    TEST_MATCH("∪ { A, C }"   , test_union       , AC_values , set_union_results_match),
    TEST_MATCH("∪ { B, C }"   , test_union       , BC_values , set_union_results_match),
    TEST_MATCH("∪ { A, B, C }", test_union       , ABC_values, set_union_results_match),

    TEST_MATCH("∩ { A }"      , test_intersection, A_values  , set_intersection_results_match),
    TEST_MATCH("∩ { B }"      , test_intersection, B_values  , set_intersection_results_match),
    TEST_MATCH("∩ { C }"      , test_intersection, C_values  , set_intersection_results_match),
    TEST_MATCH("∩ { A, B }"   , test_intersection, AB_values , set_intersection_results_match),
    TEST_MATCH("∩ { A, C }"   , test_intersection, AC_values , set_intersection_results_match),
    TEST_MATCH("∩ { B, C }"   , test_intersection, BC_values , set_intersection_results_match),
    TEST_MATCH("∩ { A, B, C }", test_intersection, ABC_values, set_intersection_results_match),

    TEST_MATCH("Δ { A }"      , test_difference  , A_values  , set_difference_results_match),
    TEST_MATCH("Δ { B }"      , test_difference  , B_values  , set_difference_results_match),
    TEST_MATCH("Δ { C }"      , test_difference  , C_values  , set_difference_results_match),
    TEST_MATCH("Δ { A, B }"   , test_difference  , AB_values , set_difference_results_match),
    TEST_MATCH("Δ { A, C }"   , test_difference  , AC_values , set_difference_results_match),
    TEST_MATCH("Δ { B, C }"   , test_difference  , BC_values , set_difference_results_match),
    TEST_MATCH("Δ { A, B, C }", test_difference  , ABC_values, set_difference_results_match),

    TEST_MATCH("⊂ { A }"      , test_issubset    , A_values  , set_subset_results_match),
    TEST_MATCH("⊂ { B }"      , test_issubset    , B_values  , set_subset_results_match),
    TEST_MATCH("⊂ { C }"      , test_issubset    , C_values  , set_subset_results_match),
    TEST_MATCH("⊂ { A, B }"   , test_issubset    , AB_values , set_subset_results_match),
    TEST_MATCH("⊂ { A, C }"   , test_issubset    , AC_values , set_subset_results_match),
    TEST_MATCH("⊂ { B, C }"   , test_issubset    , BC_values , set_subset_results_match),
    TEST_MATCH("⊂ { A, B, C }", test_issubset    , ABC_values, set_subset_results_match),

    TEST_MATCH("⊃ { A }"      , test_issuperset  , A_values  , set_superset_results_match),
    TEST_MATCH("⊃ { B }"      , test_issuperset  , B_values  , set_superset_results_match),
    TEST_MATCH("⊃ { C }"      , test_issuperset  , C_values  , set_superset_results_match),
    TEST_MATCH("⊃ { A, B }"   , test_issuperset  , AB_values , set_superset_results_match),
    TEST_MATCH("⊃ { A, C }"   , test_issuperset  , AC_values , set_superset_results_match),
    TEST_MATCH("⊃ { B, C }"   , test_issuperset  , BC_values , set_superset_results_match),
    TEST_MATCH("⊃ { A, B, C }", test_issuperset  , ABC_values, set_superset_results_match),
};

test_case _three_element_test_cases[] = 
{
    TEST_CASE ("add A"        , test_add         , A_VALUE   , TEST_RESULT_ONE),
    TEST_CASE ("add B"        , test_add         , B_VALUE   , TEST_RESULT_ONE),
    TEST_CASE ("add C"        , test_add         , C_VALUE   , TEST_RESULT_ONE),
    TEST_MATCH("remove A"     , test_remove      , A_VALUE   , remove_results_match),
    TEST_MATCH("remove B"     , test_remove      , B_VALUE   , remove_results_match),
    TEST_MATCH("remove C"     , test_remove      , C_VALUE   , remove_results_match),
    TEST_MATCH("count"        , test_count       , NULL      , count_results_match),

    TEST_MATCH("∪ { A }"      , test_union       , A_values  , set_union_results_match),
    TEST_MATCH("∪ { B }"      , test_union       , B_values  , set_union_results_match),
    TEST_MATCH("∪ { C }"      , test_union       , C_values  , set_union_results_match),
    TEST_MATCH("∪ { A, B }"   , test_union       , AB_values , set_union_results_match),
    TEST_MATCH("∪ { A, C }"   , test_union       , AC_values , set_union_results_match),
    TEST_MATCH("∪ { B, C }"   , test_union       , BC_values , set_union_results_match),
    TEST_MATCH("∪ { A, B, C }", test_union       , ABC_values, set_union_results_match),

    TEST_MATCH("∩ { A }"      , test_intersection, A_values  , set_intersection_results_match),
    TEST_MATCH("∩ { B }"      , test_intersection, B_values  , set_intersection_results_match),
    TEST_MATCH("∩ { C }"      , test_intersection, C_values  , set_intersection_results_match),
    TEST_MATCH("∩ { A, B }"   , test_intersection, AB_values , set_intersection_results_match),
    TEST_MATCH("∩ { A, C }"   , test_intersection, AC_values , set_intersection_results_match),
    TEST_MATCH("∩ { B, C }"   , test_intersection, BC_values , set_intersection_results_match),
    TEST_MATCH("∩ { A, B, C }", test_intersection, ABC_values, set_intersection_results_match),

    TEST_MATCH("Δ { A }"      , test_difference  , A_values  , set_difference_results_match),
    TEST_MATCH("Δ { B }"      , test_difference  , B_values  , set_difference_results_match),
    TEST_MATCH("Δ { C }"      , test_difference  , C_values  , set_difference_results_match),
    TEST_MATCH("Δ { A, B }"   , test_difference  , AB_values , set_difference_results_match),
    TEST_MATCH("Δ { A, C }"   , test_difference  , AC_values , set_difference_results_match),
    TEST_MATCH("Δ { B, C }"   , test_difference  , BC_values , set_difference_results_match),
    TEST_MATCH("Δ { A, B, C }", test_difference  , ABC_values, set_difference_results_match),

    TEST_MATCH("⊂ { A }"      , test_issubset    , A_values  , set_subset_results_match),
    TEST_MATCH("⊂ { B }"      , test_issubset    , B_values  , set_subset_results_match),
    TEST_MATCH("⊂ { C }"      , test_issubset    , C_values  , set_subset_results_match),
    TEST_MATCH("⊂ { A, B }"   , test_issubset    , AB_values , set_subset_results_match),
    TEST_MATCH("⊂ { A, C }"   , test_issubset    , AC_values , set_subset_results_match),
    TEST_MATCH("⊂ { B, C }"   , test_issubset    , BC_values , set_subset_results_match),
    TEST_MATCH("⊂ { A, B, C }", test_issubset    , ABC_values, set_subset_results_match),

    TEST_MATCH("⊃ { A }"      , test_issuperset  , A_values  , set_superset_results_match),
    TEST_MATCH("⊃ { B }"      , test_issuperset  , B_values  , set_superset_results_match),
    TEST_MATCH("⊃ { C }"      , test_issuperset  , C_values  , set_superset_results_match),
    TEST_MATCH("⊃ { A, B }"   , test_issuperset  , AB_values , set_superset_results_match),
    TEST_MATCH("⊃ { A, C }"   , test_issuperset  , AC_values , set_superset_results_match),
    TEST_MATCH("⊃ { B, C }"   , test_issuperset  , BC_values , set_superset_results_match),
    TEST_MATCH("⊃ { A, B, C }", test_issuperset  , ABC_values, set_superset_results_match),
};

/// scenarios
test_scenario _scenarios[] = 
{
    TEST_SCENARIO("empty"          , _values, _empty_test_cases, construct_empty          , destruct_set),
    TEST_SCENARIO("A_removeA_empty", _values, _empty_test_cases, construct_A_removeA_empty, destruct_set),
    TEST_SCENARIO("B_removeB_empty", _values, _empty_test_cases, construct_B_removeB_empty, destruct_set),
    TEST_SCENARIO("C_removeC_empty", _values, _empty_test_cases, construct_C_removeC_empty, destruct_set),

    TEST_SCENARIO("empty_addA_A", A_values, _one_element_test_cases, construct_empty_addA_A, destruct_set),
    TEST_SCENARIO("empty_addB_B", B_values, _one_element_test_cases, construct_empty_addB_B, destruct_set),
    TEST_SCENARIO("empty_addC_C", C_values, _one_element_test_cases, construct_empty_addC_C, destruct_set),
    TEST_SCENARIO("AB_removeB_A", A_values, _one_element_test_cases, construct_AB_removeB_A, destruct_set),
    TEST_SCENARIO("AB_removeA_B", B_values, _one_element_test_cases, construct_AB_removeA_B, destruct_set),
    TEST_SCENARIO("AC_removeC_A", A_values, _one_element_test_cases, construct_AC_removeC_A, destruct_set),
    TEST_SCENARIO("AC_removeA_C", C_values, _one_element_test_cases, construct_AC_removeA_C, destruct_set),
    TEST_SCENARIO("BC_removeB_C", C_values, _one_element_test_cases, construct_BC_removeB_C, destruct_set),
    TEST_SCENARIO("BC_removeC_B", B_values, _one_element_test_cases, construct_BC_removeC_B, destruct_set),

    TEST_SCENARIO("A_addB_AB"     , AB_values, _two_element_test_cases, construct_A_addB_AB     , destruct_set),
    TEST_SCENARIO("A_addC_AC"     , AC_values, _two_element_test_cases, construct_A_addC_AC     , destruct_set),
    TEST_SCENARIO("B_addA_AB"     , AB_values, _two_element_test_cases, construct_B_addA_AB     , destruct_set),
    TEST_SCENARIO("B_addC_BC"     , BC_values, _two_element_test_cases, construct_B_addC_BC     , destruct_set),
    TEST_SCENARIO("C_addB_BC"     , BC_values, _two_element_test_cases, construct_C_addB_BC     , destruct_set),
    TEST_SCENARIO("C_addA_AC"     , AC_values, _two_element_test_cases, construct_C_addA_AC     , destruct_set),
    TEST_SCENARIO("ABC_removeC_AB", AB_values, _two_element_test_cases, construct_ABC_removeC_AB, destruct_set),
    TEST_SCENARIO("ABC_removeB_AC", AC_values, _two_element_test_cases, construct_ABC_removeB_AC, destruct_set),
    TEST_SCENARIO("ABC_removeA_BC", BC_values, _two_element_test_cases, construct_ABC_removeA_BC, destruct_set),

    TEST_SCENARIO("AB_addC_ABC", ABC_values, _three_element_test_cases, construct_AB_addC_ABC, destruct_set),
    TEST_SCENARIO("AC_addB_ABC", ABC_values, _three_element_test_cases, construct_AC_addB_ABC, destruct_set),
    TEST_SCENARIO("BC_addA_ABC", ABC_values, _three_element_test_cases, construct_BC_addA_ABC, destruct_set),
};

/// suites
test_suite _suite = TEST_SUITE("set", _scenarios);

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
    return set_construct((set **)pp_result, 4, (fn_comparator *) strcmp);
}

int construct_A_removeA_empty ( void **pp_result ) 
{ 

    // { A }
    construct_empty_addA_A(pp_result);

    // { A } -> remove(A) -> { }
    return set_remove(*((set **)pp_result), A_VALUE); 
}

int construct_B_removeB_empty ( void **pp_result ) 
{ 

    // { B }
    construct_empty_addB_B(pp_result);

    // { B } -> remove(B) -> { }
    return set_remove(*((set **)pp_result), B_VALUE); 
}

int construct_C_removeC_empty ( void **pp_result ) 
{ 

    // { C }
    construct_empty_addC_C(pp_result);

    // { C } -> remove(C) -> { }
    return set_remove(*((set **)pp_result), C_VALUE); 
}

int construct_empty_addA_A ( void **pp_result ) 
{ 

    // { }
    construct_empty(pp_result);

    // { } -> add(A) -> { A }
    return set_add(*((set **)pp_result), A_VALUE); 
}

int construct_empty_addB_B ( void **pp_result ) 
{

    // { }
    construct_empty(pp_result);

    // { } -> add(B) -> { B }
    return set_add(*((set **)pp_result), B_VALUE); 
}

int construct_empty_addC_C ( void **pp_result ) 
{

    // { }
    construct_empty(pp_result);

    // { } -> add(C) -> { C }
    return set_add(*((set **)pp_result), C_VALUE); 
}

int construct_AB_removeB_A ( void **pp_result ) 
{

    // { A, B }
    construct_A_addB_AB(pp_result);

    // { A, B } -> remove(B) -> { A }
    return set_remove(*((set **)pp_result), B_VALUE); 
}

int construct_AB_removeA_B ( void **pp_result ) 
{

    // { A, B }
    construct_A_addB_AB(pp_result);

    // { A, B } -> remove(A) -> { B }
    return set_remove(*((set **)pp_result), A_VALUE); 
}

int construct_AC_removeC_A ( void **pp_result ) 
{

    // { A, C }
    construct_A_addC_AC(pp_result);

    // { A, C } -> remove(C) -> { A }
    return set_remove(*((set **)pp_result), C_VALUE); 
}

int construct_AC_removeA_C ( void **pp_result ) 
{

    // { A, C }
    construct_A_addC_AC(pp_result);

    // { A, C } -> remove(A) -> { C }
    return set_remove(*((set **)pp_result), A_VALUE); 
}

int construct_BC_removeB_C ( void **pp_result ) 
{

    // { B, C }
    construct_B_addC_BC(pp_result);

    // { B, C } -> remove(B) -> { C }
    return set_remove(*((set **)pp_result), B_VALUE); 
}

int construct_BC_removeC_B ( void **pp_result ) 
{

    // { B, C }
    construct_B_addC_BC(pp_result);

    // { B, C } -> remove(C) -> { B }
    return set_remove(*((set **)pp_result), C_VALUE); 
}

int construct_A_addB_AB ( void **pp_result ) 
{

    // { A }
    construct_empty_addA_A(pp_result);

    // { A } -> add(B) -> { A, B }
    return set_add(*((set **)pp_result), B_VALUE); 
}

int construct_A_addC_AC ( void **pp_result ) 
{

    // { A }
    construct_empty_addA_A(pp_result);

    // { A } -> add(C) -> { A, C }
    return set_add(*((set **)pp_result), C_VALUE); 
}

int construct_B_addA_AB ( void **pp_result ) 
{

    // { B }
    construct_empty_addB_B(pp_result);

    // { B } -> add(A) -> { A, B }
    return set_add(*((set **)pp_result), A_VALUE); 
}

int construct_B_addC_BC ( void **pp_result ) 
{

    // { B }
    construct_empty_addB_B(pp_result);
    
    // { B } -> add(C) -> { B, C }
    return set_add(*((set **)pp_result), C_VALUE); 
}

int construct_C_addB_BC ( void **pp_result ) 
{

    // { C }
    construct_empty_addC_C(pp_result);

    // { C } -> add(B) -> { B, C }
    return set_add(*((set **)pp_result), B_VALUE); 
}

int construct_C_addA_AC ( void **pp_result ) 
{

    // { C }
    construct_empty_addC_C(pp_result);

    // { C } -> add(A) -> { A, C }
    return set_add(*((set **)pp_result), A_VALUE); 
}

int construct_ABC_removeC_AB ( void **pp_result ) 
{

    // { A, B, C }
    construct_AB_addC_ABC(pp_result);

    // { A, B, C } -> remove(C) -> { A, B }
    return set_remove(*((set **)pp_result), C_VALUE); 
}

int construct_ABC_removeB_AC ( void **pp_result ) 
{

    // { A, B, C }
    construct_AC_addB_ABC(pp_result);

    // { A, B, C } -> remove(B) -> { A, C}
    return set_remove(*((set **)pp_result), B_VALUE); 
}

int construct_ABC_removeA_BC ( void **pp_result ) 
{

    // { A, B, C }
    construct_BC_addA_ABC(pp_result);

    // { A, B, C } -> remove(A) -> { B, C }
    return set_remove(*((set **)pp_result), A_VALUE); 
}

int construct_AB_addC_ABC ( void **pp_result ) 
{

    // { A, B }
    construct_A_addB_AB(pp_result);

    // { A, B } -> add(C) -> { A, B, C }
    return set_add(*((set **)pp_result), C_VALUE); 
}

int construct_AC_addB_ABC ( void **pp_result ) 
{

    // { A, C }
    construct_A_addC_AC(pp_result);

    // { A, C } -> add(B) -> { A, B, C }
    return set_add(*((set **)pp_result), B_VALUE); 
}

int construct_BC_addA_ABC ( void **pp_result ) 
{

    // { B, C }
    construct_B_addC_BC(pp_result);

    // { B, C } -> add(A) -> { A, B, C }
    return set_add(*((set **)pp_result), A_VALUE); 
}

void *test_add ( test_case *p_test_case, void *p_subject ) 
{ 

    // done
    return (void *)(size_t)set_add((set *)p_subject, p_test_case->p_parameters); 
}

void *test_remove ( test_case *p_test_case, void *p_subject ) 
{ 
    
    // initialized data
    const char *key = (const char *)p_test_case->p_parameters;

    // clear the result
    p_test_case->p_out = NULL;
    
    // key?
    if ( key == NULL ) 
    {

        // initialized data
        const char **pp_keys = (const char **)p_test_case->p_data;

        // keys?
        if ( pp_keys ) 

            // key?
            if ( pp_keys[0] )

                // store the key
                key = pp_keys[0];
    }

    // test
    if ( 0 == set_remove((set *)p_subject, (void *)key) ) return NULL;

    // store the result
    p_test_case->p_out = (void *)key;

    // success
    return (void *)1;
}

void *test_pop ( test_case *p_test_case, void *p_subject ) 
{ 

    // clear the result
    p_test_case->p_out = NULL;

    // test
    if ( 0 == set_pop((set *)p_subject, &p_test_case->p_out) ) return NULL;

    // success
    return (void *)1;
}

void *test_count ( test_case *p_test_case, void *p_subject ) 
{ 

    // unused
    (void) p_test_case;

    // done
    return (void *)set_count((set *)p_subject);
}

void *test_union ( test_case *p_test_case, void *p_subject )
{

    // initialized data
    set         *p_set_a       = (set *)p_subject;
    set         *p_set_b       = NULL;
    set         *p_result      = NULL;
    const char **pp_b_elements = (const char **)p_test_case->p_parameters;
    size_t       b_count       = 0;
    void       **pp_contents   = NULL;
    size_t       res_count     = 0;

    // clear the result
    p_test_case->p_out = NULL;

    // elements?
    if ( pp_b_elements )

        // count
        while ( pp_b_elements[b_count] ) b_count++;

    // construct a set
    set_from_elements(&p_set_b, (void **)pp_b_elements, b_count, (fn_comparator *) strcmp);

    // union
    set_union(&p_result, p_set_a, p_set_b);
    
    // count 
    res_count = set_count(p_result);

    // error check
    if ( 0 == res_count ) goto no_results;

    // allocate memory for the elements
    pp_contents = default_allocator(NULL, (res_count + 1) * sizeof(void *));

    // store the contents
    set_contents(p_result, pp_contents);

    // store an end marker
    pp_contents[res_count] = NULL;

    no_results:
    
    // store the contents
    p_test_case->p_out = pp_contents;
    
    // release the auxiliary sets
    set_destroy(&p_set_b, NULL);
    set_destroy(&p_result, NULL);
    
    // success
    return (void *)1;
}

void *test_intersection ( test_case *p_test_case, void *p_subject )
{

    // initialized data
    set         *p_set_a       = (set *)p_subject;
    set         *p_set_b       = NULL;
    set         *p_result      = NULL;
    const char **pp_b_elements = (const char **)p_test_case->p_parameters;
    size_t       b_count       = 0;
    void       **pp_contents   = NULL;
    size_t       res_count     = 0;

    // clear the result
    p_test_case->p_out = NULL;

    // count
    while(pp_b_elements[b_count]) b_count++;

    // construct a set
    set_from_elements(&p_set_b, (void **)pp_b_elements, b_count, (fn_comparator *) strcmp);

    // intersection
    set_intersection(&p_result, p_set_a, p_set_b);
    
    // count
    res_count = set_count(p_result);

    // error check
    if ( 0 == res_count ) goto no_results;

    // allocate memory for the elements
    pp_contents = default_allocator(NULL, (res_count + 1) * sizeof(void *));

    // store the contents
    set_contents(p_result, pp_contents);

    // store an end marker
    pp_contents[res_count] = NULL;
    
    no_results:

    // store the contents
    p_test_case->p_out = pp_contents;
    
    // release the auxiliary sets
    set_destroy(&p_set_b, NULL);
    set_destroy(&p_result, NULL);
    
    // success
    return (void *)1;
}

void *test_difference ( test_case *p_test_case, void *p_subject )
{

    // initialized data
    set         *p_set_a       = (set *)p_subject;
    set         *p_set_b       = NULL;
    set         *p_result      = NULL;
    const char **pp_b_elements = (const char **)p_test_case->p_parameters;
    size_t       b_count       = 0;
    void       **pp_contents   = NULL;
    size_t       res_count     = 0;

    // clear the result
    p_test_case->p_out = NULL;

    // count
    while(pp_b_elements[b_count]) b_count++;

    // construct a set
    set_from_elements(&p_set_b, (void **)pp_b_elements, b_count, (fn_comparator *) strcmp);

    // difference
    set_difference(&p_result, p_set_a, p_set_b);
    
    // count
    res_count = set_count(p_result);

    // error check
    if ( 0 == res_count ) goto no_results;

    // allocate memory for the elements
    pp_contents = default_allocator(NULL, (res_count + 1) * sizeof(void *));

    // store the contents
    set_contents(p_result, pp_contents);

    // store an end marker
    pp_contents[res_count] = NULL;

    no_results:

    // store the contents
    p_test_case->p_out = pp_contents;
    
    // release the auxiliary sets
    set_destroy(&p_set_b, NULL);
    set_destroy(&p_result, NULL);
    
    // success
    return (void *)1;
}

void *test_issubset ( test_case *p_test_case, void *p_subject )
{

    // initialized data
    set         *p_set_a       = (set *)p_subject;
    set         *p_set_b       = NULL;
    const char **pp_b_elements = (const char **)p_test_case->p_parameters;
    size_t       b_count       = 0;
    bool         res           = false;

    // count
    while ( pp_b_elements[b_count] ) b_count++;

    // construct a set
    set_from_elements(&p_set_b, (void **)pp_b_elements, b_count, (fn_comparator *) strcmp);

    // subset?
    res = set_issubset(p_set_a, p_set_b);

    // release the auxiliary set
    set_destroy(&p_set_b, NULL);

    // done
    return (void *)(size_t)res;
}

void *test_issuperset ( test_case *p_test_case, void *p_subject )
{

    // initialized data
    set         *p_set_a       = (set *)p_subject;
    set         *p_set_b       = NULL;
    const char **pp_b_elements = (const char **)p_test_case->p_parameters;
    size_t       b_count       = 0;
    bool         res           = false;

    // count
    while ( pp_b_elements[b_count] ) b_count++;

    // construct a set
    set_from_elements(&p_set_b, (void **)pp_b_elements, b_count, (fn_comparator *) strcmp);

    // superset?
    res = set_issuperset(p_set_a, p_set_b);

    // release the auxiliary set
    set_destroy(&p_set_b, NULL);

    // done
    return (void *)(size_t)res;
}

bool remove_results_match ( test_scenario *p_scenario, test_case *p_case, void *p_subject, void *p_result )
{

    // unused
    (void) p_subject; 
    (void) p_result;

    // initialized data
    const char **pp_expected = (const char **)p_scenario->p_data;
    const char  *p_expected  = (const char *)p_case->p_parameters;
    bool         was_in_set  = false;
    
    // iterate through expected
    for (size_t i = 0; pp_expected[i]; i++) 

        // match?
        if ( p_expected == pp_expected[i] ) 
        {

            // set the present flag
            was_in_set = true;

            // done
            break;
        }

    // done
    return (was_in_set) ? (p_case->p_out == p_expected) : (p_case->p_out == NULL);
}

bool count_results_match ( test_scenario *p_scenario, test_case *p_case, void *p_subject, void *p_result )
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

bool set_union_results_match ( test_scenario *p_scenario, test_case *p_case, void *p_subject, void *p_result )
{

    // unused
    (void) p_subject;
    (void) p_result;

    // initialized data
    const char **pp_a           = (const char **)p_scenario->p_data;
    const char **pp_b           = (const char **)p_case->p_parameters;
    void       **pp_actual      = (void **)p_case->p_out;
    size_t       actual_count   = 0;
    const char  *expected[32]   = { 0 };
    size_t       expected_count = 0;
    bool         match          = true;

    // count
    while(pp_actual[actual_count]) actual_count++;

    // iterate through A
    for (size_t i = 0; pp_a[i]; i++) 

        // store elements
        expected[expected_count++] = pp_a[i];
    
    // iterate through B
    for (size_t i = 0; pp_b[i]; i++) 
    {

        // initialized data
        bool found = false;

        // search B
        for (size_t j = 0; j < expected_count; j++) 

            // match?
            if ( 0 == strcmp(pp_b[i], expected[j]) ) 
            { 
                
                // set the found flag
                found = true; 

                // done
                break;
            }
        
        // miss?
        if ( false == found )

            // store the element
            expected[expected_count++] = pp_b[i];
    }

    
    // test
    if ( expected_count != actual_count )
    {

        // release elements
        if ( pp_actual ) 
            pp_actual = default_allocator(pp_actual, 0);

        // error
        return false;
    }

    // test
    for (size_t i = 0; i < actual_count; i++)
    {

        // initialized data
        bool found = false;
    
        // iterate through expected elements
        for (size_t j = 0; j < expected_count; j++) 

            // match?
            if ( pp_actual[i] == expected[j] ) 
            { 
                
                // set the found flag
                found = true; 
                
                // done
                break; 
            }
        
        // miss?
        if ( false == found ) 
        { 
            
            // clear the match flag
            match = false; 
            
            // done
            break; 
        }
    }

    // release elements
    if ( pp_actual )
        pp_actual = default_allocator(pp_actual, 0);

    // done
    return match;
}

bool set_intersection_results_match ( test_scenario *p_scenario, test_case *p_case, void *p_subject, void *p_result )
{

    // unused
    (void) p_subject;
    (void) p_result;

    // initialized data
    const char **pp_a           = (const char **)p_scenario->p_data;
    const char **pp_b           = (const char **)p_case->p_parameters;
    void       **pp_actual      = (void **)p_case->p_out;
    size_t       actual_count   = 0;
    const char  *expected[32]   = { 0 };
    size_t       expected_count = 0;
    bool         match          = true;

    // elements?
    if ( pp_actual ) 

        // count
        while(pp_actual[actual_count]) actual_count++;

    // A?
    if ( NULL == pp_a ) goto no_a;

    // iterate through A
    for (size_t i = 0; pp_a[i]; i++) 
    {

        // initialized data
        bool found = false;

        // B?
        if ( pp_b ) 
        {

            // iterate through B
            for (size_t j = 0; pp_b && pp_b[j]; j++) 

                // match?
                if ( 0 == strcmp(pp_a[i], pp_b[j]) )
                { 
                    
                    // set the found flag
                    found = true; 
                    
                    // done
                    break; 
                }
            
            // found?
            if ( found ) 
                
                // store the element
                expected[expected_count++] = pp_a[i];
        }
    }

    no_a:
    // test
    if ( expected_count != actual_count )
    {

        // release elements
        if ( pp_actual ) 
            pp_actual = default_allocator(pp_actual, 0);

        // error
        return false;
    }

    // test
    for (size_t i = 0; i < actual_count; i++)
    {

        // initialized data
        bool found = false;
    
        // iterate through expected elements
        for (size_t j = 0; j < expected_count; j++) 

            // match?
            if ( pp_actual[i] == expected[j] ) 
            { 
                
                // set the found flag
                found = true; 
                
                // done
                break; 
            }
        
        // miss?
        if ( false == found ) 
        { 
            
            // clear the match flag
            match = false; 
            
            // done
            break; 
        }
    }

    // release elements
    if ( pp_actual )
        pp_actual = default_allocator(pp_actual, 0);

    // done
    return match;
}

bool set_difference_results_match ( test_scenario *p_scenario, test_case *p_case, void *p_subject, void *p_result )
{

    // unused
    (void) p_subject;
    (void) p_result;

    // initialized data
    const char **pp_a           = (const char **)p_scenario->p_data;
    const char **pp_b           = (const char **)p_case->p_parameters;
    void       **pp_actual      = (void **)p_case->p_out;
    size_t       actual_count   = 0;
    const char  *expected[32]   = { 0 };
    size_t       expected_count = 0;
    bool         match          = true;

    // elements?
    if ( pp_actual ) 

        // count
        while(pp_actual[actual_count]) actual_count++;

    // A?
    if ( NULL == pp_a ) goto no_a;

    // iterate through A
    for (size_t i = 0; pp_a[i]; i++) 
    {

        // initialized data
        bool found = false;

        // B?
        if ( pp_b ) 
        {

            // iterate through B
            for (size_t j = 0; pp_b && pp_b[j]; j++) 

                // match?
                if ( 0 == strcmp(pp_a[i], pp_b[j]) )
                { 
                    
                    // set the found flag
                    found = true; 
                    
                    // done
                    break; 
                }
            
            // found?
            if ( !found ) 
                
                // store the element
                expected[expected_count++] = pp_a[i];
        }
    }

    no_a:
    // test
    if ( expected_count != actual_count )
    {

        // release elements
        if ( pp_actual ) 
            pp_actual = default_allocator(pp_actual, 0);

        // error
        return false;
    }

    // test
    for (size_t i = 0; i < actual_count; i++)
    {

        // initialized data
        bool found = false;
    
        // iterate through expected elements
        for (size_t j = 0; j < expected_count; j++) 

            // match?
            if ( pp_actual[i] == expected[j] ) 
            { 
                
                // set the found flag
                found = true; 
                
                // done
                break; 
            }
        
        // miss?
        if ( false == found ) 
        { 
            
            // clear the match flag
            match = false; 
            
            // done
            break; 
        }
    }

    // release elements
    if ( pp_actual )
        pp_actual = default_allocator(pp_actual, 0);

    // done
    return match;
}

bool set_subset_results_match ( test_scenario *p_scenario, test_case *p_case, void *p_subject, void *p_result )
{

    // unused
    (void) p_subject;

    // initialized data
    const char **pp_a     = (const char **)p_scenario->p_data;
    const char **pp_b     = (const char **)p_case->p_parameters;
    bool         expected = true;

    // iterate through A
    for (size_t i = 0; pp_a[i]; i++)
    {

        // initialized data
        bool found = false;

        // iterate through B
        for (size_t j = 0; pp_b && pp_b[j]; j++) 

            // match?
            if ( 0 == strcmp(pp_a[i], pp_b[j]) ) found = true;
        

        // found?
        if ( false == found )    
            expected = false;
    }

    // done
    return (bool)(size_t)p_result == expected;
}

bool set_superset_results_match ( test_scenario *p_scenario, test_case *p_case, void *p_subject, void *p_result )
{

    // unused
    (void) p_subject;

    // initialized data
    const char **pp_a     = (const char **)p_scenario->p_data;
    const char **pp_b     = (const char **)p_case->p_parameters;
    bool         expected = true;

    // iterate through B
    for (size_t i = 0; pp_b[i]; i++)
    {

        // initialized data
        bool found = false;

        // iterate through A
        for (size_t j = 0; pp_a && pp_a[j]; j++) 

            // match?
            if ( 0 == strcmp(pp_b[i], pp_a[j]) ) found = true;
        
        // found?
        if ( false == found ) expected = false;
    }

    // done
    return (bool)(size_t)p_result == expected;
}

void *destruct_set ( void *p_pointer, unsigned long long size )
{

    // unused
    (void) size;

    // initialized data
    set *p_set = (set *)p_pointer;

    // release the set
    set_destroy(&p_set, NULL);

    // success
    return NULL;
}
