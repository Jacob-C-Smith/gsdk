/** !
 * sync example program
 * 
 * @file main.c
 * 
 * @author Jacob Smith
 */

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// log module
#include <core/log.h>

// sync 
#include <core/sync.h>

// preprocessor definitions
#define NTH_FIBONACCI_NUMBER 1000000000

// enumeration definitions
enum sync_examples_e
{
    SYNC_TIMER_EXAMPLE              = 0,
    SYNC_MUTEX_EXAMPLE              = 1,
    SYNC_SPINLOCK_EXAMPLE           = 2,
    SYNC_RW_LOCK_EXAMPLE            = 3,
    SYNC_SEMAPHORE_EXAMPLE          = 4,
    SYNC_CONDITION_VARIABLE_EXAMPLE = 5,
    SYNC_MONITOR_EXAMPLE            = 6,
    SYNC_BARRIER_EXAMPLE            = 7,
    SYNC_EXAMPLE_QUANTITY           = 8
};

// forward declarations
/** !
 * Print a usage message to standard out
 * 
 * @param argv0 the name of the program
 * 
 * @return void
 */
void print_usage ( const char *argv0 );

/** !
 * Parse command line arguments
 * 
 * @param argc            the argc parameter of the entry point
 * @param argv            the argv parameter of the entry point
 * @param examples_to_run return
 * 
 * @return void on success, program abort on failure
 */
void parse_command_line_arguments ( int argc, const char *argv[], bool *examples_to_run );

/** !
 * Timer example program
 * 
 * @param argc the argc parameter of the entry point
 * @param argv the argv parameter of the entry point
 * 
 * @return 1 on success, 0 on error
 */
int sync_timer_example ( int argc, const char *argv[] );

/** !
 * Mutex example program
 * 
 * @param argc the argc parameter of the entry point
 * @param argv the argv parameter of the entry point
 * 
 * @return 1 on success, 0 on error
 */
int sync_mutex_example ( int argc, const char *argv[] );

/** !
 * Spinlock example program
 * 
 * @param argc the argc parameter of the entry point
 * @param argv the argv parameter of the entry point
 * 
 * @return 1 on success, 0 on error
 */
// int sync_spinlock_example ( int argc, const char *argv[] );

/** !
 * Read Write Lock example program
 * 
 * @param argc the argc parameter of the entry point
 * @param argv the argv parameter of the entry point
 * 
 * @return 1 on success, 0 on error
 */
// int sync_read_write_lock_example ( int argc, const char *argv[] );

/** !
 * Semaphone example program
 * 
 * @param argc the argc parameter of the entry point
 * @param argv the argv parameter of the entry point
 * 
 * @return 1 on success, 0 on error
 */
int sync_semaphore_example ( int argc, const char *argv[] );

/** !
 * Condition variable example program
 * 
 * @param argc the argc parameter of the entry point
 * @param argv the argv parameter of the entry point
 * 
 * @return 1 on success, 0 on error
 */
int sync_condition_variable_example ( int argc, const char *argv[] );

/** !
 * Monitor example program
 * 
 * @param argc the argc parameter of the entry point
 * @param argv the argv parameter of the entry point
 * 
 * @return 1 on success, 0 on error
 */
int sync_monitor_example ( int argc, const char *argv[] );

/** !
 * Barrier example program
 * 
 * @param argc the argc parameter of the entry point
 * @param argv the argv parameter of the entry point
 * 
 * @return 1 on success, 0 on error
 */
// int sync_barrier_example ( int argc, const char *argv[] );

