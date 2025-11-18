/** !
 * Tester for cache module
 * 
 * @file cache_test.c
 * 
 * @author Jacob Smith
 */

// Include
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// log module
#include <core/log.h>

// sync module
#include <core/sync.h>

// cache module
#include <data/cache.h>

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

// Test data
const char *A_element = "A",
           *B_element = "B",
           *C_element = "C",
           *D_element = "D",
           *X_element = "X";

const char *A_elements[]   = { "A", NULL };
const char *BA_elements[]  = { "B", "A", NULL };
const char *CBA_elements[] = { "C", "B", "A", NULL };
const char *ACB_elements[] = { "A", "C", "B", NULL };
const char *DCB_elements[] = { "D", "C", "B", NULL };
const char *CA_elements[]  = { "C", "A", NULL };

// Forward declarations
void print_time_pretty ( double seconds );
void run_tests ( void );
void print_final_summary ( void );
void print_test ( const char *scenario_name, const char *test_name, bool passed );

// Custom equality and key accessor for string keys
int string_equality(const void *a, const void *b) {
    if (a == NULL || b == NULL) return a == b;
    return strcmp((const char *)a, (const char *)b) == 0;
}

const void *key_accessor(const void *value) {
    return value;
}

// Test helpers
bool test_insert(void (*constructor)(cache **), const char *key, const void *value, result_t expected);
bool test_find(void (*constructor)(cache **), const char *key, const void *expected_value, result_t expected);
bool test_remove(void (*constructor)(cache **), const char *key, const void *expected_value, result_t expected);
bool test_contents(void (*constructor)(cache **), const char **expected_contents, size_t expected_count);

// Constructor functions
void construct_empty(cache **pp_cache);
void construct_A(cache **pp_cache);
void construct_BA(cache **pp_cache);
void construct_CBA(cache **pp_cache);
void construct_ACB(cache **pp_cache);
void construct_DCB(cache **pp_cache);
void construct_CA(cache **pp_cache);

// Test scenarios
void test_empty_cache_scenario();
void test_one_element_cache_scenario();
void test_two_element_cache_scenario();
void test_full_cache_scenario();
void test_eviction_scenario();
void test_lru_find_scenario();
void test_remove_scenario();

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
        "│ cache tester │\n"\
        "╰──────────────╯\n\n"
    );
    
    // Start
    t0 = timer_high_precision();

    // Run tests
    run_tests();

    // Stop
    t1 = timer_high_precision();

    // Report the time it took to run the tests
    log_info("cache took ");
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

    // empty 
    test_empty_cache_scenario();

    // one element
    test_one_element_cache_scenario();

    // two element
    test_two_element_cache_scenario();

    // full
    test_full_cache_scenario();

    
    test_eviction_scenario();
    test_lru_find_scenario();
    test_remove_scenario();
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

void construct_empty(cache **pp_cache) {
    cache_construct(pp_cache, 3, string_equality, NULL);
}

void construct_A(cache **pp_cache) {
    construct_empty(pp_cache);
    cache_insert(*pp_cache, A_element, A_element);
}

void construct_BA(cache **pp_cache) {
    construct_A(pp_cache);
    cache_insert(*pp_cache, B_element, B_element);
}

void construct_CBA(cache **pp_cache) {
    construct_BA(pp_cache);
    cache_insert(*pp_cache, C_element, C_element);
}

void construct_ACB(cache **pp_cache) {
    construct_CBA(pp_cache);
    cache_find(*pp_cache, A_element, NULL);
}

void construct_DCB(cache **pp_cache) {
    construct_CBA(pp_cache);
    cache_insert(*pp_cache, D_element, D_element);
}

void construct_CA(cache **pp_cache) {
    construct_CBA(pp_cache);
    cache_remove(*pp_cache, B_element, NULL);
}

bool test_insert(void (*constructor)(cache **), const char *key, const void *value, result_t expected) {
    cache *p_cache = NULL;
    constructor(&p_cache);
    result_t result = (result_t)cache_insert(p_cache, key, value);
    cache_destroy(&p_cache);
    return result == expected;
}

bool test_find(void (*constructor)(cache **), const char *key, const void *expected_value, result_t expected) {
    cache *p_cache = NULL;
    constructor(&p_cache);
    void *found_value = NULL;
    result_t result;

    int ret = cache_find(p_cache, key, &found_value);

    if (ret == 1) { // Hit
        if (expected_value != NULL && found_value != NULL && strcmp(found_value, expected_value) == 0)
            result = match;
        else
            result = one;
    } else { // Miss
        result = zero;
    }
    
    cache_destroy(&p_cache);
    return result == expected;
}

