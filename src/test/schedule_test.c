/** !
 * schedule tester
 * 
 * @file src/test/schedule_test.c
 * 
 * @author Jacob Smith
 */

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// gsdk
/// core
#include <core/log.h>
#include <core/test.h>
#include <core/stream.h>
#include <core/pack.h>

/// performance
#include <performance/parallel.h>
#include <performance/schedule.h>

// preprocessor macros
#define DEFINE_TASK(id) void *task_##id ( void *p ) { return generic_task(p, id); }

// structure definitions
struct test_environment_s
{
    mutex   lock;
    stream *p_trace;
};

// type definitions
typedef struct test_environment_s test_environment;

// function declarations
/// scenario constructors
fn_scenario_constructor construct_nothing;

/// test cases
fn_test_case test_schedule_json;

/// allocators
fn_allocator destruct_nothing;

/// validator
int validate_trace(stream *p_trace, const char *path);

/// tasks
void *generic_task ( void *p_parameter, int id )
{

    // initialized data
    test_environment *p_environment = (test_environment *)p_parameter;

    // lock
    mutex_lock(&p_environment->lock);

    // write the ID to the stream
    pack_pack(p_environment->p_trace, "%i32", id);

    // unlock
    mutex_unlock(&p_environment->lock);

    // success
    return (void *) 1;
}

DEFINE_TASK(0)
DEFINE_TASK(1)
DEFINE_TASK(2)
DEFINE_TASK(3)
DEFINE_TASK(4)
DEFINE_TASK(5)
DEFINE_TASK(6)
DEFINE_TASK(7)
DEFINE_TASK(8)
DEFINE_TASK(9)
DEFINE_TASK(10)
DEFINE_TASK(11)
DEFINE_TASK(12)
DEFINE_TASK(13)
DEFINE_TASK(14)
DEFINE_TASK(15)
DEFINE_TASK(16)
DEFINE_TASK(17)
DEFINE_TASK(18)
DEFINE_TASK(19)
DEFINE_TASK(20)
DEFINE_TASK(21)
DEFINE_TASK(22)
DEFINE_TASK(23)
DEFINE_TASK(24)
DEFINE_TASK(25)
DEFINE_TASK(26)
DEFINE_TASK(27)
DEFINE_TASK(28)
DEFINE_TASK(29)
DEFINE_TASK(30)
DEFINE_TASK(31)

// test
/// cases
test_case _simple_test_cases[] = 
{
    TEST_CASE("base"      , test_schedule_json, "resources/test/schedule/base.json"      , TEST_RESULT_ONE),
    TEST_CASE("parallel"  , test_schedule_json, "resources/test/schedule/parallel.json"  , TEST_RESULT_ONE),
    TEST_CASE("sequential", test_schedule_json, "resources/test/schedule/sequential.json", TEST_RESULT_ONE),
};

test_case _fan_test_cases[] = 
{
    TEST_CASE("big fan"   , test_schedule_json, "resources/test/schedule/big_fan.json", TEST_RESULT_ONE),
    TEST_CASE("fan"       , test_schedule_json, "resources/test/schedule/fan.json"    , TEST_RESULT_ONE),
    TEST_CASE("fanin"     , test_schedule_json, "resources/test/schedule/fanin.json"  , TEST_RESULT_ONE),
    TEST_CASE("fanout"    , test_schedule_json, "resources/test/schedule/fanout.json" , TEST_RESULT_ONE),
};

test_case _wavefront_test_cases[] = 
{
    TEST_CASE("wavefront"    , test_schedule_json, "resources/test/schedule/wavefront.json"    , TEST_RESULT_ONE),
    TEST_CASE("wavefront 3x3", test_schedule_json, "resources/test/schedule/wavefront_3x3.json", TEST_RESULT_ONE),
    TEST_CASE("wavefront 4x4", test_schedule_json, "resources/test/schedule/wavefront_4x4.json", TEST_RESULT_ONE),
    TEST_CASE("wavefront 3x5", test_schedule_json, "resources/test/schedule/wavefront_3x5.json", TEST_RESULT_ONE),
};

/// scenarios
test_scenario _scenarios[] = 
{
    TEST_SCENARIO("simple"   , NULL, _simple_test_cases   , construct_nothing, destruct_nothing),
    TEST_SCENARIO("fan"      , NULL, _fan_test_cases      , construct_nothing, destruct_nothing),
    TEST_SCENARIO("wavefront", NULL, _wavefront_test_cases, construct_nothing, destruct_nothing),
};

/// suites
test_suite _suite = TEST_SUITE("schedule", _scenarios);