// entry point
int main ( int argc, const char *argv[] )
{

    // unused
    (void) argc;
    (void) argv;

    // initialized data
    bool examples_to_run[SYNC_EXAMPLE_QUANTITY] = { 0 };

    // Parse command line arguments
    parse_command_line_arguments(argc, argv, examples_to_run);

    // Formatting
    log_info("╔══════════════╗\n");
    log_info("║ sync example ║\n");
    log_info("╚══════════════╝\n");
    printf(
        "The sync library provides cross platform synchronization primitives and high-precision timing\n"\
        "Sync provides %d abstractions. The timer, the mutex, the spinlock, the semaphore, and the monitor.\n\n"\
        "A timer is used to collect high percision timestsamps.\n"\
        "A mutex is the most basic synchronization primitive. It provides mutual exclusion to code.\n"\
        "A spinlock is a type of mutex. It provides mutual exclusion to code.\n"\
        "A semaphore is an abstraction for controlling access to a resource.\n"\
        "A monitor is an abstraction for blocking and signaling threads in a critical section.\n\n",
        SYNC_EXAMPLE_QUANTITY
    );

    //////////////////////
    // Run the examples //
    //////////////////////

    // Run the thread example program
    if ( examples_to_run[SYNC_TIMER_EXAMPLE] )

        // error check
        if ( sync_timer_example(argc, argv) == 0 ) goto failed_to_run_timer_example;
    
    // Run the mutex example program
    if ( examples_to_run[SYNC_MUTEX_EXAMPLE] )

        // error check
        if ( sync_mutex_example(argc, argv) == 0 ) goto failed_to_run_mutex_example;
    
    // Run the spinlock example program
    if ( examples_to_run[SYNC_SPINLOCK_EXAMPLE] )

        // error check
        ; //if ( sync_spinlock_example(argc, argv) == 0 ) goto failed_to_run_spinlock_example;

    // Run the read write lock example program
    if ( examples_to_run[SYNC_RW_LOCK_EXAMPLE] )

        // error check
        ; //if ( sync_read_write_lock_example(argc, argv) == 0 ) goto failed_to_run_read_write_lock_example;
    
    // Run the semaphore example program
    if ( examples_to_run[SYNC_SEMAPHORE_EXAMPLE] )

        // error check
        if ( sync_semaphore_example(argc, argv) == 0 ) goto failed_to_run_semaphore_example;
    
    // Run the condition variable example program
    if ( examples_to_run[SYNC_CONDITION_VARIABLE_EXAMPLE] )

        // error check
        if ( sync_condition_variable_example(argc, argv) == 0 ) goto failed_to_run_condition_variable_example;
    
    // Run the monitor example program
    if ( examples_to_run[SYNC_MONITOR_EXAMPLE] )

        // error check
        if ( sync_monitor_example(argc, argv) == 0 ) goto failed_to_run_monitor_example;
    
    // Run the barrier example program
    if ( examples_to_run[SYNC_BARRIER_EXAMPLE] )

        // error check
        ; //if ( sync_barrier_example(argc, argv) == 0 ) goto failed_to_run_barrier_example;
    
    // success
    return EXIT_SUCCESS;

    // error handling
    {
        failed_to_run_timer_example:

            // Write an error message to standard out
            log_error("Error: Failed to run timer example!\n");

            // error
            return EXIT_FAILURE;

        failed_to_run_mutex_example:

            // Write an error message to standard out
            log_error("Error: Failed to run mutex example!\n");

            // error
            return EXIT_FAILURE;
            
        failed_to_run_semaphore_example:

            // Write an error message to standard out
            log_error("Error: Failed to run semaphore example!\n");

            // error
            return EXIT_FAILURE;
        
        failed_to_run_condition_variable_example:
        
            // Write an error message to standard out
            log_error("Error: Failed to run condition variable example!\n");

            // error
            return EXIT_FAILURE;

        failed_to_run_monitor_example:

            // Write an error message to standard out
            log_error("Error: Failed to run monitor example!\n");

            // error
            return EXIT_FAILURE;
    }
}

void print_usage ( const char *argv0 )
{

    // argument check
    if ( argv0 == (void *) 0 ) exit(EXIT_FAILURE);

    // Print a usage message to standard out
    printf("Usage: %s [timer] [mutex] [spinlock] [read-write] [semaphore] [condition-variable] [monitor] [barrier]\n", argv0);

    // done
    return;
}

