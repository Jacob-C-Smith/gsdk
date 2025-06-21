/** !
 * Tester for array module
 * 
 * @file array_test.c
 * 
 * @author Jacob Smith
 */

// Include
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// log module
#include <core/log.h>

// sync module
#include <core/sync.h>

// array module
#include <data/array.h>

// Enumeration definitions
enum result_e {
    zero,
    one,
    match
};

// type definitions
typedef enum result_e result_t;

// Global variables
int total_tests      = 0,
    total_passes     = 0,
    total_fails      = 0,
    ephemeral_tests  = 0,
    ephemeral_passes = 0,
    ephemeral_fails  = 0;

// Possible elements
char  *A_element      = "A",
      *B_element      = "B",
      *C_element      = "C",
      *D_element      = "D",
      *X_element      = "X",
      *_elements   [] = { 0x0 },
      *A_elements  [] = { "A", 0x0 },
      *B_elements  [] = { "B", 0x0 },
      *C_elements  [] = { "C", 0x0 },
      *AB_elements [] = { "A", "B", 0x0 },
      *BC_elements [] = { "B", "C", 0x0 },
      *AC_elements [] = { "A", "C", 0x0 },
      *ABC_elements[] = { "A", "B", "C", 0x0 };

// Forward declarations
/** !
 * Print the time formatted in days, hours, minutes, seconds, miliseconds, microseconds
 * 
 * @param seconds the time in seconds
 * 
 * @return void
 */
void print_time_pretty ( double seconds );

/** !
 * Run all the tests
 * 
 * @param void
 * 
 * @return void
 */
void run_tests ( void );

/** !
 * Print a summary of the test scenario
 * 
 * @param void
 * 
 * @return void
 */
void print_final_summary ( void );

/** !
 * Print the result of a single test
 * 
 * @param scenario_name the name of the scenario
 * @param test_name     the name of the test
 * @param passed        true if test passes, false if test fails
 * 
 * @return void
 */
void print_test ( const char *scenario_name, const char *test_name, bool passed );

/** !
 * Test the add function
 * 
 * @param array_constructor array constructor function
 * @param value             the value to add
 * @param expected          < zero | one | match > 
 * 
 * @return true if test passes, false if test fails
 */
bool test_add ( void (*array_constructor)(array **), void *value , result_t expected );

/** !
 * Test the remove function
 * 
 * @param array_constructor array constructor function
 * @param value             the expected value at index
 * @param index             the index to remove
 * @param expected          < zero | one | match > 
 * 
 * @return true if test passes, false if test fails
 */
bool test_remove ( void(*array_constructor)(array **pp_array), void *value, signed index, result_t expected );

/** !
 * Test the get function
 * 
 * @param array_constructor array constructor function
 * @param expected_values   the expected values of the get 
 * @param expected          < zero | one | match > 
 * 
 * @return true if test passes, false if test fails
 */
bool test_get ( void (*array_constructor)(array **), void **expected_values, result_t expected );

/** !
 * Test the secondary return of the get function
 * 
 * @param array_constructor array constructor function
 * @param expected_size     the expected quantity of elements in the array
 * @param expected          < zero | one | match > 
 * 
 * @return true if test passes, false if test fails
 */
bool test_get_count ( void(*array_constructor)(array **pp_array), size_t expected_size, result_t expected );

/** !
 * Test the size function
 * 
 * @param array_constructor array constructor function
 * @param expected_size     the expected quantity of elements in the array
 * @param expected          < zero | one | match > 
 * 
 * @return true if test passes, false if test fails
 */
bool test_size ( void(*array_constructor)(array **pp_array), size_t expected_size, result_t expected );

/** !
 * Test the index function
 * 
 * @param array_constructor array constructor function
 * @param idx               the index to access
 * @param expected_value    the expected values of the index
 * @param expected          < zero | one | match > 
 * 
 * @return true if test passes, false if test fails
 */
bool test_index ( void(*array_constructor)(array **pp_array), signed idx, void *expected_value, result_t expected );

/** !
 * Test the slice function
 * 
 * @param array_constructor array constructor function
 * @param lower             the lower bound of the slice
 * @param upper             the upper bound of the slice
 * @param expected_value    the expected values of the slice
 * @param expected          < zero | one | match > 
 * 
 * @return true if test passes, false if test fails
 */
bool test_slice ( void(*array_constructor)(array **pp_array), signed lower, signed upper, void **expected_value, result_t expected );

