/** !
 * Tests path module
 *
 * @file path_test.c
 *
 * @author Jacob Smith
 */

/** !
 * Commentary
 *
 *   Testing paths is a lot like testing JSON. Paths are an abstraction of file
 *   systems, and file systems exist on hard drives. Hard drives are linearly
 *   indexed. That is to say, the file system can be serialized. JSON can also
 *   be serialized. Thus, the file system can be encoded as JSON. The representation
 *   I've chosen looks like this:
 *
 *    - FILE SYSTEM -                          | - JSON -
 *      +--- A                                 |   {
 *      |    +                                 |       "A" : { },
 *      |                                      |       "B" : {
 *      +--- B                                 |          "email address.txt" : 30,
 *      |    +--- email address.txt (30 bytes) |          "birthday.txt" : 6,
 *      |    +--- birthday.txt (6 bytes)       |       },
 *      |                                      |       "C" : {
 *      +--- C                                 |          "file.c" : 8,
 *          +--- file C.txt (8 byte)           |          "code" : { "path.h" : 600 }
 *           +--- code                         |       }
 *                + path.h (600 bytes)         |   }
 *
 *   TODO: The value_equals function has been included from the json_test.h
 *         header. The path_as_json function shall serialize a file system from a path
 *         in accordance with the above encoding. The value_equals and path_as_json
 *         functions make it trivial to compare a path object to an expected result
 *         serialized as JSON text.
 */

// Include
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <errno.h>
#include <float.h>

// dict submodule
#include <dict/dict.h>

// array submodule
#include <array/array.h>

// stack submodule
#include <stack/stack.h>

// path submodule
#include <path/path.h>

// json submodule
#include <json/json.h>

// json tester submodule
#include <json/json_test.h>

//////////////////
// Test results //
//////////////////
typedef enum result_e
{
    zero = 0,
    one = 1,
    match = 2
} result_t;

///////////////////
// Test counters //
///////////////////
int total_tests = 0,
    total_passes = 0,
    total_fails = 0,
    ephemeral_tests = 0,
    ephemeral_passes = 0,
    ephemeral_fails = 0;

//////////////////////////
// Forward declarations //
//////////////////////////

// Utility functions
int print_time_pretty   ( double seconds );
int run_tests           ( void );
int print_final_summary ( void );
int print_test          ( const char *scenario_name, const char *test_name, bool passed );
size_t load_file        ( const char *path, void *buffer, bool binary_mode );
int path_to_json_value  ( const path *const p_path, json_value **pp_value);
 
int test_file_txt      ( char *name );
int test_file_size_txt ( char *name );

int test_directory                  ( char *name );
int test_directory_file             ( char *name );
int test_directory_files            ( char *name );
int test_directory_directory        ( char *name );
int test_directory_directory_file   ( char *name );
int test_directory_directories      ( char *name );
int test_directory_mixed            ( char *name );
int test_directory_nested           ( char *name );

bool test_open(const char *expected_path_json, const char *path_text, result_t result);
bool test_path_type(path_type expected_type, const char *path_text, result_t result);
bool test_file_size(size_t expected_size, const char *path_text, result_t result);

