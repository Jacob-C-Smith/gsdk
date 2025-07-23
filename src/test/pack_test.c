/** !
 * Tester for pack module
 * 
 * @file src/test/pack_test.c
 * 
 * @author Jacob Smith
 */

// standard library
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

// core
#include <core/log.h>
#include <core/sync.h>
#include <core/pack.h>

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

// forward declarations
void print_time_pretty ( double seconds );
void run_tests ( void );
void print_final_summary ( void );
void print_test ( const char *scenario_name, const char *test_name, bool passed );
bool test_pack ( char *format_string, const char *p_expected_file, result_t expected, ... );

bool test_pack_1i8 ( char *format_string, char _character, const char *p_expected, result_t expected );
bool test_pack_2i8 ( char *format_string, char _character1, char _character2, const char *p_expected, result_t expected );
bool test_pack_3i8 ( char *format_string, char _character1, char _character2, char _character3, const char *p_expected, result_t expected );

bool test_pack_1i16 ( char *format_string, short _short, const char *p_expected, result_t expected );
bool test_pack_2i16 ( char *format_string, short _short1, short _short2, const char *p_expected, result_t expected );
bool test_pack_3i16 ( char *format_string, short _short1, short _short2, short _short3, const char *p_expected, result_t expected );

bool test_pack_1i32 ( char *format_string, int _int, const char *p_expected, result_t expected );
bool test_pack_2i32 ( char *format_string, int _int1, int _int2, const char *p_expected, result_t expected );
bool test_pack_3i32 ( char *format_string, int _int1, int _int2, int _int3, const char *p_expected, result_t expected );

bool test_pack_1i64 ( char *format_string, long long _long_long, const char *p_expected, result_t expected );
bool test_pack_2i64 ( char *format_string, long long _long_long1, long long _long_long2, const char *p_expected, result_t expected );
bool test_pack_3i64 ( char *format_string, long long _long_long1, long long _long_long2, long long _long_long3, const char *p_expected, result_t expected );

bool test_pack_1f32 ( char *format_string, float _float, const char *p_expected, result_t expected );
bool test_pack_2f32 ( char *format_string, float _float1, float _float2, const char *p_expected, result_t expected );
bool test_pack_3f32 ( char *format_string, float _float1, float _float2, float _float3, const char *p_expected, result_t expected );

bool test_pack_1f64 ( char *format_string, double _double, const char *p_expected, result_t expected );
bool test_pack_2f64 ( char *format_string, double _double1, double _double2, const char *p_expected, result_t expected );
bool test_pack_3f64 ( char *format_string, double _double1, double _double2, double _double3, const char *p_expected, result_t expected );

bool test_pack_1s ( char *format_string, char *_string, const char *p_expected, result_t expected );
bool test_pack_2s ( char *format_string, char *_string1, char *_string2, const char *p_expected, result_t expected );
bool test_pack_3s ( char *format_string, char *_string1, char *_string2, char *_string3, const char *p_expected, result_t expected );

// Test scenarios
void test_pack_i8    ( char *name );
void test_pack_i16   ( char *name );
void test_pack_i32   ( char *name );
void test_pack_i64   ( char *name );
void test_pack_f32   ( char *name );
void test_pack_f64   ( char *name );
void test_pack_s     ( char *name );

