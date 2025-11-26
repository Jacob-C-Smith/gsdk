/** !
 * Tester for set module
 * 
 * @file set_test.c
 * 
 * @author Jacob Smith
 */

// Include
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// gsdk
/// core
#include <core/log.h>
#include <data/set.h>

// enumeration definitions
enum result_e {
    zero,
    one,
    match
};

// type definitions
typedef enum result_e result_t;

// global variables
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

// forward declarations
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
 * @param set_constructor set constructor function
 * @param value             the value to add
 * @param expected          < zero | one | match > 
 * 
 * @return true if test passes, false if test fails
 */
bool test_add ( void (*set_constructor)(set **), void *value , result_t expected );

/** !
 * Test the count function
 * 
 * @param set_constructor set constructor function
 * @param count           expected size
 * @param expected        < zero | match > 
 * 
 * @return true if test passes, false if test fails
 */
bool test_count ( void(*set_constructor)(set **pp_set), size_t count, result_t expected );

bool test_union ( void(*set_constructor)(set **pp_set), void(*set_constructor2)(set **pp_set), void **values1, void **values2, result_t expected );

bool test_intersection ( void(*set_constructor)(set **pp_set), void(*set_constructor2)(set **pp_set), void **values1, void **values2, result_t expected );

bool test_difference( void(*set_constructor)(set **pp_set), void(*set_constructor2)(set **pp_set), void **values1, void **values2, result_t expected );

/** !
 * Test an set with no elements
 * 
 * @param set_constructor function to construct set
 * @param name              the name of the test
 * 
 * @return void
 */
void test_empty_set ( void (*set_constructor)(set **), char *name );

/** !
 * Test an set with one element
 * 
 * @param set_constructor function to construct set
 * @param name              the name of the test
 * @param values            the expected values of the set
 * 
 * @return void
 */
void test_one_element_set ( void (*set_constructor)(set **), char *name, void **values );

/** !
 * Test an set with two elements
 * 
 * @param set_constructor function to construct set
 * @param name              the name of the test
 * @param values            the expected values of the set
 * 
 * @return void
 */
void test_two_element_set ( void (*set_constructor)(set **), char *name, void **values );

/** !
 * Test an set with three elements
 * 
 * @param set_constructor function to construct set
 * @param name              the name of the test
 * @param values            the expected values of the set
 * 
 * @return void
 */
void test_three_element_set ( void (*set_constructor)(set **), char *name, void **values );

/** !
 * Construct an empty set, return the result 
 * 
 * @param pp_set { }
 * 
 * @return void
 */
void construct_empty ( set **pp_set );

/** !
 * Construct { A } set, remove "A", return the result 
 * 
 * @param pp_set { }
 * 
 * @return void
 */
void construct_A_removeA_empty ( set **pp_set );

/** !
 * Construct { B } set, remove "B", return the result 
 * 
 * @param pp_set { }
 * 
 * @return void
 */
void construct_B_removeB_empty ( set **pp_set );

/** !
 * Construct { C } set, remove "C", return the result 
 * 
 * @param pp_set { }
 * 
 * @return void
 */
void construct_C_removeC_empty ( set **pp_set );

/** !
 * Construct an empty set, add "A", return the result 
 * 
 * @param pp_set { }
 * 
 * @return void
 */
void construct_empty_addA_A ( set **pp_set );

/** !
 * Construct an empty set, add "B", return the result 
 * 
 * @param pp_set { B }
 * 
 * @return void
 */
void construct_empty_addB_B ( set **pp_set );

/** !
 * Construct an empty set, add "C", return the result 
 * 
 * @param pp_set { C }
 * 
 * @return void
 */
void construct_empty_addC_C ( set **pp_set );

void construct_AB_removeB_A ( set **pp_set );

void construct_AB_removeA_B ( set **pp_set );

void construct_AC_removeC_A ( set **pp_set );

void construct_AC_removeA_C ( set **pp_set );

void construct_BC_removeB_C ( set **pp_set );

void construct_BC_removeC_B ( set **pp_set );

void construct_ABC_removeA_BC ( set **pp_set );

/** !
 * Construct an { A } set, add { B }, return the result 
 * 
 * @param pp_set { A, B }
 * 
 * @return void
 */
void construct_A_addB_AB ( set **pp_set ); 

/** !
 * Construct an { A } set, add { C }, return the result 
 * 
 * @param pp_set { A, C }
 * 
 * @return void
 */
void construct_A_addC_AC ( set **pp_set ); 

void construct_B_addA_AB ( set **pp_set );

void construct_C_addB_BC ( set **pp_set );