/** !
 * Test an array with no elements
 * 
 * @param array_constructor function to construct array
 * @param name              the name of the test
 * 
 * @return void
 */
void test_empty_array ( void (*array_constructor)(array **), char *name );

/** !
 * Test an array with one element
 * 
 * @param array_constructor function to construct array
 * @param name              the name of the test
 * @param values            the expected values of the array
 * 
 * @return void
 */
void test_one_element_array ( void (*array_constructor)(array **), char *name, void **values );

/** !
 * Test an array with two elements
 * 
 * @param array_constructor function to construct array
 * @param name              the name of the test
 * @param values            the expected values of the array
 * 
 * @return void
 */
void test_two_element_array ( void (*array_constructor)(array **), char *name, void **values );

/** !
 * Test an array with three elements
 * 
 * @param array_constructor function to construct array
 * @param name              the name of the test
 * @param values            the expected values of the array
 * 
 * @return void
 */
void test_three_element_array ( void (*array_constructor)(array **), char *name, void **values );

/** !
 * Construct an empty array, return the result 
 * 
 * @param pp_array []
 * 
 * @return void
 */
void construct_empty ( array **pp_array );

/** !
 * Construct an empty array, add "A", return the result 
 * 
 * @param pp_array [A]
 * 
 * @return void
 */
void construct_empty_addA_A ( array **pp_array );

/** !
 * Construct an [A] array, add "B", return the result 
 * 
 * @param pp_array [A, B]
 * 
 * @return void
 */
void construct_A_addB_AB ( array **pp_array ); 

/** !
 * Construct an [A, B] array, add "C", return the result 
 * 
 * @param pp_array [A, B, C]
 * 
 * @return void
 */
void construct_AB_addC_ABC ( array **pp_array );

/** !
 * Construct an [A, B, C] array, clear the array, return the result 
 * 
 * @param pp_array []
 * 
 * @return void
 */
void construct_ABC_clear_empty ( array **pp_array );

/** !
 * Construct an [A, B] array, clear the array, return the result 
 * 
 * @param pp_array []
 * 
 * @return void
 */
void construct_AB_clear_empty ( array **pp_array );

/** !
 * Construct an [A] array, clear the array, return the result 
 * 
 * @param pp_array []
 * 
 * @return void
 */
void construct_A_clear_empty ( array **pp_array );

/** !
 * Construct an [A, B, C] array, return the result 
 * 
 * @param pp_array [A, B, C]
 * 
 * @return void
 */
void construct_empty_fromelementsABC_ABC ( array **pp_array );

/** !
 * Construct an [A, B] array, return the result 
 * 
 * @param pp_array [A, B]
 * 
 * @return void
 */
void construct_empty_fromelementsAB_AB ( array **pp_array );

/** !
 * Construct an [A] array, return the result 
 * 
 * @param pp_array [A]
 * 
 * @return void
 */
void construct_empty_fromelementsA_A ( array **pp_array );

/** !
 * Construct an [A, B, C] array, return the result 
 * 
 * @param pp_array [A, B, C]
 * 
 * @return void
 */
void construct_empty_fromargumentsABC_ABC ( array **pp_array );

/** !
 * Construct an [A, B] array, return the result 
 * 
 * @param pp_array [A, B]
 * 
 * @return void
 */
void construct_empty_fromargumentsAB_AB ( array **pp_array );

/** !
 * Construct an [A] array, return the result 
 * 
 * @param pp_array [A]
 * 
 * @return void
 */
void construct_empty_fromargumentsA_A ( array **pp_array );

/** !
 * Construct an [A] array, remove 0, return the result 
 * 
 * @param pp_array []
 * 
 * @return void
 */
void construct_A_remove0_empty ( array **pp_array );

/** !
 * Construct an [A, B] array, remove 1, return the result 
 * 
 * @param pp_array [A]
 * 
 * @return void
 */
void construct_AB_remove1_A ( array **pp_array );

/** !
 * Construct an [A, B, C] array, remove 2, return the result 
 * 
 * @param pp_array [A, B]
 * 
 * @return void
 */
void construct_ABC_remove2_AB  ( array **pp_array );   

