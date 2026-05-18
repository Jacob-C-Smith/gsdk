/** !
 * Tester interface
 * 
 * @file src/core/test/test.h
 * 
 * @author Jacob Smith 
 */

// header guard
#pragma once

// standard library
#include <stdio.h>
#include <time.h>
#include <stdint.h>

// gsdk
/// core
#include <core/log.h>
#include <core/interfaces.h>
#include <core/sync.h>

// preprocessor macros
#define TEST_CASE(_name, _test_fn, _params, _expected) \
    { \
        .p_name = (_name), \
        .pfn_test_case = (_test_fn), \
        .p_parameters = (void *)(uintptr_t)(_params), \
        .result = (_expected) \
    }

#define TEST_MATCH(_name, _test_fn, _params, _match_fn, _expected) \
    { \
        .p_name = (_name), \
        .pfn_test_case = (_test_fn), \
        .p_parameters = (void *)(uintptr_t)(_params), \
        .pfn_results_match = (_match_fn), \
        .result = (_expected) \
    }

#define TEST_CASE_ENV(_name, _test_fn, _params, _env, _expected) \
    { \
        .p_name = (_name), \
        .pfn_test_case = (_test_fn), \
        .p_parameters = (void *)(uintptr_t)(_params), \
        .p_environment = (void *)(_env), \
        .result = (_expected) \
    }

#define TEST_SCENARIO(_name, _data, _cases_array, _constructor, _destructor) \
    { \
        .p_name          = (_name), \
        .p_data          = (void *)(_data), \
        .p_cases         = (_cases_array), \
        .case_quantity   = sizeof(_cases_array) / sizeof(*(_cases_array)), \
        .pfn_constructor = (_constructor), \
        .pfn_destructor  = (_destructor) \
    }

#define TEST_SUITE(_name, _scenarios_array) \
    { \
        .p_name            = (_name), \
        .p_scenarios       = (_scenarios_array), \
        .scenario_quantity = sizeof(_scenarios_array) / sizeof(*(_scenarios_array)) \
    }

// enumeration definitions
enum test_result_e
{
    TEST_RESULT_ZERO,
    TEST_RESULT_ONE,
    TEST_RESULT_MATCH,
    TEST_RESULT_QUANTITY,
};

// structure declarations
struct test_suite_s;
struct test_scenario_s;
struct test_case_s;

// type definitions
typedef struct test_suite_s    test_suite;
typedef struct test_scenario_s test_scenario;
typedef struct test_case_s     test_case;

typedef void *(fn_test_case)( test_case *p_test_case, void *p_subject );
typedef int (fn_scenario_constructor)( void **pp_result );
typedef int (fn_scenario)( fn_scenario_constructor *pfn_scenario_constructor, void *p_env );
typedef bool (fn_results_match) ( test_scenario *p_scenario, test_case *p_case, void *p_subject, void *p_result );

// structure declarations
struct test_suite_s
{
    char          *p_name;
    test_scenario *p_scenarios;
    size_t         scenario_quantity;
    timestamp t0, t1;
    struct 
    {
        struct 
        {
            int tests, passes, fails;
        } total, ephemeral;
    } counters;
};

struct test_scenario_s
{
    char                    *p_name;
    void                    *p_data;
    test_case               *p_cases;
    size_t                   case_quantity;
    fn_scenario             *pfn_scenario;
    fn_scenario_constructor *pfn_constructor;
    fn_allocator            *pfn_destructor;
};

struct test_case_s
{
    char               *p_name;
    fn_test_case       *pfn_test_case;
    fn_results_match   *pfn_results_match;
    void               *p_parameters;
    void               *p_environment;
    void               *p_result;
    void               *p_data;
    void               *p_out;
    enum test_result_e  result;
};

// function declarations
int test_suite_test ( test_suite *p_test_suite );
int test_scenario_test 
( 
    test_scenario *p_test_scenario, 
    test_suite    *p_test_suite
);
int test_case_test 
( 
    test_case     *p_test_case, 
    test_scenario *p_test_scenario, 
    test_suite    *p_test_suite 
);