void construct_C_addA_AC ( set **pp_set );
void construct_ABC_removeB_AC ( set **pp_set );
/** !
 * Construct an { B } set, add { C }, return the result 
 * 
 * @param pp_set { B, C }
 * 
 * @return void
 */
void construct_B_addC_BC ( set **pp_set ); 

/** !
 * Construct an {A, B} set, add "C", return the result 
 * 
 * @param pp_set {A, B, C}
 * 
 * @return void
 */
void construct_AB_addC_ABC ( set **pp_set );

void construct_AC_addB_ABC ( set **pp_set );

void construct_BC_addA_ABC ( set **pp_set );

/** !
 * Construct an {A, B, C} set, clear the set, return the result 
 * 
 * @param pp_set { }
 * 
 * @return void
 */
void construct_ABC_clear_empty ( set **pp_set );

/** !
 * Construct an {A, B} set, clear the set, return the result 
 * 
 * @param pp_set { }
 * 
 * @return void
 */
void construct_AB_clear_empty ( set **pp_set );

/** !
 * Construct an {A} set, clear the set, return the result 
 * 
 * @param pp_set {}
 * 
 * @return void
 */
void construct_A_clear_empty ( set **pp_set );

/** !
 * Construct an {A, B, C} set, return the result 
 * 
 * @param pp_set {A, B, C}
 * 
 * @return void
 */
void construct_empty_fromelementsABC_ABC ( set **pp_set );

/** !
 * Construct an {A, B} set, return the result 
 * 
 * @param pp_set {A, B}
 * 
 * @return void
 */
void construct_empty_fromelementsAB_AB ( set **pp_set );

/** !
 * Construct an {A} set, return the result 
 * 
 * @param pp_set {A}
 * 
 * @return void
 */
void construct_empty_fromelementsA_A ( set **pp_set );

/** !
 * Construct an {A, B, C} set, return the result 
 * 
 * @param pp_set {A, B, C}
 * 
 * @return void
 */
void construct_empty_fromargumentsABC_ABC ( set **pp_set );

/** !
 * Construct an {A, B} set, return the result 
 * 
 * @param pp_set {A, B}
 * 
 * @return void
 */
void construct_empty_fromargumentsAB_AB ( set **pp_set );

/** !
 * Construct an {A} set, return the result 
 * 
 * @param pp_set {A}
 * 
 * @return void
 */
void construct_empty_fromargumentsA_A ( set **pp_set );

/** !
 * Construct an {A} set, remove 0, return the result 
 * 
 * @param pp_set {}
 * 
 * @return void
 */
void construct_A_remove0_empty ( set **pp_set );

/** !
 * Construct an {A, B} set, remove 1, return the result 
 * 
 * @param pp_set {A}
 * 
 * @return void
 */
void construct_AB_remove1_A ( set **pp_set );

/** !
 * Construct an {A, B, C} set, remove 2, return the result 
 * 
 * @param pp_set {A, B}
 * 
 * @return void
 */
void construct_ABC_remove2_AB  ( set **pp_set );   

// entry point
int main ( int argc, const char* argv[] )
{
    
    // unused
    (void) argc;
    (void) argv;

    // initialized data
    timestamp t0 = 0,
              t1 = 0;

    // Formatting
    printf(
        "╭────────────╮\n"\
        "│ set tester │\n"\
        "╰────────────╯\n\n"
    );
    
    // Start
    t0 = timer_high_precision();

    // Run tests
    run_tests();

    // Stop
    t1 = timer_high_precision();

    // Report the time it took to run the tests
    log_info("set took ");
    print_time_pretty ( (double)(t1-t0)/(double)timer_seconds_divisor() );
    log_info(" to test\n");

    // Flush stdio
    fflush(stdout);

    // exit
    return ( total_passes == total_tests ) ? EXIT_SUCCESS : EXIT_FAILURE;
}

void print_time_pretty ( double seconds )
{

    // initialized data
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
    
    // done
    return;
}