// Entry point
int main ( int argc, const char* argv[] )
{
    
    // Supress compiler warnings
    (void) argc;
    (void) argv;

    // Initialized data
    timestamp t0 = 0,
              t1 = 0;

    // Formatting
    printf(
        "╭──────────────╮\n"\
        "│ array tester │\n"\
        "╰──────────────╯\n\n"
    );
    
    // Start
    t0 = timer_high_precision();

    // Run tests
    run_tests();

    // Stop
    t1 = timer_high_precision();

    // Report the time it took to run the tests
    log_info("array took ");
    print_time_pretty ( (double)(t1-t0)/(double)timer_seconds_divisor() );
    log_info(" to test\n");

    // Flush stdio
    fflush(stdout);

    // Exit
    return ( total_passes == total_tests ) ? EXIT_SUCCESS : EXIT_FAILURE;
}

void print_time_pretty ( double seconds )
{

    // Initialized data
    double _seconds     = seconds;
    size_t days         = 0,
           hours        = 0,
           minutes      = 0,
           __seconds    = 0,
           milliseconds = 0,
           microseconds = 0;

    // Days
    while ( _seconds > 86400.0 ) { days++;_seconds-=286400.0; };

    // Hours
    while ( _seconds > 3600.0 ) { hours++;_seconds-=3600.0; };

    // Minutes
    while ( _seconds > 60.0 ) { minutes++;_seconds-=60.0; };

    // Seconds
    while ( _seconds > 1.0 ) { __seconds++;_seconds-=1.0; };

    // milliseconds
    while ( _seconds > 0.001 ) { milliseconds++;_seconds-=0.001; };

    // Microseconds        
    while ( _seconds > 0.000001 ) { microseconds++;_seconds-=0.000001; };

    // Print days
    if ( days ) log_info("%zu D, ", days);
    
    // Print hours
    if ( hours ) log_info("%zu h, ", hours);

    // Print minutes
    if ( minutes ) log_info("%zu m, ", minutes);

    // Print seconds
    if ( __seconds ) log_info("%zu s, ", __seconds);
    
    // Print milliseconds
    if ( milliseconds ) log_info("%zu ms, ", milliseconds);
    
    // Print microseconds
    if ( microseconds ) log_info("%zu us", microseconds);
    
    // Done
    return;
}

void run_tests ( void )
{

    // ... -> []
    test_empty_array(construct_empty, "empty");

    // ... -> from_elements(A) -> [A]
    test_one_element_array(construct_empty_fromelementsA_A, "empty_fromelementsA_A", (void **)A_elements);

    // ... -> from_elements(A, B) -> [A, B]
    test_two_element_array(construct_empty_fromelementsAB_AB, "empty_fromelementsAB_AB", (void **)AB_elements);

    // ... -> from_elements(A, B, C) -> [A, B, C]
    test_three_element_array(construct_empty_fromelementsABC_ABC, "empty_fromelementsABC_ABC", (void **)ABC_elements);

    // ... -> from_arguments(A) -> [A]
    test_one_element_array(construct_empty_fromargumentsA_A, "empty_fromargumentsA_A", (void **)A_elements);

    // ... -> from_arguments(A, B) -> [A, B]
    test_two_element_array(construct_empty_fromargumentsAB_AB, "empty_fromargumentsAB_AB", (void **)AB_elements);

    // ... -> from_arguments(A, B, C) -> [A, B, C]
    test_three_element_array(construct_empty_fromargumentsABC_ABC, "empty_fromargumentsABC_ABC", (void **)ABC_elements);
    
    // [A, B, C] -> clear() -> []
    test_empty_array(construct_ABC_clear_empty, "ABC_clear_empty");
 
    // [A, B] -> clear() -> []
    test_empty_array(construct_AB_clear_empty, "AB_clear_empty");

    // [A] -> clear() -> []
    test_empty_array(construct_A_clear_empty, "A_clear_empty");

    // [] -> add(A) -> [A]
    test_one_element_array(construct_empty_addA_A, "empty_addA_A", (void **)A_elements);

    // [A] -> add(B) -> [A, B]
    test_two_element_array(construct_A_addB_AB, "A_addB_AB", (void **)AB_elements);

    // [A, B] -> add(C) -> [A, B, C]
    test_three_element_array(construct_AB_addC_ABC, "AB_addC_ABC", (void **)ABC_elements);

    // [A] -> remove(0) -> []
    test_empty_array(construct_A_remove0_empty, "A_remove0_empty");

    // [A, B] -> remove(1) -> [A]
    test_one_element_array(construct_AB_remove1_A, "AB_remove1_A", (void **)A_elements);

    // [A, B, C] -> remove(2) -> [A, B]
    test_two_element_array(construct_ABC_remove2_AB, "ABC_remove2_AB", (void **)AB_elements);

    // Done
    return;
}