bool test_remove(void (*constructor)(cache **), const char *key, const void *expected_value, result_t expected) {
    cache *p_cache = NULL;
    constructor(&p_cache);
    void *removed_value = (void*)-1;
    result_t result;

    cache_remove(p_cache, key, &removed_value);

    if (expected_value != NULL) { // Expect to find and remove
        if (removed_value != (void*)-1 && removed_value != NULL && strcmp(removed_value, expected_value) == 0)
            result = match;
        else
            result = zero;
    } else { // Expect not to find
        if (removed_value == NULL)
            result = one;
        else
            result = zero;
    }

    cache_destroy(&p_cache);
    return result == expected;
}

bool test_contents(void (*constructor)(cache **), const char **expected_contents, size_t expected_count) {
    cache *p_cache = NULL;
    constructor(&p_cache);

    if (p_cache->properties.count != expected_count) {
        cache_destroy(&p_cache);
        return false;
    }

    for (size_t i = 0; i < expected_count; i++) {
        if (strcmp(p_cache->properties.pp_data[i], expected_contents[i]) != 0) {
            cache_destroy(&p_cache);
            return false;
        }
    }

    cache_destroy(&p_cache);
    return true;
}

void test_empty_cache_scenario() {
    log_scenario("empty cache\n");
    print_test("empty", "insert A", test_insert(construct_empty, A_element, A_element, one));
    print_test("empty", "find X", test_find(construct_empty, X_element, NULL, zero));
    print_test("empty", "remove X", test_remove(construct_empty, X_element, NULL, one));
    print_test("empty", "contents", test_contents(construct_empty, (const char*[]){NULL}, 0));
    print_final_summary();
}

void test_one_element_cache_scenario() {
    log_scenario("one element cache\n");
    print_test("A", "insert B", test_insert(construct_A, B_element, B_element, one));
    print_test("A", "find A", test_find(construct_A, A_element, A_element, match));
    print_test("A", "find X", test_find(construct_A, X_element, NULL, zero));
    print_test("A", "contents", test_contents(construct_A, A_elements, 1));
    print_final_summary();
}

void test_two_element_cache_scenario() {
    log_scenario("two element cache\n");
    print_test("BA", "insert C", test_insert(construct_BA, C_element, C_element, one));
    print_test("BA", "find A", test_find(construct_BA, A_element, A_element, match));
    print_test("BA", "find B", test_find(construct_BA, B_element, B_element, match));
    print_test("BA", "contents", test_contents(construct_BA, BA_elements, 2));
    print_final_summary();
}

void test_full_cache_scenario() {
    log_scenario("full cache\n");
    print_test("CBA", "insert D (evict)", test_insert(construct_CBA, D_element, D_element, one));
    print_test("CBA", "find A", test_find(construct_CBA, A_element, A_element, match));
    print_test("CBA", "find B", test_find(construct_CBA, B_element, B_element, match));
    print_test("CBA", "find C", test_find(construct_CBA, C_element, C_element, match));
    print_test("CBA", "contents", test_contents(construct_CBA, CBA_elements, 3));
    print_final_summary();
}

void test_eviction_scenario() {
    log_scenario("eviction scenario\n");
    print_test("DCB", "find A (miss)", test_find(construct_DCB, A_element, NULL, zero));
    print_test("DCB", "find D", test_find(construct_DCB, D_element, D_element, match));
    print_test("DCB", "contents", test_contents(construct_DCB, DCB_elements, 3));
    print_final_summary();
}

void test_lru_find_scenario() {
    log_scenario("lru find scenario\n");
    print_test("ACB", "find B", test_find(construct_ACB, B_element, B_element, match));
    print_test("ACB", "find C", test_find(construct_ACB, C_element, C_element, match));
    print_test("ACB", "find A", test_find(construct_ACB, A_element, A_element, match));
    print_test("ACB", "contents", test_contents(construct_ACB, ACB_elements, 3));
    print_final_summary();
}

void test_remove_scenario() {
    log_scenario("remove scenario\n");
    print_test("CBA", "remove B", test_remove(construct_CBA, B_element, B_element, match));
    print_test("CA", "contents after remove", test_contents(construct_CA, CA_elements, 2));
    print_test("CBA", "remove X", test_remove(construct_CBA, X_element, NULL, one));
    print_final_summary();
}