void run_tests ( void )
{

    // ... -> { }
    test_empty_set(construct_empty, "... -> { }");

    // { A } -> { }
    test_empty_set(construct_A_removeA_empty, "{ A } -> { }");

    // { B } -> { }
    test_empty_set(construct_B_removeB_empty, "{ B } -> { }");

    // { C } -> { }
    test_empty_set(construct_C_removeC_empty, "{ C } -> { }");

    // { } -> { A }
    test_one_element_set(construct_empty_addA_A, "{ } -> { A }", A_elements);
    
    // { } -> { B }
    test_one_element_set(construct_empty_addB_B, "{ } -> { B }", B_elements);
    
    // { } -> { C }
    test_one_element_set(construct_empty_addC_C, "{ } -> { C }", C_elements);
    
    // { A, B } -> { A }
    test_one_element_set(construct_AB_removeB_A, "{ A, B } -> { A }", A_elements);

    // { A, B } -> { B }
    test_one_element_set(construct_AB_removeA_B, "{ A, B } -> { B }", B_elements);

    // { A, C } -> { A }
    test_one_element_set(construct_AC_removeC_A, "{ A, C } -> { A }", A_elements);

    // { A, C } -> { C }
    test_one_element_set(construct_AC_removeA_C, "{ A, C } -> { C }", C_elements);

    // { B, C } -> { C }
    test_one_element_set(construct_BC_removeB_C, "{ B, C } -> { C }", C_elements);

    // { B, C } -> { B }
    test_one_element_set(construct_BC_removeC_B, "{ B, C } -> { B }", B_elements);

    // { A } -> { A, B }
    test_two_element_set(construct_A_addB_AB, "{ A } -> { A, B }", AB_elements);

    // { A } -> { A, C }
    test_two_element_set(construct_A_addC_AC, "{ A } -> { A, C }", AC_elements);
    
    // { B } -> { A, B }
    test_two_element_set(construct_B_addA_AB, "{ B } -> { A, B }", AB_elements);
    
    // { B } -> { B, C }
    test_two_element_set(construct_B_addC_BC, "{ B } -> { B, C }", BC_elements);
    
    // { C } -> { B, C }
    test_two_element_set(construct_C_addB_BC, "{ C } -> { B, C }", BC_elements);
    
    // { C } -> { A, C }
    test_two_element_set(construct_C_addA_AC, "{ C } -> { A, C }", BC_elements);

    // { A, B, C } -> { A, B }
    test_two_element_set(construct_C_addA_AC, "{ A, B, C } -> { A, B }", AB_elements);

    // { A, B, C } -> { A, C }
    test_two_element_set(construct_C_addA_AC, "{ A, B, C } -> { A, C }", AC_elements);

    // { A, B, C } -> { B, C }
    test_two_element_set(construct_C_addA_AC, "{ A, B, C } -> { B, C }", BC_elements);

    // { A, B } -> { A, B, C }
    test_three_element_set(construct_AB_addC_ABC, "{ A, B } -> { A, B, C }", ABC_elements);

    // { A, C } -> { A, B, C }
    test_three_element_set(construct_AC_addB_ABC, "{ A, C } -> { A, B, C }", ABC_elements);

    // { B, C } -> { A, B, C }
    test_three_element_set(construct_BC_addA_ABC, "{ B, C } -> { A, B, C }", ABC_elements);

    // done
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

    // done
    return;
}

void print_test ( const char *scenario_name, const char *test_name, bool passed )
{

    // initialized data
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

    // done
    return;
}

