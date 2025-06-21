/** !
 * double_queue tester
 * 
 * @file double_queue_test.c
 * 
 * @author Jacob Smith
 */

// standard library
#include <math.h>

// core
#include <core/sync.h>
#include <core/log.h>

// data
#include <data/double_queue.h>

// enumeration definitions
enum result_e {
    zero     = 0,
    one      = 1,
    match    = 2,
    not_null = 3
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

// Possible elements
void *A_element = (void *)0x1,
     *B_element = (void *)0x2,
     *C_element = (void *)0x3,
     *D_element = (void *)0x4,
     *X_element = (void *)0xFFFFFFFFFFFFFFFF;

// Expected results
void  *_contents    [] = { (void *)0x0 };

void  *A_contents   [] = { (void *)0x1, (void *)0x0 };
void  *B_contents   [] = { (void *)0x2, (void *)0x0 };
void  *C_contents   [] = { (void *)0x3, (void *)0x0 };

void  *AB_contents  [] = { (void *)0x1, (void *)0x2, (void *)0x0 };
void  *AC_contents  [] = { (void *)0x1, (void *)0x3, (void *)0x0 };
void  *BA_contents  [] = { (void *)0x2, (void *)0x1, (void *)0x0 };
void  *BC_contents  [] = { (void *)0x2, (void *)0x3, (void *)0x0 };
void  *CA_contents  [] = { (void *)0x3, (void *)0x1, (void *)0x0 };
void  *CB_contents  [] = { (void *)0x3, (void *)0x2, (void *)0x0 };

void  *ABC_contents [] = { (void *)0x1, (void *)0x2, (void *)0x3,  (void *)0x0 };
void  *ACB_contents [] = { (void *)0x1, (void *)0x3, (void *)0x2,  (void *)0x0 };
void  *BAC_contents [] = { (void *)0x2, (void *)0x1, (void *)0x3,  (void *)0x0 };
void  *BCA_contents [] = { (void *)0x2, (void *)0x3, (void *)0x1,  (void *)0x0 };
void  *CAB_contents [] = { (void *)0x3, (void *)0x1, (void *)0x2,  (void *)0x0 };
void  *CBA_contents [] = { (void *)0x3, (void *)0x2, (void *)0x1,  (void *)0x0 };

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
void double_queue_empty_tests ( const char *name );
void double_queue_one_element_tests ( const char *name );
void double_queue_two_element_tests ( const char *name );
void double_queue_three_element_tests ( const char *name );

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
    log_info("╭─────────────────────╮\n");
    log_info("│ double queue tester │\n");
    log_info("╰─────────────────────╯\n\n");

    // Start
    t0 = timer_high_precision();

    // Run tests
    run_tests();

    // Stop
    t1 = timer_high_precision();

    // Report the time it took to run the tests
    log_info("double queue tests lasted ");
    print_time_pretty ( (double) ( t1 - t0 ) / (double) timer_seconds_divisor() );
    putchar('\n');

    // exit
    return ( total_passes == total_tests ) ? EXIT_SUCCESS : EXIT_FAILURE;
}

void print_time_pretty ( double seconds )
{

    // initialized data
    double _seconds = seconds;
    size_t days = 0,
           hours = 0,
           minutes = 0,
           __seconds = 0,
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

    // Test empty double queue
    double_queue_empty_tests ("≷ ε ≷");

    // Test [ A ] double queue
    double_queue_one_element_tests ("≷[ A ]≷");

    // Test [ B ] double queue
    double_queue_one_element_tests ("≷[ B ]≷");

    // Test [ C ] double queue
    double_queue_one_element_tests ("≷[ C ]≷");

    // Test [ A, B ] double queue
    double_queue_two_element_tests ("≷[ A, B ]≷");

    // Test [ A, C ] double queue
    double_queue_two_element_tests ("≷[ A, C ]≷");

    // Test [ B, A ] double queue
    double_queue_two_element_tests ("≷[ B, A ]≷");

    // Test [ B, C ] double queue
    double_queue_two_element_tests ("≷[ B, C ]≷");

    // Test [ C, A ] double queue
    double_queue_two_element_tests ("≷[ C, A ]≷");

    // Test [ C, B ] double queue
    double_queue_two_element_tests ("≷[ C, B ]≷");

    // Test [ A, B, C ] double queue
    double_queue_three_element_tests ("≷[ A, B, C ]≷");

    // Test [ A, C, B ] double queue
    double_queue_three_element_tests ("≷[ A, C, B ]≷");

    // Test [ B, A, C ] double queue
    double_queue_three_element_tests ("≷[ B, A, C ]≷");

    // Test [ B, C, A ] double queue
    double_queue_three_element_tests ("≷[ B, C, A ]≷");

    // Test [ C, A, B ] double queue
    double_queue_three_element_tests ("≷[ C, A, B ]≷");

    // Test [ C, B, A ] double queue
    double_queue_three_element_tests ("≷[ C, B, A ]≷");


    // Done
    return;
}