void parse_command_line_arguments ( int argc, const char *argv[], bool *examples_to_run )
{

    // If no command line arguments are supplied, run all the examples
    if ( argc == 1 ) goto all_examples;

    // error check
    if ( argc > SYNC_EXAMPLE_QUANTITY + 1 ) goto invalid_arguments;

    // iterate through each command line argument
    for (int i = 1; i < argc; i++)
    {
        
        // Timer example?
        if ( strcmp(argv[i], "timer") == 0 )

            // Set the timer example flag
            examples_to_run[SYNC_TIMER_EXAMPLE] = true;

        // Mutex example?
        else if ( strcmp(argv[i], "mutex") == 0 )
            
            // Set the mutex example flag
            examples_to_run[SYNC_MUTEX_EXAMPLE] = true;

        // Spinlock example?
        else if ( strcmp(argv[i], "spinlock") == 0 )
            
            // Set the spinlock example flag
            examples_to_run[SYNC_SPINLOCK_EXAMPLE] = true;

        // Read Write Lock example?
        else if ( strcmp(argv[i], "read-write") == 0 )
            
            // Set the read write lock example flag
            examples_to_run[SYNC_RW_LOCK_EXAMPLE] = true;

        // Semaphore example?
        else if ( strcmp(argv[i], "semaphore") == 0 )

            // Set the semaphore flag
            examples_to_run[SYNC_SEMAPHORE_EXAMPLE] = true;
        
        // Monitor example?
        else if ( strcmp(argv[i], "monitor") == 0 )

            // Set the monitor flag
            examples_to_run[SYNC_MONITOR_EXAMPLE] = true;

        // Default
        else goto invalid_arguments;
    }
    
    // success
    return;

    // Set each example flag
    all_examples:
    {

        // For each example ...
        for (size_t i = 0; i < SYNC_EXAMPLE_QUANTITY; i++)
        
            // ... set the example flag
            examples_to_run[i] = true;
        
        // success
        return;
    }

    // error handling
    {

        // argument errors
        {
            invalid_arguments:
                
                // Print a usage message to standard out
                print_usage(argv[0]);

                // Abort
                exit(EXIT_FAILURE);
        }
    }
}

int sync_timer_example ( int argc, const char *argv[] )
{

    // Suppress warnings
    (void) argc;
    (void) argv;

    // initialized data
    size_t    c   = 0,
              d   = 1;
    double    sec = 0;
    timestamp t1  = 0, 
              t2  = 0,
              td  = 0;

    // Formatting
    log_info("╭───────────────╮\n");
    log_info("│ timer example │\n");
    log_info("╰───────────────╯\n");
    printf(
        "In this example, the timer is initialized. The program samples a timestamp t0,\n"\
        "computes the 1,000,000,000th fibonacci number, and samples another timestamp t1.\n"\
        "The difference of the timestamps (t1 - t0) is printed to standard out.\n\n"
    );

    // Record a timestamp
    t1 = timer_high_precision();

    // Iteratively calculate fib(NTH_FIBONACCI_NUMBER) 
    for (size_t i = 0; i < NTH_FIBONACCI_NUMBER; i++)
    {

        // initialized data
        size_t e = c + d;

        // Iteration
        c = d, d = e;
    }
    
    // Record another timestamp
    t2 = timer_high_precision();

    // Compute the difference
    td = t2 - t1;

    // Convert the difference to seconds
    sec = (double) td / (double) ( timer_seconds_divisor() );

    // Print the time difference in seconds
    (void)printf("%zu\r[sync] [timer] It took %lf seconds to calculate fib(%d)\n", d, sec, NTH_FIBONACCI_NUMBER);

    // Format
    putchar('\n');

    // success
    return 1;
}

int sync_mutex_example ( int argc, const char *argv[] )
{

    // Suppress warnings
    (void) argc;
    (void) argv;

    // initialized data
    mutex m = { 0 };

    // Formatting
    log_info("╭───────────────╮\n");
    log_info("│ mutex example │\n");
    log_info("╰───────────────╯\n");
    printf(
        "In this example, a mutex is created and locked. The program does a pretend\n"\
        "critical section, then the mutex is unlocked. Finally, the mutex is destroyed\n\n"
    );

    // Create
    if ( mutex_create(&m) == 0 ) return EXIT_FAILURE;

    // lock
    printf("[WAIT] Locking mutex ... \r");
    (void) mutex_lock(&m);
    log_info("[DONE]\n");

    // ... (Pretend) critical section ...
    printf("This message was printed from a critical section\n");

    // unlock
    printf("[WAIT] Unlocking mutex ... \r");
    (void) mutex_unlock(&m);
    log_info("[DONE]\n");

    // Destroy
    (void) mutex_destroy(&m);

    // Format
    putchar('\n');

    // success
    return 1;
}

