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

/// data
#include <data/bitmap.h>

// enumeration definitions
enum result_e 
{
    zero  = 0,
    one   = 1,
    match = 2
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

// forward declarations
void print_time_pretty ( double seconds );
void run_tests ( void );
void print_final_summary ( void );
void print_test ( const char *scenario_name, const char *test_name, bool passed );

bool test_set ( void (*bitmap_constructor)(bitmap **), size_t index, result_t expected );
bool test_clear ( void (*bitmap_constructor)(bitmap **), size_t index, result_t expected );
bool test_is_set ( void (*bitmap_constructor)(bitmap **), size_t index, result_t expected );

void test_one_bit_bitmap ( void (*bitmap_constructor)(bitmap **), char *name, size_t value );
void test_two_bit_bitmap ( void (*bitmap_constructor)(bitmap **), char *name, size_t value );
void test_three_bit_bitmap ( void (*bitmap_constructor)(bitmap **), char *name, size_t value );

// constructors
/// one element
void one_construct_0 ( bitmap **pp_bitmap );
void one_0_set0_1 ( bitmap **pp_bitmap );
void one_1_clear0_0 ( bitmap **pp_bitmap );

// two element
void two_construct_00 ( bitmap **pp_bitmap );
void two_00_set0_01 ( bitmap **pp_bitmap );
void two_00_set1_10 ( bitmap **pp_bitmap );
void two_10_set0_11 ( bitmap **pp_bitmap );
void two_11_clear0_10 ( bitmap **pp_bitmap );
void two_11_clear1_01 ( bitmap **pp_bitmap );

/// three element
void three_construct_000 ( bitmap **pp_bitmap );

void three_000_set0_001 ( bitmap **pp_bitmap );
void three_000_set1_010 ( bitmap **pp_bitmap );
void three_000_set2_100 ( bitmap **pp_bitmap );

void three_100_set0_101 ( bitmap **pp_bitmap );

void three_101_set1_111 ( bitmap **pp_bitmap );


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
        "╭───────────────╮\n"\
        "│ bitmap tester │\n"\
        "╰───────────────╯\n\n"
    );

    // start
    t0 = timer_high_precision();

    // run tests
    run_tests();

    // stop
    t1 = timer_high_precision();

    // Report the time it took to run the tests
    log_info("bitmap took ");
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

    // ... -> 0
    test_one_bit_bitmap(one_construct_0, "construct1_0", 0b0);

    // 0 -> set(0) -> 1
    test_one_bit_bitmap(one_0_set0_1, "0_set0_1", 0b1);

    // 1 -> clear(0) -> 0
    test_one_bit_bitmap(one_1_clear0_0, "1_clear0_0", 0b0);

    // ... -> 00
    test_two_bit_bitmap(two_construct_00, "construct2_00", 0b00);

    // 00 -> set(0) -> 01
    test_two_bit_bitmap(two_00_set0_01, "00_set0_01", 0b01);

    // 00 -> set(1) -> 10
    test_two_bit_bitmap(two_00_set1_10, "00_set1_10", 0b10);

    // 10 -> set(0) -> 11
    test_two_bit_bitmap(two_10_set0_11, "10_set0_11", 0b11);

    // 11 -> clear(0) -> 10
    test_two_bit_bitmap(two_11_clear0_10, "11_clear0_10", 0b10);

    // 11 -> clear(1) -> 01
    test_two_bit_bitmap(two_11_clear1_01, "11_clear1_01", 0b01);

    // ... -> 000
    test_three_bit_bitmap(three_construct_000, "construct3_000", 0b000);

    // 000 -> set(0) -> 001
    test_three_bit_bitmap(three_000_set0_001, "000_set0_001", 0b001);

    // 000 -> set(1) -> 010
    test_three_bit_bitmap(three_000_set1_010, "000_set1_010", 0b010);

    // 000 -> set(2) -> 100
    test_three_bit_bitmap(three_000_set2_100, "000_set2_100", 0b100);

    // 100 -> set(0) -> 101
    test_three_bit_bitmap(three_100_set0_101, "100_set0_101", 0b101);

    // 101 -> set(1) -> 111
    test_three_bit_bitmap(three_101_set1_111, "101_set1_111", 0b111);

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