void test_empty_set ( void (*set_constructor)(set **), char *name )
{
    
    // Log
    log_scenario("%s\n", name);

    // Add A
    print_test(name, "add A", test_add(set_constructor, "A", one));
    print_test(name, "add B", test_add(set_constructor, "B", one));
    print_test(name, "add C", test_add(set_constructor, "C", one));

    // Count
    print_test(name, "count", test_count(set_constructor, 0, match));

    // Union
    print_test(name, "∪ { A }", test_union(set_constructor, construct_empty_addA_A, _elements, A_elements, match));
    print_test(name, "∪ { B }", test_union(set_constructor, construct_empty_addB_B, _elements, B_elements, match));
    print_test(name, "∪ { C }", test_union(set_constructor, construct_empty_addC_C, _elements, C_elements, match));
    print_test(name, "∪ { A, B }", test_union(set_constructor, construct_A_addB_AB, _elements, AB_elements, match));
    print_test(name, "∪ { A, C }", test_union(set_constructor, construct_A_addC_AC, _elements, AC_elements, match));
    print_test(name, "∪ { B, C }", test_union(set_constructor, construct_B_addC_BC, _elements, BC_elements, match));
    print_test(name, "∪ { A, B, C }", test_union(set_constructor, construct_AB_addC_ABC, _elements, ABC_elements, match));

    // Intersection
    print_test(name, "∩ { A }", test_intersection(set_constructor, construct_empty_addA_A, _elements, _elements, match));
    print_test(name, "∩ { B }", test_intersection(set_constructor, construct_empty_addB_B, _elements, _elements, match));
    print_test(name, "∩ { C }", test_intersection(set_constructor, construct_empty_addC_C, _elements, _elements, match));
    print_test(name, "∩ { A, B }", test_intersection(set_constructor, construct_A_addB_AB, _elements, _elements, match));
    print_test(name, "∩ { A, C }", test_intersection(set_constructor, construct_A_addC_AC, _elements, _elements, match));
    print_test(name, "∩ { B, C }", test_intersection(set_constructor, construct_B_addC_BC, _elements, _elements, match));
    print_test(name, "∩ { A, B, C }", test_intersection(set_constructor, construct_AB_addC_ABC, _elements, _elements, match));

    // Difference
    print_test(name, "Δ { A }", test_difference(set_constructor, construct_empty_addA_A, _elements, A_elements, match));
    print_test(name, "Δ { B }", test_difference(set_constructor, construct_empty_addB_B, _elements, B_elements, match));
    print_test(name, "Δ { C }", test_difference(set_constructor, construct_empty_addC_C, _elements, C_elements, match));
    print_test(name, "Δ { A, B }", test_difference(set_constructor, construct_A_addB_AB, _elements, AB_elements, match));
    print_test(name, "Δ { A, C }", test_difference(set_constructor, construct_A_addC_AC, _elements, AC_elements, match));
    print_test(name, "Δ { B, C }", test_difference(set_constructor, construct_B_addC_BC, _elements, BC_elements, match));
    print_test(name, "Δ { A, B, C }", test_difference(set_constructor, construct_AB_addC_ABC, _elements, ABC_elements, match));

    // Is subset?
    //print_test(name, "⊂ { A }", test_subset(set_constructor, construct_empty_addA_A));
    //print_test(name, "⊂ { B }"
    //print_test(name, "⊂ { C }"   
    //print_test(name, "⊂ { A, B }"
    //print_test(name, "⊂ { A, C }"
    //print_test(name, "⊂ { B, C }"
    //print_test(name, "⊂ { A, B, C }"

    // Is superset?
    // TODO: 
    //print_test(name, "⊃ { A }", test_subset(set_constructor, construct_empty_addA_A));
    //print_test(name, "⊃ { B }");
    //print_test(name, "⊃ { C }");    
    //print_test(name, "⊃ { A, B }", test_difference(set_constructor, construct_A_addB_AB, _elements, AB_elements, match));
    //print_test(name, "⊃ { A, C }", test_difference(set_constructor, construct_A_addC_AC, _elements, AC_elements, match));
    //print_test(name, "⊃ { B, C }", test_difference(set_constructor, construct_B_addC_BC, _elements, BC_elements, match));
    //print_test(name, "⊃ { A, B, C }", test_difference(set_constructor, construct_AB_addC_ABC, _elements, ABC_elements, match));

    // Print the final summary
    print_final_summary();
    
    // done
    return;
}

void test_one_element_set ( void (*set_constructor)(set **), char *name, void **values )
{
    
    // Log
    log_scenario("%s\n", name);

    // Add
    print_test(name, "add A", test_add(set_constructor, "A", one));
    print_test(name, "add B", test_add(set_constructor, "B", one));
    print_test(name, "add C", test_add(set_constructor, "C", one));

    // Count
    print_test(name, "count", test_count(set_constructor, 1, match));
    
    // Union
    print_test(name, "∪ { A }", test_union(set_constructor, construct_empty_addA_A, values, A_elements, match));
    print_test(name, "∪ { B }", test_union(set_constructor, construct_empty_addB_B, values, B_elements, match));
    print_test(name, "∪ { C }", test_union(set_constructor, construct_empty_addC_C, values, C_elements, match));
    print_test(name, "∪ { A, B }", test_union(set_constructor, construct_A_addB_AB, values, AB_elements, match));
    print_test(name, "∪ { A, C }", test_union(set_constructor, construct_A_addC_AC, values, AC_elements, match));
    print_test(name, "∪ { B, C }", test_union(set_constructor, construct_B_addC_BC, values, BC_elements, match));
    print_test(name, "∪ { A, B, C }", test_union(set_constructor, construct_AB_addC_ABC, values, ABC_elements, match));

    // Intersection
    print_test(name, "∩ { A }", test_intersection(set_constructor, construct_empty_addA_A, values, _elements, match));
    print_test(name, "∩ { B }", test_intersection(set_constructor, construct_empty_addB_B, values, _elements, match));
    print_test(name, "∩ { C }", test_intersection(set_constructor, construct_empty_addC_C, values, _elements, match));
    print_test(name, "∩ { A, B }", test_intersection(set_constructor, construct_A_addB_AB, values, _elements, match));
    print_test(name, "∩ { A, C }", test_intersection(set_constructor, construct_A_addC_AC, values, _elements, match));
    print_test(name, "∩ { B, C }", test_intersection(set_constructor, construct_B_addC_BC, values, _elements, match));
    print_test(name, "∩ { A, B, C }", test_intersection(set_constructor, construct_AB_addC_ABC, values, _elements, match));

    // Difference
    //print_test(name, "Δ { A }", test_difference(set_constructor, construct_empty_addA_A, values, A_elements, match));
    //print_test(name, "Δ { B }", test_difference(set_constructor, construct_empty_addB_B, values, B_elements, match));
    //print_test(name, "Δ { C }", test_difference(set_constructor, construct_empty_addC_C, values, C_elements, match));
    //print_test(name, "Δ { A, B }", test_difference(set_constructor, construct_A_addB_AB, values, AB_elements, match));
    //print_test(name, "Δ { A, C }", test_difference(set_constructor, construct_A_addC_AC, values, AC_elements, match));
    //print_test(name, "Δ { B, C }", test_difference(set_constructor, construct_B_addC_BC, values, BC_elements, match));
    //print_test(name, "Δ { A, B, C }", test_difference(set_constructor, construct_AB_addC_ABC, values, ABC_elements, match));

    // Is subset?
    // Is superset?

    // Print the final summary
    print_final_summary();
    
    // done
    return;
}

