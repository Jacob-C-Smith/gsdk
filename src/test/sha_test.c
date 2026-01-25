/** !
 * Tester for sha module
 * 
 * @file sha_test.c
 * 
 * @author Jacob Smith
 */

// core
#include <core/log.h>
#include <core/sync.h>
#include <core/sha.h>

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

char _hegel_logic[922886] = { 0 };

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
 * @param name the name of the test suite
 * 
 * @return void
 */
void run_tests ( const char *name );

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

void sha256_test ( const char *scenario_name );
void sha512_test ( const char *scenario_name );

bool test_sha256 ( void *k, size_t len, sha256_hash *p_expected );
bool test_sha512 ( void *k, size_t len, sha512_hash *p_expected );

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
        "│ sha tester │\n"\
        "╰────────────╯\n\n"
    );

    // load a test file
    {

        // initialized data
        FILE *p_f = fopen("include/test/hegel_logic.txt", "r");
        fread(_hegel_logic, sizeof(_hegel_logic), 1, p_f);
        fclose(p_f);
    }

    // Start
    t0 = timer_high_precision();

    // Run tests
    run_tests("sha256");

    // Stop
    t1 = timer_high_precision();

    // Report the time it took to run the tests
    log_info("sha tests took ");
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
    if ( milliseconds ) log_info("%3zu ms, ", milliseconds);
    
    // Print microseconds
    if ( microseconds ) log_info("%03zu us", microseconds);
    
    // done
    return;
}

void run_tests ( const char *name )
{

    // test sha256
    sha256_test("sha256");

    // test sha512
    sha512_test("sha512");

    // done
    return;
}

void sha256_test ( const char *name )
{

    // initialized data
    sha256_hash _expected[] =
    { 
        { 0xe3, 0xb0, 0xc4, 0x42, 0x98, 0xfc, 0x1c, 0x14, 0x9a, 0xfb, 0xf4, 0xc8, 0x99, 0x6f, 0xb9, 0x24, 0x27, 0xae, 0x41, 0xe4, 0x64, 0x9b, 0x93, 0x4c, 0xa4, 0x95, 0x99, 0x1b, 0x78, 0x52, 0xb8, 0x55 },
        { 0xca, 0x97, 0x81, 0x12, 0xca, 0x1b, 0xbd, 0xca, 0xfa, 0xc2, 0x31, 0xb3, 0x9a, 0x23, 0xdc, 0x4d, 0xa7, 0x86, 0xef, 0xf8, 0x14, 0x7c, 0x4e, 0x72, 0xb9, 0x80, 0x77, 0x85, 0xaf, 0xee, 0x48, 0xbb },
        { 0x3e, 0x23, 0xe8, 0x16, 0x00, 0x39, 0x59, 0x4a, 0x33, 0x89, 0x4f, 0x65, 0x64, 0xe1, 0xb1, 0x34, 0x8b, 0xbd, 0x7a, 0x00, 0x88, 0xd4, 0x2c, 0x4a, 0xcb, 0x73, 0xee, 0xae, 0xd5, 0x9c, 0x00, 0x9d },
        { 0x2e, 0x7d, 0x2c, 0x03, 0xa9, 0x50, 0x7a, 0xe2, 0x65, 0xec, 0xf5, 0xb5, 0x35, 0x68, 0x85, 0xa5, 0x33, 0x93, 0xa2, 0x02, 0x9d, 0x24, 0x13, 0x94, 0x99, 0x72, 0x65, 0xa1, 0xa2, 0x5a, 0xef, 0xc6 },
        { 0xdf, 0xfd, 0x60, 0x21, 0xbb, 0x2b, 0xd5, 0xb0, 0xaf, 0x67, 0x62, 0x90, 0x80, 0x9e, 0xc3, 0xa5, 0x31, 0x91, 0xdd, 0x81, 0xc7, 0xf7, 0x0a, 0x4b, 0x28, 0x68, 0x8a, 0x36, 0x21, 0x82, 0x98, 0x6f },
        { 0x40, 0xb5, 0xdc, 0x08, 0x3e, 0xc0, 0x53, 0x40, 0xe0, 0xb5, 0x01, 0xdc, 0x18, 0x6c, 0x56, 0xa2, 0x54, 0xb7, 0x61, 0x26, 0xd6, 0xd3, 0x64, 0x7c, 0xba, 0xec, 0x8e, 0x04, 0xe4, 0xc8, 0xaa, 0xf0 }
    };

    // test SHA
    print_test(name, "(empty)"      , test_sha256(""             , 0 , &_expected[0]));
    print_test(name, "a"            , test_sha256("a"            , 1 , &_expected[1]));
    print_test(name, "b"            , test_sha256("b"            , 1 , &_expected[2]));
    print_test(name, "c"            , test_sha256("c"            , 1 , &_expected[3]));
    print_test(name, "Hello, World!", test_sha256("Hello, World!", 13, &_expected[4]));
    print_test(name, "Hegel Logic"  , test_sha256(_hegel_logic, sizeof(_hegel_logic), &_expected[5]));

    // print the summary of this test
    print_final_summary();

}