bool test_set ( void (*bitmap_constructor)(bitmap **), size_t index, result_t expected )
{
    // initialized data
    result_t result = 0;
    bitmap *p_bitmap = 0;

    // Build the bitmap
    bitmap_constructor(&p_bitmap);

    // Set the bit
    // bitmap_set returns 1 on success
    if ( bitmap_set(p_bitmap, index) == 1 )
        result = one;
    else
        result = zero;

    // Free the bitmap
    bitmap_destroy(&p_bitmap);

    // return result
    return (result == expected);
}

bool test_clear ( void (*bitmap_constructor)(bitmap **), size_t index, result_t expected )
{
    // initialized data
    result_t result = 0;
    bitmap *p_bitmap = 0;

    // Build the bitmap
    bitmap_constructor(&p_bitmap);

    // Clear the bit
    // bitmap_clear returns 0 on success
    if ( bitmap_clear(p_bitmap, index) == 0 )
        result = zero;
    else
        result = one;

    // Free the bitmap
    bitmap_destroy(&p_bitmap);

    // return result
    return (result == expected);
}

bool test_is_set ( void (*bitmap_constructor)(bitmap **), size_t index, result_t expected )
{
    // initialized data
    result_t result = 0;
    bitmap *p_bitmap = 0;

    // Build the bitmap
    bitmap_constructor(&p_bitmap);

    // Test the bit
    // bitmap_test returns 1 if set, 0 if clear
    if ( bitmap_test(p_bitmap, index) == 1 )
        result = one;
    else
        result = zero;

    // Free the bitmap
    bitmap_destroy(&p_bitmap);

    // return result
    return (result == expected);
}

void test_one_bit_bitmap ( void (*bitmap_constructor)(bitmap **), char *name, size_t value )
{

    // log
    log_scenario("%s\n", name);

    // test
    print_test(name, "bitmap_test_0", test_is_set(bitmap_constructor, 0, (bool)(value & 0b1)));
    print_test(name, "bitmap_set_0" , test_set(bitmap_constructor, 0, 1));
    print_test(name, "bitmap_clear_0", test_clear(bitmap_constructor, 0, 0));

    // results
    print_final_summary();

    // done
    return;
}

void test_two_bit_bitmap ( void (*bitmap_constructor)(bitmap **), char *name, size_t value )
{

    // log
    log_scenario("%s\n", name);

    // test
    print_test(name, "bitmap_test_0" , test_is_set(bitmap_constructor, 0, (bool)(value & 0b01)));
    print_test(name, "bitmap_test_1" , test_is_set(bitmap_constructor, 1, (bool)(value & 0b10)));
    print_test(name, "bitmap_set_0"  , test_set(bitmap_constructor, 0, 1));
    print_test(name, "bitmap_set_1"  , test_set(bitmap_constructor, 1, 1));
    print_test(name, "bitmap_clear_0", test_clear(bitmap_constructor, 0, 0));
    print_test(name, "bitmap_clear_1", test_clear(bitmap_constructor, 1, 0));

    // results
    print_final_summary();

    // done
    return;
}

void test_three_bit_bitmap ( void (*bitmap_constructor)(bitmap **), char *name, size_t value )
{

    // log
    log_scenario("%s\n", name);

    // test
    print_test(name, "bitmap_test_0" , test_is_set(bitmap_constructor, 0, (bool)(value & 0b001)));
    print_test(name, "bitmap_test_1" , test_is_set(bitmap_constructor, 1, (bool)(value & 0b010)));
    print_test(name, "bitmap_test_2" , test_is_set(bitmap_constructor, 2, (bool)(value & 0b100)));
    print_test(name, "bitmap_set_0"  , test_set(bitmap_constructor, 0, 1));
    print_test(name, "bitmap_set_1"  , test_set(bitmap_constructor, 1, 1));
    print_test(name, "bitmap_set_1"  , test_set(bitmap_constructor, 2, 1));
    print_test(name, "bitmap_clear_0", test_clear(bitmap_constructor, 0, 0));
    print_test(name, "bitmap_clear_1", test_clear(bitmap_constructor, 1, 0));
    print_test(name, "bitmap_clear_1", test_clear(bitmap_constructor, 2, 0));

    // results
    print_final_summary();

    // done
    return;
}