void double_queue_empty_tests ( const char *name )
{
    
    // Formatting
    log_scenario("%s\n", name);

    // constructors
    print_test(name, "construct", false);//test_g_init(0, (void *) 0, match));
    print_test(name, "from contents", false);//test_g_init(0, (void *) 0, match));

    // accessors
    print_test(name, "front", false);//test_g_init("test cases/core/empty.json", (void *) 0, match));
    print_test(name, "rear", false);//test_g_init("test cases/core/empty.json", (void *) 0, match));
    print_test(name, "empty", false);//test_g_init("test cases/core/empty.json", (void *) 0, match));

    // mutators
    print_test(name, "front add", false);//test_g_init("test cases/core/empty_number.json", (void *) 0, match));
    print_test(name, "front remove", false);//test_g_init("test cases/core/empty_number.json", (void *) 0, match));
    print_test(name, "rear add", false);//test_g_init("test cases/core/empty_number.json", (void *) 0, match));
    print_test(name, "rear remove", false);//test_g_init("test cases/core/empty_number.json", (void *) 0, match));

    // destructors
    print_test(name, "destroy", false);//test_g_init("test cases/core/empty_object.json", (void *) 0, match));

    // Print the summary of this test
    print_final_summary();

    // success
    return;
}
void double_queue_one_element_tests ( const char *name )
{
    
    // Formatting
    log_scenario("%s\n", name);

    // constructors
    print_test(name, "construct", false);//test_g_init(0, (void *) 0, match));
    print_test(name, "from contents", false);//test_g_init(0, (void *) 0, match));

    // accessors
    print_test(name, "front", false);//test_g_init("test cases/core/empty.json", (void *) 0, match));
    print_test(name, "rear", false);//test_g_init("test cases/core/empty.json", (void *) 0, match));
    print_test(name, "empty", false);//test_g_init("test cases/core/empty.json", (void *) 0, match));

    // mutators
    print_test(name, "front add", false);//test_g_init("test cases/core/empty_number.json", (void *) 0, match));
    print_test(name, "front remove", false);//test_g_init("test cases/core/empty_number.json", (void *) 0, match));
    print_test(name, "rear add", false);//test_g_init("test cases/core/empty_number.json", (void *) 0, match));
    print_test(name, "rear remove", false);//test_g_init("test cases/core/empty_number.json", (void *) 0, match));

    // destructors
    print_test(name, "destroy", false);//test_g_init("test cases/core/empty_object.json", (void *) 0, match));

    // Print the summary of this test
    print_final_summary();

    // success
    return;
}
void double_queue_two_element_tests ( const char *name )
{
    
    // Formatting
    log_scenario("%s\n", name);

    // constructors
    print_test(name, "construct", false);
    print_test(name, "from contents", false);

    // accessors
    print_test(name, "front", false);
    print_test(name, "rear", false);
    print_test(name, "empty", false);

    // mutators
    print_test(name, "front add", false);
    print_test(name, "front remove", false);
    print_test(name, "rear add", false);
    print_test(name, "rear remove", false);

    // destructors
    print_test(name, "destroy", false);

    // Print the summary of this test
    print_final_summary();

    // success
    return;
}
void double_queue_three_element_tests ( const char *name )
{
    
    // Formatting
    log_scenario("%s\n", name);

    // constructors
    print_test(name, "construct", false);//test_g_init(0, (void *) 0, match));
    print_test(name, "from contents", false);//test_g_init(0, (void *) 0, match));

    // accessors
    print_test(name, "front", false);//test_g_init("test cases/core/empty.json", (void *) 0, match));
    print_test(name, "rear", false);//test_g_init("test cases/core/empty.json", (void *) 0, match));
    print_test(name, "empty", false);//test_g_init("test cases/core/empty.json", (void *) 0, match));

    // mutators
    print_test(name, "front add", false);//test_g_init("test cases/core/empty_number.json", (void *) 0, match));
    print_test(name, "front remove", false);//test_g_init("test cases/core/empty_number.json", (void *) 0, match));
    print_test(name, "rear add", false);//test_g_init("test cases/core/empty_number.json", (void *) 0, match));
    print_test(name, "rear remove", false);//test_g_init("test cases/core/empty_number.json", (void *) 0, match));

    // destructors
    print_test(name, "destroy", false);//test_g_init("test cases/core/empty_object.json", (void *) 0, match));

    // Print the summary of this test
    print_final_summary();

    // success
    return;
}

void print_test ( const char *scenario_name, const char *test_name, bool passed )
{

    // initialized data
    if ( passed )
        log_pass("%s %s\n",scenario_name, test_name),
        ephemeral_passes++;
    else 
        log_fail("%s %s\n", scenario_name, test_name),
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
    ephemeral_tests  = 0,
    ephemeral_passes = 0,
    ephemeral_fails  = 0;

    // Done
    return;
}