void test_two_element_set ( void (*set_constructor)(set **), char *name, void **values )
{
    
    // Log
    log_scenario("%s\n", name);

    // Add
    print_test(name, "add A", test_add(set_constructor, "A", one));
    print_test(name, "add B", test_add(set_constructor, "B", one));
    print_test(name, "add C", test_add(set_constructor, "C", one));

    // Count
    print_test(name, "count", test_count(set_constructor, 2, match));
    
    // Union
    print_test(name, "∪ { A }", test_union(set_constructor, construct_empty_addA_A, values, A_elements, match));
    print_test(name, "∪ { B }", test_union(set_constructor, construct_empty_addB_B, values, B_elements, match));
    print_test(name, "∪ { C }", test_union(set_constructor, construct_empty_addC_C, values, C_elements, match));
    print_test(name, "∪ { A, B }", test_union(set_constructor, construct_A_addB_AB, values, AB_elements, match));
    print_test(name, "∪ { A, C }", test_union(set_constructor, construct_A_addC_AC, values, AC_elements, match));
    print_test(name, "∪ { B, C }", test_union(set_constructor, construct_B_addC_BC, values, BC_elements, match));
    print_test(name, "∪ { A, B, C }", test_union(set_constructor, construct_AB_addC_ABC, values, ABC_elements, match));

    // Intersection
    print_test(name, "∩ { A }", test_intersection(set_constructor, construct_empty_addA_A, values, _elements, match));
    print_test(name, "∩ { B }", test_intersection(set_constructor, construct_empty_addB_B, values, _elements, match));
    print_test(name, "∩ { C }", test_intersection(set_constructor, construct_empty_addC_C, values, _elements, match));
    print_test(name, "∩ { A, B }", test_intersection(set_constructor, construct_A_addB_AB, values, _elements, match));
    print_test(name, "∩ { A, C }", test_intersection(set_constructor, construct_A_addC_AC, values, _elements, match));
    print_test(name, "∩ { B, C }", test_intersection(set_constructor, construct_B_addC_BC, values, _elements, match));
    print_test(name, "∩ { A, B, C }", test_intersection(set_constructor, construct_AB_addC_ABC, values, _elements, match));

    // Difference
    //print_test(name, "Δ { A }", test_difference(set_constructor, construct_empty_addA_A, values, A_elements, match));
    //print_test(name, "Δ { B }", test_difference(set_constructor, construct_empty_addB_B, values, B_elements, match));
    //print_test(name, "Δ { C }", test_difference(set_constructor, construct_empty_addC_C, values, C_elements, match));
    //print_test(name, "Δ { A, B }", test_difference(set_constructor, construct_A_addB_AB, values, AB_elements, match));
    //print_test(name, "Δ { A, C }", test_difference(set_constructor, construct_A_addC_AC, values, AC_elements, match));
    //print_test(name, "Δ { B, C }", test_difference(set_constructor, construct_B_addC_BC, values, BC_elements, match));
    //print_test(name, "Δ { A, B, C }", test_difference(set_constructor, construct_AB_addC_ABC, values, ABC_elements, match));

    // Is subset?
    // Is superset?

    // Print the final summary
    print_final_summary();
    
    // done
    return;
}