// entry point
int main ( int argc, const char* argv[] )
{
    
    // Suppress compiler warnings
    (void) argc;
    (void) argv;

    // initialized data
    timestamp t0 = 0,
              t1 = 0;

    // Formatting
    printf(
        "╭─────────────╮\n"\
        "│ pack tester │\n"\
        "╰─────────────╯\n\n"
    );
    
    // Start
    t0 = timer_high_precision();

    // Run tests
    run_tests();

    // Stop
    t1 = timer_high_precision();

    // Report the time it took to run the tests
    log_info("\npack tests took ");
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
   
    // initialized data
    timestamp parser_t0 = 0,
              parser_t1 = 0,
              serial_t0 = 0,
              serial_t1 = 0;

    /////////////////////
    // Test the parser //
    /////////////////////

    // start
    parser_t0 = timer_high_precision();

    // stop
    parser_t1 = timer_high_precision();

    /////////////////////////
    // Test the serializer //
    /////////////////////////

    // start
    serial_t0 = timer_high_precision();

    // test packing i8
    test_pack_i8("pack i8");

    // test packing i16
    test_pack_i16("pack i16");

    // test packing i32
    test_pack_i32("pack i32");

    // test packing i64
    test_pack_i64("pack i64");

    // test packing f32
    test_pack_f32("pack f32");

    // test packing f64
    test_pack_f64("pack f64");

    // test packing strings
    test_pack_s("pack s");

    // stop
    serial_t1 = timer_high_precision();

    // Report the time it took to run the parser tests
    log_info("parser tests took: ");
    print_time_pretty ( (double)(parser_t1-parser_t0)/(double)timer_seconds_divisor() );
    log_info(" to test\n");

    // Report the time it took to run the serializer tests
    log_info("serial tests took: ");
    print_time_pretty ( (double)(serial_t1-serial_t0)/(double)timer_seconds_divisor() );
    log_info(" to test\n");

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

size_t load_file ( const char *path, void *buffer, bool binary_mode )
{

    // argument checking 
    if ( path == 0 ) goto no_path;

    // initialized data
    size_t  ret = 0;
    FILE   *f   = fopen(path, (binary_mode) ? "rb" : "r");
    
    // Check if file is valid
    if ( f == NULL ) goto invalid_file;

    // Find file size and prep for read
    fseek(f, 0, SEEK_END);
    ret = (size_t) ftell(f);
    fseek(f, 0, SEEK_SET);
    
    // Read to data
    if ( buffer ) 
        ret = fread(buffer, 1, ret, f);

    // The file is no longer needed
    fclose(f);
    
    // success
    return ret;

    // error handling
    {

        // argument errors
        {
            no_path:
                #ifndef NDEBUG
                    log_error("[json] Null path provided to function \"%s\n", __FUNCTION__);
                #endif

            // error
            return 0;
        }

        // File errors
        {
            invalid_file:
                #ifndef NDEBUG
                    printf("[Standard library] Failed to load file \"%s\". %s\n", path, strerror(errno));
                #endif

            // error
            return 0;
        }
    }
}

void test_pack_i8 ( char *name )
{

    // formatting
    log_scenario("%s\n", name);
    
    // tests
    print_test(name, "\"%i8\"  { 1 }", test_pack_1i8("%i8" , 1, "resources/test/pack/i8/1/1.bin", match));
    print_test(name, "\"%i8\"  { 2 }", test_pack_1i8("%i8" , 2, "resources/test/pack/i8/1/2.bin", match));
    print_test(name, "\"%i8\"  { 3 }", test_pack_1i8("%i8" , 3, "resources/test/pack/i8/1/3.bin", match));

    print_test(name, "\"%2i8\" { 1, 1 }", test_pack_2i8("%2i8", 1, 1, "resources/test/pack/i8/2/1_1.bin", match));
    print_test(name, "\"%2i8\" { 1, 2 }", test_pack_2i8("%2i8", 1, 2, "resources/test/pack/i8/2/1_2.bin", match));
    print_test(name, "\"%2i8\" { 1, 3 }", test_pack_2i8("%2i8", 1, 3, "resources/test/pack/i8/2/1_3.bin", match));
    print_test(name, "\"%2i8\" { 2, 1 }", test_pack_2i8("%2i8", 2, 1, "resources/test/pack/i8/2/2_1.bin", match));
    print_test(name, "\"%2i8\" { 2, 2 }", test_pack_2i8("%2i8", 2, 2, "resources/test/pack/i8/2/2_2.bin", match));
    print_test(name, "\"%2i8\" { 2, 3 }", test_pack_2i8("%2i8", 2, 3, "resources/test/pack/i8/2/2_3.bin", match));
    print_test(name, "\"%2i8\" { 3, 1 }", test_pack_2i8("%2i8", 3, 1, "resources/test/pack/i8/2/3_1.bin", match));
    print_test(name, "\"%2i8\" { 3, 2 }", test_pack_2i8("%2i8", 3, 2, "resources/test/pack/i8/2/3_2.bin", match));
    print_test(name, "\"%2i8\" { 3, 3 }", test_pack_2i8("%2i8", 3, 3, "resources/test/pack/i8/2/3_3.bin", match));

    print_test(name, "\"%3i8\" { 1, 1, 1 }", test_pack_3i8("%3i8", 1, 1, 1, "resources/test/pack/i8/3/1_1_1.bin", match));
    print_test(name, "\"%3i8\" { 1, 1, 2 }", test_pack_3i8("%3i8", 1, 1, 2, "resources/test/pack/i8/3/1_1_2.bin", match));
    print_test(name, "\"%3i8\" { 1, 1, 3 }", test_pack_3i8("%3i8", 1, 1, 3, "resources/test/pack/i8/3/1_1_3.bin", match));
    print_test(name, "\"%3i8\" { 1, 2, 1 }", test_pack_3i8("%3i8", 1, 2, 1, "resources/test/pack/i8/3/1_2_1.bin", match));
    print_test(name, "\"%3i8\" { 1, 2, 2 }", test_pack_3i8("%3i8", 1, 2, 2, "resources/test/pack/i8/3/1_2_2.bin", match));
    print_test(name, "\"%3i8\" { 1, 2, 3 }", test_pack_3i8("%3i8", 1, 2, 3, "resources/test/pack/i8/3/1_2_3.bin", match));
    print_test(name, "\"%3i8\" { 1, 3, 1 }", test_pack_3i8("%3i8", 1, 3, 1, "resources/test/pack/i8/3/1_3_1.bin", match));
    print_test(name, "\"%3i8\" { 1, 3, 2 }", test_pack_3i8("%3i8", 1, 3, 2, "resources/test/pack/i8/3/1_3_2.bin", match));
    print_test(name, "\"%3i8\" { 1, 3, 3 }", test_pack_3i8("%3i8", 1, 3, 3, "resources/test/pack/i8/3/1_3_3.bin", match));
    print_test(name, "\"%3i8\" { 2, 1, 1 }", test_pack_3i8("%3i8", 2, 1, 1, "resources/test/pack/i8/3/2_1_1.bin", match));
    print_test(name, "\"%3i8\" { 2, 1, 2 }", test_pack_3i8("%3i8", 2, 1, 2, "resources/test/pack/i8/3/2_1_2.bin", match));
    print_test(name, "\"%3i8\" { 2, 1, 3 }", test_pack_3i8("%3i8", 2, 1, 3, "resources/test/pack/i8/3/2_1_3.bin", match));
    print_test(name, "\"%3i8\" { 2, 2, 1 }", test_pack_3i8("%3i8", 2, 2, 1, "resources/test/pack/i8/3/2_2_1.bin", match));
    print_test(name, "\"%3i8\" { 2, 2, 2 }", test_pack_3i8("%3i8", 2, 2, 2, "resources/test/pack/i8/3/2_2_2.bin", match));
    print_test(name, "\"%3i8\" { 2, 2, 3 }", test_pack_3i8("%3i8", 2, 2, 3, "resources/test/pack/i8/3/2_2_3.bin", match));
    print_test(name, "\"%3i8\" { 2, 3, 1 }", test_pack_3i8("%3i8", 2, 3, 1, "resources/test/pack/i8/3/2_3_1.bin", match));
    print_test(name, "\"%3i8\" { 2, 3, 2 }", test_pack_3i8("%3i8", 2, 3, 2, "resources/test/pack/i8/3/2_3_2.bin", match));
    print_test(name, "\"%3i8\" { 2, 3, 3 }", test_pack_3i8("%3i8", 2, 3, 3, "resources/test/pack/i8/3/2_3_3.bin", match));
    print_test(name, "\"%3i8\" { 3, 1, 1 }", test_pack_3i8("%3i8", 3, 1, 1, "resources/test/pack/i8/3/3_1_1.bin", match));
    print_test(name, "\"%3i8\" { 3, 1, 2 }", test_pack_3i8("%3i8", 3, 1, 2, "resources/test/pack/i8/3/3_1_2.bin", match));
    print_test(name, "\"%3i8\" { 3, 1, 3 }", test_pack_3i8("%3i8", 3, 1, 3, "resources/test/pack/i8/3/3_1_3.bin", match));
    print_test(name, "\"%3i8\" { 3, 2, 1 }", test_pack_3i8("%3i8", 3, 2, 1, "resources/test/pack/i8/3/3_2_1.bin", match));
    print_test(name, "\"%3i8\" { 3, 2, 2 }", test_pack_3i8("%3i8", 3, 2, 2, "resources/test/pack/i8/3/3_2_2.bin", match));
    print_test(name, "\"%3i8\" { 3, 2, 3 }", test_pack_3i8("%3i8", 3, 2, 3, "resources/test/pack/i8/3/3_2_3.bin", match));
    print_test(name, "\"%3i8\" { 3, 3, 1 }", test_pack_3i8("%3i8", 3, 3, 1, "resources/test/pack/i8/3/3_3_1.bin", match));
    print_test(name, "\"%3i8\" { 3, 3, 2 }", test_pack_3i8("%3i8", 3, 3, 2, "resources/test/pack/i8/3/3_3_2.bin", match));
    print_test(name, "\"%3i8\" { 3, 3, 3 }", test_pack_3i8("%3i8", 3, 3, 3, "resources/test/pack/i8/3/3_3_3.bin", match));
    
    // Print the summary of this test
    print_final_summary();

    // done
    return;

    /*
        // Test i64
        long long i64_in = 1234567890123456789LL, i64_out = 0;
        pack_pack(buffer, "%i64", i64_in);
        pack_unpack(buffer, "%i64", &i64_out);
        print_test("Integers", "pack/unpack i64", i64_in == i64_out);
    */
}

void test_pack_i16 ( char *name )
{

    // formatting
    log_scenario("%s\n", name);
    
    // tests
    print_test(name, "\"%i16\"  { 1 }", test_pack_1i16("%i16" , 1, "resources/test/pack/i16/1/1.bin", match));
    print_test(name, "\"%i16\"  { 2 }", test_pack_1i16("%i16" , 2, "resources/test/pack/i16/1/2.bin", match));
    print_test(name, "\"%i16\"  { 3 }", test_pack_1i16("%i16" , 3, "resources/test/pack/i16/1/3.bin", match));

    print_test(name, "\"%2i16\" { 1, 1 }", test_pack_2i16("%2i16", 1, 1, "resources/test/pack/i16/2/1_1.bin", match));
    print_test(name, "\"%2i16\" { 1, 2 }", test_pack_2i16("%2i16", 1, 2, "resources/test/pack/i16/2/1_2.bin", match));
    print_test(name, "\"%2i16\" { 1, 3 }", test_pack_2i16("%2i16", 1, 3, "resources/test/pack/i16/2/1_3.bin", match));
    print_test(name, "\"%2i16\" { 2, 1 }", test_pack_2i16("%2i16", 2, 1, "resources/test/pack/i16/2/2_1.bin", match));
    print_test(name, "\"%2i16\" { 2, 2 }", test_pack_2i16("%2i16", 2, 2, "resources/test/pack/i16/2/2_2.bin", match));
    print_test(name, "\"%2i16\" { 2, 3 }", test_pack_2i16("%2i16", 2, 3, "resources/test/pack/i16/2/2_3.bin", match));
    print_test(name, "\"%2i16\" { 3, 1 }", test_pack_2i16("%2i16", 3, 1, "resources/test/pack/i16/2/3_1.bin", match));
    print_test(name, "\"%2i16\" { 3, 2 }", test_pack_2i16("%2i16", 3, 2, "resources/test/pack/i16/2/3_2.bin", match));
    print_test(name, "\"%2i16\" { 3, 3 }", test_pack_2i16("%2i16", 3, 3, "resources/test/pack/i16/2/3_3.bin", match));
    
    print_test(name, "\"%3i16\" { 1, 1, 1 }", test_pack_3i16("%3i16", 1, 1, 1, "resources/test/pack/i16/3/1_1_1.bin", match));
    print_test(name, "\"%3i16\" { 1, 1, 2 }", test_pack_3i16("%3i16", 1, 1, 2, "resources/test/pack/i16/3/1_1_2.bin", match));
    print_test(name, "\"%3i16\" { 1, 1, 3 }", test_pack_3i16("%3i16", 1, 1, 3, "resources/test/pack/i16/3/1_1_3.bin", match));
    print_test(name, "\"%3i16\" { 1, 2, 1 }", test_pack_3i16("%3i16", 1, 2, 1, "resources/test/pack/i16/3/1_2_1.bin", match));
    print_test(name, "\"%3i16\" { 1, 2, 2 }", test_pack_3i16("%3i16", 1, 2, 2, "resources/test/pack/i16/3/1_2_2.bin", match));
    print_test(name, "\"%3i16\" { 1, 2, 3 }", test_pack_3i16("%3i16", 1, 2, 3, "resources/test/pack/i16/3/1_2_3.bin", match));
    print_test(name, "\"%3i16\" { 1, 3, 1 }", test_pack_3i16("%3i16", 1, 3, 1, "resources/test/pack/i16/3/1_3_1.bin", match));
    print_test(name, "\"%3i16\" { 1, 3, 2 }", test_pack_3i16("%3i16", 1, 3, 2, "resources/test/pack/i16/3/1_3_2.bin", match));
    print_test(name, "\"%3i16\" { 1, 3, 3 }", test_pack_3i16("%3i16", 1, 3, 3, "resources/test/pack/i16/3/1_3_3.bin", match));
    print_test(name, "\"%3i16\" { 2, 1, 1 }", test_pack_3i16("%3i16", 2, 1, 1, "resources/test/pack/i16/3/2_1_1.bin", match));
    print_test(name, "\"%3i16\" { 2, 1, 2 }", test_pack_3i16("%3i16", 2, 1, 2, "resources/test/pack/i16/3/2_1_2.bin", match));
    print_test(name, "\"%3i16\" { 2, 1, 3 }", test_pack_3i16("%3i16", 2, 1, 3, "resources/test/pack/i16/3/2_1_3.bin", match));
    print_test(name, "\"%3i16\" { 2, 2, 1 }", test_pack_3i16("%3i16", 2, 2, 1, "resources/test/pack/i16/3/2_2_1.bin", match));
    print_test(name, "\"%3i16\" { 2, 2, 2 }", test_pack_3i16("%3i16", 2, 2, 2, "resources/test/pack/i16/3/2_2_2.bin", match));
    print_test(name, "\"%3i16\" { 2, 2, 3 }", test_pack_3i16("%3i16", 2, 2, 3, "resources/test/pack/i16/3/2_2_3.bin", match));
    print_test(name, "\"%3i16\" { 2, 3, 1 }", test_pack_3i16("%3i16", 2, 3, 1, "resources/test/pack/i16/3/2_3_1.bin", match));
    print_test(name, "\"%3i16\" { 2, 3, 2 }", test_pack_3i16("%3i16", 2, 3, 2, "resources/test/pack/i16/3/2_3_2.bin", match));
    print_test(name, "\"%3i16\" { 2, 3, 3 }", test_pack_3i16("%3i16", 2, 3, 3, "resources/test/pack/i16/3/2_3_3.bin", match));
    print_test(name, "\"%3i16\" { 3, 1, 1 }", test_pack_3i16("%3i16", 3, 1, 1, "resources/test/pack/i16/3/3_1_1.bin", match));
    print_test(name, "\"%3i16\" { 3, 1, 2 }", test_pack_3i16("%3i16", 3, 1, 2, "resources/test/pack/i16/3/3_1_2.bin", match));
    print_test(name, "\"%3i16\" { 3, 1, 3 }", test_pack_3i16("%3i16", 3, 1, 3, "resources/test/pack/i16/3/3_1_3.bin", match));
    print_test(name, "\"%3i16\" { 3, 2, 1 }", test_pack_3i16("%3i16", 3, 2, 1, "resources/test/pack/i16/3/3_2_1.bin", match));
    print_test(name, "\"%3i16\" { 3, 2, 2 }", test_pack_3i16("%3i16", 3, 2, 2, "resources/test/pack/i16/3/3_2_2.bin", match));
    print_test(name, "\"%3i16\" { 3, 2, 3 }", test_pack_3i16("%3i16", 3, 2, 3, "resources/test/pack/i16/3/3_2_3.bin", match));
    print_test(name, "\"%3i16\" { 3, 3, 1 }", test_pack_3i16("%3i16", 3, 3, 1, "resources/test/pack/i16/3/3_3_1.bin", match));
    print_test(name, "\"%3i16\" { 3, 3, 2 }", test_pack_3i16("%3i16", 3, 3, 2, "resources/test/pack/i16/3/3_3_2.bin", match));
    print_test(name, "\"%3i16\" { 3, 3, 3 }", test_pack_3i16("%3i16", 3, 3, 3, "resources/test/pack/i16/3/3_3_3.bin", match));
    

    // Print the summary of this test
    print_final_summary();

    // done
    return;
}

void test_pack_i32 ( char *name )
{

    // formatting
    log_scenario("%s\n", name);
    
    // tests
    print_test(name, "\"%i32\"  { 1 }", test_pack_1i32("%i32" , 1, "resources/test/pack/i32/1/1.bin", match));
    print_test(name, "\"%i32\"  { 2 }", test_pack_1i32("%i32" , 2, "resources/test/pack/i32/1/2.bin", match));
    print_test(name, "\"%i32\"  { 3 }", test_pack_1i32("%i32" , 3, "resources/test/pack/i32/1/3.bin", match));

    print_test(name, "\"%2i32\" { 1, 1 }", test_pack_2i32("%2i32", 1, 1, "resources/test/pack/i32/2/1_1.bin", match));
    print_test(name, "\"%2i32\" { 1, 2 }", test_pack_2i32("%2i32", 1, 2, "resources/test/pack/i32/2/1_2.bin", match));
    print_test(name, "\"%2i32\" { 1, 3 }", test_pack_2i32("%2i32", 1, 3, "resources/test/pack/i32/2/1_3.bin", match));
    print_test(name, "\"%2i32\" { 2, 1 }", test_pack_2i32("%2i32", 2, 1, "resources/test/pack/i32/2/2_1.bin", match));
    print_test(name, "\"%2i32\" { 2, 2 }", test_pack_2i32("%2i32", 2, 2, "resources/test/pack/i32/2/2_2.bin", match));
    print_test(name, "\"%2i32\" { 2, 3 }", test_pack_2i32("%2i32", 2, 3, "resources/test/pack/i32/2/2_3.bin", match));
    print_test(name, "\"%2i32\" { 3, 1 }", test_pack_2i32("%2i32", 3, 1, "resources/test/pack/i32/2/3_1.bin", match));
    print_test(name, "\"%2i32\" { 3, 2 }", test_pack_2i32("%2i32", 3, 2, "resources/test/pack/i32/2/3_2.bin", match));
    print_test(name, "\"%2i32\" { 3, 3 }", test_pack_2i32("%2i32", 3, 3, "resources/test/pack/i32/2/3_3.bin", match));
    
    print_test(name, "\"%3i32\" { 1, 1, 1 }", test_pack_3i32("%3i32", 1, 1, 1, "resources/test/pack/i32/3/1_1_1.bin", match));
    print_test(name, "\"%3i32\" { 1, 1, 2 }", test_pack_3i32("%3i32", 1, 1, 2, "resources/test/pack/i32/3/1_1_2.bin", match));
    print_test(name, "\"%3i32\" { 1, 1, 3 }", test_pack_3i32("%3i32", 1, 1, 3, "resources/test/pack/i32/3/1_1_3.bin", match));
    print_test(name, "\"%3i32\" { 1, 2, 1 }", test_pack_3i32("%3i32", 1, 2, 1, "resources/test/pack/i32/3/1_2_1.bin", match));
    print_test(name, "\"%3i32\" { 1, 2, 2 }", test_pack_3i32("%3i32", 1, 2, 2, "resources/test/pack/i32/3/1_2_2.bin", match));
    print_test(name, "\"%3i32\" { 1, 2, 3 }", test_pack_3i32("%3i32", 1, 2, 3, "resources/test/pack/i32/3/1_2_3.bin", match));
    print_test(name, "\"%3i32\" { 1, 3, 1 }", test_pack_3i32("%3i32", 1, 3, 1, "resources/test/pack/i32/3/1_3_1.bin", match));
    print_test(name, "\"%3i32\" { 1, 3, 2 }", test_pack_3i32("%3i32", 1, 3, 2, "resources/test/pack/i32/3/1_3_2.bin", match));
    print_test(name, "\"%3i32\" { 1, 3, 3 }", test_pack_3i32("%3i32", 1, 3, 3, "resources/test/pack/i32/3/1_3_3.bin", match));
    print_test(name, "\"%3i32\" { 2, 1, 1 }", test_pack_3i32("%3i32", 2, 1, 1, "resources/test/pack/i32/3/2_1_1.bin", match));
    print_test(name, "\"%3i32\" { 2, 1, 2 }", test_pack_3i32("%3i32", 2, 1, 2, "resources/test/pack/i32/3/2_1_2.bin", match));
    print_test(name, "\"%3i32\" { 2, 1, 3 }", test_pack_3i32("%3i32", 2, 1, 3, "resources/test/pack/i32/3/2_1_3.bin", match));
    print_test(name, "\"%3i32\" { 2, 2, 1 }", test_pack_3i32("%3i32", 2, 2, 1, "resources/test/pack/i32/3/2_2_1.bin", match));
    print_test(name, "\"%3i32\" { 2, 2, 2 }", test_pack_3i32("%3i32", 2, 2, 2, "resources/test/pack/i32/3/2_2_2.bin", match));
    print_test(name, "\"%3i32\" { 2, 2, 3 }", test_pack_3i32("%3i32", 2, 2, 3, "resources/test/pack/i32/3/2_2_3.bin", match));
    print_test(name, "\"%3i32\" { 2, 3, 1 }", test_pack_3i32("%3i32", 2, 3, 1, "resources/test/pack/i32/3/2_3_1.bin", match));
    print_test(name, "\"%3i32\" { 2, 3, 2 }", test_pack_3i32("%3i32", 2, 3, 2, "resources/test/pack/i32/3/2_3_2.bin", match));
    print_test(name, "\"%3i32\" { 2, 3, 3 }", test_pack_3i32("%3i32", 2, 3, 3, "resources/test/pack/i32/3/2_3_3.bin", match));
    print_test(name, "\"%3i32\" { 3, 1, 1 }", test_pack_3i32("%3i32", 3, 1, 1, "resources/test/pack/i32/3/3_1_1.bin", match));
    print_test(name, "\"%3i32\" { 3, 1, 2 }", test_pack_3i32("%3i32", 3, 1, 2, "resources/test/pack/i32/3/3_1_2.bin", match));
    print_test(name, "\"%3i32\" { 3, 1, 3 }", test_pack_3i32("%3i32", 3, 1, 3, "resources/test/pack/i32/3/3_1_3.bin", match));
    print_test(name, "\"%3i32\" { 3, 2, 1 }", test_pack_3i32("%3i32", 3, 2, 1, "resources/test/pack/i32/3/3_2_1.bin", match));
    print_test(name, "\"%3i32\" { 3, 2, 2 }", test_pack_3i32("%3i32", 3, 2, 2, "resources/test/pack/i32/3/3_2_2.bin", match));
    print_test(name, "\"%3i32\" { 3, 2, 3 }", test_pack_3i32("%3i32", 3, 2, 3, "resources/test/pack/i32/3/3_2_3.bin", match));
    print_test(name, "\"%3i32\" { 3, 3, 1 }", test_pack_3i32("%3i32", 3, 3, 1, "resources/test/pack/i32/3/3_3_1.bin", match));
    print_test(name, "\"%3i32\" { 3, 3, 2 }", test_pack_3i32("%3i32", 3, 3, 2, "resources/test/pack/i32/3/3_3_2.bin", match));
    print_test(name, "\"%3i32\" { 3, 3, 3 }", test_pack_3i32("%3i32", 3, 3, 3, "resources/test/pack/i32/3/3_3_3.bin", match));

    // Print the summary of this test
    print_final_summary();

    // done
    return;
}

void test_pack_i64 ( char *name )
{

    // formatting
    log_scenario("%s\n", name);
    
    // tests
    print_test(name, "\"%i64\"  { 1 }", test_pack_1i64("%i64" , 1, "resources/test/pack/i64/1/1.bin", match));
    print_test(name, "\"%i64\"  { 2 }", test_pack_1i64("%i64" , 2, "resources/test/pack/i64/1/2.bin", match));
    print_test(name, "\"%i64\"  { 3 }", test_pack_1i64("%i64" , 3, "resources/test/pack/i64/1/3.bin", match));

    print_test(name, "\"%2i64\" { 1, 1 }", test_pack_2i64("%2i64", 1, 1, "resources/test/pack/i64/2/1_1.bin", match));
    print_test(name, "\"%2i64\" { 1, 2 }", test_pack_2i64("%2i64", 1, 2, "resources/test/pack/i64/2/1_2.bin", match));
    print_test(name, "\"%2i64\" { 1, 3 }", test_pack_2i64("%2i64", 1, 3, "resources/test/pack/i64/2/1_3.bin", match));
    print_test(name, "\"%2i64\" { 2, 1 }", test_pack_2i64("%2i64", 2, 1, "resources/test/pack/i64/2/2_1.bin", match));
    print_test(name, "\"%2i64\" { 2, 2 }", test_pack_2i64("%2i64", 2, 2, "resources/test/pack/i64/2/2_2.bin", match));
    print_test(name, "\"%2i64\" { 2, 3 }", test_pack_2i64("%2i64", 2, 3, "resources/test/pack/i64/2/2_3.bin", match));
    print_test(name, "\"%2i64\" { 3, 1 }", test_pack_2i64("%2i64", 3, 1, "resources/test/pack/i64/2/3_1.bin", match));
    print_test(name, "\"%2i64\" { 3, 2 }", test_pack_2i64("%2i64", 3, 2, "resources/test/pack/i64/2/3_2.bin", match));
    print_test(name, "\"%2i64\" { 3, 3 }", test_pack_2i64("%2i64", 3, 3, "resources/test/pack/i64/2/3_3.bin", match));
    
    print_test(name, "\"%3i64\" { 1, 1, 1 }", test_pack_3i64("%3i64", 1, 1, 1, "resources/test/pack/i64/3/1_1_1.bin", match));
    print_test(name, "\"%3i64\" { 1, 1, 2 }", test_pack_3i64("%3i64", 1, 1, 2, "resources/test/pack/i64/3/1_1_2.bin", match));
    print_test(name, "\"%3i64\" { 1, 1, 3 }", test_pack_3i64("%3i64", 1, 1, 3, "resources/test/pack/i64/3/1_1_3.bin", match));
    print_test(name, "\"%3i64\" { 1, 2, 1 }", test_pack_3i64("%3i64", 1, 2, 1, "resources/test/pack/i64/3/1_2_1.bin", match));
    print_test(name, "\"%3i64\" { 1, 2, 2 }", test_pack_3i64("%3i64", 1, 2, 2, "resources/test/pack/i64/3/1_2_2.bin", match));
    print_test(name, "\"%3i64\" { 1, 2, 3 }", test_pack_3i64("%3i64", 1, 2, 3, "resources/test/pack/i64/3/1_2_3.bin", match));
    print_test(name, "\"%3i64\" { 1, 3, 1 }", test_pack_3i64("%3i64", 1, 3, 1, "resources/test/pack/i64/3/1_3_1.bin", match));
    print_test(name, "\"%3i64\" { 1, 3, 2 }", test_pack_3i64("%3i64", 1, 3, 2, "resources/test/pack/i64/3/1_3_2.bin", match));
    print_test(name, "\"%3i64\" { 1, 3, 3 }", test_pack_3i64("%3i64", 1, 3, 3, "resources/test/pack/i64/3/1_3_3.bin", match));
    print_test(name, "\"%3i64\" { 2, 1, 1 }", test_pack_3i64("%3i64", 2, 1, 1, "resources/test/pack/i64/3/2_1_1.bin", match));
    print_test(name, "\"%3i64\" { 2, 1, 2 }", test_pack_3i64("%3i64", 2, 1, 2, "resources/test/pack/i64/3/2_1_2.bin", match));
    print_test(name, "\"%3i64\" { 2, 1, 3 }", test_pack_3i64("%3i64", 2, 1, 3, "resources/test/pack/i64/3/2_1_3.bin", match));
    print_test(name, "\"%3i64\" { 2, 2, 1 }", test_pack_3i64("%3i64", 2, 2, 1, "resources/test/pack/i64/3/2_2_1.bin", match));
    print_test(name, "\"%3i64\" { 2, 2, 2 }", test_pack_3i64("%3i64", 2, 2, 2, "resources/test/pack/i64/3/2_2_2.bin", match));
    print_test(name, "\"%3i64\" { 2, 2, 3 }", test_pack_3i64("%3i64", 2, 2, 3, "resources/test/pack/i64/3/2_2_3.bin", match));
    print_test(name, "\"%3i64\" { 2, 3, 1 }", test_pack_3i64("%3i64", 2, 3, 1, "resources/test/pack/i64/3/2_3_1.bin", match));
    print_test(name, "\"%3i64\" { 2, 3, 2 }", test_pack_3i64("%3i64", 2, 3, 2, "resources/test/pack/i64/3/2_3_2.bin", match));
    print_test(name, "\"%3i64\" { 2, 3, 3 }", test_pack_3i64("%3i64", 2, 3, 3, "resources/test/pack/i64/3/2_3_3.bin", match));
    print_test(name, "\"%3i64\" { 3, 1, 1 }", test_pack_3i64("%3i64", 3, 1, 1, "resources/test/pack/i64/3/3_1_1.bin", match));
    print_test(name, "\"%3i64\" { 3, 1, 2 }", test_pack_3i64("%3i64", 3, 1, 2, "resources/test/pack/i64/3/3_1_2.bin", match));
    print_test(name, "\"%3i64\" { 3, 1, 3 }", test_pack_3i64("%3i64", 3, 1, 3, "resources/test/pack/i64/3/3_1_3.bin", match));
    print_test(name, "\"%3i64\" { 3, 2, 1 }", test_pack_3i64("%3i64", 3, 2, 1, "resources/test/pack/i64/3/3_2_1.bin", match));
    print_test(name, "\"%3i64\" { 3, 2, 2 }", test_pack_3i64("%3i64", 3, 2, 2, "resources/test/pack/i64/3/3_2_2.bin", match));
    print_test(name, "\"%3i64\" { 3, 2, 3 }", test_pack_3i64("%3i64", 3, 2, 3, "resources/test/pack/i64/3/3_2_3.bin", match));
    print_test(name, "\"%3i64\" { 3, 3, 1 }", test_pack_3i64("%3i64", 3, 3, 1, "resources/test/pack/i64/3/3_3_1.bin", match));
    print_test(name, "\"%3i64\" { 3, 3, 2 }", test_pack_3i64("%3i64", 3, 3, 2, "resources/test/pack/i64/3/3_3_2.bin", match));
    print_test(name, "\"%3i64\" { 3, 3, 3 }", test_pack_3i64("%3i64", 3, 3, 3, "resources/test/pack/i64/3/3_3_3.bin", match));

    // Print the summary of this test
    print_final_summary();

    // done
    return;
}

void test_pack_f32 ( char *name )
{

    // formatting
    log_scenario("%s\n", name);
    
    // tests
    print_test(name, "\"%f32\"  { 1.0 }", test_pack_1f32("%f32" , 1.0f, "resources/test/pack/f32/1/1.bin", match));
    print_test(name, "\"%f32\"  { 2.0 }", test_pack_1f32("%f32" , 2.0f, "resources/test/pack/f32/1/2.bin", match));
    print_test(name, "\"%f32\"  { 3.0 }", test_pack_1f32("%f32" , 3.0f, "resources/test/pack/f32/1/3.bin", match));

    print_test(name, "\"%2f32\" { 1.0, 1.0 }", test_pack_2f32("%2f32", 1.0f, 1.0f, "resources/test/pack/f32/2/1_1.bin", match));
    print_test(name, "\"%2f32\" { 1.0, 2.0 }", test_pack_2f32("%2f32", 1.0f, 2.0f, "resources/test/pack/f32/2/1_2.bin", match));
    print_test(name, "\"%2f32\" { 1.0, 3.0 }", test_pack_2f32("%2f32", 1.0f, 3.0f, "resources/test/pack/f32/2/1_3.bin", match));
    print_test(name, "\"%2f32\" { 2.0, 1.0 }", test_pack_2f32("%2f32", 2.0f, 1.0f, "resources/test/pack/f32/2/2_1.bin", match));
    print_test(name, "\"%2f32\" { 2.0, 2.0 }", test_pack_2f32("%2f32", 2.0f, 2.0f, "resources/test/pack/f32/2/2_2.bin", match));
    print_test(name, "\"%2f32\" { 2.0, 3.0 }", test_pack_2f32("%2f32", 2.0f, 3.0f, "resources/test/pack/f32/2/2_3.bin", match));
    print_test(name, "\"%2f32\" { 3.0, 1.0 }", test_pack_2f32("%2f32", 3.0f, 1.0f, "resources/test/pack/f32/2/3_1.bin", match));
    print_test(name, "\"%2f32\" { 3.0, 2.0 }", test_pack_2f32("%2f32", 3.0f, 2.0f, "resources/test/pack/f32/2/3_2.bin", match));
    print_test(name, "\"%2f32\" { 3.0, 3.0 }", test_pack_2f32("%2f32", 3.0f, 3.0f, "resources/test/pack/f32/2/3_3.bin", match));
    
    print_test(name, "\"%3f32\" { 1.0, 1.0, 1.0 }", test_pack_3f32("%3f32", 1.0f, 1.0f, 1.0f, "resources/test/pack/f32/3/1_1_1.bin", match));
    print_test(name, "\"%3f32\" { 1.0, 1.0, 2.0 }", test_pack_3f32("%3f32", 1.0f, 1.0f, 2.0f, "resources/test/pack/f32/3/1_1_2.bin", match));
    print_test(name, "\"%3f32\" { 1.0, 1.0, 3.0 }", test_pack_3f32("%3f32", 1.0f, 1.0f, 3.0f, "resources/test/pack/f32/3/1_1_3.bin", match));
    print_test(name, "\"%3f32\" { 1.0, 2.0, 1.0 }", test_pack_3f32("%3f32", 1.0f, 2.0f, 1.0f, "resources/test/pack/f32/3/1_2_1.bin", match));
    print_test(name, "\"%3f32\" { 1.0, 2.0, 2.0 }", test_pack_3f32("%3f32", 1.0f, 2.0f, 2.0f, "resources/test/pack/f32/3/1_2_2.bin", match));
    print_test(name, "\"%3f32\" { 1.0, 2.0, 3.0 }", test_pack_3f32("%3f32", 1.0f, 2.0f, 3.0f, "resources/test/pack/f32/3/1_2_3.bin", match));
    print_test(name, "\"%3f32\" { 1.0, 3.0, 1.0 }", test_pack_3f32("%3f32", 1.0f, 3.0f, 1.0f, "resources/test/pack/f32/3/1_3_1.bin", match));
    print_test(name, "\"%3f32\" { 1.0, 3.0, 2.0 }", test_pack_3f32("%3f32", 1.0f, 3.0f, 2.0f, "resources/test/pack/f32/3/1_3_2.bin", match));
    print_test(name, "\"%3f32\" { 1.0, 3.0, 3.0 }", test_pack_3f32("%3f32", 1.0f, 3.0f, 3.0f, "resources/test/pack/f32/3/1_3_3.bin", match));
    print_test(name, "\"%3f32\" { 2.0, 1.0, 1.0 }", test_pack_3f32("%3f32", 2.0f, 1.0f, 1.0f, "resources/test/pack/f32/3/2_1_1.bin", match));
    print_test(name, "\"%3f32\" { 2.0, 1.0, 2.0 }", test_pack_3f32("%3f32", 2.0f, 1.0f, 2.0f, "resources/test/pack/f32/3/2_1_2.bin", match));
    print_test(name, "\"%3f32\" { 2.0, 1.0, 3.0 }", test_pack_3f32("%3f32", 2.0f, 1.0f, 3.0f, "resources/test/pack/f32/3/2_1_3.bin", match));
    print_test(name, "\"%3f32\" { 2.0, 2.0, 1.0 }", test_pack_3f32("%3f32", 2.0f, 2.0f, 1.0f, "resources/test/pack/f32/3/2_2_1.bin", match));
    print_test(name, "\"%3f32\" { 2.0, 2.0, 2.0 }", test_pack_3f32("%3f32", 2.0f, 2.0f, 2.0f, "resources/test/pack/f32/3/2_2_2.bin", match));
    print_test(name, "\"%3f32\" { 2.0, 2.0, 3.0 }", test_pack_3f32("%3f32", 2.0f, 2.0f, 3.0f, "resources/test/pack/f32/3/2_2_3.bin", match));
    print_test(name, "\"%3f32\" { 2.0, 3.0, 1.0 }", test_pack_3f32("%3f32", 2.0f, 3.0f, 1.0f, "resources/test/pack/f32/3/2_3_1.bin", match));
    print_test(name, "\"%3f32\" { 2.0, 3.0, 2.0 }", test_pack_3f32("%3f32", 2.0f, 3.0f, 2.0f, "resources/test/pack/f32/3/2_3_2.bin", match));
    print_test(name, "\"%3f32\" { 2.0, 3.0, 3.0 }", test_pack_3f32("%3f32", 2.0f, 3.0f, 3.0f, "resources/test/pack/f32/3/2_3_3.bin", match));
    print_test(name, "\"%3f32\" { 3.0, 1.0, 1.0 }", test_pack_3f32("%3f32", 3.0f, 1.0f, 1.0f, "resources/test/pack/f32/3/3_1_1.bin", match));
    print_test(name, "\"%3f32\" { 3.0, 1.0, 2.0 }", test_pack_3f32("%3f32", 3.0f, 1.0f, 2.0f, "resources/test/pack/f32/3/3_1_2.bin", match));
    print_test(name, "\"%3f32\" { 3.0, 1.0, 3.0 }", test_pack_3f32("%3f32", 3.0f, 1.0f, 3.0f, "resources/test/pack/f32/3/3_1_3.bin", match));
    print_test(name, "\"%3f32\" { 3.0, 2.0, 1.0 }", test_pack_3f32("%3f32", 3.0f, 2.0f, 1.0f, "resources/test/pack/f32/3/3_2_1.bin", match));
    print_test(name, "\"%3f32\" { 3.0, 2.0, 2.0 }", test_pack_3f32("%3f32", 3.0f, 2.0f, 2.0f, "resources/test/pack/f32/3/3_2_2.bin", match));
    print_test(name, "\"%3f32\" { 3.0, 2.0, 3.0 }", test_pack_3f32("%3f32", 3.0f, 2.0f, 3.0f, "resources/test/pack/f32/3/3_2_3.bin", match));
    print_test(name, "\"%3f32\" { 3.0, 3.0, 1.0 }", test_pack_3f32("%3f32", 3.0f, 3.0f, 1.0f, "resources/test/pack/f32/3/3_3_1.bin", match));
    print_test(name, "\"%3f32\" { 3.0, 3.0, 2.0 }", test_pack_3f32("%3f32", 3.0f, 3.0f, 2.0f, "resources/test/pack/f32/3/3_3_2.bin", match));
    print_test(name, "\"%3f32\" { 3.0, 3.0, 3.0 }", test_pack_3f32("%3f32", 3.0f, 3.0f, 3.0f, "resources/test/pack/f32/3/3_3_3.bin", match));

    // Print the summary of this test
    print_final_summary();

    // done
    return;
}

void test_pack_f64 ( char *name )
{

    // formatting
    log_scenario("%s\n", name);
    
    // tests
    print_test(name, "\"%f64\"  { 1.0 }", test_pack_1f64("%f64" , 1.0, "resources/test/pack/f64/1/1.bin", match));
    print_test(name, "\"%f64\"  { 2.0 }", test_pack_1f64("%f64" , 2.0, "resources/test/pack/f64/1/2.bin", match));
    print_test(name, "\"%f64\"  { 3.0 }", test_pack_1f64("%f64" , 3.0, "resources/test/pack/f64/1/3.bin", match));

    print_test(name, "\"%2f64\" { 1.0, 1.0 }", test_pack_2f64("%2f64", 1.0, 1.0, "resources/test/pack/f64/2/1_1.bin", match));
    print_test(name, "\"%2f64\" { 1.0, 2.0 }", test_pack_2f64("%2f64", 1.0, 2.0, "resources/test/pack/f64/2/1_2.bin", match));
    print_test(name, "\"%2f64\" { 1.0, 3.0 }", test_pack_2f64("%2f64", 1.0, 3.0, "resources/test/pack/f64/2/1_3.bin", match));
    print_test(name, "\"%2f64\" { 2.0, 1.0 }", test_pack_2f64("%2f64", 2.0, 1.0, "resources/test/pack/f64/2/2_1.bin", match));
    print_test(name, "\"%2f64\" { 2.0, 2.0 }", test_pack_2f64("%2f64", 2.0, 2.0, "resources/test/pack/f64/2/2_2.bin", match));
    print_test(name, "\"%2f64\" { 2.0, 3.0 }", test_pack_2f64("%2f64", 2.0, 3.0, "resources/test/pack/f64/2/2_3.bin", match));
    print_test(name, "\"%2f64\" { 3.0, 1.0 }", test_pack_2f64("%2f64", 3.0, 1.0, "resources/test/pack/f64/2/3_1.bin", match));
    print_test(name, "\"%2f64\" { 3.0, 2.0 }", test_pack_2f64("%2f64", 3.0, 2.0, "resources/test/pack/f64/2/3_2.bin", match));
    print_test(name, "\"%2f64\" { 3.0, 3.0 }", test_pack_2f64("%2f64", 3.0, 3.0, "resources/test/pack/f64/2/3_3.bin", match));
    
    print_test(name, "\"%3f64\" { 1.0, 1.0, 1.0 }", test_pack_3f64("%3f64", 1.0, 1.0, 1.0, "resources/test/pack/f64/3/1_1_1.bin", match));
    print_test(name, "\"%3f64\" { 1.0, 1.0, 2.0 }", test_pack_3f64("%3f64", 1.0, 1.0, 2.0, "resources/test/pack/f64/3/1_1_2.bin", match));
    print_test(name, "\"%3f64\" { 1.0, 1.0, 3.0 }", test_pack_3f64("%3f64", 1.0, 1.0, 3.0, "resources/test/pack/f64/3/1_1_3.bin", match));
    print_test(name, "\"%3f64\" { 1.0, 2.0, 1.0 }", test_pack_3f64("%3f64", 1.0, 2.0, 1.0, "resources/test/pack/f64/3/1_2_1.bin", match));
    print_test(name, "\"%3f64\" { 1.0, 2.0, 2.0 }", test_pack_3f64("%3f64", 1.0, 2.0, 2.0, "resources/test/pack/f64/3/1_2_2.bin", match));
    print_test(name, "\"%3f64\" { 1.0, 2.0, 3.0 }", test_pack_3f64("%3f64", 1.0, 2.0, 3.0, "resources/test/pack/f64/3/1_2_3.bin", match));
    print_test(name, "\"%3f64\" { 1.0, 3.0, 1.0 }", test_pack_3f64("%3f64", 1.0, 3.0, 1.0, "resources/test/pack/f64/3/1_3_1.bin", match));
    print_test(name, "\"%3f64\" { 1.0, 3.0, 2.0 }", test_pack_3f64("%3f64", 1.0, 3.0, 2.0, "resources/test/pack/f64/3/1_3_2.bin", match));
    print_test(name, "\"%3f64\" { 1.0, 3.0, 3.0 }", test_pack_3f64("%3f64", 1.0, 3.0, 3.0, "resources/test/pack/f64/3/1_3_3.bin", match));
    print_test(name, "\"%3f64\" { 2.0, 1.0, 1.0 }", test_pack_3f64("%3f64", 2.0, 1.0, 1.0, "resources/test/pack/f64/3/2_1_1.bin", match));
    print_test(name, "\"%3f64\" { 2.0, 1.0, 2.0 }", test_pack_3f64("%3f64", 2.0, 1.0, 2.0, "resources/test/pack/f64/3/2_1_2.bin", match));
    print_test(name, "\"%3f64\" { 2.0, 1.0, 3.0 }", test_pack_3f64("%3f64", 2.0, 1.0, 3.0, "resources/test/pack/f64/3/2_1_3.bin", match));
    print_test(name, "\"%3f64\" { 2.0, 2.0, 1.0 }", test_pack_3f64("%3f64", 2.0, 2.0, 1.0, "resources/test/pack/f64/3/2_2_1.bin", match));
    print_test(name, "\"%3f64\" { 2.0, 2.0, 2.0 }", test_pack_3f64("%3f64", 2.0, 2.0, 2.0, "resources/test/pack/f64/3/2_2_2.bin", match));
    print_test(name, "\"%3f64\" { 2.0, 2.0, 3.0 }", test_pack_3f64("%3f64", 2.0, 2.0, 3.0, "resources/test/pack/f64/3/2_2_3.bin", match));
    print_test(name, "\"%3f64\" { 2.0, 3.0, 1.0 }", test_pack_3f64("%3f64", 2.0, 3.0, 1.0, "resources/test/pack/f64/3/2_3_1.bin", match));
    print_test(name, "\"%3f64\" { 2.0, 3.0, 2.0 }", test_pack_3f64("%3f64", 2.0, 3.0, 2.0, "resources/test/pack/f64/3/2_3_2.bin", match));
    print_test(name, "\"%3f64\" { 2.0, 3.0, 3.0 }", test_pack_3f64("%3f64", 2.0, 3.0, 3.0, "resources/test/pack/f64/3/2_3_3.bin", match));
    print_test(name, "\"%3f64\" { 3.0, 1.0, 1.0 }", test_pack_3f64("%3f64", 3.0, 1.0, 1.0, "resources/test/pack/f64/3/3_1_1.bin", match));
    print_test(name, "\"%3f64\" { 3.0, 1.0, 2.0 }", test_pack_3f64("%3f64", 3.0, 1.0, 2.0, "resources/test/pack/f64/3/3_1_2.bin", match));
    print_test(name, "\"%3f64\" { 3.0, 1.0, 3.0 }", test_pack_3f64("%3f64", 3.0, 1.0, 3.0, "resources/test/pack/f64/3/3_1_3.bin", match));
    print_test(name, "\"%3f64\" { 3.0, 2.0, 1.0 }", test_pack_3f64("%3f64", 3.0, 2.0, 1.0, "resources/test/pack/f64/3/3_2_1.bin", match));
    print_test(name, "\"%3f64\" { 3.0, 2.0, 2.0 }", test_pack_3f64("%3f64", 3.0, 2.0, 2.0, "resources/test/pack/f64/3/3_2_2.bin", match));
    print_test(name, "\"%3f64\" { 3.0, 2.0, 3.0 }", test_pack_3f64("%3f64", 3.0, 2.0, 3.0, "resources/test/pack/f64/3/3_2_3.bin", match));
    print_test(name, "\"%3f64\" { 3.0, 3.0, 1.0 }", test_pack_3f64("%3f64", 3.0, 3.0, 1.0, "resources/test/pack/f64/3/3_3_1.bin", match));
    print_test(name, "\"%3f64\" { 3.0, 3.0, 2.0 }", test_pack_3f64("%3f64", 3.0, 3.0, 2.0, "resources/test/pack/f64/3/3_3_2.bin", match));
    print_test(name, "\"%3f64\" { 3.0, 3.0, 3.0 }", test_pack_3f64("%3f64", 3.0, 3.0, 3.0, "resources/test/pack/f64/3/3_3_3.bin", match));

    // Print the summary of this test
    print_final_summary();

    // done
    return;
}

void test_pack_s ( char *name )
{
    
    // formatting
    log_scenario("%s\n", name);
    
    // empty string
    print_test(name, "\"%s\"  { \"\" }", test_pack_1s("%s", "", "resources/test/pack/s/1/empty.bin", match));

    // single character
    print_test(name, "\"%s\"  { \"a\" }", test_pack_1s("%s", "a", "resources/test/pack/s/1/a.bin", match));
    print_test(name, "\"%s\"  { \"b\" }", test_pack_1s("%s", "b", "resources/test/pack/s/1/b.bin", match));
    print_test(name, "\"%s\"  { \"c\" }", test_pack_1s("%s", "c", "resources/test/pack/s/1/c.bin", match));

    // text
    print_test(name, "\"%s\"  { \"jake\" }", test_pack_1s("%s", "jake", "resources/test/pack/s/1/jake.bin", match));

    // Print the summary of this test
    print_final_summary();

    // done
    return;
}

bool test_pack_1i8 ( char *format_string, char _character, const char *p_expected, result_t expected )
{
    
    // initialized data
    char     expected_buffer[1024] = { 0 },
             result_buffer  [1024] = { 0 };
    int      expected_size         = load_file(p_expected, 0, true),
             result_size           = 0;
    result_t result                = 0;

    // load the expected value
    load_file(p_expected, expected_buffer, true);

    // Pack the data
    result_size = pack_pack(result_buffer, format_string, _character);

    // check if the result size matches the expected size
    if ( result_size != expected_size ) result = zero;

    // check if the result matches the expected 
    if ( 0 == memcmp(expected_buffer, result_buffer, result_size) ) result = match;

    // Check if the result matches the expected value
    return (result == expected);
}

bool test_pack_2i8 ( char *format_string, char _character1, char _character2, const char *p_expected, result_t expected )
{
    
    // initialized data
    char     expected_buffer[1024] = { 0 },
             result_buffer  [1024] = { 0 };
    int      expected_size         = load_file(p_expected, 0, true),
             result_size           = 0;
    result_t result                = 0;

    // load the expected value
    load_file(p_expected, expected_buffer, true);

    // Pack the data
    result_size = pack_pack(result_buffer, format_string, _character1, _character2);

    // check if the result size matches the expected size
    if ( result_size != expected_size ) result = zero;

    // check if the result matches the expected 
    if ( 0 == memcmp(expected_buffer, result_buffer, result_size) ) result = match;

    // Check if the result matches the expected value
    return (result == expected);
}

bool test_pack_3i8 ( char *format_string, char _character1, char _character2, char _character3, const char *p_expected, result_t expected )
{
    
    // initialized data
    char     expected_buffer[1024] = { 0 },
             result_buffer  [1024] = { 0 };
    int      expected_size         = load_file(p_expected, 0, true),
             result_size           = 0;
    result_t result                = 0;

    // load the expected value
    load_file(p_expected, expected_buffer, true);

    // Pack the data
    result_size = pack_pack(result_buffer, format_string, _character1, _character2, _character3);

    // check if the result size matches the expected size
    if ( result_size != expected_size ) result = zero;

    // check if the result matches the expected 
    if ( 0 == memcmp(expected_buffer, result_buffer, result_size) ) result = match;

    // Check if the result matches the expected value
    return (result == expected);
}

bool test_pack_1i16 ( char *format_string, short _short, const char *p_expected, result_t expected )
{
    char     expected_buffer[1024] = { 0 },
             result_buffer  [1024] = { 0 };
    int      expected_size         = load_file(p_expected, 0, true),
             result_size           = 0;
    result_t result                = 0;

    load_file(p_expected, expected_buffer, true);

    result_size = pack_pack(result_buffer, format_string, _short);

    if ( result_size != expected_size ) result = zero;

    if ( 0 == memcmp(expected_buffer, result_buffer, result_size) ) result = match;

    return (result == expected);
}

bool test_pack_2i16 ( char *format_string, short _short1, short _short2, const char *p_expected, result_t expected )
{
    char     expected_buffer[1024] = { 0 },
             result_buffer  [1024] = { 0 };
    int      expected_size         = load_file(p_expected, 0, true),
             result_size           = 0;
    result_t result                = 0;

    load_file(p_expected, expected_buffer, true);

    result_size = pack_pack(result_buffer, format_string, _short1, _short2);

    if ( result_size != expected_size ) result = zero;

    if ( 0 == memcmp(expected_buffer, result_buffer, result_size) ) result = match;

    return (result == expected);
}

bool test_pack_3i16 ( char *format_string, short _short1, short _short2, short _short3, const char *p_expected, result_t expected )
{
    char     expected_buffer[1024] = { 0 },
             result_buffer  [1024] = { 0 };
    int      expected_size         = load_file(p_expected, 0, true),
             result_size           = 0;
    result_t result                = 0;

    load_file(p_expected, expected_buffer, true);

    result_size = pack_pack(result_buffer, format_string, _short1, _short2, _short3);

    if ( result_size != expected_size ) result = zero;

    if ( 0 == memcmp(expected_buffer, result_buffer, result_size) ) result = match;

    return (result == expected);
}

bool test_pack_1i32 ( char *format_string, int _int, const char *p_expected, result_t expected )
{
    char     expected_buffer[1024] = { 0 },
             result_buffer  [1024] = { 0 };
    int      expected_size         = load_file(p_expected, 0, true),
             result_size           = 0;
    result_t result                = 0;

    load_file(p_expected, expected_buffer, true);

    result_size = pack_pack(result_buffer, format_string, _int);

    if ( result_size != expected_size ) result = zero;

    if ( 0 == memcmp(expected_buffer, result_buffer, result_size) ) result = match;

    return (result == expected);
}

bool test_pack_2i32 ( char *format_string, int _int1, int _int2, const char *p_expected, result_t expected )
{
    char     expected_buffer[1024] = { 0 },
             result_buffer  [1024] = { 0 };
    int      expected_size         = load_file(p_expected, 0, true),
             result_size           = 0;
    result_t result                = 0;

    load_file(p_expected, expected_buffer, true);

    result_size = pack_pack(result_buffer, format_string, _int1, _int2);

    if ( result_size != expected_size ) result = zero;

    if ( 0 == memcmp(expected_buffer, result_buffer, result_size) ) result = match;

    return (result == expected);
}

bool test_pack_3i32 ( char *format_string, int _int1, int _int2, int _int3, const char *p_expected, result_t expected )
{
    char     expected_buffer[1024] = { 0 },
             result_buffer  [1024] = { 0 };
    int      expected_size         = load_file(p_expected, 0, true),
             result_size           = 0;
    result_t result                = 0;

    load_file(p_expected, expected_buffer, true);

    result_size = pack_pack(result_buffer, format_string, _int1, _int2, _int3);

    if ( result_size != expected_size ) result = zero;

    if ( 0 == memcmp(expected_buffer, result_buffer, result_size) ) result = match;

    return (result == expected);
}

bool test_pack_1i64 ( char *format_string, long long _long_long, const char *p_expected, result_t expected )
{
    char     expected_buffer[1024] = { 0 },
             result_buffer  [1024] = { 0 };
    int      expected_size         = load_file(p_expected, 0, true),
             result_size           = 0;
    result_t result                = 0;

    load_file(p_expected, expected_buffer, true);

    result_size = pack_pack(result_buffer, format_string, _long_long);

    if ( result_size != expected_size ) result = zero;

    if ( 0 == memcmp(expected_buffer, result_buffer, result_size) ) result = match;

    return (result == expected);
}

bool test_pack_2i64 ( char *format_string, long long _long_long1, long long _long_long2, const char *p_expected, result_t expected )
{
    char     expected_buffer[1024] = { 0 },
             result_buffer  [1024] = { 0 };
    int      expected_size         = load_file(p_expected, 0, true),
             result_size           = 0;
    result_t result                = 0;

    load_file(p_expected, expected_buffer, true);

    result_size = pack_pack(result_buffer, format_string, _long_long1, _long_long2);

    if ( result_size != expected_size ) result = zero;

    if ( 0 == memcmp(expected_buffer, result_buffer, result_size) ) result = match;

    return (result == expected);
}

bool test_pack_3i64 ( char *format_string, long long _long_long1, long long _long_long2, long long _long_long3, const char *p_expected, result_t expected )
{
    char     expected_buffer[1024] = { 0 },
             result_buffer  [1024] = { 0 };
    int      expected_size         = load_file(p_expected, 0, true),
             result_size           = 0;
    result_t result                = 0;

    load_file(p_expected, expected_buffer, true);

    result_size = pack_pack(result_buffer, format_string, _long_long1, _long_long2, _long_long3);

    if ( result_size != expected_size ) result = zero;

    if ( 0 == memcmp(expected_buffer, result_buffer, result_size) ) result = match;

    return (result == expected);
}

bool test_pack_1f32 ( char *format_string, float _float, const char *p_expected, result_t expected )
{
    char     expected_buffer[1024] = { 0 },
             result_buffer  [1024] = { 0 };
    int      expected_size         = load_file(p_expected, 0, true),
             result_size           = 0;
    result_t result                = 0;

    load_file(p_expected, expected_buffer, true);

    result_size = pack_pack(result_buffer, format_string, _float);

    if ( result_size != expected_size ) result = zero;

    if ( 0 == memcmp(expected_buffer, result_buffer, result_size) ) result = match;

    return (result == expected);
}

bool test_pack_2f32 ( char *format_string, float _float1, float _float2, const char *p_expected, result_t expected )
{
    char     expected_buffer[1024] = { 0 },
             result_buffer  [1024] = { 0 };
    int      expected_size         = load_file(p_expected, 0, true),
             result_size           = 0;
    result_t result                = 0;

    load_file(p_expected, expected_buffer, true);

    result_size = pack_pack(result_buffer, format_string, _float1, _float2);

    if ( result_size != expected_size ) result = zero;

    if ( 0 == memcmp(expected_buffer, result_buffer, result_size) ) result = match;

    return (result == expected);
}

bool test_pack_3f32 ( char *format_string, float _float1, float _float2, float _float3, const char *p_expected, result_t expected )
{
    char     expected_buffer[1024] = { 0 },
             result_buffer  [1024] = { 0 };
    int      expected_size         = load_file(p_expected, 0, true),
             result_size           = 0;
    result_t result                = 0;

    load_file(p_expected, expected_buffer, true);

    result_size = pack_pack(result_buffer, format_string, _float1, _float2, _float3);

    if ( result_size != expected_size ) result = zero;

    if ( 0 == memcmp(expected_buffer, result_buffer, result_size) ) result = match;

    return (result == expected);
}

bool test_pack_1f64 ( char *format_string, double _double, const char *p_expected, result_t expected )
{
    char     expected_buffer[1024] = { 0 },
             result_buffer  [1024] = { 0 };
    int      expected_size         = load_file(p_expected, 0, true),
             result_size           = 0;
    result_t result                = 0;

    load_file(p_expected, expected_buffer, true);

    result_size = pack_pack(result_buffer, format_string, _double);

    if ( result_size != expected_size ) result = zero;

    if ( 0 == memcmp(expected_buffer, result_buffer, result_size) ) result = match;

    return (result == expected);
}

bool test_pack_2f64 ( char *format_string, double _double1, double _double2, const char *p_expected, result_t expected )
{
    char     expected_buffer[1024] = { 0 },
             result_buffer  [1024] = { 0 };
    int      expected_size         = load_file(p_expected, 0, true),
             result_size           = 0;
    result_t result                = 0;

    load_file(p_expected, expected_buffer, true);

    result_size = pack_pack(result_buffer, format_string, _double1, _double2);

    if ( result_size != expected_size ) result = zero;

    if ( 0 == memcmp(expected_buffer, result_buffer, result_size) ) result = match;

    return (result == expected);
}

bool test_pack_3f64 ( char *format_string, double _double1, double _double2, double _double3, const char *p_expected, result_t expected )
{
    char     expected_buffer[1024] = { 0 },
             result_buffer  [1024] = { 0 };
    int      expected_size         = load_file(p_expected, 0, true),
             result_size           = 0;
    result_t result                = 0;

    load_file(p_expected, expected_buffer, true);

    result_size = pack_pack(result_buffer, format_string, _double1, _double2, _double3);

    if ( result_size != expected_size ) result = zero;

    if ( 0 == memcmp(expected_buffer, result_buffer, result_size) ) result = match;

    return (result == expected);
}

bool test_pack_1s ( char *format_string, char *_string, const char *p_expected, result_t expected )
{
    char     expected_buffer[1024] = { 0 },
             result_buffer  [1024] = { 0 };
    int      expected_size         = load_file(p_expected, 0, true),
             result_size           = 0;
    result_t result                = 0;

    load_file(p_expected, expected_buffer, true);

    result_size = pack_pack(result_buffer, format_string, _string);

    if ( result_size != expected_size ) result = zero;

    if ( 0 == memcmp(expected_buffer, result_buffer, result_size) ) result = match;

    return (result == expected);
}

bool test_pack_2s ( char *format_string, char *_string1, char *_string2, const char *p_expected, result_t expected )
{
    char     expected_buffer[1024] = { 0 },
             result_buffer  [1024] = { 0 };
    int      expected_size         = load_file(p_expected, 0, true),
             result_size           = 0;
    result_t result                = 0;

    load_file(p_expected, expected_buffer, true);

    result_size = pack_pack(result_buffer, format_string, _string1, _string2);

    if ( result_size != expected_size ) result = zero;

    if ( 0 == memcmp(expected_buffer, result_buffer, result_size) ) result = match;

    return (result == expected);
}

bool test_pack_3s ( char *format_string, char *_string1, char *_string2, char *_string3, const char *p_expected, result_t expected )
{
    char     expected_buffer[1024] = { 0 },
             result_buffer  [1024] = { 0 };
    int      expected_size         = load_file(p_expected, 0, true),
             result_size           = 0;
    result_t result                = 0;

    load_file(p_expected, expected_buffer, true);

    result_size = pack_pack(result_buffer, format_string, _string1, _string2, _string3);

    if ( result_size != expected_size ) result = zero;

    if ( 0 == memcmp(expected_buffer, result_buffer, result_size) ) result = match;

    return (result == expected);
}
