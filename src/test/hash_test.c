/** !
 * Tester for hash module
 * 
 * @file hash_test.c
 * 
 * @author Jacob Smith
 */

// core
#include <core/log.h>
#include <core/sync.h>
#include <core/hash.h>

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
 * Test hash decoding
 * 
 * @param name the name of the test
 * 
 * @return void
 */
void test_decode ( char *name );

/** !
 * Test fnv64 hash 
 * 
 * @param name the name of the test
 * 
 * @return void
 */
void test_fnv64 ( char *name );

/** !
 * Test crc64 hash 
 * 
 * @param name the name of the test
 * 
 * @return void
 */
void test_crc64 ( char *name );

/** !
 * Test mmh64 hash 
 * 
 * @param name the name of the test
 * 
 * @return void
 */
void test_mmh64 ( char *name );

bool test_hash ( fn_hash64 *pfn_hash64, char *k, size_t len, hash64 expected );

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
        "╭─────────────╮\n"\
        "│ hash tester │\n"\
        "╰─────────────╯\n\n"
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
    run_tests();

    // Stop
    t1 = timer_high_precision();

    // Report the time it took to run the tests
    log_info("hash tests took ");
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
    
    // Done
    return;
}

void run_tests ( void )
{

    // initialized data
    timestamp fnv64_t0 = 0,
              fnv64_t1 = 0,
              crc64_t0 = 0,
              crc64_t1 = 0,
              mmh64_t0 = 0,
              mmh64_t1 = 0;

    ////////////////////////////////
    // Test the hashing functions //
    ////////////////////////////////

    // start
    fnv64_t0 = timer_high_precision();

    // test the Fowler–Noll–Vo hash function
    test_fnv64("fnv64");

    // stop 
    fnv64_t1 = timer_high_precision();

    // Report the time it took to run the fnv tests
    log_info("fnv tests took: ");
    print_time_pretty ( (double)(fnv64_t1-fnv64_t0)/(double)timer_seconds_divisor() );
    log_info(" to test\n");

    // start
    crc64_t0 = timer_high_precision();

    // Test the cyclic redundancy check hash function
    test_crc64("crc64");

    // stop 
    crc64_t1 = timer_high_precision();

    // report the time it took to run the crc tests
    log_info("crc tests took: ");
    print_time_pretty ( (double)(crc64_t1-crc64_t0)/(double)timer_seconds_divisor() );
    log_info(" to test\n");

    // start
    mmh64_t0 = timer_high_precision();
    
    // test the mur mur hash function
    test_mmh64("mmh64");
    
    // stop 
    mmh64_t1 = timer_high_precision();

    // report the time it took to run the mmh tests
    log_info("mmh tests took: ");
    print_time_pretty ( (double)(mmh64_t1-mmh64_t0)/(double)timer_seconds_divisor() );
    log_info(" to test\n");

    // Done
    return;
}

void test_fnv64 ( char *name )
{

    // Formatting
    log_scenario("%s\n", name);

    // Test encoding hash
    print_test(name, "(empty)"      , test_hash(hash_fnv64, "\0"             , 0 , (hash64)0xc6a4a7935bd1e995));
    print_test(name, "a"            , test_hash(hash_fnv64, "a\0"            , 1 , (hash64)0x5bb2b36505b0899c));
    print_test(name, "b"            , test_hash(hash_fnv64, "b\0"            , 1 , (hash64)0x5bb2b66505b08eb5));
    print_test(name, "c"            , test_hash(hash_fnv64, "c\0"            , 1 , (hash64)0x5bb2b56505b08d02));
    print_test(name, "Hello, World!", test_hash(hash_fnv64, "Hello, World!\0", 14, (hash64)0xc7e6fdb452cc02c));
    print_test(name, "Hegel Logic"  , test_hash(hash_fnv64, _hegel_logic, sizeof(_hegel_logic), (hash64)0x16145b6b24cdc959));

    // Print the summary of this test
    print_final_summary();

    // Done
    return;
}

void test_crc64 ( char *name )
{

    // Formatting
    log_scenario("%s\n", name);

    // Test encoding hash
    print_test(name, "(empty)"      , test_hash(hash_crc64, "\0"             , 0 , (hash64)0x0));
    print_test(name, "a"            , test_hash(hash_crc64, "a\0"            , 1 , (hash64)0x330284772e652b05));
    print_test(name, "b"            , test_hash(hash_crc64, "b\0"            , 1 , (hash64)0x74a8fe9e8582d431));
    print_test(name, "c"            , test_hash(hash_crc64, "c\0"            , 1 , (hash64)0xc786b22086258b5e));
    print_test(name, "Hello, World!", test_hash(hash_crc64, "Hello, World!\0", 14, (hash64)0x68c3a1bd36cc75e5));
    print_test(name, "Hegel Logic"  , test_hash(hash_crc64, _hegel_logic, sizeof(_hegel_logic), (hash64)0xf201a56362a49880));

    // Print the summary of this test
    print_final_summary();

    // Done
    return;
}

void test_mmh64 ( char *name )
{

    // Formatting
    log_scenario("%s\n", name);

    // Test encoding hash
    print_test(name, "(empty)"      , test_hash(hash_mmh64, "\0"             , 0 , (hash64)0xc57390618bc85096));
    print_test(name, "a"            , test_hash(hash_mmh64, "a\0"            , 1 , (hash64)0x39d65d2080211453));
    print_test(name, "b"            , test_hash(hash_mmh64, "b\0"            , 1 , (hash64)0x8336f55eb0901fb4));
    print_test(name, "c"            , test_hash(hash_mmh64, "c\0"            , 1 , (hash64)0xb9232f902f66bf65));
    print_test(name, "Hello, World!", test_hash(hash_mmh64, "Hello, World!\0", 14, (hash64)0xb39ddc5fc6ef2a52));
    print_test(name, "Hegel Logic"  , test_hash(hash_mmh64, _hegel_logic, sizeof(_hegel_logic), (hash64)0x9f246debc321d559));

    // Print the summary of this test
    print_final_summary();

    // Done
    return;
}

bool test_hash ( fn_hash64 *pfn_hash64, char *k, size_t len, hash64 expected )
{
    
    // initialized data
    hash64 result = pfn_hash64(k, len);

    // success
    if ( result == expected ) 
        return 1;
    
    // log the error
    log_error("[hash] given \"%s\", expected 0x%llx, got 0x%llx\n", k, expected, result);

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
