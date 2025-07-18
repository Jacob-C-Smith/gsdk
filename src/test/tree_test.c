/** !
 * Tester for tree module
 * 
 * @file tree_test.c
 * 
 * @author Jacob Smith
 */

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// core
#include <core/log.h>
#include <core/sync.h>

// data
#include <data/tree.h>
#include <data/binary.h>

// enumeration definitions
enum result_e
{
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

// Test data
struct number_and_string_s
{
    char _string[12];
    int  number;
};

typedef struct number_and_string_s number_and_string;

number_and_string _values[] =
{
    { ._string = "eight"   , .number = 8  },
    { ._string = "four"    , .number = 4  },
    { ._string = "twelve"  , .number = 12 },
    { ._string = "two"     , .number = 2  },
    { ._string = "six"     , .number = 6  },
    { ._string = "ten"     , .number = 10 },
    { ._string = "fourteen", .number = 14 },
    { ._string = "one"     , .number = 1  },
    { ._string = "three"   , .number = 3  },
    { ._string = "five"    , .number = 5  },
    { ._string = "seven"   , .number = 7  },
    { ._string = "nine"    , .number = 9  },
    { ._string = "eleven"  , .number = 11 },
    { ._string = "thirteen", .number = 13 },
    { ._string = "fifteen" , .number = 15 }
};

const size_t value_count = sizeof(_values) / sizeof(number_and_string);

// forward declarations
void print_time_pretty   ( double seconds );
void run_tests           ( void );
void print_final_summary ( void );
void print_test          ( const char *scenario_name, const char *test_name, bool passed );

int         binary_tree_example_comparator   ( const void *const p_a, const void *const p_b );
const void *binary_tree_example_key_accessor ( const void *const p_value );
int         binary_tree_print_node           ( void *p_value );

// Constructors
void construct_empty ( binary_tree **pp_tree );
void construct_8 ( binary_tree **pp_tree );
void construct_4 ( binary_tree **pp_tree );
void construct_12 ( binary_tree **pp_tree );
void construct_8_4 ( binary_tree **pp_tree );
void construct_8_12 ( binary_tree **pp_tree );
void construct_4_8 ( binary_tree **pp_tree );
void construct_12_8 ( binary_tree **pp_tree );
void construct_8_4_12 ( binary_tree **pp_tree );
void construct_full ( binary_tree **pp_tree );
void construct_full_remove_8 ( binary_tree **pp_tree );
void construct_full_remove_15 ( binary_tree **pp_tree );
void construct_full_remove_6 ( binary_tree **pp_tree );

bool test_insert ( void (*tree_constructor)(binary_tree **), void *value, result_t expected );
bool test_search ( void (*tree_constructor)(binary_tree **), const void *key, void *expected_value, result_t expected );
bool test_remove ( void (*tree_constructor)(binary_tree **), const void *key, void *expected_value, result_t expected );

void test_empty_tree_scenario(void);
void test_one_node_scenarios(void);
void test_two_node_scenarios(void);
void test_three_node_scenario(void);
void test_full_tree_scenario(void);
void test_remove_scenarios(void);

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
        "╭──────────────╮\n"\
        "│ tree tester  │\n"\
        "╰──────────────╯\n\n"
    );
    
    // Start
    t0 = timer_high_precision();

    // Run tests
    run_tests();

    // Stop
    t1 = timer_high_precision();

    // Report the time it took to run the tests
    log_info("tree tests took ");
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
    
    // Done
    return;
}

void run_tests ( void )
{
    test_empty_tree_scenario();
    test_one_node_scenarios();
    test_two_node_scenarios();
    test_three_node_scenario();
    test_full_tree_scenario();
    test_remove_scenarios();
}

void print_final_summary ( void )
{

    // accumulate
    total_tests  += ephemeral_tests,
    total_passes += ephemeral_passes,
    total_fails  += ephemeral_fails;

    // print
    log_info("\nTests: %d, Passed: %d, Failed: %d (%%%f)\n",  ephemeral_tests, ephemeral_passes, ephemeral_fails, ((float)ephemeral_passes/(float)ephemeral_tests*100.f));
    log_info("Total: %d, Passed: %d, Failed: %d (%%%f)\n\n",  total_tests, total_passes, total_fails, ((float)total_passes/(float)total_tests*100.f));
    
    // clear test counters for this test
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
        log_pass("[%s] %s\n", scenario_name, test_name);
    else
        log_fail("[%s] %s\n", scenario_name, test_name);

    // increment the pass/fail counter
    if (passed)
        ephemeral_passes++;
    else
        ephemeral_fails++;

    // increment the test counter
    ephemeral_tests++;

    // done
    return;
}

