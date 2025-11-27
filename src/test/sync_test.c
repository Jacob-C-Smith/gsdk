/** !
 * Tester for sync module
 * 
 * @file src/test/sync_test.c
 * 
 * @author Jacob Smith
 */

// options
#define TIMER_BIAS_POSITIVE
// #define TIMER_BIAS_NEGATIVE

// posix
#include <pthread.h>

// gsdk
/// core
#include <core/log.h>
#include <core/sync.h>

// enumeration definitions
enum result_e
{
    zero  = 0,    
    one   = 1,
    match = 2
};

enum workers_e
{
    ALICE           = 0,
    BOB             = 1,
    CAROL           = 2,
    DAVID           = 3,
    WORKER_QUANTITY = 4
};

// structure definitions
struct counter_s
{
    volatile size_t c;
    size_t upto;
    mutex  _mutex;
};

struct room_s
{
    size_t lim;
    size_t occupied;
    bool _spots[WORKER_QUANTITY];
    semaphore _semaphore;
};

// type definitions
typedef enum result_e result_t;
typedef struct counter_s counter;
typedef struct room_s room;

// global variables
int total_tests      = 0,
    total_passes     = 0,
    total_fails      = 0,
    ephemeral_tests  = 0,
    ephemeral_passes = 0,
    ephemeral_fails  = 0;

// workers
struct 
{

    // id
    enum workers_e _id;

    // name
    char *name;

    // posix
    pthread_t thread;
} 
_workers[WORKER_QUANTITY] = 
{
    [ALICE] = 
    { 
        ._id    = ALICE,
        .name   = "alice",
        .thread = { 0 }
    },
    [BOB] = 
    { 
        ._id    = BOB,
        .name   = "bob",
        .thread = { 0 }
    },
    [CAROL] = 
    { 
        ._id    = CAROL,
        .name   = "carol",
        .thread = { 0 }
    },
    [DAVID] = 
    { 
        ._id    = DAVID,
        .name   = "david",
        .thread = { 0 }
    }
};

// forward declarations
/// logs
/** !
 * Print the time formatted in days, hours, minutes, seconds, miliseconds, microseconds
 * 
 * @param seconds the time in seconds
 * 
 * @return void
 */
void print_time_pretty ( double seconds );

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

/// tests
/** !
 * Run all the tests
 * 
 * @param name the name of the test suite
 * 
 * @return void
 */
void run_tests ( const char *name );

/** !
 * Create a test scenario with a shared environment and worker threads
 * 
 * @param env            the environment
 * @param name           the name of the scenario
 * @param pfn_before     function run before test on environment IF NOT NULL ELSE unused
 * @param pfn_after      function run after test on environment IF NOT NULL ELSE unused
 * @param pfn_alice_task task alice does IF NOT NULL ELSE unused
 * @param pfn_bob_task   task bob does IF NOT NULL ELSE unused
 * @param pfn_carol_task task carol does IF NOT NULL ELSE unused
 * @param pfn_david_task task david does IF NOT NULL ELSE unused
 * 
 * @return true on success, false on failure
 */
bool test_factory
(
    void        *env,
    const char  *name,
    void       (*pfn_before)     ( void *env ),
    void       (*pfn_after)      ( void *env ),
    void       (*pfn_alice_task) ( void *env ),
    void       (*pfn_bob_task)   ( void *env ),
    void       (*pfn_carol_task) ( void *env ),
    void       (*pfn_david_task) ( void *env )
);

/** !
 * Helper for test factory
 * 
 * @param env      the environment
 * @param _id      the worker
 * @param pfn_task pointer to task for <_id> to do
 * 
 * @return void
 */
void double_dispatch
( 
    void *env, 
    enum workers_e _id, 
    void (*pfn_task)(void *env) 
);

/** !
 * Test counting without synchronization
 * 
 * @param name the name of the scenario
 * 
 * @return void
 */
void test_counter ( const char *name );

/** !
 * Test counting with a lock
 * 
 * @param name the name of the scenario
 * 
 * @return void
 */
void test_mutex ( const char *name );

/** !
 * Test limits with a semaphore
 * 
 * @param name the name of the scenario
 * 
 * @return void
 */
void test_semaphore ( const char *name );

/// counter
void increment_counter ( void *env );
void increment_locked_counter ( void *env );
void print_counter ( void *env );
void clear_counter ( void *env );

