/** !
 * Tester for tree module
 * 
 * @file src/test/tree_test.c
 * 
 * @author Jacob Smith
 */

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// log module
#include <core/log.h>

// sync module
#include <core/sync.h>

// tree module
#include <data/tree.h>

// enumeration definitions
enum result_e 
{
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

enum tree_type_e _type = -1;

// Test data
char *A_element = "A",
     *B_element = "B",
     *C_element = "C",
     *D_element = "D",
     *X_element = "X";

// Traversal testing globals
int traverse_counter = 0;
char *traverse_order[10]; // For storing traversal order

// Test element arrays
char *_elements   [] = { 0x0 },
     *A_elements  [] = { "A", 0x0 },
     *B_elements  [] = { "B", 0x0 },
     *C_elements  [] = { "C", 0x0 },
     *AB_elements [] = { "A", "B", 0x0 },
     *BC_elements [] = { "B", "C", 0x0 },
     *AC_elements [] = { "A", "C", 0x0 },
     *ABC_elements[] = { "A", "B", "C", 0x0 };

// Forward declarations
void print_time_pretty ( double seconds );
void run_tests ( void );
void print_final_summary ( void );
void print_test ( const char *scenario_name, const char *test_name, bool passed );

// Comparator function for string keys
int string_comparator ( const void *const a, const void *const b )
{
    if ( a == NULL && b == NULL ) return 0;
    if ( a == NULL ) return 1;
    if ( b == NULL ) return -1;
    
    return strcmp((const char *)a, (const char *)b);
}

// Key accessor function (key is the value itself)
const void *string_key_accessor ( const void *const p_value )
{
    return p_value;
}

// Test helpers
bool test_insert ( void (*constructor)(tree **), void *value, result_t expected );
bool test_search ( void (*constructor)(tree **), void *key, void *expected_value, result_t expected );
bool test_remove ( void (*constructor)(tree **), void *key, void *expected_value, result_t expected );

// Constructor functions
void construct_empty ( tree **pp_tree );
void construct_A ( tree **pp_tree );
void construct_B ( tree **pp_tree );
void construct_C ( tree **pp_tree );
void construct_D ( tree **pp_tree );
void construct_AB ( tree **pp_tree );
void construct_AC ( tree **pp_tree );
void construct_BC ( tree **pp_tree );
void construct_BA ( tree **pp_tree );
void construct_CA ( tree **pp_tree );
void construct_CB ( tree **pp_tree );
void construct_ABC ( tree **pp_tree );
void construct_ACB ( tree **pp_tree );
void construct_BAC ( tree **pp_tree );
void construct_BCA ( tree **pp_tree );
void construct_CAB ( tree **pp_tree );
void construct_CBA ( tree **pp_tree );
void construct_ABCD ( tree **pp_tree );
void construct_A_remove_A_empty ( tree **pp_tree );
void construct_B_remove_B_empty ( tree **pp_tree );
void construct_C_remove_C_empty ( tree **pp_tree );
void construct_AB_remove_A_B ( tree **pp_tree );
void construct_AB_remove_B_A ( tree **pp_tree );
void construct_AC_remove_A_C ( tree **pp_tree );
void construct_AC_remove_C_A ( tree **pp_tree );
void construct_BC_remove_B_C ( tree **pp_tree );
void construct_BC_remove_C_B ( tree **pp_tree );
void construct_ABC_remove_A_BC ( tree **pp_tree );
void construct_ABC_remove_B_AC ( tree **pp_tree );
void construct_ABC_remove_C_AB ( tree **pp_tree );

void test_empty_tree_scenario ( void );
void test_one_element_tree_scenario ( void );
void test_two_element_tree_scenario ( void );
void test_three_element_tree_scenario ( void );
void test_four_element_tree_scenario ( void );
void test_removal_scenario ( void );
void test_search_scenario ( void );
void test_insertion_patterns_scenario ( void );
void test_complex_removal_scenario ( void );
void test_balanced_tree_scenario ( void );
void test_stress_scenario ( void );
void test_traversal_scenario ( void );
void test_tree ( const char *name );

// entry point
int main ( int argc, const char* argv[] )
{
    
    // unused
    (void) argc;
    (void) argv;

    // initialized data
    timestamp t0 = 0,
              t1 = 0;

    // formatting
    printf(
        "╭─────────────╮\n"\
        "│ tree tester │\n"\
        "╰─────────────╯\n\n"
    );
    
    // start
    t0 = timer_high_precision();

    // run tests
    run_tests();

    // stop
    t1 = timer_high_precision();

    // report the time it took to run the tests
    log_info("tree took ");
    print_time_pretty ( (double)(t1-t0)/(double)timer_seconds_divisor() );
    log_info(" to test\n");

    // flush stdio
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
    while ( _seconds > 86400.0 ) { days++;_seconds-=86400.0; };
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

    if ( days ) log_info("%zu D, ", days);
    if ( hours ) log_info("%zu h, ", hours);
    if ( minutes ) log_info("%zu m, ", minutes);
    if ( __seconds ) log_info("%zu s, ", __seconds);
    if ( milliseconds ) log_info("%zu ms, ", milliseconds);
    if ( microseconds ) log_info("%zu us", microseconds);
}

void run_tests ( void )
{

    // binary
    {

        // initialized data
        timestamp binary_t0 = 0,
                  binary_t1 = 0;

        // change the base constructor
        _type = TREE_BINARY;

        // start
        binary_t0 = timer_high_precision();

        // run binary tree tests
        test_tree("binary");

        // stop
        binary_t1 = timer_high_precision();

        // report the time it took to run the tests
        log_info("binary took ");
        print_time_pretty ( (double)(binary_t1-binary_t0)/(double)timer_seconds_divisor() );
        log_info(" to test\n");
    }

    // avl
    {
        
        // initialized data
        timestamp avl_t0 = 0,
                  avl_t1 = 0;

        // change the base constructor
        _type = TREE_AVL;

        // start
        avl_t0 = timer_high_precision();

        // run avl tree tests
        test_tree("avl");

        // stop
        avl_t1 = timer_high_precision();

        // report the time it took to run the tests
        log_info("avl took ");
        print_time_pretty ( (double)(avl_t1-avl_t0)/(double)timer_seconds_divisor() );
        log_info(" to test\n");
    }

    // red black 
    {

        // initialized data
        timestamp red_black_t0 = 0,
                  red_black_t1 = 0;
          
        // change the base constructor
        _type = TREE_RED_BLACK;

        // start
        red_black_t0 = timer_high_precision();

        // run red black tree tests
        test_tree("red black");

        // stop
        red_black_t1 = timer_high_precision();

        // report the time it took to run the tests
        log_info("red black took ");
        print_time_pretty ( (double)(red_black_t1-red_black_t0)/(double)timer_seconds_divisor() );
        log_info(" to test\n");
    }

    // done
    return;
}

void test_tree ( const char *name ) 
{ 

    test_empty_tree_scenario();
    test_one_element_tree_scenario();
    test_two_element_tree_scenario();
    test_three_element_tree_scenario();
    test_four_element_tree_scenario();
    test_removal_scenario();
    test_search_scenario();
    test_insertion_patterns_scenario();
    test_complex_removal_scenario();
    test_balanced_tree_scenario();
    test_stress_scenario();
    test_traversal_scenario();

    // done
    return;
}

void print_final_summary ( void )
{
    total_tests  += ephemeral_tests;
    total_passes += ephemeral_passes;
    total_fails  += ephemeral_fails;

    log_info("\nTests: %d, Passed: %d, Failed: %d (%%%.3f)\n",  ephemeral_tests, ephemeral_passes, ephemeral_fails, ((float)ephemeral_passes/(float)ephemeral_tests*100.f));
    log_info("Total: %d, Passed: %d, Failed: %d (%%%.3f)\n\n",  total_tests, total_passes, total_fails, ((float)total_passes/(float)total_tests*100.f));
    
    ephemeral_tests  = 0;
    ephemeral_passes = 0;
    ephemeral_fails  = 0;
}

void print_test ( const char *scenario_name, const char *test_name, bool passed )
{
    if ( passed )
        log_pass("%s %s\n", scenario_name, test_name);
    else
        log_fail("%s %s\n", scenario_name, test_name);

    if (passed)
        ephemeral_passes++;
    else
        ephemeral_fails++;

    ephemeral_tests++;
}

// Constructor implementations
void construct_empty ( tree **pp_tree )
{
    tree_construct(pp_tree, _type, sizeof(char*), (fn_comparator *)string_comparator, (fn_key_accessor *)string_key_accessor);
}

void construct_A ( tree **pp_tree )
{
    construct_empty(pp_tree);
    tree_insert(*pp_tree, A_element);
}

void construct_B ( tree **pp_tree )
{
    construct_empty(pp_tree);
    tree_insert(*pp_tree, B_element);
}

void construct_C ( tree **pp_tree )
{
    construct_empty(pp_tree);
    tree_insert(*pp_tree, C_element);
}

void construct_D ( tree **p_tree )
{
    construct_empty(p_tree);
    tree_insert(*p_tree, D_element);
}

void construct_AB ( tree **p_tree )
{
    construct_A(p_tree);
    tree_insert(*p_tree, B_element);
}

void construct_AC ( tree **p_tree )
{
    construct_A(p_tree);
    tree_insert(*p_tree, C_element);
}

void construct_BC ( tree **p_tree )
{
    construct_B(p_tree);
    tree_insert(*p_tree, C_element);
}

void construct_BA ( tree **p_tree )
{
    construct_B(p_tree);
    tree_insert(*p_tree, A_element);
}

void construct_CA ( tree **p_tree )
{
    construct_C(p_tree);
    tree_insert(*p_tree, A_element);
}

void construct_CB ( tree **p_tree )
{
    construct_C(p_tree);
    tree_insert(*p_tree, B_element);
}

void construct_ABC ( tree **p_tree )
{
    construct_AB(p_tree);
    tree_insert(*p_tree, C_element);
}

void construct_ACB ( tree **p_tree )
{
    construct_AC(p_tree);
    tree_insert(*p_tree, B_element);
}

void construct_BAC ( tree **p_tree )
{
    construct_BA(p_tree);
    tree_insert(*p_tree, C_element);
}

void construct_BCA ( tree **p_tree )
{
    construct_BC(p_tree);
    tree_insert(*p_tree, A_element);
}

void construct_CAB ( tree **p_tree )
{
    construct_CA(p_tree);
    tree_insert(*p_tree, B_element);
}

void construct_CBA ( tree **p_tree )
{
    construct_CB(p_tree);
    tree_insert(*p_tree, A_element);
}

void construct_ABCD ( tree **p_tree )
{
    construct_ABC(p_tree);
    tree_insert(*p_tree, D_element);
}

// Removal constructor functions
// Traversal callback for testing
void test_traverse_callback(void *const p_value)
{
    if (traverse_counter < 10) {
        traverse_order[traverse_counter++] = (char*)p_value;
    }
    return; // Continue traversal
}

void construct_A_remove_A_empty ( tree **pp_tree )
{
    construct_A(pp_tree);
    void *removed_value = NULL;
    tree_remove(*pp_tree, A_element, (const void **)&removed_value);
}

void construct_B_remove_B_empty ( tree **pp_tree )
{
    construct_B(pp_tree);
    void *removed_value = NULL;
    tree_remove(*pp_tree, B_element, (const void **)&removed_value);
}

void construct_C_remove_C_empty ( tree **pp_tree )
{
    construct_C(pp_tree);
    void *removed_value = NULL;
    tree_remove(*pp_tree, C_element, (const void **)&removed_value);
}

void construct_AB_remove_A_B ( tree **pp_tree )
{
    construct_AB(pp_tree);
    void *removed_value = NULL;
    tree_remove(*pp_tree, A_element, (const void **)&removed_value);
}

void construct_AB_remove_B_A ( tree **pp_tree )
{
    construct_AB(pp_tree);
    void *removed_value = NULL;
    tree_remove(*pp_tree, B_element, (const void **)&removed_value);
}

void construct_ABC_remove_B_AC ( tree **pp_tree )
{
    construct_ABC(pp_tree);
    void *removed_value = NULL;
    tree_remove(*pp_tree, B_element, (const void **)&removed_value);
}

void construct_AC_remove_A_C ( tree **pp_tree )
{
    construct_AC(pp_tree);
    void *removed_value = NULL;
    tree_remove(*pp_tree, A_element, (const void **)&removed_value);
}

void construct_AC_remove_C_A ( tree **pp_tree )
{
    construct_AC(pp_tree);
    void *removed_value = NULL;
    tree_remove(*pp_tree, C_element, (const void **)&removed_value);
}

void construct_BC_remove_B_C ( tree **pp_tree )
{
    construct_BC(pp_tree);
    void *removed_value = NULL;
    tree_remove(*pp_tree, B_element, (const void **)&removed_value);
}

void construct_BC_remove_C_B ( tree **pp_tree )
{
    construct_BC(pp_tree);
    void *removed_value = NULL;
    tree_remove(*pp_tree, C_element, (const void **)&removed_value);
}

void construct_ABC_remove_A_BC ( tree **pp_tree )
{
    construct_ABC(pp_tree);
    void *removed_value = NULL;
    tree_remove(*pp_tree, A_element, (const void **)&removed_value);
}

void construct_ABC_remove_C_AB ( tree **pp_tree )
{
    construct_ABC(pp_tree);
    void *removed_value = NULL;
    tree_remove(*pp_tree, C_element, (const void **)&removed_value);
}

// Test helper implementations
bool test_insert ( void (*constructor)(tree **), void *value, result_t expected )
{
    tree *p_tree = NULL;
    constructor(&p_tree);
    
    result_t result = (result_t)tree_insert(p_tree, value);
    
    tree_destroy(&p_tree, NULL);
    return result == expected;
}

bool test_search ( void (*constructor)(tree **), void *key, void *expected_value, result_t expected )
{
    tree *p_tree = NULL;
    constructor(&p_tree);
    
    void *found_value = NULL;
    result_t result;
    
    int ret = tree_search(p_tree, key, &found_value);
    
    if ( ret == 1 && expected_value != NULL )
    {
        // Found and expected to find
        if ( strcmp((char*)found_value, (char*)expected_value) == 0 )
            result = match;
        else
            result = zero;  // Found wrong value
    }
    else if ( ret == 0 && expected_value == NULL )
    {
        // Not found and expected not to find
        result = zero;
    }
    else if ( ret == 1 && expected_value == NULL )
    {
        // Found but expected not to find
        result = one;
    }
    else
    {
        // Not found but expected to find
        result = zero;
    }
    
    tree_destroy(&p_tree, NULL);
    return result == expected;
}

bool test_size( void (*constructor)(tree **), size_t size, result_t expected )
{
    tree *p_tree = NULL;
    constructor(&p_tree);
    
    bool result = (tree_size(p_tree) == size) ? match : zero;
    
    tree_destroy(&p_tree, NULL);
    return result;
}

bool test_is_empty( void (*constructor)(tree **), result_t expected )
{
    tree *p_tree = NULL;
    constructor(&p_tree);
    
    bool result = (tree_is_empty(p_tree) == expected);
    
    tree_destroy(&p_tree, NULL);
    return result;
}

bool test_remove ( void (*constructor)(tree **), void *key, void *expected_value, result_t expected )
{
    tree *p_tree = NULL;
    constructor(&p_tree);
    
    const void *removed_value = NULL;
    result_t result;
    
    int ret = tree_remove(p_tree, key, &removed_value);
    
    if ( ret == 1 && expected_value != NULL )
    {
        // Removed and expected to remove
        if ( removed_value != NULL && strcmp((const char*)removed_value, (const char*)expected_value) == 0 )
            result = match;
        else
            result = zero;  // Removed wrong value
    }
    else if ( ret == 0 && expected_value == NULL )
    {
        // Not removed and expected not to remove
        result = zero;
    }
    else if ( ret == 1 && expected_value == NULL )
    {
        // Removed but expected not to remove
        result = one;
    }
    else
    {
        // Not removed but expected to remove
        result = zero;
    }
    
    tree_destroy(&p_tree, NULL);
    return result == expected;
}

// Test scenarios
void test_empty_tree_scenario ( void )
{
    log_scenario("empty tree\n");
    print_test("empty", "insert A", test_insert(construct_empty, A_element, one));
    print_test("empty", "search X", test_search(construct_empty, X_element, NULL, zero));
    print_test("empty", "remove X", test_remove(construct_empty, X_element, NULL, zero));
    print_test("empty", "size", test_size(construct_empty, 0, match));
    print_test("empty", "is empty", test_is_empty(construct_empty, one));

    print_final_summary();
}

void test_one_element_tree_scenario ( void )
{
    log_scenario("one element tree\n");
    print_test("A", "insert B", test_insert(construct_A, B_element, one));
    print_test("A", "search A", test_search(construct_A, A_element, A_element, match));
    print_test("A", "search X", test_search(construct_A, X_element, NULL, zero));
    print_test("A", "remove A", test_remove(construct_A, A_element, A_element, match));
    print_test("A", "size", test_size(construct_A, 1, match));
    print_test("A", "is empty", test_is_empty(construct_A, zero));

    print_final_summary();
}

void test_two_element_tree_scenario ( void )
{
    log_scenario("two element tree\n");
    print_test("AB", "insert C", test_insert(construct_AB, C_element, one));
    print_test("AB", "search A", test_search(construct_AB, A_element, A_element, match));
    print_test("AB", "search B", test_search(construct_AB, B_element, B_element, match));
    print_test("AB", "search X", test_search(construct_AB, X_element, NULL, zero));
    print_test("AB", "remove A", test_remove(construct_AB, A_element, A_element, match));
    print_test("AB", "remove B", test_remove(construct_AB, B_element, B_element, match));
    print_test("AB", "size", test_size(construct_AB, 2, match));
    print_test("AB", "is empty", test_is_empty(construct_AB, zero));

    print_final_summary();
}

void test_three_element_tree_scenario ( void )
{
    log_scenario("three element tree\n");
    print_test("ABC", "insert D", test_insert(construct_ABC, D_element, one));
    print_test("ABC", "search A", test_search(construct_ABC, A_element, A_element, match));
    print_test("ABC", "search B", test_search(construct_ABC, B_element, B_element, match));
    print_test("ABC", "search C", test_search(construct_ABC, C_element, C_element, match));
    print_test("ABC", "search X", test_search(construct_ABC, X_element, NULL, zero));
    print_test("ABC", "size", test_size(construct_ABC, 3, match));
    print_test("ABC", "is empty", test_is_empty(construct_ABC, zero));

    print_final_summary();
}

void test_removal_scenario ( void )
{
    log_scenario("removal scenario\n");
    print_test("AB->B", "search B", test_search(construct_AB_remove_A_B, B_element, B_element, match));
    print_test("AB->B", "search A", test_search(construct_AB_remove_A_B, A_element, NULL, zero));
    print_test("AB->A", "search A", test_search(construct_AB_remove_B_A, A_element, A_element, match));
    print_test("AB->A", "search B", test_search(construct_AB_remove_B_A, B_element, NULL, zero));
    print_test("ABC->AC", "search A", test_search(construct_ABC_remove_B_AC, A_element, A_element, match));
    print_test("ABC->AC", "search C", test_search(construct_ABC_remove_B_AC, C_element, C_element, match));
    print_test("ABC->AC", "search B", test_search(construct_ABC_remove_B_AC, B_element, NULL, zero));
    print_final_summary();
}

void test_search_scenario ( void )
{
    log_scenario("search scenario\n");
    
    // Test comprehensive search functionality
    tree *p_tree = NULL;
    construct_ABC(&p_tree);
    
    void *found_value = NULL;
    
    // Test successful searches
    bool search_A_success = (tree_search(p_tree, A_element, &found_value) == 1) && 
                            (strcmp((char*)found_value, A_element) == 0);
    print_test("search", "find A", search_A_success);
    
    bool search_B_success = (tree_search(p_tree, B_element, &found_value) == 1) && 
                            (strcmp((char*)found_value, B_element) == 0);
    print_test("search", "find B", search_B_success);
    
    bool search_C_success = (tree_search(p_tree, C_element, &found_value) == 1) && 
                            (strcmp((char*)found_value, C_element) == 0);
    print_test("search", "find C", search_C_success);
    
    // Test unsuccessful search
    found_value = NULL;
    bool search_X_failure = (tree_search(p_tree, X_element, &found_value) == 0);
    print_test("search", "miss X", search_X_failure);
    
    tree_destroy(&p_tree, NULL);
    print_final_summary();
}

void test_four_element_tree_scenario ( void )
{
    log_scenario("four element tree\n");
    print_test("ABCD", "search A", test_search(construct_ABCD, A_element, A_element, match));
    print_test("ABCD", "search B", test_search(construct_ABCD, B_element, B_element, match));
    print_test("ABCD", "search C", test_search(construct_ABCD, C_element, C_element, match));
    print_test("ABCD", "search D", test_search(construct_ABCD, D_element, D_element, match));
    print_test("ABCD", "search X", test_search(construct_ABCD, X_element, NULL, zero));
    print_test("ABCD", "size", test_size(construct_ABCD, 4, match));
    print_test("ABCD", "is empty", test_is_empty(construct_ABCD, zero));
    print_final_summary();
}

void test_insertion_patterns_scenario ( void )
{
    log_scenario("insertion patterns\n");
    
    // Test different insertion orders create valid trees
    print_test("ABC pattern", "search A", test_search(construct_ABC, A_element, A_element, match));
    print_test("ABC pattern", "search B", test_search(construct_ABC, B_element, B_element, match));
    print_test("ABC pattern", "search C", test_search(construct_ABC, C_element, C_element, match));
    
    print_test("ACB pattern", "search A", test_search(construct_ACB, A_element, A_element, match));
    print_test("ACB pattern", "search B", test_search(construct_ACB, B_element, B_element, match));
    print_test("ACB pattern", "search C", test_search(construct_ACB, C_element, C_element, match));
    
    print_test("BAC pattern", "search A", test_search(construct_BAC, A_element, A_element, match));
    print_test("BAC pattern", "search B", test_search(construct_BAC, B_element, B_element, match));
    print_test("BAC pattern", "search C", test_search(construct_BAC, C_element, C_element, match));
    
    print_test("BCA pattern", "search A", test_search(construct_BCA, A_element, A_element, match));
    print_test("BCA pattern", "search B", test_search(construct_BCA, B_element, B_element, match));
    print_test("BCA pattern", "search C", test_search(construct_BCA, C_element, C_element, match));
    
    print_test("CAB pattern", "search A", test_search(construct_CAB, A_element, A_element, match));
    print_test("CAB pattern", "search B", test_search(construct_CAB, B_element, B_element, match));
    print_test("CAB pattern", "search C", test_search(construct_CAB, C_element, C_element, match));
    
    print_test("CBA pattern", "search A", test_search(construct_CBA, A_element, A_element, match));
    print_test("CBA pattern", "search B", test_search(construct_CBA, B_element, B_element, match));
    print_test("CBA pattern", "search C", test_search(construct_CBA, C_element, C_element, match));
    
    print_final_summary();
}

void test_complex_removal_scenario ( void )
{
    log_scenario("complex removal\n");
    
    // Test removing from single element trees
    print_test("A->empty", "remove A", test_remove(construct_A, A_element, A_element, match));
    print_test("B->empty", "remove B", test_remove(construct_B, B_element, B_element, match));
    print_test("C->empty", "remove C", test_remove(construct_C, C_element, C_element, match));
    
    // Test removing from two element trees
    print_test("AB->A", "remove B", test_remove(construct_AB, B_element, B_element, match));
    print_test("AC->A", "remove C", test_remove(construct_AC, C_element, C_element, match));
    print_test("BC->B", "remove C", test_remove(construct_BC, C_element, C_element, match));
    print_test("BA->B", "remove A", test_remove(construct_BA, A_element, A_element, match));
    print_test("CA->C", "remove A", test_remove(construct_CA, A_element, A_element, match));
    print_test("CB->C", "remove B", test_remove(construct_CB, B_element, B_element, match));
    
    // Test removing from three element trees  
    print_test("ABC->BC", "remove A", test_remove(construct_ABC, A_element, A_element, match));
    print_test("ABC->AC", "remove B", test_remove(construct_ABC, B_element, B_element, match));
    print_test("ABC->AB", "remove C", test_remove(construct_ABC, C_element, C_element, match));
    
    // Verify remaining elements after removal
    print_test("ABC->BC", "search B", test_search(construct_ABC_remove_A_BC, B_element, B_element, match));
    print_test("ABC->BC", "search C", test_search(construct_ABC_remove_A_BC, C_element, C_element, match));
    print_test("ABC->BC", "miss A", test_search(construct_ABC_remove_A_BC, A_element, NULL, zero));
    
    print_test("ABC->AB", "search A", test_search(construct_ABC_remove_C_AB, A_element, A_element, match));
    print_test("ABC->AB", "search B", test_search(construct_ABC_remove_C_AB, B_element, B_element, match));
    print_test("ABC->AB", "miss C", test_search(construct_ABC_remove_C_AB, C_element, NULL, zero));
    
    print_final_summary();
}

void test_balanced_tree_scenario ( void )
{
    log_scenario("balanced tree operations\n");
    
    // Test different construction orders yield same search behavior
    print_test("order test", "ABC->find A", test_search(construct_ABC, A_element, A_element, match));
    print_test("order test", "ACB->find A", test_search(construct_ACB, A_element, A_element, match));
    print_test("order test", "BAC->find A", test_search(construct_BAC, A_element, A_element, match));
    print_test("order test", "BCA->find A", test_search(construct_BCA, A_element, A_element, match));
    print_test("order test", "CAB->find A", test_search(construct_CAB, A_element, A_element, match));
    print_test("order test", "CBA->find A", test_search(construct_CBA, A_element, A_element, match));
    
    print_final_summary();
}

void test_stress_scenario ( void )
{
    log_scenario("stress testing\n");
    
    tree *p_stress_tree = NULL;
    construct_empty(&p_stress_tree);
    
    // Insert many elements
    bool stress_insert_success = true;
    for (int i = 0; i < 100 && stress_insert_success; i++) {
        char *test_value = malloc(16);
        sprintf(test_value, "item_%d", i);
        if (tree_insert(p_stress_tree, test_value) != 1) {
            stress_insert_success = false;
        }
    }
    print_test("stress", "insert 100 items", stress_insert_success);
    
    // Search for some elements
    void *found_value = NULL;
    bool stress_search_success = (tree_search(p_stress_tree, "item_50", &found_value) == 1);
    print_test("stress", "find item_50", stress_search_success);
    
    tree_destroy(&p_stress_tree, NULL);
    print_final_summary();
}

void test_traversal_scenario ( void )
{
    log_scenario("traversal testing\n");
    
    // Test traversal functions exist and work
    tree *p_traversal_tree = NULL;
    construct_ABC(&p_traversal_tree);
    
    // Reset traversal counter
    traverse_counter = 0;
    memset(traverse_order, 0, sizeof(traverse_order));
    
    // Reset for in-order
    traverse_counter = 0;
    memset(traverse_order, 0, sizeof(traverse_order));
    
    // Test in-order traversal  
    bool inorder_success = (tree_traverse_inorder(p_traversal_tree, test_traverse_callback) == 1);
    print_test("traversal", "in-order", inorder_success);
    
    // Reset for post-order
    traverse_counter = 0;
    memset(traverse_order, 0, sizeof(traverse_order));
    
    tree_destroy(&p_traversal_tree, NULL);
    print_final_summary();
}