void sha512_test ( const char *name )
{

    // initialized data
    sha512_hash _expected[] =
    { 
        { 0xcf, 0x83, 0xe1, 0x35, 0x7e, 0xef, 0xb8, 0xbd, 0xf1, 0x54, 0x28, 0x50, 0xd6, 0x6d, 0x80, 0x07, 0xd6, 0x20, 0xe4, 0x05, 0x0b, 0x57, 0x15, 0xdc, 0x83, 0xf4, 0xa9, 0x21, 0xd3, 0x6c, 0xe9, 0xce, 0x47, 0xd0, 0xd1, 0x3c, 0x5d, 0x85, 0xf2, 0xb0, 0xff, 0x83, 0x18, 0xd2, 0x87, 0x7e, 0xec, 0x2f, 0x63, 0xb9, 0x31, 0xbd, 0x47, 0x41, 0x7a, 0x81, 0xa5, 0x38, 0x32, 0x7a, 0xf9, 0x27, 0xda, 0x3e },
        { 0x1f, 0x40, 0xfc, 0x92, 0xda, 0x24, 0x16, 0x94, 0x75, 0x09, 0x79, 0xee, 0x6c, 0xf5, 0x82, 0xf2, 0xd5, 0xd7, 0xd2, 0x8e, 0x18, 0x33, 0x5d, 0xe0, 0x5a, 0xbc, 0x54, 0xd0, 0x56, 0x0e, 0x0f, 0x53, 0x02, 0x86, 0x0c, 0x65, 0x2b, 0xf0, 0x8d, 0x56, 0x02, 0x52, 0xaa, 0x5e, 0x74, 0x21, 0x05, 0x46, 0xf3, 0x69, 0xfb, 0xbb, 0xce, 0x8c, 0x12, 0xcf, 0xc7, 0x95, 0x7b, 0x26, 0x52, 0xfe, 0x9a, 0x75 },
        { 0x52, 0x67, 0x76, 0x88, 0x22, 0xee, 0x62, 0x4d, 0x48, 0xfc, 0xe1, 0x5e, 0xc5, 0xca, 0x79, 0xcb, 0xd6, 0x02, 0xcb, 0x7f, 0x4c, 0x21, 0x57, 0xa5, 0x16, 0x55, 0x69, 0x91, 0xf2, 0x2e, 0xf8, 0xc7, 0xb5, 0xef, 0x7b, 0x18, 0xd1, 0xff, 0x41, 0xc5, 0x93, 0x70, 0xef, 0xb0, 0x85, 0x86, 0x51, 0xd4, 0x4a, 0x93, 0x6c, 0x11, 0xb7, 0xb1, 0x44, 0xc4, 0x8f, 0xe0, 0x4d, 0xf3, 0xc6, 0xa3, 0xe8, 0xda },
        { 0xac, 0xc2, 0x8d, 0xb2, 0xbe, 0xb7, 0xb4, 0x2b, 0xaa, 0x1c, 0xb0, 0x24, 0x3d, 0x40, 0x1c, 0xcb, 0x4e, 0x3f, 0xce, 0x44, 0xd7, 0xb0, 0x28, 0x79, 0xa5, 0x27, 0x99, 0xaa, 0xdf, 0xf5, 0x41, 0x52, 0x2d, 0x88, 0x22, 0x59, 0x8b, 0x2f, 0xa6, 0x64, 0xf9, 0xd5, 0x15, 0x6c, 0x00, 0xc9, 0x24, 0x80, 0x5d, 0x75, 0xc3, 0x86, 0x8b, 0xd5, 0x6c, 0x2a, 0xcb, 0x81, 0xd3, 0x7e, 0x98, 0xe3, 0x5a, 0xdc },
        { 0x37, 0x4d, 0x79, 0x4a, 0x95, 0xcd, 0xcf, 0xd8, 0xb3, 0x59, 0x93, 0x18, 0x5f, 0xef, 0x9b, 0xa3, 0x68, 0xf1, 0x60, 0xd8, 0xda, 0xf4, 0x32, 0xd0, 0x8b, 0xa9, 0xf1, 0xed, 0x1e, 0x5a, 0xbe, 0x6c, 0xc6, 0x92, 0x91, 0xe0, 0xfa, 0x2f, 0xe0, 0x00, 0x6a, 0x52, 0x57, 0x0e, 0xf1, 0x8c, 0x19, 0xde, 0xf4, 0xe6, 0x17, 0xc3, 0x3c, 0xe5, 0x2e, 0xf0, 0xa6, 0xe5, 0xfb, 0xe3, 0x18, 0xcb, 0x03, 0x87 },
        { 0x4e, 0xe8, 0xf3, 0xdb, 0x73, 0xcb, 0x3f, 0x07, 0x22, 0x01, 0x1b, 0x2b, 0x52, 0xe8, 0x97, 0x56, 0xd5, 0x99, 0x92, 0x6d, 0xe1, 0x24, 0xf3, 0x6a, 0x3a, 0xac, 0xb2, 0x10, 0x62, 0x4f, 0xa0, 0x16, 0x1a, 0x94, 0x79, 0xaf, 0x14, 0x29, 0x81, 0x54, 0xfc, 0x25, 0xd1, 0x0b, 0xba, 0xe4, 0xdb, 0x06, 0xee, 0x97, 0x1c, 0x36, 0x68, 0x3f, 0x46, 0xd9, 0x90, 0x6a, 0x9e, 0x74, 0x1c, 0xd2, 0xee, 0xe7 }
    };

    // test SHA
    print_test(name, "(empty)"      , test_sha512(""             , 0 , &_expected[0]));
    print_test(name, "a"            , test_sha512("a"            , 1 , &_expected[1]));
    print_test(name, "b"            , test_sha512("b"            , 1 , &_expected[2]));
    print_test(name, "c"            , test_sha512("c"            , 1 , &_expected[3]));
    print_test(name, "Hello, World!", test_sha512("Hello, World!", 13, &_expected[4]));
    print_test(name, "Hegel Logic"  , test_sha512(_hegel_logic, sizeof(_hegel_logic), &_expected[5]));

    // print the summary of this test
    print_final_summary();
}