void test_three_element_set ( void (*set_constructor)(set **), char *name, void **values )
{
    
    // Log
    log_scenario("%s\n", name);

    // Add
    print_test(name, "add A", test_add(set_constructor, "A", one));
    print_test(name, "add B", test_add(set_constructor, "B", one));
    print_test(name, "add C", test_add(set_constructor, "C", one));

    // Count
    print_test(name, "count", test_count(set_constructor, 3, match));
    
    // Union
    print_test(name, "∪ { A }", test_union(set_constructor, construct_empty_addA_A, values, A_elements, match));
    print_test(name, "∪ { B }", test_union(set_constructor, construct_empty_addB_B, values, B_elements, match));
    print_test(name, "∪ { C }", test_union(set_constructor, construct_empty_addC_C, values, C_elements, match));
    print_test(name, "∪ { A, B }", test_union(set_constructor, construct_A_addB_AB, values, AB_elements, match));
    print_test(name, "∪ { A, C }", test_union(set_constructor, construct_A_addC_AC, values, AC_elements, match));
    print_test(name, "∪ { B, C }", test_union(set_constructor, construct_B_addC_BC, values, BC_elements, match));
    print_test(name, "∪ { A, B, C }", test_union(set_constructor, construct_AB_addC_ABC, values, ABC_elements, match));

    // Intersection
    print_test(name, "∩ { A }", test_intersection(set_constructor, construct_empty_addA_A, values, _elements, match));
    print_test(name, "∩ { B }", test_intersection(set_constructor, construct_empty_addB_B, values, _elements, match));
    print_test(name, "∩ { C }", test_intersection(set_constructor, construct_empty_addC_C, values, _elements, match));
    print_test(name, "∩ { A, B }", test_intersection(set_constructor, construct_A_addB_AB, values, _elements, match));
    print_test(name, "∩ { A, C }", test_intersection(set_constructor, construct_A_addC_AC, values, _elements, match));
    print_test(name, "∩ { B, C }", test_intersection(set_constructor, construct_B_addC_BC, values, _elements, match));
    print_test(name, "∩ { A, B, C }", test_intersection(set_constructor, construct_AB_addC_ABC, values, _elements, match));

    // Difference
    // print_test(name, "Δ { A }", test_difference(set_constructor, construct_empty_addA_A, values, A_elements, match));
    // print_test(name, "Δ { B }", test_difference(set_constructor, construct_empty_addB_B, values, B_elements, match));
    // print_test(name, "Δ { C }", test_difference(set_constructor, construct_empty_addC_C, values, C_elements, match));
    // print_test(name, "Δ { A, B }", test_difference(set_constructor, construct_A_addB_AB, values, AB_elements, match));
    // print_test(name, "Δ { A, C }", test_difference(set_constructor, construct_A_addC_AC, values, AC_elements, match));
    // print_test(name, "Δ { B, C }", test_difference(set_constructor, construct_B_addC_BC, values, BC_elements, match));
    // print_test(name, "Δ { A, B, C }", test_difference(set_constructor, construct_AB_addC_ABC, values, ABC_elements, match));

    // Is subset?
    // Is superset?

    // Print the final summary
    print_final_summary();
    
    // done
    return;
}

bool test_add ( void(*set_constructor)(set **pp_set), void *value, result_t expected )
{

    // initialized data
    result_t  result = 0;
    set     *p_set = 0;

    // Build the set
    set_constructor(&p_set);

    // Add an element
    result = (result_t) set_add(p_set, value);

    // Free the set
    set_destroy(&p_set);

    // return result
    return (result == expected);
}

bool test_count ( void(*set_constructor)(set **pp_set), size_t count, result_t expected )
{

    // initialized data
    result_t  result = 0;
    set      *p_set  = 0;

    // Build the set
    set_constructor(&p_set);

    // Add an element
    result = (result_t) (set_count(p_set) == count) ? match : zero;

    // Free the set
    set_destroy(&p_set);

    // return result
    return (result == expected);
}

bool test_union ( void(*set_constructor)(set **pp_set), void(*set_constructor2)(set **pp_set), void **values1, void **values2, result_t expected )
{

    // initialized data
    result_t  result = 0;
    set      *p_set1 = 0;
    set      *p_set2 = 0;
    set      *p_set  = 0;
    size_t    count = 0, 
              count1 = 0, 
              count2 = 0;
    void     *contents[4] = { 0 };
    void     *contents1[4] = { 0 };
    void     *contents2[4] = { 0 };
    bool      found = false;

    // Build the set
    set_constructor(&p_set1);
    set_constructor2(&p_set2);

    // Compute the union of set1 and set2
    set_union(&p_set, p_set1, p_set2);

    // Get the contents of the set
    set_contents(p_set, contents);
    set_contents(p_set1, contents1);
    set_contents(p_set2, contents2);

    // Get the quantity of elements in the set
    count  = set_contents(p_set, 0);
    count1 = set_contents(p_set1, 0);
    count2 = set_contents(p_set2, 0);

    // Check each value
    for (size_t i = 0; i < count; i++)
    {
        found = false;

        for (size_t j = 0; j < count1; j++)
        {
            found = ( strcmp( contents[i], contents1[j] ) == 0 );

            if ( found == true ) break;
            
        }
        
        if ( found == true ) continue;

        for (size_t j = 0; j < count2; j++)
        {
            found = ( strcmp( contents[i], contents2[j] ) == 0 );

            if ( found == true ) break;
            
        }
        
        if ( found == false ) break;
    }
    
    result = (found) ? match : zero;

    // Free the set
    set_destroy(&p_set1);
    set_destroy(&p_set2);
    set_destroy(&p_set);

    // return result
    return (result == expected);
}

