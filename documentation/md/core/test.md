# [gsdk](../../../README.md) > [core](../core.md) > test

## Abstractions for testing

 > 1 [Example](#example)
 >
 > 2 [Tester](#tester)
 >
 > 3 [Definitions](#definitions)
 >
 >> 3.1 [Preprocessor macros](#preprocessor-macros)
 >>
 >> 3.2 [Enumeration definitions](#enumeration-definitions)
 >>
 >> 3.3 [Type definitions](#type-definitions)
 >>
 >> 3.4 [Function declarations](#function-declarations)

 ## Example
 There is no tester example, but there are a lot of [testers programs](../../../src/test/). I'd recommend looking there.

 ## Tester
 A tester for the test module is too meta for me. If you want to write a tester for the test module, go ahead and make a PR. All I ask is that you include a tester for the test tester... and a tester for the test tester tester... and a ...

 ## Definitions
 ### Preprocessor macros
 ```c
#define TEST_CASE(_name, _test_fn, _params, _expected) ...
#define TEST_MATCH(_name, _test_fn, _params, _match_fn) ...
#define TEST_ENV(_name, _test_fn, _params, _env, _expected) ...
#define TEST_SCENARIO(_name, _data, _cases_array, _constructor, _destructor) ...
#define TEST_SUITE(_name, _scenarios_array) ...
 ```

 ### Enumeration definitions
 ```c
// enumeration definitions
enum test_result_e
{
    TEST_RESULT_ZERO,
    TEST_RESULT_ONE,
    TEST_RESULT_MATCH,
    TEST_RESULT_QUANTITY,
};
 ```

 ### Type definitions
 ```c
// type definitions
typedef struct test_suite_s    test_suite;
typedef struct test_scenario_s test_scenario;
typedef struct test_case_s     test_case;

typedef void *(fn_test_case)            ( test_case *p_test_case, void *p_subject );
typedef int   (fn_scenario_constructor) ( void **pp_result );
typedef int   (fn_scenario)             ( fn_scenario_constructor *pfn_scenario_constructor, void *p_env );
typedef bool  (fn_results_match)        ( test_scenario *p_scenario, test_case *p_case, void *p_subject, void *p_result );
 ```

 ### Function declarations
 ```c 
// function declarations
/// test
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
 ```