void one_construct_0 ( bitmap **pp_bitmap )
{

    // construct a 1-bit bitmap
    bitmap_construct(pp_bitmap, 1);
    
    // success
    return;
}

void one_0_set0_1 ( bitmap **pp_bitmap )
{

    // construct a 1-bit bitmap
    one_construct_0(pp_bitmap);

    // set bit 0
    bitmap_set(*pp_bitmap, 0);

    // success;
    return;
}

void one_1_clear0_0 ( bitmap **pp_bitmap )
{

    // construct a 1-bit bitmap
    one_0_set0_1(pp_bitmap);

    // clear bit 0
    bitmap_clear(*pp_bitmap, 0);

    // success;
    return;
}

void two_construct_00 ( bitmap **pp_bitmap )
{

    // construct a 2-bit bitmap
    bitmap_construct(pp_bitmap, 2);
    
    // success
    return;
}

void two_00_set0_01 ( bitmap **pp_bitmap )
{

    // construct a 2-bit bitmap
    two_construct_00(pp_bitmap);

    // set bit 0
    bitmap_set(*pp_bitmap, 0);

    // success;
    return;
}

void two_00_set1_10 ( bitmap **pp_bitmap )
{

    // construct a 2-bit bitmap
    two_construct_00(pp_bitmap);

    // set bit 1
    bitmap_set(*pp_bitmap, 1);

    // success;
    return;
}

void two_10_set0_11 ( bitmap **pp_bitmap )
{

    // construct a 2-bit bitmap
    two_00_set1_10(pp_bitmap);

    // set bit 0
    bitmap_set(*pp_bitmap, 0);

    // success;
    return;
}

void two_11_clear0_10 ( bitmap **pp_bitmap )
{
    
    // construct a 2-bit bitmap
    two_10_set0_11(pp_bitmap);

    // clear bit 0
    bitmap_clear(*pp_bitmap, 0);

    // success;
    return;
}

void two_11_clear1_01 ( bitmap **pp_bitmap )
{
    
    // construct a 2-bit bitmap
    two_10_set0_11(pp_bitmap);

    // clear bit 0
    bitmap_clear(*pp_bitmap, 1);

    // success
    return;
}

void three_construct_000 ( bitmap **pp_bitmap )
{

    // construct a 3-bit bitmap
    bitmap_construct(pp_bitmap, 3);
    
    // success
    return;
}

void three_000_set0_001 ( bitmap **pp_bitmap )
{

    // construct a 3-bit bitmap
    three_construct_000(pp_bitmap);

    // set bit 0
    bitmap_set(*pp_bitmap, 0);

    // success
    return;
}

void three_000_set1_010 ( bitmap **pp_bitmap )
{

    // construct a 3-bit bitmap
    three_construct_000(pp_bitmap);

    // set bit 1
    bitmap_set(*pp_bitmap, 1);

    // success
    return;
}

void three_000_set2_100 ( bitmap **pp_bitmap )
{

    // construct a 3-bit bitmap
    three_construct_000(pp_bitmap);

    // set bit 2
    bitmap_set(*pp_bitmap, 2);

    // success
    return;
}

void three_100_set0_101 ( bitmap **pp_bitmap )
{

    // construct a 3-bit bitmap
    three_000_set2_100(pp_bitmap);

    // set bit 0
    bitmap_set(*pp_bitmap, 0);

    // success
    return;
}

void three_101_set1_111 ( bitmap **pp_bitmap )
{

    // construct a 3-bit bitmap
    three_100_set0_101(pp_bitmap);

    // set bit 1
    bitmap_set(*pp_bitmap, 1);

    // success
    return;
}