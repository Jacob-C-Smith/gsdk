/** !
 * Tester for x25519 module
 * 
 * @file src/test/x25519_test.c
 * 
 * @author Jacob Smith
 */

// gsdk
/// core
#include <core/log.h>
#include <core/sync.h>

/// crypto
#include <crypto/x25519.h>
#include <crypto/x25519_test.h>

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
 * Test x25519 
 * 
 * @param name the name of the test
 * 
 * @return void
 */
void test_x25519 ( char *name );

/** !
 * Test an x25519 vector
 * 
 * @param p_x25519_test_vector pointer to test vector
 * 
 * @return void
 */
bool test_x25519_vector ( x25519_test_vector *p_x25519_test_vector );

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
        "│ x25519 tester │\n"\
        "╰───────────────╯\n\n"
    );

    // start
    t0 = timer_high_precision();

    // run tests
    run_tests();

    // stop
    t1 = timer_high_precision();

    // report the time it took to run the tests
    log_info("x25519 tests took ");
    print_time_pretty ( (double) ( t1 - t0 ) / (double) timer_seconds_divisor() );
    log_info(" to test\n");

    // exit
    return ( total_passes == total_tests ) ? EXIT_SUCCESS : EXIT_FAILURE;
}

void run_tests ( void )
{

    // formatting
    log_scenario("%s\n", "x25519");

    // test vectors
    for (int i = 0; i < (int)(sizeof(_x25519_test_vectors)/sizeof(*_x25519_test_vectors)); i++)
    {

        // initialized data
        x25519_test_vector *p_test_vector = &_x25519_test_vectors[i];
        char _test_name[64] = { 0 };

        // construct the test name
        snprintf(_test_name, sizeof(_test_name), "test #%d", i);
        
        // run the test
        print_test("x25519", _test_name, test_x25519_vector(p_test_vector));
    }

    // print the summary of this test
    print_final_summary();

    // done
    return;
}

bool test_x25519_vector ( x25519_test_vector *p_x25519_test_vector )
{
    
    // initialized data
    x25519_shared_secret _shared_secret = { 0 };

    // derive the shared secret
    if ( 0 == x25519_shared_secret_derive(
        &p_x25519_test_vector->_private,
        &p_x25519_test_vector->_public,
        &_shared_secret
    ) ) 
        // error
        return false;

    // success
    if ( 0 == memcmp(_shared_secret, p_x25519_test_vector->_shared, sizeof(x25519_shared_secret)) ) 
        return true;
    
    // error
    return false;
}

void print_test ( const char *scenario_name, const char *test_name, bool passed )
{

    // initialized data
    if ( passed )
        log_pass("%s %s\n", scenario_name, test_name);
    else
        log_fail("%s %s\n", scenario_name, test_name);

    // increment the pass/fail counter
    if ( passed )
        ephemeral_passes++;
    else
        ephemeral_fails++;

    // increment the test counter
    ephemeral_tests++;

    // done
    return;
}

void print_final_summary ( void )
{

    // accumulate
    total_tests  += ephemeral_tests,
    total_passes += ephemeral_passes,
    total_fails  += ephemeral_fails;

    // print
    log_info("\nTests: %d, Passed: %d, Failed: %d (%%%.3f)\n",  ephemeral_tests, ephemeral_passes, ephemeral_fails, ((float)ephemeral_passes/(float)ephemeral_tests*100.f));
    log_info("Total: %d, Passed: %d, Failed: %d (%%%.3f)\n\n",  total_tests, total_passes, total_fails, ((float)total_passes/(float)total_tests*100.f));
    
    // clear test counters for this test
    ephemeral_tests  = 0;
    ephemeral_passes = 0;
    ephemeral_fails  = 0;

    // done
    return;
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