bool test_intersection ( void(*set_constructor)(set **pp_set), void(*set_constructor2)(set **pp_set), void **values1, void **values2, result_t expected )
{

    // initialized data
    result_t  result = 0;
    set      *p_set1 = 0;
    set      *p_set2 = 0;
    set      *p_set  = 0;
    size_t    count = 0, 
              count1 = 0, 
              count2 = 0;
    void     *contents[4] = { 0 };
    void     *contents1[4] = { 0 };
    void     *contents2[4] = { 0 };
    bool      found = false;

    // Build the set
    set_constructor(&p_set1);
    set_constructor2(&p_set2);

    // Compute the union of set1 and set2
    set_intersection(&p_set, p_set1, p_set2);

    // Get the contents of the set
    set_contents(p_set, contents);
    set_contents(p_set1, contents1);
    set_contents(p_set2, contents2);

    // Get the quantity of elements in the set
    count  = set_contents(p_set, 0);
    count1 = set_contents(p_set1, 0);
    count2 = set_contents(p_set2, 0);

    // Check each value in set 1
    for (size_t j = 0; j < count1; j++)
    {

        // Check each value in set 2
        for (size_t k = 0; k < count2; k++)
        {
            
            // If A == B ...
            if ( strcmp(contents1[j], contents2[k]) == 0 )
            {

                for (size_t i = 0; i < count; i++)
                {
                    // ... and it is in the intersection
                    if ( strcmp(contents1[j], contents[i]) != 0 )
                    {
                        result |= 1;
                    }
                }                
            }
        }
    }
    
    result = (found == false) ? match : zero;

    // Free the set
    set_destroy(&p_set1);
    set_destroy(&p_set2);
    set_destroy(&p_set);

    // return result
    return (result == expected);
}

bool test_difference ( void(*set_constructor)(set **pp_set), void(*set_constructor2)(set **pp_set), void **values1, void **values2, result_t expected )
{
    
    // initialized data
    result_t  result = 0;
    set      *p_set1 = 0;
    set      *p_set2 = 0;
    set      *p_set  = 0;
    size_t    count = 0, 
              count1 = 0, 
              count2 = 0;
    void     *contents[4] = { 0 };
    void     *contents1[4] = { 0 };
    void     *contents2[4] = { 0 };
    bool      found = false;

    // Build the set
    set_constructor(&p_set1);
    set_constructor2(&p_set2);

    // Compute the union of set1 and set2
    set_difference(&p_set, p_set1, p_set2);

    // Get the contents of the set
    set_contents(p_set, contents);
    set_contents(p_set1, contents1);
    set_contents(p_set2, contents2);

    // Get the quantity of elements in the set
    count  = set_contents(p_set, 0);
    count1 = set_contents(p_set1, 0);
    count2 = set_contents(p_set2, 0);

    // Check each value
    for (size_t i = 0; i < count; i++)
    {
        found = false;

        for (size_t j = 0; j < count1; j++)
        {
            found = ( strcmp( contents[i], contents1[j] ) == 0 );

            if ( found == true ) break;
            
        }
        
        if ( found == true ) continue;

        for (size_t j = 0; j < count2; j++)
        {
            found = ( strcmp( contents[i], contents2[j] ) == 0 );

            if ( found == true ) break;
            
        }
        
        if ( found == false ) break;
    }
    
    result = (found) ? match : zero;

    // Free the set
    set_destroy(&p_set1);
    set_destroy(&p_set2);
    set_destroy(&p_set);

    // return result
    return (result == expected);
}

void construct_empty ( set **pp_set )
{

    // Construct the set
    set_construct(pp_set, 4, (fn_equality*)strcmp);

    // done
    return;
}

void construct_empty_addA_A ( set **pp_set )
{

    // Construct the set
    construct_empty(pp_set);

    // Add A
    set_add(*pp_set, A_element);

    // done
    return;
}