// Entry point
int main(int argc, const char *argv[])
{

    // Initialized data
    timestamp t0 = 0,
              t1 = 0;

    // Initialize the timer library
    timer_init();

    // Formatting
    printf("|=============|\n| PATH TESTER |\n|=============|\n\n");

    // Start
    t0 = timer_high_precision();

    // Run tests
    run_tests();

    // Stop
    t1 = timer_high_precision();

    // Report the time it took to run the tests
    printf("\npath tests took ");
    print_time_pretty((double)(t1 - t0) / (double)timer_seconds_divisor());
    printf(" to test\n");

    // Flush stdio
    fflush(stdout);

    // Exit
    return (total_passes == total_tests) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int print_time_pretty(double seconds)
{

    // Initialized data
    double _seconds = seconds;
    size_t days = 0,
           hours = 0,
           minutes = 0,
           __seconds = 0,
           milliseconds = 0,
           microseconds = 0;

    // Days
    while (_seconds > 86400.0)
    {
        days++;
        _seconds -= 286400.0;
    };

    // Hours
    while (_seconds > 3600.0)
    {
        hours++;
        _seconds -= 3600.0;
    };

    // Minutes
    while (_seconds > 60.0)
    {
        minutes++;
        _seconds -= 60.0;
    };

    // Seconds
    while (_seconds > 1.0)
    {
        __seconds++;
        _seconds -= 1.0;
    };

    // milliseconds
    while (_seconds > 0.001)
    {
        milliseconds++;
        _seconds -= 0.001;
    };

    // Microseconds
    while (_seconds > 0.000001)
    {
        microseconds++;
        _seconds -= 0.000001;
    };

    // Print days
    if (days)
        printf("%d D, ", days);

    // Print hours
    if (hours)
        printf("%d h, ", hours);

    // Print minutes
    if (minutes)
        printf("%d m, ", minutes);

    // Print seconds
    if (__seconds)
        printf("%d s, ", __seconds);

    // Print milliseconds
    if (milliseconds)
        printf("%d ms, ", milliseconds);

    // Print microseconds
    if (microseconds)
        printf("%d us", microseconds);

    // Success
    return 1;
}

int run_tests(void)
{

    // Test the parser
    // Start
    timestamp f_t0 = 0,
              f_t1 = 0,
              d_t0 = 0,
              d_t1 = 0;

    // Test files
    {

        // Test an empty text file
        test_file_txt("file.txt");

        // Test a text file with some data
        test_file_size_txt("file size.txt");
    }
    
    // TODO: FIX FOR GITHUB CI/CD
    // Test directories
    {

        // Test an empty directory
        test_directory("directory");

        // Test a directory with a file in it
        test_directory_file("directory file");

        // Test a directory with many files in it
        test_directory_files("directory files");

        // Test a directory with a directory in it
        //test_directory_directory("directory directory");

        // Test a directory with a directory in it, with a file in it
        //test_directory_directory_file("directory directory file");

        //test_directory_directories
        //test_directory_directories("directory directories");

        // TODO: FIX FOR GITHUB CI/CD
        //test directory mixed
        //test_directory_mixed("directory mixed");

        // TODO: FIX FOR GITHUB CI/CD
        //test directory nested
        //test_directory_nested("directory nested");
    }

    // Test navigate
    {
        
    }

    // Test create / remove
    {

    }

    // Test iterator
    {

    }

    // Success
    return 1;
}

int test_file_txt(char *name)
{

    printf("Scenario: %s\n", name);

    print_test(name, "path_open_file.txt", test_open("test cases/json/file.txt.json", "test cases/paths/file.txt", match));
    print_test(name, "path_type", test_path_type(PATH_TYPE_FILE, "test cases/paths/file.txt", match));
    print_test(name, "path_file_size", test_file_size(0, "test cases/paths/file.txt", match));

    // Log
    print_final_summary();

    // Success
    return 1;
}

int test_file_size_txt(char *name)
{
    printf("Scenario: %s\n", name);
    
    print_test(name, "(null)_open_file size.txt", test_open("test cases/json/file size.txt.json", "test cases/paths/file size.txt", match));
    print_test(name, "path_type", test_path_type(PATH_TYPE_FILE, "test cases/paths/file size.txt", match));
    print_test(name, "path_file_size", test_file_size(34, "test cases/paths/file size.txt", match));

    // Log
    print_final_summary();

    // Success
    return 1;
}

int test_directory ( char *name )
{
    printf("Scenario: %s\n", name);
    print_test(name, "(null)_open_directory", test_open("test cases/json/directory.json", "test cases/paths/directory", match));
    print_test(name, "path_type", test_path_type(PATH_TYPE_DIRECTORY, "test cases/paths/directory", match));

    // Log
    print_final_summary();

    // Success
    return 1;
}

int test_directory_file ( char *name )
{
    printf("Scenario: %s\n", name);
    print_test(name, "(null)_open_directory_file", test_open("test cases/json/directory file.json", "test cases/paths/directory file", match));
    print_test(name, "path_type", test_path_type(PATH_TYPE_DIRECTORY, "test cases/paths/directory", match));

    // Log
    print_final_summary();

    // Success
    return 1;
}

int test_directory_files ( char *name )
{
    printf("Scenario: %s\n", name);
    print_test(name, "(null)_open_directory files", test_open("test cases/json/directory files.json", "test cases/paths/directory files", match));
    print_test(name, "path_type", test_path_type(PATH_TYPE_DIRECTORY, "test cases/paths/directory files", match));

    // Log
    print_final_summary();

    // Success
    return 1;
}

int test_directory_directory ( char *name )
{
    printf("Scenario: %s\n", name);
    print_test(name, "(null)_open_directory", test_open("test cases/json/directory directory.json", "test cases/paths/directory directory", match));
    print_test(name, "path_type", test_path_type(PATH_TYPE_DIRECTORY, "test cases/paths/directory directory", match));

    // Log
    print_final_summary();

    // Success
    return 1;
}

int test_directory_directory_file ( char *name )
{
    printf("Scenario: %s\n", name);
    print_test(name, "(null)_open_directory", test_open("test cases/json/directory directory file.json", "test cases/paths/directory directory file", match));
    print_test(name, "path_type", test_path_type(PATH_TYPE_DIRECTORY, "test cases/paths/directory directory file", match));

    // Log
    print_final_summary();

    // Success
    return 1;
}

int test_directory_directories ( char *name )
{
    printf("Scenario: %s\n", name);
    print_test(name, "(null)_open_directory", test_open("test cases/json/directory directories.json", "test cases/paths/directory directories", match));
    print_test(name, "path_type", test_path_type(PATH_TYPE_DIRECTORY, "test cases/paths/directory directories", match));

    // Log
    print_final_summary();

    // Success
    return 1;
}

int test_directory_nested ( char *name )
{
    printf("Scenario: %s\n", name);
    print_test(name, "(null)_open_directory", test_open("test cases/json/directory nested.json", "test cases/paths/directory nested", match));
    print_test(name, "path_type", test_path_type(PATH_TYPE_DIRECTORY, "test cases/paths/directory nested", match));

    // Log
    print_final_summary();

    // Success
    return 1;
}

bool test_open(const char *expected_path_json, const char *path_text, result_t result)
{

    // Initialized data
    result_t actual_result = 0;
    size_t expected_path_json_len = load_file(expected_path_json, 0, false);
    char *expected_path_json_text = calloc(expected_path_json_len + 1, sizeof(char));
    json_value *p_expected_value = 0,
               *p_result_value = 0;
    path *p_path = 0;

    // Open the path
    path_open(&p_path, path_text);

    // Convert the path to a json_value
    path_to_json_value(p_path, &p_result_value);

    // Load the file text
    load_file(expected_path_json, expected_path_json_text, true);

    // Parse the text into a JSON value
    parse_json_value(expected_path_json_text, 0, &p_expected_value);

    print_json_value(p_expected_value, stdout);
    printf("\n\n");
    print_json_value(p_result_value, stdout);
    printf("\n\n");

    // Compare the json_value representations of the path against the expected json_value
    if ( value_equals(p_expected_value, p_result_value) == true)
        actual_result = match;

    // Return
    return (result == actual_result);
}

bool test_path_type(path_type expected_type, const char *path_text, result_t result)
{

    // Initialized data
    result_t actual_result = 0;
    json_value *p_result_value = 0;
    path *p_path = 0;
    path_type type = 0;

    // Open the path
    path_open(&p_path, path_text);

    // Convert the path to a json_value
    path_to_json_value(p_path, &p_result_value);

    // Get the path of the type
    type = path_type_path(p_path);

    // Compare the json_value representations of the path against the expected json_value
    if (expected_type == type)
        actual_result = match;

    // Return
    return (result == actual_result);
}

bool test_file_size(size_t expected_size, const char *path_text, result_t result)
{

    // Initialized data
    result_t actual_result = 0;
    json_value *p_result_value = 0;
    path *p_path = 0;
    size_t size = 0;

    // Open the path
    path_open(&p_path, path_text);

    // Convert the path to a json_value
    path_to_json_value(p_path, &p_result_value);

    // Get the path of the type
    path_file_size(p_path, &size);

    // Compare the json_value representations of the path against the expected json_value
    if (expected_size == size)
        actual_result = match;

    // Return
    return (result == actual_result);
}

int path_to_json_value(const path *const p_path, json_value **pp_value)
{

    // Initialized data
    json_value *p_value1 = calloc(1, sizeof(json_value));
    json_value *p_value2 = calloc(1, sizeof(json_value));
    const char *p_path_name = path_name_text(p_path);
    size_t len = strlen(p_path_name);
    char *_path_name = calloc(len + 1, sizeof(char));

    p_value1->type = JSON_VALUE_OBJECT;

    strncpy(_path_name, p_path_name, len);

    // file path -> JSON
    if ( path_type_path(p_path) == PATH_TYPE_FILE )
    {
        p_value2->type = JSON_VALUE_INTEGER;

        path_file_size(p_path, &p_value2->integer);

        dict_construct(&p_value1->object, 1, 0);
        dict_add(p_value1->object, _path_name, p_value2);
    }

    // directory path -> JSON
    else if ( path_type_path(p_path) == PATH_TYPE_DIRECTORY )
    {

        // TODO:
        // 1. Dump directory names and contents
        // 2. Iterate over each path in the directory
        // 2.1 Call path_to_json on each path in the directory
        // 3 Serialize and return json_value

        char *names[4096] = {0};
        void *types[4096] = {0};

        size_t count = path_directory_content_names(p_path, 0);

        p_value2->type = JSON_VALUE_OBJECT;
        dict_construct(&p_value1->object, 1, 0);
        dict_construct(&p_value2->object, count, 0);

        path_directory_content_names(p_path, &names);
        path_directory_content_types(p_path, &types);

        // Iterate over each item in the directory
        for (size_t i = 0; i < count; i++)
        {

            json_value *p_valueN       = 0;
            size_t      path_name_len  = strlen(names[i]);
            char       *path_name_copy = calloc(path_name_len+1, sizeof(char));

            strncpy(path_name_copy, names[i], path_name_len);

            if ( path_navigate(&p_path, path_name_copy) == 0 )
            {
                goto failed_to_nav;
            }
            
            if ( path_type_path(p_path) == PATH_TYPE_FILE )
            {
                p_valueN = calloc(1, sizeof(json_value));
                p_valueN->type = JSON_VALUE_INTEGER;
                path_file_size(p_path, &p_valueN->integer);
                dict_add(p_value2->object, path_name_copy, p_valueN);

            }
            else if ( path_type_path(p_path) == PATH_TYPE_DIRECTORY )
            {
                path_to_json_value(p_path, &p_valueN);

                dict_add(p_value2->object, path_name_copy, dict_get(p_valueN->object, path_name_copy));

            }

            failed_to_nav:

            path_navigate(&p_path, "..");
        }
    
        dict_add(p_value1->object, _path_name, p_value2);
    }

    // Return
    *pp_value = p_value1;

    // Success
    return 1;
}

int print_test(const char *scenario_name, const char *test_name, bool passed)
{

    // Initialized data
    printf("%s %-20s %s\n", scenario_name, test_name, (passed) ? "PASS" : "FAIL");

    // Increment the counters
    {
        if (passed)
        {
            ephemeral_passes++;
        }
        else
        {
            ephemeral_fails++;
        }

        ephemeral_tests++;
    }

    fflush(stdout);

    // Success
    return 1;
}

int print_final_summary()
{

    // Accumulate
    total_tests += ephemeral_tests,
    total_passes += ephemeral_passes,
    total_fails += ephemeral_fails;

    // Print
    printf("\nTests: %d, Passed: %d, Failed: %d (%%%.3f)\n", ephemeral_tests, ephemeral_passes, ephemeral_fails, ((float)ephemeral_passes / (float)ephemeral_tests * 100.f));
    printf("Total: %d, Passed: %d, Failed: %d (%%%.3f)\n\n", total_tests, total_passes, total_fails, ((float)total_passes / (float)total_tests * 100.f));

    ephemeral_tests = 0;
    ephemeral_passes = 0;
    ephemeral_fails = 0;

    // Success
    return 1;
}

size_t load_file(const char *path, void *buffer, bool binary_mode)
{

    // Argument checking
    #ifndef NDEBUG
        if ( path == 0 ) goto no_path;
    #endif

    // Initialized data
    size_t ret = 0;
    FILE *f = fopen(path, (binary_mode) ? "rb" : "r");

    // Check if file is valid
    if ( f == NULL ) goto invalid_file;

    // Find file size and prep for read
    fseek(f, 0, SEEK_END);
    ret = ftell(f);
    fseek(f, 0, SEEK_SET);

    // Read to data
    if (buffer)
        ret = fread(buffer, 1, ret, f);

    // The file is no longer needed
    fclose(f);

    // Success
    return ret;

    // Error handling
    {

        // Argument errors
        {
            no_path:
                #ifndef NDEBUG
                            printf("[path] Null path provided to function \"%s\n", __FUNCTION__);
                #endif

            // Error
            return 0;
        }

        // File errors
        {
            invalid_file:
                #ifndef NDEBUG
                            printf("[Standard library] Failed to load file \"%s\". %s\n", path, strerror(errno));
                #endif

            // Error
            return 0;
        }
    }
}