bool test_sha256 ( void *k, size_t len, sha256_hash *p_expected )
{
    
    // initialized data
    sha256_state state = { 0 };
    sha256_hash result = { 0 };
    sha256_hash expected = { 0 };

    // copy the expected hash onto the stack
    memcpy(&expected, p_expected, sizeof(sha256_hash));

    // construct a hasher
    sha256_construct(&state);

    // feed the hasher
    if (len) sha256_update(&state, k, len);

    // digest the input
    sha256_final(&state, result);

    // success
    if ( memcmp(result, expected, sizeof(sha256_hash)) == 0 ) return 1;
    
    // log the error
    printf("\033[91m[sha512] given \"%s\", expected 0x", k),
    sha256_print(expected),
    printf(", got 0x"),
    sha256_print(result),
    printf("\033[0m\n");
    
    // error
    return 0;
}

bool test_sha512 ( void *k, size_t len, sha512_hash *p_expected )
{
    
    // initialized data
    sha512_state state = { 0 };
    sha512_hash result = { 0 };
    sha512_hash expected = { 0 };

    // copy the expected hash onto the stack
    memcpy(&expected, p_expected, sizeof(sha512_hash));

    // construct a hasher
    sha512_construct(&state);

    // feed the hasher
    if (len) sha512_update(&state, k, len);

    // digest the input
    sha512_final(&state, result);

    // success
    if ( memcmp(result, expected, sizeof(sha512_hash)) == 0 ) return 1;
    
    // log the error
    printf("\033[91m[sha512] given \"%s\", expected 0x", k),
    sha512_print(expected),
    printf(", got 0x"),
    sha512_print(result),
    printf("\033[0m\n");
    
    // error
    return 0;
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
