/** !
 * Tester for base64 module
 * 
 * @file base64_test.c
 * 
 * @author Jacob Smith
 */

// core
#include <core/log.h>
#include <core/sync.h>

// reflection
#include <reflection/base64.h>

// enumeration definitions
enum result_e {
    zero  = 0,    
    one   = 1,
    match = 2
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
 * Test base64 decoding
 * 
 * @param name the name of the test
 * 
 * @return void
 */
void test_decode ( char *name );

/** !
 * Test base64 encoding 
 * 
 * @param name the name of the test
 * 
 * @return void
 */
void test_encode ( char *name );

bool test_encode_base64 ( char *test_text, char *expected_text, result_t expected );
bool test_decode_base64 ( char *test_text, char *expected_text, result_t expected );

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
        "╭───────────────╮\n"\
        "│ base64 tester │\n"\
        "╰───────────────╯\n\n"
    );

    // Start
    t0 = timer_high_precision();

    // Run tests
    run_tests();

    // Stop
    t1 = timer_high_precision();

    // Report the time it took to run the tests
    log_info("base64 tests took ");
    print_time_pretty ( (double) ( t1 - t0 ) / (double) timer_seconds_divisor() );
    log_info(" to test\n");

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

    // initialized data
    timestamp encode_t0 = 0,
              encode_t1 = 0,
              decode_t0 = 0,
              decode_t1 = 0;

    //////////////////////
    // Test the encoder //
    //////////////////////

    // Start
    encode_t0 = timer_high_precision();

    // Test encoding base64
    test_encode("encode");

    // Stop
    encode_t1 = timer_high_precision();

    // Report the time it took to run the serializer tests
    log_info("encode tests took: ");
    print_time_pretty ( (double)(encode_t1-encode_t0)/(double)timer_seconds_divisor() );
    log_info(" to test\n");

    //////////////////////
    // Test the decoder //
    //////////////////////

    // Start
    decode_t0 = timer_high_precision();

    // Test decoding base64
    test_decode("decode");
    
    // Stop
    decode_t1 = timer_high_precision();

    // Report the time it took to run the parser tests
    log_info("decode tests took: ");
    print_time_pretty ( (double)(decode_t1-decode_t0)/(double)timer_seconds_divisor() );
    log_info(" to test\n");

    // Done
    return;
}

void test_encode ( char *name )
{

    // Formatting
    log_scenario("%s\n", name);

    // Test encoding base64
    print_test(name, "(empty)", test_encode_base64("\0", "\0", match));
    print_test(name, "YQ", test_encode_base64("a\0", "YQ\0", match));
    print_test(name, "YWI", test_encode_base64("ab\0", "YWI\0", match));
    print_test(name, "YWJj", test_encode_base64("abc\0", "YWJj\0", match));
    print_test(name, "SGVsbG8sIFdvcmxkIQ", test_encode_base64("Hello, World!\0", "SGVsbG8sIFdvcmxkIQ\0", match));

    // Print the summary of this test
    print_final_summary();

    // Done
    return;
}

void test_decode ( char *name )
{

    // Formatting
    log_scenario("%s\n", name);

    // Test decoding base64
    print_test(name, "(empty)", test_decode_base64("\0", "\0", match));
    print_test(name, "\"a\"", test_decode_base64("YQ==\0", "a\0", match));
    print_test(name, "\"ab\"", test_decode_base64("YWI=\0", "ab\0", match));
    print_test(name, "\"abc\"", test_decode_base64("YWJj\0", "abc\0", match));
    print_test(name, "\"Hello, World!\"", test_decode_base64("SGVsbG8sIFdvcmxkIQ==\0", "Hello, World!\0", match));

    // Print the summary of this test
    print_final_summary();

    // Done
    return;
}

bool test_encode_base64 ( char *test_text, char *expected_text, result_t expected )
{
    
    // initialized data
    result_t result = zero;
    size_t input_length = strlen(test_text);
    char _encoded_text[65535] = { '\0' };
    memset(_encoded_text, 0, 65535);

    // Decode the text
    result = (result_t) base64_encode(test_text, input_length, _encoded_text);

    // Fast exit
    if ( result == zero ) goto done;

    // Check for equality
    if ( strcmp(_encoded_text, expected_text) == 0 ) result = match;

    done:

    // success
    return (result == expected);}

bool test_decode_base64 ( char *test_text, char *expected_text, result_t expected )
{
    
    // initialized data
    result_t result = zero;
    size_t input_length = strlen(test_text);
    char _decoded_text[65535];
    memset(_decoded_text, 0, 65535);

    // Decode the text
    result = (result_t) base64_decode(test_text, input_length, _decoded_text);

    // Fast exit
    if ( result == zero ) goto done;

    // Check for equality
    if ( strcmp(_decoded_text, expected_text) == 0 ) result = match;

    done:

    // success
    return (result == expected);
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