/// room
void bathroom ( void *env );

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
        "│ sync tester │\n"\
        "╰─────────────╯\n\n"
    );

    // start
    t0 = timer_high_precision();

    // Run tests
    run_tests("sync");

    // Stop
    t1 = timer_high_precision();

    // Report the time it took to run the tests
    log_info("sync tests took ");
    print_time_pretty ( (double) ( (unsigned long long)t1 - (unsigned long long)t0 ) / (double) timer_seconds_divisor() );
    log_info(" to test\n");

    // exit
    return ( total_passes == total_tests ) ? EXIT_SUCCESS : EXIT_FAILURE;
}

void run_tests ( const char *name )
{

    // run counter tests
    test_counter("count");

    // run mutex tests
    test_mutex("mutex");

    // run semaphore tests
    test_semaphore("semaphore");

    // done
    return;
}

void test_counter ( const char *name )
{

    // initialized data
    counter _c = 
    {
        .c = 0,
        .upto = 1000000,
        ._mutex = { 0 }
    };
    
    // log
    log_scenario("Count to 1,000,000\n");

    // test alice counting
    print_test(name, "alice", 
        test_factory(
            &_c,               // env
            "alice counting",    // name
            clear_counter,     // before
            print_counter,     // after

            increment_counter, // alice
            NULL,              // bob    
            NULL,              // carol    
            NULL               // dave 
        )
    );

    // test bob counting
    print_test(name, "bob", 
        test_factory(
            &_c,               // env
            "bob counting",    // name
            clear_counter,     // before
            print_counter,     // after

            NULL,              // alice
            increment_counter, // bob    
            NULL,              // carol    
            NULL               // dave 
        )
    );

    // test alice and bob counting
    print_test(name, "alice and bob", 
        test_factory(
            &_c,                      // env
            "alice and bob counting", // name
            clear_counter,            // before
            print_counter,            // after

            increment_counter,        // alice
            increment_counter,        // bob    
            NULL,                     // carol    
            NULL                      // dave   
        )
    );

    // test everyone counting
    print_test(name, "everyone", 
        test_factory(
            &_c,                 // env
            "everyone counting", // name
            clear_counter,       // before
            print_counter,       // after

            increment_counter,   // alice
            increment_counter,   // bob    
            increment_counter,   // carol    
            increment_counter    // dave   
        )
    );

    // results
    print_final_summary();

    // done
    return;
}

void test_mutex ( const char *name )
{

    // initialized data
    counter _c = 
    {
        .c = 0,
        .upto = 1000000,
        ._mutex = { 0 }
    };

    // log
    log_scenario("Count to 1,000,000 (Mutex)\n");

    // construct a mutex
    mutex_create(&_c._mutex);

    // test alice counting
    print_test(name, "alice", 
        test_factory(
            &_c,              // env
            "alice counting", // name
            clear_counter,    // before
            print_counter,    // after

            increment_locked_counter, // alice
            NULL,                     // bob    
            NULL,                     // carol    
            NULL                      // dave   
        )
    );

    // test bob, carol, and david counting
    print_test(name, "bob carol david", 
        test_factory(
            &_c,                          // env
            "bob, carol, david counting", // name
            clear_counter,                // before
            print_counter,                // after

            NULL,                         // alice
            increment_locked_counter,     // bob    
            increment_locked_counter,     // carol    
            increment_locked_counter      // dave   
        )
    );

    // test everyone counting
    print_test(name, "everyone", 
        test_factory(
            &_c,                 // env
            "everyone counting", // name
            clear_counter,       // before
            print_counter,       // after

            increment_locked_counter, // alice
            increment_locked_counter, // bob    
            increment_locked_counter, // carol    
            increment_locked_counter  // dave   
        )
    );

    // destroy the mutex
    mutex_destroy(&_c._mutex);

    // results
    print_final_summary();

    // done
    return;
}

void test_semaphore ( const char *name )
{

    // initialized data
    room _r = 
    {
        .lim = 1,
        ._spots = { 0 },
        ._semaphore = { 0 }
    };

    // log
    log_scenario("Semaphore\n");

    // construct a semaphore
    semaphore_create(&_r._semaphore, _r.lim);

    // test everyone 1 bathroom
    print_test(name, "everyone 1 bathroom", 
        test_factory(
            &_r,                   // env
            "everyone 1 bathroom", // name
            NULL,                  // before
            NULL,                  // after

            bathroom, // alice
            bathroom, // bob
            bathroom, // carol
            bathroom  // dave
        )
    );

    // make another bathroom
    _r.lim++,
    semaphore_destroy(&_r._semaphore),
    semaphore_create(&_r._semaphore, _r.lim);

    // test everyone 2 bathrooms
    print_test(name, "everyone 2 bathrooms", 
        test_factory(
            &_r,                    // env
            "everyone 2 bathrooms", // name
            NULL,                   // before
            NULL,                   // after

            bathroom, // alice
            bathroom, // bob    
            bathroom, // carol    
            bathroom  // dave   
        )
    );

    // make another bathroom
    _r.lim++,
    semaphore_destroy(&_r._semaphore),
    semaphore_create(&_r._semaphore, _r.lim);

    // test everyone 3 bathrooms
    print_test(name, "everyone 3 bathrooms", 
        test_factory(
            &_r,                    // env
            "everyone 3 bathrooms", // name
            NULL,                   // before
            NULL,                   // after

            bathroom, // alice
            bathroom, // bob    
            bathroom, // carol    
            bathroom  // dave   
        )
    );
}