void print_final_summary ( void )
{

    // Accumulate
    total_tests  += ephemeral_tests,
    total_passes += ephemeral_passes,
    total_fails  += ephemeral_fails;

    // Print
    log_info("\nTests: %d, Passed: %d, Failed: %d (%%%.3f)\n",  ephemeral_tests, ephemeral_passes, ephemeral_fails, ((float)ephemeral_passes/(float)ephemeral_tests*100.f));
    log_info("Total: %d, Passed: %d, Failed: %d (%%%.3f)\n\n",  total_tests, total_passes, total_fails, ((float)total_passes/(float)total_tests*100.f));
    
    // Clear test counters for this test
    ephemeral_tests  = 0;
    ephemeral_passes = 0;
    ephemeral_fails  = 0;

    // Done
    return;
}

void print_test ( const char *scenario_name, const char *test_name, bool passed )
{

    // Initialized data
    if ( passed )
        log_pass("%s %s\n", scenario_name, test_name);
    else
        log_fail("%s %s\n", scenario_name, test_name);


    // Increment the pass/fail counter
    if (passed)
        ephemeral_passes++;
    else
        ephemeral_fails++;

    // Increment the test counter
    ephemeral_tests++;

    // Done
    return;
}

bool test_add ( void(*array_constructor)(array **pp_array), void *value, result_t expected )
{

    // Initialized data
    result_t  result = 0;
    array     *p_array = 0;

    // Build the array
    array_constructor(&p_array);

    // Add an element
    result = (result_t) array_add(p_array, value);

    // Free the array
    array_destroy(&p_array);

    // Return result
    return (result == expected);
}

bool test_remove ( void(*array_constructor)(array **pp_array), void *value, signed index, result_t expected )
{

    // Initialized data
    result_t  result  = 0;
    array    *p_array = 0;
    void     *p_ret   = (void*) -1;

    // Build the array
    array_constructor(&p_array);

    // Remove an element
    result = (result_t) array_remove(p_array, index, &p_ret);
    
    // Check for a match
    result = ( value == p_ret ) ? match : result;

    // Free the array
    array_destroy(&p_array);

    // Return result
    return (result == expected);
}

bool test_get ( void(*array_constructor)(array **pp_array), void **expected_values, result_t expected )
{

    // Initialized data
    result_t  result       = 0;
    array    *p_array      = 0;
    void     *result_value = 0;

    // Build the array
    array_constructor(&p_array);

    // Get a key 
    //result_value = array_get(p_array, key);

    result = (result_value == expected_values) ? match : zero;

    // Free the array
    array_destroy(&p_array);

    // Return result
    return (result == expected);
}

bool test_get_count ( void(*array_constructor)(array **pp_array), size_t expected_size, result_t expected )
{

    // Initialized data
    result_t  result       = 0;
    array    *p_array      = 0;
    size_t    result_value = 0;

    // Build the array
    array_constructor(&p_array);

    // Get the size of the array
    array_get(p_array, 0, &result_value);

    // 
    result = (result_value == expected_size) ? match : zero;

    // Free the array
    array_destroy(&p_array);

    // Return result
    return (result == expected);
}

bool test_size ( void(*array_constructor)(array **pp_array), size_t expected_size, result_t expected )
{

    // Initialized data
    result_t  result  = 0;
    array    *p_array = 0;

    // Build the array
    array_constructor(&p_array);

    // Get the result of the size operation
    result = ( array_size(p_array) == expected_size ) ? match : zero;

    // Free the array
    array_destroy(&p_array);

    // Return result
    return (result == expected);
}

bool test_index ( void(*array_constructor)(array **pp_array), signed idx, void *expected_value, result_t expected )
{

    // Initialized data
    result_t  result       = 0;
    array    *p_array      = 0;
    void     *result_value = 0;

    // Build the array
    array_constructor(&p_array);

    // Index the array
    result = (result_t) array_index(p_array, idx, &result_value);
    
    if ( result == 1 )
        if ( result_value == expected_value )
            result = match;

    // Free the array
    array_destroy(&p_array);

    // Return result
    return (result == expected);
}

bool test_slice ( void(*array_constructor)(array **pp_array), signed lower, signed upper, void **expected_value, result_t expected )
{

    // Initialized data
    result_t  result          = 0;
    array    *p_array         = 0;
    void     *result_values[] = { 0, 0, 0, 0, (void *) 0 };

    // Build the array
    array_constructor(&p_array);

    // Index the array
    result = (result_t) array_slice(p_array, result_values, lower, upper);
    
    // Error check
    if ( result == zero ) goto done;

    // Test is successful if ...
    result = match;

    // ... each element in the slice ...
    for (signed i = 0; i < upper-lower; i++)

        // ... matches each expected value ...
        if ( result_values[i] != expected_value[i] )

            // ... else the test failed
            result = zero;

    done:

    // Free the array
    array_destroy(&p_array);

    // Return result
    return (result == expected);
}