// entry point
int main ( int argc, const char *argv[] )
{

    // unused
    (void) argc;
    (void) argv;

    // register tasks
    parallel_register_task("0", task_0);
    parallel_register_task("1", task_1);
    parallel_register_task("2", task_2);
    parallel_register_task("3", task_3);
    parallel_register_task("4", task_4);
    parallel_register_task("5", task_5);
    parallel_register_task("6", task_6);
    parallel_register_task("7", task_7);
    parallel_register_task("8", task_8);
    parallel_register_task("9", task_9);
    parallel_register_task("10", task_10);
    parallel_register_task("11", task_11);
    parallel_register_task("12", task_12);
    parallel_register_task("13", task_13);
    parallel_register_task("14", task_14);
    parallel_register_task("15", task_15);
    parallel_register_task("16", task_16);
    parallel_register_task("17", task_17);
    parallel_register_task("18", task_18);
    parallel_register_task("19", task_19);
    parallel_register_task("20", task_20);
    parallel_register_task("21", task_21);
    parallel_register_task("22", task_22);
    parallel_register_task("23", task_23);
    parallel_register_task("24", task_24);
    parallel_register_task("25", task_25);
    parallel_register_task("26", task_26);
    parallel_register_task("27", task_27);
    parallel_register_task("28", task_28);
    parallel_register_task("29", task_29);
    parallel_register_task("30", task_30);
    parallel_register_task("31", task_31);

    // run the tests
    test_suite_test(&_suite);

    // done
    return (_suite.counters.total.fails == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int construct_nothing ( void **pp_result )
{

    // return a pointer to the caller
    *pp_result = NULL;

    // success
    return 1;
}

void *destruct_nothing ( void *p_pointer, unsigned long long size )
{
    
    // unused
    (void) p_pointer;
    (void) size;

    // success
    return NULL;
}

void *test_schedule_json ( test_case *p_test_case, void *p_subject )
{

    // unused
    (void) p_subject;
    
    // initialized data
    const char       *path          = (const char *) p_test_case->p_parameters;
    schedule         *p_schedule    = NULL;
    test_environment  environment   = { 0 };
    char              txt_path[256] = { 0 };
    char             *ext           = NULL;

    // construct a lock
    mutex_create(&environment.lock);

    // construct a stream
    stream_from_dynamic_buffer(&environment.p_trace);

    // load the schedule
    if ( 0 == schedule_load(&p_schedule, path) ) goto error;

    // start the schedule
    if ( 0 == schedule_start(p_schedule, &environment) )
    {

        // release the schedule
        schedule_destroy(&p_schedule);

        // error
        goto error;
    }

    // wait for the schedule to idle
    schedule_wait_idle(p_schedule);

    // store the path
    strcpy(txt_path, path);

    // find the '.'
    ext = strrchr(txt_path, '.');

    // '.json' -> '.txt'
    if ( ext ) 
        strcpy(ext, ".txt");

    // validate the trace
    if ( 0 == validate_trace(environment.p_trace, txt_path) )
    {

        // release the schedule
        schedule_destroy(&p_schedule);

        // error
        goto error;
    }

    // release the schedule
    schedule_destroy(&p_schedule);

    // release the stream
    stream_destroy(&environment.p_trace);

    // release the lock
    mutex_destroy(&environment.lock);

    // success
    return (void *) 1;

    error:

        // release the stream
        if (environment.p_trace) 
            stream_destroy(&environment.p_trace);

        // release the lock
        mutex_destroy(&environment.lock);

        // error
        return NULL;
}

int validate_trace ( stream *p_trace, const char *path )
{

    // initialized data
    FILE *p_f       = NULL;
    char  line[256] = { 0 };

    // open the file
    p_f = fopen(path, "r");
    if ( NULL == p_f ) return 0;

    // seek start
    stream_seek(p_trace, 0, STREAM_SEEK_SET);

    // read the file
    while (fgets(line, sizeof(line), p_f)) 
    {

        // initialized data
        int     expected_ids[10] = { 0 };
        int     actual_ids[10]   = { 0 };
        size_t  expected_count   = 0;
        char   *token            = strtok(line, ",\n ");
        
        // tokenize the line
        while (token) 
        {

            // store the ID
            expected_ids[expected_count++] = atoi(token);

            // next
            token = strtok(NULL, ",\n ");
        }

        // skip
        if ( 0 == expected_count ) continue;

        // build the trace
        for (size_t i = 0; i < expected_count; i++) 
        {

            // unpack the id
            if ( 0 == pack_unpack(p_trace, "%i32", &actual_ids[i]) ) 
            {

                // close the file
                fclose(p_f);

                // error
                return 0;
            }
        }

        // match?
        for (size_t i = 0; i < expected_count; i++) 
        {
            
            // initialized data
            bool found = false;

            // find
            for (size_t j = 0; j < expected_count; j++) 
            {

                // found?
                if ( actual_ids[i] == expected_ids[j] ) 
                {

                    // set the found flag
                    found = true;

                    // store a tombstone
                    expected_ids[j] = -1; 

                    // done
                    break;
                }
            }

            // found?
            if ( false == found )
            {

                // release the file
                fclose(p_f);

                // error
                return 0;
            }
        }
    }

    // release the file
    fclose(p_f);

    // success
    return 1;
}