/*
int sync_spinlock_example ( int argc, const char *argv[] )
{

    // Suppress warnings
    (void) argc;
    (void) argv;

    // initialized data
    spinlock sl = { 0 };

    // Formatting
    log_info("╭──────────────────╮\n");
    log_info("│ spinlock example │\n");
    log_info("╰──────────────────╯\n");
    printf(
        "In this example, a spinlock is created and locked. The program does a pretend\n"\
        "critical section, then the spinlock is unlocked. Finally, the spinlock is destroyed\n\n"
    );

    // Create
    if ( spinlock_create(&sl) == 0 ) return EXIT_FAILURE;

    // lock
    printf("[WAIT] Locking spinlock ... \r");
    spinlock_lock(&sl);
    log_info("[DONE]\n");

    // ... (Pretend) critical section ...
    printf("This message was printed from a critical section\n");

    // unlock
    printf("[WAIT] Unlocking spinlock ... \r");
    (void) spinlock_unlock(&sl);
    log_info("[DONE]\n");

    // Destroy
    (void) spinlock_destroy(&sl);

    // Format
    putchar('\n');

    // success
    return 1;
}

int sync_read_write_lock_example ( int argc, const char *argv[] )
{

    // Suppress warnings
    (void) argc;
    (void) argv;

    // initialized data
    rwlock rw = { 0 };

    // Formatting
    log_info("╭─────────────────────────╮\n");
    log_info("│ read write lock example │\n");
    log_info("╰─────────────────────────╯\n");
    printf(
        "In this example, a read write lock is created. [TODO] Finally, the read write lock is destroyed\n\n"
    );

    // Create
    if ( rwlock_create(&rw) ) return EXIT_FAILURE;

    // lock
    printf("[WAIT] Locking read write lock ... \r");
    rwlock_lock_wr(&rw);
    log_info("[DONE]\n");

    // ... (Pretend) critical section ...
    printf("This message was printed from a critical section\n");

    // unlock
    printf("[WAIT] Unlocking read write lock ... \r");
    rwlock_unlock(&rw);
    log_info("[DONE]\n");

    // Destroy
    (void) rwlock_destroy(&rw);

    // Format
    putchar('\n');

    // success
    return 1;
}
*/

int sync_semaphore_example ( int argc, const char *argv[] )
{

    // Suppress warnings
    (void) argc;
    (void) argv;

    // initialized data
    semaphore s = { 0 };
    
    // Formatting
    log_info("╭───────────────────╮\n");
    log_info("│ semaphore example │\n");
    log_info("╰───────────────────╯\n");
    printf(
        "In this example, a semaphore is created and locked. The program does a pretend\n"\
        "critical section, then the semaphore is unlocked. Finally, the semaphore is destroyed\n\n"
    );

    // Create
    if ( semaphore_create(&s, 1) == 0 ) return EXIT_FAILURE;
    
    // Wait
    (void) semaphore_wait(s);

    // ... (Pretend) critical section ...
    printf("This message was printed from a critical section\n");

    // Signal
    (void) semaphore_signal(s);

    // Destroy
    (void) semaphore_destroy(&s);

    // Format
    putchar('\n');

    // success
    return 1;
}

int sync_condition_variable_example ( int argc, const char *argv[] )
{

    // Suppress warnings
    (void) argc;
    (void) argv;

    // initialized data
    condition_variable c = { 0 };
    
    // Formatting
    log_info("╭────────────────────────────╮\n");
    log_info("│ condition variable example │\n");
    log_info("╰────────────────────────────╯\n");
    printf(
        "TODO\n\n"
    );

    // Create
    if ( condition_variable_create(&c) == 0 ) return EXIT_FAILURE;

    // TODO
    // 

    // Destroy
    (void) condition_variable_destroy(&c);

    // success
    return 1;
}

int sync_monitor_example ( int argc, const char *argv[] )
{

    // Suppress warnings
    (void) argc;
    (void) argv;

    // initialized data
    monitor m = { 0 };
    
    // Formatting
    log_info("╭─────────────────╮\n");
    log_info("│ monitor example │\n");
    log_info("╰─────────────────╯\n");
    printf(
        "TODO\n\n"
    );

    // Create
    if ( monitor_create(&m) == 0 ) return EXIT_FAILURE;

    // TODO
    //

    // Destroy
    (void) monitor_destroy(&m);

    // success
    return 1;
}

/*
int sync_barrier_example ( int argc, const char *argv[] )
{

    // Suppress warnings
    (void) argc;
    (void) argv;

    // initialized data
    barrier b = { 0 };
    
    // Formatting
    log_info("╭─────────────────╮\n");
    log_info("│ barrier example │\n");
    log_info("╰─────────────────╯\n");
    printf(
        "TODO\n\n"
    );

    // Create
    if ( barrier_create(&b, 0) == 0 ) return EXIT_FAILURE;

    // TODO: 

    // Destroy
    (void) barrier_destroy(&b);

    // Format
    putchar('\n');
    
    // success
    return 1;
}
*/