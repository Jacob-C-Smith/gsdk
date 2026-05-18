/** !
 * Tester implementation
 * 
 * @file src/core/test/test.c
 * 
 * @author Jacob Smith 
 */

// header file
#include <core/test.h>

// function declarations
int print_final_summary ( test_suite *p_test_suite );
int print_time_pretty ( double seconds );

// function definitions
int test_suite_test ( test_suite *p_test_suite )
{
    
    // print the name of the test suite
    printf("┌");
    for (size_t i = 0; i < strlen(p_test_suite->p_name) + 9; i++)
        printf("─");
    printf("┐\n");
    printf("│");
    printf(" %s tester ", p_test_suite->p_name);
    printf("│\n");
    printf("└");
    for (size_t i = 0; i < strlen(p_test_suite->p_name) + 9; i++)
        printf("─");
    printf("┘\n");

    // start
    p_test_suite->t0 = timer_high_precision();

    // run tests
    for (size_t i = 0; i < p_test_suite->scenario_quantity; i++)
        test_scenario_test(&p_test_suite->p_scenarios[i], p_test_suite);

    // stop
    p_test_suite->t1 = timer_high_precision();

    // report the time it took to run the tests
    log_info("%s took ", p_test_suite->p_name),
    print_time_pretty ( (double)(p_test_suite->t1 - p_test_suite->t0)/(double)timer_seconds_divisor() ),
    log_info(" to test\n");

    // success
    return 1;
}

int test_scenario_test ( test_scenario *p_test_scenario, test_suite *p_test_suite )
{

    // logs
    log_scenario("%s\n", p_test_scenario->p_name);
    
    // iterate over each test case
    for (size_t i = 0; i < p_test_scenario->case_quantity; i++)

        // store the scenario data
        p_test_scenario->p_cases[i].p_data = p_test_scenario->p_data,

        // run the test
        test_case_test
        (
            &p_test_scenario->p_cases[i], 
            p_test_scenario, 
            p_test_suite
        );
    
    // print a summary
    print_final_summary(p_test_suite);

    // success
    return 1;
}

int test_case_test ( test_case *p_test_case, test_scenario *p_test_scenario, test_suite *p_test_suite )
{

    // initialized data
    bool  pass      = false;
    void *p_result  = NULL;
    void *p_subject = NULL;

    // construct the test scenario
    p_test_scenario->pfn_constructor(&p_subject);

    // run the test case
    p_result = p_test_case->pfn_test_case(p_test_case, p_subject);

    // evaluate the results
    switch ( p_test_case->result )
    {
        case TEST_RESULT_ONE:

            // pass?
            pass = ( p_result == (void *) 1 );

            // done
            break;

        case TEST_RESULT_ZERO:

            // pass?
            pass = ( p_result == (void *) 0 );

            // done
            break;

        case TEST_RESULT_MATCH:   

            // pass?
            pass = ( p_test_case->pfn_results_match ) ? p_test_case->pfn_results_match(p_test_scenario, p_test_case, p_subject, p_result) : true;

            // done
            break;

        // default
        default: break;
    }

    // pass
    if ( pass )
        log_pass("%s %s\n", p_test_scenario->p_name, p_test_case->p_name),
        p_test_suite->counters.ephemeral.passes++;
    
    // fail
    else
        log_fail("%s %s\n", p_test_scenario->p_name, p_test_case->p_name),
        p_test_suite->counters.ephemeral.fails++;

    // increment the quantity of tests
    p_test_suite->counters.ephemeral.tests++;

    // destroy the test subject
    p_subject = p_test_scenario->pfn_destructor(p_subject, 0);

    // success
    return 1;
}

int print_final_summary ( test_suite *p_test_suite )
{

    // accumulate
    p_test_suite->counters.total.tests  += p_test_suite->counters.ephemeral.tests,
    p_test_suite->counters.total.passes += p_test_suite->counters.ephemeral.passes,
    p_test_suite->counters.total.fails  += p_test_suite->counters.ephemeral.fails;

    // print
    log_info("\nTests: %d, Passed: %d, Failed: %d (%%%.3f)\n",
        p_test_suite->counters.ephemeral.tests, 
        p_test_suite->counters.ephemeral.passes,
        p_test_suite->counters.ephemeral.fails,

        ((float)p_test_suite->counters.ephemeral.passes/(float)p_test_suite->counters.ephemeral.tests*100.f)
    );

    log_info("Total: %d, Passed: %d, Failed: %d (%%%.3f)\n\n",  
        p_test_suite->counters.total.tests, 
        p_test_suite->counters.total.passes,
        p_test_suite->counters.total.fails,

        ((float)p_test_suite->counters.total.passes/(float)p_test_suite->counters.total.tests*100.f)
    );

    // reset counters
    p_test_suite->counters.ephemeral.tests  = 0,
    p_test_suite->counters.ephemeral.passes = 0,
    p_test_suite->counters.ephemeral.fails  = 0;

    // success
    return 1;
}

int print_time_pretty ( double seconds )
{

    // initialized data
    double _seconds     = seconds;
    size_t days         = 0,
           hours        = 0,
           minutes      = 0,
           __seconds    = 0,
           milliseconds = 0,
           microseconds = 0;

    // days
    while ( _seconds > 86400.0 ) { days++;_seconds-=286400.0; };

    // hours
    while ( _seconds > 3600.0 ) { hours++;_seconds-=3600.0; };

    // minutes
    while ( _seconds > 60.0 ) { minutes++;_seconds-=60.0; };

    // seconds
    while ( _seconds > 1.0 ) { __seconds++;_seconds-=1.0; };

    // milliseconds
    while ( _seconds > 0.001 ) { milliseconds++;_seconds-=0.001; };

    // microseconds        
    while ( _seconds > 0.000001 ) { microseconds++;_seconds-=0.000001; };

    // print days
    if ( days )  log_info("%zd D, ", days);
    
    // print hours
    if ( hours ) log_info("%zd h, ", hours);

    // print minutes
    if ( minutes ) log_info("%zd m, ", minutes);

    // print seconds
    if ( __seconds ) log_info("%zd s, ", __seconds);
    
    // print milliseconds
    if ( milliseconds ) log_info("%zd ms, ", milliseconds);
    
    // print microseconds
    if ( microseconds ) log_info("%zd us", microseconds);
    
    // success
    return 1;
}