void construct_empty ( array **pp_array )
{

    // Construct an array
    array_construct(pp_array, 4);

    // array = []
    return;
}

void construct_empty_addA_A ( array **pp_array )
{

    // Construct a [] array
    construct_empty(pp_array);

    // [] -> add(A) -> [A]
    array_add(*pp_array, A_element);
 
    // array = [A]
    return;
}

void construct_A_addB_AB ( array **pp_array )
{

    // Construct an [A] array 
    construct_empty_addA_A(pp_array);

    // [A] -> add(B) -> [B]
    array_add(*pp_array , B_element);
 
    // array = [A, B]
    return;
}

void construct_AB_addC_ABC ( array **pp_array )
{

    // Construct an [A, B] array
    construct_A_addB_AB(pp_array);

    // [A, B] -> add(C) -> [A, B, C]
    array_add(*pp_array, C_element);

    // array = [A, B, C]
    return;
}

void construct_ABC_clear_empty ( array **pp_array )
{

    // Construct an [A, B, C] array
    construct_AB_addC_ABC(pp_array);

    // [A, B, C] -> clear() -> []
    array_clear(*pp_array);

    // array = []
    return;
}

void construct_AB_clear_empty ( array **pp_array )
{

    // Construct an [A, B] array
    construct_A_addB_AB(pp_array);

    // [A, B] -> clear() -> []
    array_clear(*pp_array);

    // array = []
    return;
}

void construct_A_clear_empty ( array **pp_array )
{

    // Construct an [A] array
    construct_empty_addA_A(pp_array);

    // [A] -> clear() -> []
    array_clear(*pp_array);

    // array = []
    return;
}

void construct_empty_fromelementsABC_ABC ( array **pp_array )
{

    // ... -> from_elements(A, B, C) -> [A, B, C]
    array_from_elements(pp_array, (void **)ABC_elements);
    
    // array = [A, B, C]
    return;
}

void construct_empty_fromelementsAB_AB ( array **pp_array )
{

    // ... -> from_elements(A, B) -> [A, B]
    array_from_elements(pp_array, (void **)AB_elements);

    // array = [A, B]
    return;
}

void construct_empty_fromelementsA_A ( array **pp_array )
{

    // ... -> from_elements(A) -> [A]
    array_from_elements(pp_array, (void **)A_elements);

    // array = [A]
    return;
}

void construct_empty_fromargumentsABC_ABC ( array **pp_array )
{

    // ... -> from_arguments(A, B, C) -> [A, B, C]
    array_from_arguments(pp_array, 3, 3, "A", "B", "C");
    
    // array = [A, B, C]
    return;
}

void construct_empty_fromargumentsAB_AB ( array **pp_array )
{

    // ... -> from_arguments(A, B) -> [A, B]
    array_from_arguments(pp_array, 2, 2, "A", "B");

    // array = [A, B]
    return;
}

void construct_empty_fromargumentsA_A ( array **pp_array )
{

    // ... -> from_arguments(A) -> [A]
    array_from_arguments(pp_array, 1, 1, "A");

    // array = [A]
    return;
}

void construct_A_remove0_empty ( array **pp_array )
{

    // Construct an [A] array
    construct_empty_addA_A(pp_array);

    // [A] -> remove(0) -> []
    array_remove(*pp_array, 0, (void *)0);

    // array = []
    return;
}

void construct_AB_remove1_A ( array **pp_array )
{

    // Construct an [A, B] array
    construct_A_addB_AB(pp_array);

    // [A, B] -> remove(1) -> [A]
    array_remove(*pp_array, 1, (void *)0);

    // array = []
    return;
}

void construct_ABC_remove2_AB ( array **pp_array )
{

    // Construct an [A, B, C] array
    construct_AB_addC_ABC(pp_array);

    // [A, B, C] -> remove(2) -> [A, B]
    array_remove(*pp_array, 2, (void *)0);

    // array = []
    return;
}