void construct_empty_addB_B ( set **pp_set )
{

    // Construct the set
    construct_empty(pp_set);

    // Add B
    set_add(*pp_set, B_element);

    // done
    return;
}

void construct_empty_addC_C ( set **pp_set )
{

    // Construct the set
    construct_empty(pp_set);

    // Add C
    set_add(*pp_set, C_element);

    // done
    return;
}

void construct_AB_removeB_A ( set **pp_set )
{

    // Construct { A, B }
    construct_A_addB_AB(pp_set);

    // Remove B
    set_remove(*pp_set, "B");

    // done
    return;
}

void construct_AB_removeA_B ( set **pp_set )
{

    // Construct { A, B }
    construct_A_addB_AB(pp_set);

    // Remove A
    set_remove(*pp_set, "A");

    // done
    return;
}

void construct_AC_removeC_A ( set **pp_set )
{

    // Construct { A, C }
    construct_A_addC_AC(pp_set);

    // Remove C
    set_remove(*pp_set, "C");

    // done
    return;
}

void construct_AC_removeA_C ( set **pp_set )
{

    // Construct { A, C }
    construct_A_addC_AC(pp_set);

    // Remove A
    set_remove(*pp_set, "A");

    // done
    return;
}

void construct_BC_removeB_C ( set **pp_set )
{

    // Construct { B, C }
    construct_ABC_removeA_BC(pp_set);

    // Remove B
    set_remove(*pp_set, "B");

    // done
    return;
}

void construct_BC_removeC_B ( set **pp_set )
{

    // Construct { B, C }
    construct_ABC_removeA_BC(pp_set);

    // Remove C
    set_remove(*pp_set, "C");

    // done
    return;
}

void construct_A_addB_AB ( set **pp_set )
{

    // Construct the set
    construct_empty_addA_A(pp_set);

    // Add B
    set_add(*pp_set, B_element);

    // done
    return;
}

void construct_A_removeA_empty ( set **pp_set )
{
    
    // Construct the set
    construct_empty_addA_A(pp_set);

    // Remove "A"
    set_remove(*pp_set, "A");

    // done
    return;
}

void construct_B_removeB_empty ( set **pp_set )
{
    
    // Construct the set
    construct_empty_addB_B(pp_set);

    // Remove "B"
    set_remove(*pp_set, "B");

    // done
    return;
}

void construct_C_removeC_empty ( set **pp_set )
{
    
    // Construct the set
    construct_empty_addC_C(pp_set);

    // Remove "C"
    set_remove(*pp_set, "C");

    // done
    return;
}

void construct_A_addC_AC ( set **pp_set )
{

    // Construct the set
    construct_empty_addA_A(pp_set);

    // Add C
    set_add(*pp_set, C_element);

    // done
    return;
}

void construct_B_addA_AB ( set **pp_set )
{

    // Construct the set
    construct_empty_addB_B(pp_set);

    // Add A
    set_add(*pp_set, A_element);

    // done
    return;
}

void construct_C_addB_BC ( set **pp_set )
{

    // Construct the set
    construct_empty_addC_C(pp_set);

    // Add B
    set_add(*pp_set, B_element);

    // done
    return;
}

void construct_C_addA_AC ( set **pp_set )
{

    // Construct the set
    construct_empty_addC_C(pp_set);

    // Add A
    set_add(*pp_set, A_element);

    // done
    return;
}

void construct_B_addC_BC ( set **pp_set )
{

    // Construct the set
    construct_empty_addB_B(pp_set);

    // Add B
    set_add(*pp_set, C_element);

    // done
    return;
}

void construct_AB_addC_ABC ( set **pp_set )
{

    // Construct the set
    construct_A_addB_AB(pp_set);

    // Add C
    set_add(*pp_set, C_element);

    // done
    return;
}

void construct_AC_addB_ABC ( set **pp_set )
{

    // Construct the set
    construct_ABC_removeB_AC(pp_set);

    // Add B
    set_add(*pp_set, B_element);

    // done
    return;
}

void construct_BC_addA_ABC ( set **pp_set )
{

    // Construct the set
    construct_B_addC_BC(pp_set);

    // Add A
    set_add(*pp_set, A_element);

    // done
    return;
}

void construct_ABC_removeA_BC ( set **pp_set )
{
    // Construct the set
    construct_AB_addC_ABC(pp_set);

    // Remove A
    set_remove(*pp_set, A_element);

    // done
    return;
}

void construct_ABC_removeB_AC ( set **pp_set )
{
    // Construct the set
    construct_AB_addC_ABC(pp_set);

    // Remove B
    set_remove(*pp_set, B_element);

    // done
    return;
}