int binary_tree_example_comparator ( const void *const p_a, const void *const p_b )
{
    const number_and_string *p_val_a = p_a;
    const number_and_string *p_val_b = p_b;

    if ( p_val_a->number < p_val_b->number )
        return -1;
    else if ( p_val_a->number > p_val_b->number )
        return 1;
    else
        return 0;
}

const void *binary_tree_example_key_accessor ( const void *const p_value )
{
    const number_and_string *p_number_and_string = p_value;
    return (void *) &p_number_and_string->number;
}

int binary_tree_print_node ( void *p_value )
{
    log_info("%s\n", ((number_and_string *) p_value)->_string);
    return 1;
}

// Constructors
void construct_empty(binary_tree **pp_tree) {
    binary_tree_construct(pp_tree, binary_tree_example_comparator, binary_tree_example_key_accessor, sizeof(number_and_string));
}

void construct_8(binary_tree **pp_tree) {
    construct_empty(pp_tree);
    binary_tree_insert(*pp_tree, &_values[0]); // "eight", 8
}

void construct_4(binary_tree **pp_tree) {
    construct_empty(pp_tree);
    binary_tree_insert(*pp_tree, &_values[1]); // "four", 4
}

void construct_12(binary_tree **pp_tree) {
    construct_empty(pp_tree);
    binary_tree_insert(*pp_tree, &_values[2]); // "twelve", 12
}

void construct_8_4(binary_tree **pp_tree) {
    construct_8(pp_tree);
    binary_tree_insert(*pp_tree, &_values[1]); // "four", 4
}

void construct_8_12(binary_tree **pp_tree) {
    construct_8(pp_tree);
    binary_tree_insert(*pp_tree, &_values[2]); // "twelve", 12
}

void construct_4_8(binary_tree **pp_tree) {
    construct_4(pp_tree);
    binary_tree_insert(*pp_tree, &_values[0]); // "eight", 8
}

void construct_12_8(binary_tree **pp_tree) {
    construct_12(pp_tree);
    binary_tree_insert(*pp_tree, &_values[0]); // "eight", 8
}

void construct_8_4_12(binary_tree **pp_tree) {
    construct_8(pp_tree);
    binary_tree_insert(*pp_tree, &_values[1]); // "four", 4
    binary_tree_insert(*pp_tree, &_values[2]); // "twelve", 12
}

void construct_full(binary_tree **pp_tree) {
    construct_empty(pp_tree);
    for (size_t i = 0; i < value_count; i++) {
        binary_tree_insert(*pp_tree, &_values[i]);
    }
}

void construct_full_remove_8(binary_tree **pp_tree) {
    construct_full(pp_tree);
    binary_tree_remove(*pp_tree, &_values[0].number, NULL);
}

void construct_full_remove_15(binary_tree **pp_tree) {
    construct_full(pp_tree);
    binary_tree_remove(*pp_tree, &_values[14].number, NULL);
}

void construct_full_remove_6(binary_tree **pp_tree) {
    construct_full(pp_tree);
    binary_tree_remove(*pp_tree, &_values[4].number, NULL);
}

bool test_insert(void (*tree_constructor)(binary_tree **), void *value, result_t expected) {
    binary_tree *p_tree = NULL;
    tree_constructor(&p_tree);
    result_t result = (result_t)binary_tree_insert(p_tree, value);
    binary_tree_destroy(&p_tree);
    return result == expected;
}

bool test_search(void (*tree_constructor)(binary_tree **), const void *key, void *expected_value, result_t expected) {
    binary_tree *p_tree = NULL;
    tree_constructor(&p_tree);
    void *found_value = NULL;
    result_t result;

    int ret = binary_tree_search(p_tree, key, &found_value);

    if (ret == 1) { // Hit
        if (expected_value != NULL && memcmp(found_value, expected_value, sizeof(number_and_string)) == 0)
            result = match;
        else
            result = one;
    } else { // Miss
        result = zero;
    }
    
    binary_tree_destroy(&p_tree);
    return result == expected;
}