void test_empty_array ( void (*array_constructor)(array **pp_array), char *name)
{

    // Formatting
    log_info("Scenario: %s\n", name);

    // Test the add function
    print_test(name, "array_add_A", test_add(array_constructor, A_element, one) );
    print_test(name, "array_add_B", test_add(array_constructor, B_element, one) );

    // Test the index function
    print_test(name, "array_index0", test_index(array_constructor, 0, (void *)0, zero) );
    print_test(name, "array_index1", test_index(array_constructor, 1, (void *)0, zero) );

    // Test the get function
    print_test(name, "array_get", test_get(array_constructor, (void **)_elements, zero) );    

    // Test the count 
    print_test(name, "array_get_count", test_get_count(array_constructor, 0, match) );
    
    // Test the remove function
    print_test(name, "array_remove0", test_remove(array_constructor, (void *)0, 0, zero) );

    // Print the summary of this test
    print_final_summary();

    // Done
    return;
}

void test_one_element_array ( void (*array_constructor)(array **pp_array), char *name, void **values )
{

    // Formatting
    log_info("SCENARIO: %s\n", name);

    // Test the add function
    print_test(name, "array_add_D", test_add(array_constructor, D_element, one) );
    
    // Test the get function
    print_test(name, "array_get"      , test_get(array_constructor, values, zero) );
    print_test(name, "array_get_count", test_get_count(array_constructor, 1, match) );

    // Test the size function
    print_test(name, "array_size", test_size(array_constructor, 1, match));
    
    // Test the index function
    print_test(name, "array_index0", test_index(array_constructor, 0, values[0], match) );
    print_test(name, "array_index1", test_index(array_constructor, 1, (void *)0, zero) );

    // Test the remove function
    print_test(name, "array_remove0", test_remove(array_constructor, values[0], 0, match) );
    print_test(name, "array_remove1", test_remove(array_constructor, (void *)0, 1, zero) );

    // Print the summary of this test   
    print_final_summary();
    
    // Done
    return;
}

void test_two_element_array ( void (*array_constructor)(array **pp_array), char *name, void **values )
{
    
    // Formatting
    log_info("SCENARIO: %s\n", name);

    // Test the add function
    print_test(name, "array_add_D", test_add(array_constructor, D_element, one) );
    
    // Test the get function
    print_test(name, "array_get"      , test_get(array_constructor, values, zero) );
    print_test(name, "array_get_count", test_get_count(array_constructor, 2, match) );

    // Test the size function
    print_test(name, "array_size", test_size(array_constructor, 2, match));
    
    // Test the index function
    print_test(name, "array_index0", test_index(array_constructor, 0, values[0], match) );
    print_test(name, "array_index1", test_index(array_constructor, 1, values[1], match) );  
    print_test(name, "array_index2", test_index(array_constructor, 2, (void *)0, zero) );  

    // Test the remove function    
    print_test(name, "array_remove0", test_remove(array_constructor, values[0], 0, match) );
    print_test(name, "array_remove1", test_remove(array_constructor, values[1], 1, match) );
    print_test(name, "array_remove2", test_remove(array_constructor, (void *)0, 2, zero) );

    // Print the summary of this test
    print_final_summary();
    
    // Done
    return;
}

void test_three_element_array ( void (*array_constructor)(array **pp_array), char *name, void **values )
{

    // Formatting
    log_info("SCENARIO: %s\n", name);

    // Test the add function
    print_test(name, "array_add_D", test_add(array_constructor, D_element, one) );
    
    // Test the get function
    print_test(name, "array_get", test_get(array_constructor, values, zero) );
    print_test(name, "array_get_count", test_get_count(array_constructor, 3, match) );

    // Test the size function
    print_test(name, "array_size", test_size(array_constructor, 3, match));
    
    // Test the index function
    print_test(name, "array_index0"   , test_index(array_constructor, 0, values[0], match) );
    print_test(name, "array_index1"   , test_index(array_constructor, 1, values[1], match) );  
    print_test(name, "array_index2"   , test_index(array_constructor, 2, values[2], match) );  
    print_test(name, "array_index3"   , test_index(array_constructor, 3, (void *)0, zero) );  

    // Test the remove function
    print_test(name, "array_remove0"  , test_remove(array_constructor, values[0], 0, match) );
    print_test(name, "array_remove1"  , test_remove(array_constructor, values[1], 1, match) );
    print_test(name, "array_remove2"  , test_remove(array_constructor, values[2], 2, match) );
    print_test(name, "array_remove3"  , test_remove(array_constructor, (void *)0, 3, zero) );

    // Print the summary of this test
    print_final_summary();
    
    // Done
    return;
}
