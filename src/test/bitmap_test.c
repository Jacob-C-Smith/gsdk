/** !
 * Tester for bitmap module
 *
 * @file src/test/bitmap_test.c
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
#include <data/bitmap.h>

// function declarations
/// scenario constructors
fn_scenario_constructor construct_1_0;
fn_scenario_constructor construct_1_1;
fn_scenario_constructor construct_2_00;
fn_scenario_constructor construct_2_01;
fn_scenario_constructor construct_2_10;
fn_scenario_constructor construct_2_11;
fn_scenario_constructor construct_3_000;
fn_scenario_constructor construct_3_001;
fn_scenario_constructor construct_3_010;
fn_scenario_constructor construct_3_011;
fn_scenario_constructor construct_3_100;
fn_scenario_constructor construct_3_101;
fn_scenario_constructor construct_3_110;
fn_scenario_constructor construct_3_111;

/// test cases
fn_test_case test_test;
fn_test_case test_set;
fn_test_case test_clear;

/// result evaluators
fn_results_match test_results_match;

/// allocators
fn_allocator destruct_bitmap;

// data
/// values
size_t b1_0   = 0b0;
size_t b1_1   = 0b1;
size_t b2_00  = 0b00;
size_t b2_01  = 0b01;
size_t b2_10  = 0b10;
size_t b2_11  = 0b11;
size_t b3_000 = 0b000;
size_t b3_001 = 0b001;
size_t b3_010 = 0b010;
size_t b3_011 = 0b011;
size_t b3_100 = 0b100;
size_t b3_101 = 0b101;
size_t b3_110 = 0b110;
size_t b3_111 = 0b111;

// test
/// cases
test_case _1_bit_test_cases[] = 
{
    TEST_MATCH("test 0" , test_test , (void *)0, test_results_match),
    TEST_CASE ("set 0"  , test_set  , (void *)0, TEST_RESULT_ONE),
    TEST_CASE ("clear 0", test_clear, (void *)0, TEST_RESULT_ONE),
};

test_case _2_bit_test_cases[] = 
{
    TEST_MATCH("test 0" , test_test , (void *)0, test_results_match),
    TEST_MATCH("test 1" , test_test , (void *)1, test_results_match),
    TEST_CASE ("set 0"  , test_set  , (void *)0, TEST_RESULT_ONE),
    TEST_CASE ("set 1"  , test_set  , (void *)1, TEST_RESULT_ONE),
    TEST_CASE ("clear 0", test_clear, (void *)0, TEST_RESULT_ONE),
    TEST_CASE ("clear 1", test_clear, (void *)1, TEST_RESULT_ONE),
};

test_case _3_bit_test_cases[] = 
{
    TEST_MATCH("test 0" , test_test , (void *)0, test_results_match),
    TEST_MATCH("test 1" , test_test , (void *)1, test_results_match),
    TEST_MATCH("test 2" , test_test , (void *)2, test_results_match),
    TEST_CASE ("set 0"  , test_set  , (void *)0, TEST_RESULT_ONE),
    TEST_CASE ("set 1"  , test_set  , (void *)1, TEST_RESULT_ONE),
    TEST_CASE ("set 2"  , test_set  , (void *)2, TEST_RESULT_ONE),
    TEST_CASE ("clear 0", test_clear, (void *)0, TEST_RESULT_ONE),
    TEST_CASE ("clear 1", test_clear, (void *)1, TEST_RESULT_ONE),
    TEST_CASE ("clear 2", test_clear, (void *)2, TEST_RESULT_ONE),
};

/// scenarios
test_scenario _scenarios[] = 
{
    TEST_SCENARIO("0"  , &b1_0  , _1_bit_test_cases, construct_1_0  , destruct_bitmap),
    TEST_SCENARIO("1"  , &b1_1  , _1_bit_test_cases, construct_1_1  , destruct_bitmap),
    TEST_SCENARIO("00" , &b2_00 , _2_bit_test_cases, construct_2_00 , destruct_bitmap),
    TEST_SCENARIO("01" , &b2_01 , _2_bit_test_cases, construct_2_01 , destruct_bitmap),
    TEST_SCENARIO("10" , &b2_10 , _2_bit_test_cases, construct_2_10 , destruct_bitmap),
    TEST_SCENARIO("11" , &b2_11 , _2_bit_test_cases, construct_2_11 , destruct_bitmap),
    TEST_SCENARIO("000", &b3_000, _3_bit_test_cases, construct_3_000, destruct_bitmap),
    TEST_SCENARIO("001", &b3_001, _3_bit_test_cases, construct_3_001, destruct_bitmap),
    TEST_SCENARIO("010", &b3_010, _3_bit_test_cases, construct_3_010, destruct_bitmap),
    TEST_SCENARIO("011", &b3_011, _3_bit_test_cases, construct_3_011, destruct_bitmap),
    TEST_SCENARIO("100", &b3_100, _3_bit_test_cases, construct_3_100, destruct_bitmap),
    TEST_SCENARIO("101", &b3_101, _3_bit_test_cases, construct_3_101, destruct_bitmap),
    TEST_SCENARIO("110", &b3_110, _3_bit_test_cases, construct_3_110, destruct_bitmap),
    TEST_SCENARIO("111", &b3_111, _3_bit_test_cases, construct_3_111, destruct_bitmap),
};

/// suites
test_suite _suite = TEST_SUITE("bitmap", _scenarios);

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
int construct_1_0 ( void **pp_result ) 
{ 

    // ... -> [ 0 ]
    return bitmap_construct((bitmap **)pp_result, 1);
}

int construct_1_1 ( void **pp_result ) 
{ 

    // [ 0 ] -> set(1) -> [ 1 ]
    construct_1_0(pp_result);

    // done
    return bitmap_set(*((bitmap **)pp_result), 0);
}

int construct_2_00 ( void **pp_result ) 
{ 

    // ... -> [ 0, 0 ]
    return bitmap_construct((bitmap **)pp_result, 2);
}

int construct_2_01 ( void **pp_result ) 
{ 
    
    // [ 0, 0 ] -> set(0) -> [ 0, 1 ]
    construct_2_00(pp_result);

    // done
    return bitmap_set(*((bitmap **)pp_result), 0);
}

int construct_2_10 ( void **pp_result ) 
{ 
    // [ 0, 0 ] -> set(1) -> [ 1, 0 ]
    construct_2_00(pp_result);
    
    // done
    return bitmap_set(*((bitmap **)pp_result), 1);
}

int construct_2_11 ( void **pp_result ) 
{ 
    
    // [ 1, 0 ] -> set(0) -> [ 1, 1 ]
    construct_2_10(pp_result);
    
    // done
    return bitmap_set(*((bitmap **)pp_result), 0);
}

int construct_3_000 ( void **pp_result ) 
{ 

    // ... -> [ 0, 0, 0 ]
    return bitmap_construct((bitmap **)pp_result, 3);
}

int construct_3_001 ( void **pp_result ) 
{ 

    // [ 0, 0, 0 ] -> set(0) -> [ 0, 0, 1 ]
    construct_3_000(pp_result);

    // done
    return bitmap_set(*((bitmap **)pp_result), 0);
}

int construct_3_010 ( void **pp_result ) 
{ 

    // [ 0, 0, 0 ] -> set(1) -> [ 0, 1, 0 ]
    construct_3_000(pp_result);

    // done
    return bitmap_set(*((bitmap **)pp_result), 1);
}

int construct_3_011 ( void **pp_result ) 
{ 

    // [ 0, 1, 0 ] -> set(0) -> [ 0, 1, 1 ]
    construct_3_010(pp_result);

    // done
    return bitmap_set(*((bitmap **)pp_result), 0);
}

int construct_3_100 ( void **pp_result ) 
{ 

    // [ 0, 0, 0 ] -> set(2) -> [ 1, 0, 0 ]
    construct_3_000(pp_result);

    // done
    return bitmap_set(*((bitmap **)pp_result), 2);
}

int construct_3_101 ( void **pp_result ) 
{ 

    // [ 1, 0, 0 ] -> set(0) -> [ 1, 0, 1 ]
    construct_3_100(pp_result);

    // done
    return bitmap_set(*((bitmap **)pp_result), 0);
}

int construct_3_110 ( void **pp_result ) 
{ 

    // [ 0, 1, 0 ] -> set(2) -> [ 1, 1, 0 ]
    construct_3_010(pp_result);

    // done
    return bitmap_set(*((bitmap **)pp_result), 2);
}

int construct_3_111 ( void **pp_result ) 
{ 

    // [ 1, 0, 1 ] -> set(1) -> [ 1, 1, 1 ]
    construct_3_101(pp_result);

    // done
    return bitmap_set(*((bitmap **)pp_result), 1);
}

// test cases
void *test_test ( test_case *p_test_case, void *p_subject ) 
{

    // done
    return (void *)(size_t)bitmap_test((bitmap *)p_subject, (size_t)p_test_case->p_parameters); 
}

void *test_set ( test_case *p_test_case, void *p_subject ) 
{

    // done
    return (void *)(size_t)bitmap_set((bitmap *)p_subject, (size_t)p_test_case->p_parameters); 
}

void *test_clear ( test_case *p_test_case, void *p_subject ) 
{

    // done
    return (void *)(size_t)bitmap_clear((bitmap *)p_subject, (size_t)p_test_case->p_parameters); 
}

// result evaluators
bool test_results_match ( test_scenario *p_scenario, test_case *p_case, void *p_subject, void *p_result )
{

    // unused
    (void) p_subject;

    // initialized data
    size_t expected_mask = *(size_t *)p_scenario->p_data;
    size_t index         = (size_t)p_case->p_parameters;
    bool   expected      = (expected_mask & ( 1 << index )) ? true : false;

    // done
    return p_result == (void *)(size_t)expected;
}

// allocators
void *destruct_bitmap ( void *p_pointer, unsigned long long size )
{

    // unused
    (void) size;

    // release the bitmap
    bitmap_destroy((bitmap **)&p_pointer);

    // success
    return NULL;
}