bool test_remove(void (*tree_constructor)(binary_tree **), const void *key, void *expected_value, result_t expected) {
    binary_tree *p_tree = NULL;
    tree_constructor(&p_tree);
    void *removed_value = NULL;
    result_t result;

    int ret = binary_tree_remove(p_tree, key, &removed_value);

    if (ret == 1) { // Found and removed
        if (expected_value != NULL && memcmp(removed_value, expected_value, sizeof(number_and_string)) == 0)
            result = match;
        else
            result = one;
    } else { // Not found
        result = zero;
    }

    binary_tree_destroy(&p_tree);
    return result == expected;
}

void test_empty_tree_scenario() {
    log_scenario("Empty Tree\n");
    print_test("empty", "insert 8", test_insert(construct_empty, &_values[0], one));
    print_test("empty", "search 8", test_search(construct_empty, &_values[0].number, NULL, zero));
    print_test("empty", "remove 8", test_remove(construct_empty, &_values[0].number, NULL, zero));
    print_final_summary();
}

void test_one_node_scenarios() {
    log_scenario("One Node Tree { 8 }\n");
    print_test("one node {8}", "insert 4", test_insert(construct_8, &_values[1], one));
    print_test("one node {8}", "insert 8 (duplicate)", test_insert(construct_8, &_values[0], zero));
    print_test("one node {8}", "search 8", test_search(construct_8, &_values[0].number, &_values[0], match));
    print_test("one node {8}", "search 4", test_search(construct_8, &_values[1].number, NULL, zero));
    print_test("one node {8}", "remove 8", test_remove(construct_8, &_values[0].number, &_values[0], match));
    print_test("one node {8}", "remove 4", test_remove(construct_8, &_values[1].number, NULL, zero));
    print_final_summary();
}

void test_two_node_scenarios() {
    log_scenario("Two Node Tree { 8, 4 }\n");
    print_test("two nodes {8,4}", "insert 12", test_insert(construct_8_4, &_values[2], one));
    print_test("two nodes {8,4}", "insert 8 (duplicate)", test_insert(construct_8_4, &_values[0], zero));
    print_test("two nodes {8,4}", "search 8", test_search(construct_8_4, &_values[0].number, &_values[0], match));
    print_test("two nodes {8,4}", "search 4", test_search(construct_8_4, &_values[1].number, &_values[1], match));
    print_test("two nodes {8,4}", "remove 8", test_remove(construct_8_4, &_values[0].number, &_values[0], match));
    print_test("two nodes {8,4}", "remove 4", test_remove(construct_8_4, &_values[1].number, &_values[1], match));
    print_final_summary();
}

void test_three_node_scenario() {
    log_scenario("Three Node Tree { 8, 4, 12 }\n");
    print_test("three nodes", "insert 2", test_insert(construct_8_4_12, &_values[3], one));
    print_test("three nodes", "insert 8 (duplicate)", test_insert(construct_8_4_12, &_values[0], zero));
    print_test("three nodes", "search 4", test_search(construct_8_4_12, &_values[1].number, &_values[1], match));
    print_test("three nodes", "search 12", test_search(construct_8_4_12, &_values[2].number, &_values[2], match));
    print_test("three nodes", "remove 12", test_remove(construct_8_4_12, &_values[2].number, &_values[2], match));
    print_test("three nodes", "remove 8", test_remove(construct_8_4_12, &_values[0].number, &_values[0], match));
    print_final_summary();
}

void test_full_tree_scenario() {
    log_scenario("Full Tree\n");
    for (size_t i = 0; i < value_count; i++) {
        char test_name[64];
        sprintf(test_name, "insert %s (duplicate)", _values[i]._string);
        print_test("full", test_name, test_insert(construct_full, &_values[i], zero));
    }
    for (size_t i = 0; i < value_count; i++) {
        char test_name[64];
        sprintf(test_name, "search %s", _values[i]._string);
        print_test("full", test_name, test_search(construct_full, &_values[i].number, &_values[i], match));
    }
    for (size_t i = 0; i < value_count; i++) {
        char test_name[64];
        sprintf(test_name, "remove %s", _values[i]._string);
        print_test("full", test_name, test_remove(construct_full, &_values[i].number, &_values[i], match));
    }
    print_final_summary();
}

void test_remove_scenarios() {
    log_scenario("Remove from Full Tree\n");
    print_test("remove root (8)", "search 8", test_search(construct_full_remove_8, &_values[0].number, NULL, zero));
    print_test("remove leaf (15)", "search 15", test_search(construct_full_remove_15, &_values[14].number, NULL, zero));
    print_test("remove internal (6)", "search 6", test_search(construct_full_remove_6, &_values[4].number, NULL, zero));
    print_final_summary();
}