void print_counter ( void *env )
{

    // logs
    printf("[counter] counter -> %lld\n", *(size_t*)env);

    // done
    return;
}

void increment_counter ( void *env )
{

    // initialized data
    counter *p_counter = (counter *) env;

    // count
    for (size_t i = 0; i < p_counter->upto; i++)
        p_counter->c++;
    
    // done
    return;
}

void increment_locked_counter ( void *env )
{

    // initialized data
    counter *p_counter = (counter *) env;

    // count
    for (size_t i = 0; i < p_counter->upto; i++)

        // lock
        mutex_lock(&p_counter->_mutex),

        // increment
        p_counter->c++,

        // unlock
        mutex_unlock(&p_counter->_mutex);
    
    // done
    return;
}

void clear_counter ( void *env )
{

    // initialized data
    counter *p_counter = (counter *) env;

    // clear
    p_counter->c = 0;
    
    // done
    return;
}

void bathroom ( void *env )
{

    // initialized data
    room *p_room = (room *) env;

    // wait
    sleep(1),
    semaphore_wait(p_room->_semaphore),
    p_room->occupied++,
    printf("\033[34m[bathroom]\033[0m \033[32m++\033[0m %d/%d\r", p_room->occupied, p_room->lim); fflush(stdout);

    // work
    sleep(3);

    // signal
    p_room->occupied--,
    printf("\033[34m[bathroom]\033[0m \033[31m--\033[0m %d/%d\r", p_room->occupied, p_room->lim); fflush(stdout),
    sleep(1),
    semaphore_signal(p_room->_semaphore);
    
    // done
    return;
}

bool test_factory (

    // env
    void *env,

    // name
    const char *name,

    // before the test
    void (*pfn_before) ( void *env ),

    // after the test
    void (*pfn_after) ( void *env ),
    
    // workers
    void (*pfn_alice_task) ( void *env ),
    void (*pfn_bob_task)   ( void *env ),
    void (*pfn_carol_task) ( void *env ),
    void (*pfn_david_task) ( void *env )
)
{

    // initialized data
    bool ret = true;

    // before
    if ( pfn_before ) pfn_before(env);

    // initialized data
    timestamp t0 = 0,
              t1 = 0;

    // start
    {

        // t0 before starting threads
        #ifdef TIMER_BIAS_POSITIVE 
            t0 = timer_high_precision();
        #endif

        // alice
        if ( pfn_alice_task )
            double_dispatch(env, ALICE, pfn_alice_task);
        
        // bob
        if ( pfn_bob_task )
            double_dispatch(env, BOB, pfn_bob_task);
        
        // carol
        if ( pfn_carol_task )
            double_dispatch(env, CAROL, pfn_carol_task);
        
        // david
        if ( pfn_david_task )
            double_dispatch(env, DAVID, pfn_david_task);

        // t0 after starting threads
        #ifdef TIMER_BIAS_NEGATIVE
            t0 = timer_high_precision();
        #endif
    }

    // join
    {

        // alice
        if ( pfn_alice_task )
            pthread_join(_workers[ALICE].thread, NULL);

        // bob
        if ( pfn_bob_task )
            pthread_join(_workers[BOB].thread, NULL);

        // carol
        if ( pfn_carol_task )
            pthread_join(_workers[CAROL].thread, NULL);

        // david
        if ( pfn_david_task )
            pthread_join(_workers[DAVID].thread, NULL);
    }
    
    // stop
    t1 = timer_high_precision();

    // after
    if ( pfn_after ) pfn_after(env);

    // report the time it took to run the test
    log_info("%s took ", name),
    print_time_pretty ( (double) ( (unsigned)t1 - (unsigned)t0 ) / (double) timer_seconds_divisor() ),
    putchar('\n');

    // done
    return ret;
}

void double_dispatch
( 
    void *env, 
    enum workers_e _id, 
    void (*pfn_task)(void *env) 
)
{

    // call
    pthread_create(
        &_workers[_id].thread,
        NULL,
        (void *(*)(void *))pfn_task,
        env
    );

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
