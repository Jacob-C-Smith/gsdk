/** !
 * Tester for json module
 * 
 * @file json_test.c
 * 
 * @author Jacob Smith
 */

// header
#include <reflection/json_test.h>

// log module
#include <core/log.h>

// sync module
#include <core/sync.h>

// enumeration definitions
enum result_e
{
    zero = 0,    
    one  = 1,
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
const char *alphabet[] = {
    "a",
    "b",
    "c",
    "d",
    "e",
    "f",
    "g",
    "h",
    "i",
    "j",
    "k",
    "l",
    "m",
    "n",
    "o",
    "p",
    "q",
    "r",
    "s",
    "t",
    "u",
    "v",
    "w",
    "x",
    "y",
    "z"
};

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

bool     test_parse_json     ( char         *test_file    , int         (*expected_value_constructor) (json_value **), result_t expected );
bool     test_serial_json    ( char         *test_file    , char         *expected_file                               , int(*expected_value_constructor) (json_value **), result_t expected );
result_t load_json           ( json_value  **pp_value     , char         *test_file, char **free_me );
result_t save_json           ( char         *path         , json_value   *p_value );
bool     value_equals        ( json_value   *a            , json_value   *b );
size_t   load_file           ( const char   *path         , void         *buffer                                      , bool     binary_mode );

/** !
 * Test parsing of valid and invalid null values
 * 
 * @param name the name of the test
 * 
 * @return void
 */
void test_parse_null ( char *name );

/** !
 * Test parsing of valid and invalid boolean values
 * 
 * @param name the name of the test
 * 
 * @return void
 */
void test_parse_bool ( char *name );

/** !
 * Test parsing of valid and invalid integer values
 * 
 * @param name the name of the test
 * 
 * @return void
 */
void test_parse_int ( char *name );

/** !
 * Test parsing of valid and invalid floating point values
 * 
 * @param name the name of the test
 * 
 * @return void
 */
void test_parse_float (char *name);

/** !
 * Test parsing of valid and invalid string values
 * 
 * @param name the name of the test
 * 
 * @return void
 */
void test_parse_string (char *name);

/** !
 * Test parsing of valid and invalid object values
 * 
 * @param name the name of the test
 * 
 * @return void
 */
void test_parse_object (char *name);

/** !
 * Test parsing of valid and invalid array values
 * 
 * @param name the name of the test
 * 
 * @return void
 */
void test_parse_array ( char *name );

/** !
 * Test serializing the null value
 * 
 * @param name the name of the test
 * 
 * @return void
 */
void test_serial_null ( char *name );

/** !
 * Test serializing of boolean values
 * 
 * @param name the name of the test
 * 
 * @return void
 */
int test_serial_bool ( char *name );

/** !
 * Test serializing of integer values
 * 
 * @param name the name of the test
 * 
 * @return void
 */
int test_serial_int ( char *name );

/** !
 * Test serializing of floating point values
 * 
 * @param name the name of the test
 * 
 * @return void
 */
int test_serial_float ( char *name );

/** !
 * Test serializing of string values
 * 
 * @param name the name of the test
 * 
 * @return void
 */
int test_serial_string ( char *name );

/** !
 * Test serializing of object values
 * 
 * @param name the name of the test
 * 
 * @return void
 */
int test_serial_object ( char *name );

/** !
 * Test serializing of boolean values
 * 
 * @param name the name of the test
 * 
 * @return void
 */
int test_serial_array ( char *name );

// entry point
int main ( int argc, const char* argv[] )
{
    
    // Suppress warnings
    (void) argc;
    (void) argv;

    // initialized data
    timestamp t0 = 0,
              t1 = 0;

    // Formatting
    printf(
        "╭─────────────╮\n"\
        "│ json tester │\n"\
        "╰─────────────╯\n\n"
    );

    // Start
    t0 = timer_high_precision();

    // Run tests
    run_tests();

    // Stop
    t1 = timer_high_precision();

    // Report the time it took to run the tests
    log_info("\njson tests took ");
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
    
    // done
    return;
}

void run_tests ( void )
{

    // initialized data
    timestamp parser_t0 = 0,
              parser_t1 = 0,
              serial_t0 = 0,
              serial_t1 = 0;

    /////////////////////
    // Test the parser //
    /////////////////////

    // Start
    parser_t0 = timer_high_precision();

    // Test parsing valid and invalid null
    test_parse_null("parse null");

    // Test parsing valid and invalid booleans
    test_parse_bool("parse bool");

    // Test parsing valid, invalid, and over/underflowed integers
    test_parse_int("parse int");

    // Test parsing valid, invalid, and over/underflowed floating point numbers
    test_parse_float("parse float");
    
    // Test parsing valid, invalid, and unicode strings
    test_parse_string("parse string");

    // Test parsing a variety of objects
    test_parse_object("parse object");

    // Test parsing a variety of arrays
    test_parse_array("parse array");

    // Stop
    parser_t1 = timer_high_precision();

    /////////////////////////
    // Test the serializer //
    /////////////////////////

    // Start
    serial_t0 = timer_high_precision();

    // Test serializing null
    test_serial_null("serial null");
    
    // Test serializing booleans
    test_serial_bool("serial bool");

    // Test serializing integers
    test_serial_int("serial int");

    // Test serializing floating point numbers
    test_serial_float("serial float");
    
    // Test serializing strings
    test_serial_string("serial string");

    // Test serializing objects
    test_serial_object("serial object");

    // Test serializing arrays
    test_serial_array("serial array");

    // Stop
    serial_t1 = timer_high_precision();

    // Report the time it took to run the parser tests
    log_info("parser tests took: ");
    print_time_pretty ( (double)(parser_t1-parser_t0)/(double)timer_seconds_divisor() );
    log_info(" to test\n");

    // Report the time it took to run the serializer tests
    log_info("serial tests took: ");
    print_time_pretty ( (double)(serial_t1-serial_t0)/(double)timer_seconds_divisor() );
    log_info(" to test\n");

    // done
    return;
}

void test_parse_null ( char *name )
{

    // Formatting
    log_scenario("%s\n", name);

    // Test a valid null token
    print_test(name, "null", test_parse_json("resources/test/json/parse/pass/null.json", (void *) 0, one));

    // Test an invalid null token
    print_test(name, "nul" , test_parse_json("resources/test/json/parse/fail/null.json", (void *) 0, zero));

    // Print the summary of this test
    print_final_summary();

    // success
    return;
}

void test_parse_bool ( char *name )
{

    // Formatting
    log_scenario("%s\n", name);

    // Test a valid false token
    print_test(name, "false", test_parse_json("resources/test/json/parse/pass/bool/bool_false.json", construct_bool_false, one));

    // Test a valid true token
    print_test(name, "true", test_parse_json("resources/test/json/parse/pass/bool/bool_true.json", construct_bool_true , one));

    // Test an invalid false token
    print_test(name, "fals", test_parse_json("resources/test/json/parse/fail/bool/bool_false.json", (void *) 0, zero));

    // Test an invalid true token
    print_test(name, "tru", test_parse_json("resources/test/json/parse/fail/bool/bool_true.json", (void *) 0, zero));

    // Print the summary of this test
    print_final_summary();

    // done
    return;
}

void test_parse_int ( char *name )
{

    // Formatting
    log_scenario("%s\n", name);

    // Test -1
    print_test(name, "-1", test_parse_json("resources/test/json/parse/pass/int/int_-1.json", construct_int_minus_one, one));

    // Test 0
    print_test(name, "0", test_parse_json("resources/test/json/parse/pass/int/int_0.json", construct_int_zero, one));

    // Test 1
    print_test(name, "1", test_parse_json("resources/test/json/parse/pass/int/int_1.json", construct_int_one, one));

    // Test integer max
    print_test(name, "max", test_parse_json("resources/test/json/parse/pass/int/int_max.json", construct_int_max, one));

    // Test integer min
    print_test(name, "min", test_parse_json("resources/test/json/parse/pass/int/int_min.json", construct_int_min, one));

    // Test an overflow
    print_test(name, "max +1", test_parse_json("resources/test/json/parse/fail/int/int_max.json", (void *)0, zero));

    // Test an underflow
    print_test(name, "min -1", test_parse_json("resources/test/json/parse/fail/int/int_min.json", (void *)0, zero));

    // Print the summary of this test
    print_final_summary();

    // done
    return;
}

void test_parse_float ( char *name )
{

    // Formatting
    log_scenario("%s\n", name);

    // Test -1.0
    print_test(name, "-1.0", test_parse_json("resources/test/json/parse/pass/float/float_-1.json", construct_float_minus_one, one));

    // Test 0.0
    print_test(name, "0.0", test_parse_json("resources/test/json/parse/pass/float/float_0.json", construct_float_zero, one));

    // Test 1.0
    print_test(name, "1.0", test_parse_json("resources/test/json/parse/pass/float/float_1.json", construct_float_one, one));

    // Test double precision floating point max
    print_test(name, "max", test_parse_json("resources/test/json/parse/pass/float/float_max.json", construct_float_max, one));

    // Test double precision floating point min
    print_test(name, "min", test_parse_json("resources/test/json/parse/pass/float/float_min.json", construct_float_min, one));

    // Test double precision floating point overflow
    print_test(name, "max + 1.0", test_parse_json("resources/test/json/parse/fail/float/float_max.json", (void *)0, zero));

    // Test double precision floating point underflow
    print_test(name, "min - 1.0", test_parse_json("resources/test/json/parse/fail/float/float_min.json", (void *)0, zero));

    // Print the summary of this test
    print_final_summary();

    // done
    return;
}

void test_parse_string ( char *name )
{

    // Formatting
    log_scenario("%s\n", name);
    
    print_test(name, "\"\""       , test_parse_json("resources/test/json/parse/pass/string/string_empty.json"          , construct_string_empty          , one));
    print_test(name, "\"a\""      , test_parse_json("resources/test/json/parse/pass/string/string_a.json"              , construct_string_a              , one));
    print_test(name, "\"abc\""    , test_parse_json("resources/test/json/parse/pass/string/string_abc.json"            , construct_string_abc            , one));
    print_test(name, "\"\"abc\"\"", test_parse_json("resources/test/json/parse/pass/string/string_quote_abc_quote.json", construct_string_quote_abc_quote, one));
    print_test(name, "\"\"\"\""   , test_parse_json("resources/test/json/parse/pass/string/string_quote_quote.json"    , construct_string_quote_quote    , one));
    print_test(name, "    \"abc\"", test_parse_json("resources/test/json/parse/pass/string/string_whitespaces_abc.json", construct_string_whitespaces_abc, one));
    print_test(name, "\"\"\""     , test_parse_json("resources/test/json/parse/pass/string/string_quote.json"          , construct_string_quote          , one));
    print_test(name, "\"\\\\\""   , test_parse_json("resources/test/json/parse/pass/string/string_reverse_solidus.json", construct_string_reverse_solidus, one));
    print_test(name, "\"\\/\""    , test_parse_json("resources/test/json/parse/pass/string/string_solidus.json"        , construct_string_solidus        , one));
    print_test(name, "\"\\b\""    , test_parse_json("resources/test/json/parse/pass/string/string_backspace.json"      , construct_string_backspace      , one));
    print_test(name, "\"\\f\""    , test_parse_json("resources/test/json/parse/pass/string/string_formfeed.json"       , construct_string_formfeed       , one));
    print_test(name, "\"\\n\""    , test_parse_json("resources/test/json/parse/pass/string/string_linefeed.json"       , construct_string_linefeed       , one));
    print_test(name, "\"\\r\""    , test_parse_json("resources/test/json/parse/pass/string/string_carriage_return.json", construct_string_carriage_return, one));
    print_test(name, "\"\\t\""    , test_parse_json("resources/test/json/parse/pass/string/string_horizontal_tab.json" , construct_string_horizontal_tab , one));
    //print_test(name, "string \"\u1234\"" , test_parse_json("resources/test/json/parse/pass/string/string_escape.json"         , construct_string_escape         , one));

    // Print the summary of this test
    print_final_summary();

    // done
    return;
}

void test_parse_object ( char *name )
{

    // Formatting
    log_scenario("%s\n", name);
    
    print_test(name, "{}"                                               , test_parse_json("resources/test/json/parse/pass/object/object_empty.json"        , construct_object_empty        , one));
    print_test(name, "{\"abc\":\"def\"}"                                , test_parse_json("resources/test/json/parse/pass/object/object_string.json"       , construct_object_string       , one));
    print_test(name, "{\"abc\":123}"                                    , test_parse_json("resources/test/json/parse/pass/object/object_int.json"          , construct_object_int          , one));
    print_test(name, "{\"pi\":3.14}"                                    , test_parse_json("resources/test/json/parse/pass/object/object_float.json"        , construct_object_float        , one));
    print_test(name, "{\"abc\":false}"                                  , test_parse_json("resources/test/json/parse/pass/object/object_false.json"        , construct_object_false        , one));
    print_test(name, "{\"abc\":true}"                                   , test_parse_json("resources/test/json/parse/pass/object/object_true.json"         , construct_object_true         , one));
    print_test(name, "{\"abc\":\"def\",\"ghi\":\"jkl\",\"mno\":\"pqr\"}", test_parse_json("resources/test/json/parse/pass/object/object_strings.json"      , construct_object_strings      , one));
    print_test(name, "{\"name\":\"jake\",\"age\":20,\"height\":1.779}"  , test_parse_json("resources/test/json/parse/pass/object/object_mixed_values.json" , construct_object_mixed_values , one));
    print_test(name, "{\"abc\":{\"def\":123}}"                          , test_parse_json("resources/test/json/parse/pass/object/object_object.json"       , construct_object_object       , one));
    print_test(name, "{\"abc\":{\"def\":{\"ghi\":123}}}"                , test_parse_json("resources/test/json/parse/pass/object/object_object_object.json", construct_object_object_object, one));
    print_test(name, "{\"abc\":[1,2,3]}"                                , test_parse_json("resources/test/json/parse/pass/object/object_array.json"        , construct_object_array        , one));
    print_test(name, "{\"a\":[{\"a\":1},{\"b\":2},{\"c\":3}]}"          , test_parse_json("resources/test/json/parse/pass/object/object_array_objects.json", construct_object_array_objects, one));
    print_test(name, "{\"a\":[{\"a\":1}]}"                              , test_parse_json("resources/test/json/parse/pass/object/object_array_object.json" , construct_object_array_object , one));
    print_test(name, "{\"a\":{\"b\":{\"c\":{\"d\":{...{\"z\":{ }...}"   , test_parse_json("resources/test/json/parse/pass/object/object_recursive.json"    , construct_object_recursive    , one));

    // Print the summary of this test
    print_final_summary();

    // done
    return;
}

void test_parse_array ( char *name )
{

    // Formatting
    log_scenario("%s\n", name);
    
    print_test(name, "[]"                                  , test_parse_json("resources/test/json/parse/pass/array/array_empty.json"            , construct_array_empty            , one));
    print_test(name, "[null]"                              , test_parse_json("resources/test/json/parse/pass/array/array_null.json"             , construct_array_null             , one));
    print_test(name, "[null, null, null]"                  , test_parse_json("resources/test/json/parse/pass/array/array_nulls.json"            , construct_array_nulls            , one));
    print_test(name, "[true]"                              , test_parse_json("resources/test/json/parse/pass/array/array_bool.json"             , construct_array_bool             , one));
    print_test(name, "[true, false, true]"                 , test_parse_json("resources/test/json/parse/pass/array/array_bools.json"            , construct_array_bools            , one));
    print_test(name, "[1]"                                 , test_parse_json("resources/test/json/parse/pass/array/array_int.json"              , construct_array_int              , one));
    print_test(name, "[1, 2, 3]"                           , test_parse_json("resources/test/json/parse/pass/array/array_ints.json"             , construct_array_ints             , one));
    print_test(name, "[3.14]"                              , test_parse_json("resources/test/json/parse/pass/array/array_float.json"            , construct_array_float            , one));
    print_test(name, "[1.2, 3.4, 5.6]"                     , test_parse_json("resources/test/json/parse/pass/array/array_floats.json"           , construct_array_floats           , one));
    print_test(name, "[\"\"]"                              , test_parse_json("resources/test/json/parse/pass/array/array_string_empty.json"     , construct_array_string_empty     , one));
    print_test(name, "[\"abc\"]"                           , test_parse_json("resources/test/json/parse/pass/array/array_string.json"           , construct_array_string           , one));
    print_test(name, "[\"abc\", \"def\", \"ghi\"]"         , test_parse_json("resources/test/json/parse/pass/array/array_strings.json"          , construct_array_strings          , one));
    print_test(name, "[{}]"                                , test_parse_json("resources/test/json/parse/pass/array/array_object_empty.json"     , construct_array_object_empty     , one));
    print_test(name, "[{\"a\":1}]"                         , test_parse_json("resources/test/json/parse/pass/array/array_object.json"           , construct_array_object           , one));
    print_test(name, "[[{\"a\":1}, {\"b\":2}, {\"c\":3}]]" , test_parse_json("resources/test/json/parse/pass/array/array_objects.json"          , construct_array_objects          , one));
    print_test(name, "[[]]"                                , test_parse_json("resources/test/json/parse/pass/array/array_array_empty.json"      , construct_array_array_empty      , one));
    print_test(name, "[[[]]]"                              , test_parse_json("resources/test/json/parse/pass/array/array_array_array_empty.json", construct_array_array_array_empty, one));
    print_test(name, "[[1, 2, 3],[4, 5, 6],[7, 8, 9]]"     , test_parse_json("resources/test/json/parse/pass/array/array_matrix.json"           , construct_array_matrix           , one));
    print_test(name, "[[[1, 2], [3, 4]], [[5, 6], [7, 8]]]", test_parse_json("resources/test/json/parse/pass/array/array_tensor.json"           , construct_array_tensor           , one));

    // Print the summary of this test
    print_final_summary();

    // done
    return;
}

void test_serial_null ( char *name )
{

    // Formatting
    log_scenario("%s\n", name);

    // Test serializing a null value
    print_test(name, "null", test_serial_json("resources/test/json/serial/TESTER_null.json", "resources/test/json/parse/pass/null.json", construct_null, one));

    // Print the summary of this test
    print_final_summary();

    // done
    return;
}

int test_serial_bool ( char *name )
{

    // Formatting
    log_scenario("%s\n", name);

    print_test(name, "false", test_serial_json("resources/test/json/serial/bool/TESTER_bool_false.json", "resources/test/json/parse/pass/bool/bool_false.json", construct_bool_false, one));
    print_test(name, "true" , test_serial_json("resources/test/json/serial/bool/TESTER_bool_true.json" , "resources/test/json/parse/pass/bool/bool_true.json" , construct_bool_true , one));

    // Print the summary of this test
    print_final_summary();

    return 1;

}

int test_serial_int ( char *name )
{

    // Formatting
    log_scenario("%s\n", name);

    print_test(name, "-1"    , test_serial_json("resources/test/json/serial/int/TESTER_int_-1.json" , "resources/test/json/parse/pass/int/int_-1.json" , construct_int_minus_one, one));
    print_test(name, "0"     , test_serial_json("resources/test/json/serial/int/TESTER_int_0.json"  , "resources/test/json/parse/pass/int/int_0.json"  , construct_int_zero     , one));
    print_test(name, "1"     , test_serial_json("resources/test/json/serial/int/TESTER_int_1.json"  , "resources/test/json/parse/pass/int/int_1.json"  , construct_int_one      , one));
    print_test(name, "max"   , test_serial_json("resources/test/json/serial/int/TESTER_int_max.json", "resources/test/json/parse/pass/int/int_max.json", construct_int_max      , one));
    print_test(name, "min"   , test_serial_json("resources/test/json/serial/int/TESTER_int_min.json", "resources/test/json/parse/pass/int/int_min.json", construct_int_min      , one));

    // Print the summary of this test
    print_final_summary();

    return 1;
}

int test_serial_float ( char *name )
{

    // Formatting
    log_scenario("%s\n", name);

    print_test(name, "-1.0", test_serial_json("resources/test/json/serial/float/TESTER_float_-1.json" , "resources/test/json/parse/pass/float/float_-1.json" , construct_float_minus_one, one));
    print_test(name, "0.0" , test_serial_json("resources/test/json/serial/float/TESTER_float_0.json"  , "resources/test/json/parse/pass/float/float_0.json"  , construct_float_zero     , one));
    print_test(name, "1.0" , test_serial_json("resources/test/json/serial/float/TESTER_float_1.json"  , "resources/test/json/parse/pass/float/float_1.json"  , construct_float_one      , one));
    print_test(name, "max" , test_serial_json("resources/test/json/serial/float/TESTER_float_max.json", "resources/test/json/parse/pass/float/float_max.json", construct_float_max      , one));
    print_test(name, "min" , test_serial_json("resources/test/json/serial/float/TESTER_float_min.json", "resources/test/json/parse/pass/float/float_min.json", construct_float_min      , one));

    // Print the summary of this test
    print_final_summary();

    return 1;
}

int test_serial_string ( char *name )
{

    // Formatting
    log_scenario("%s\n", name);
    
    print_test(name, "\"\""       , test_serial_json("resources/test/json/serial/string/TESTER_string_empty.json"          , "resources/test/json/parse/pass/string/string_empty.json"          , construct_string_empty          , one));
    print_test(name, "\"a\""      , test_serial_json("resources/test/json/serial/string/TESTER_string_a.json"              , "resources/test/json/parse/pass/string/string_a.json"              , construct_string_a              , one));
    print_test(name, "\"abc\""    , test_serial_json("resources/test/json/serial/string/TESTER_string_abc.json"            , "resources/test/json/parse/pass/string/string_abc.json"            , construct_string_abc            , one));
    print_test(name, "\"\"abc\"\"", test_serial_json("resources/test/json/serial/string/TESTER_string_quote_abc_quote.json", "resources/test/json/parse/pass/string/string_quote_abc_quote.json", construct_string_quote_abc_quote, one));
    print_test(name, "\"\"\"\""   , test_serial_json("resources/test/json/serial/string/TESTER_string_quote_quote.json"    , "resources/test/json/parse/pass/string/string_quote_quote.json"    , construct_string_quote_quote    , one));
    print_test(name, "\"\"\""     , test_serial_json("resources/test/json/serial/string/TESTER_string_quote.json"          , "resources/test/json/parse/pass/string/string_quote.json"          , construct_string_quote          , one));
    print_test(name, "\"\\\\\""   , test_serial_json("resources/test/json/serial/string/TESTER_string_reverse_solidus.json", "resources/test/json/parse/pass/string/string_reverse_solidus.json", construct_string_reverse_solidus, one));
    print_test(name, "\"\\/\""    , test_serial_json("resources/test/json/serial/string/TESTER_string_solidus.json"        , "resources/test/json/parse/pass/string/string_solidus.json"        , construct_string_solidus        , one));
    print_test(name, "\"\\b\""    , test_serial_json("resources/test/json/serial/string/TESTER_string_backspace.json"      , "resources/test/json/parse/pass/string/string_backspace.json"      , construct_string_backspace      , one));
    print_test(name, "\"\\f\""    , test_serial_json("resources/test/json/serial/string/TESTER_string_formfeed.json"       , "resources/test/json/parse/pass/string/string_formfeed.json"       , construct_string_formfeed       , one));
    print_test(name, "\"\\n\""    , test_serial_json("resources/test/json/serial/string/TESTER_string_linefeed.json"       , "resources/test/json/parse/pass/string/string_linefeed.json"       , construct_string_linefeed       , one));
    print_test(name, "\"\\r\""    , test_serial_json("resources/test/json/serial/string/TESTER_string_carriage_return.json", "resources/test/json/parse/pass/string/string_carriage_return.json", construct_string_carriage_return, one));
    print_test(name, "\"\\t\""    , test_serial_json("resources/test/json/serial/string/TESTER_string_horizontal_tab.json" , "resources/test/json/parse/pass/string/string_horizontal_tab.json" , construct_string_horizontal_tab , one));

    // Print the summary of this test
    print_final_summary();

    return 1;
}

int test_serial_object ( char *name )
{

    // Formatting
    log_scenario("%s\n", name);
    
    print_test(name, "{}"                                               , test_serial_json("resources/test/json/serial/object/TESTER_object_empty.json"        , "resources/test/json/parse/pass/object/object_empty.json"        , construct_object_empty        , one));
    print_test(name, "{\"abc\":\"def\"}"                                , test_serial_json("resources/test/json/serial/object/TESTER_object_string.json"       , "resources/test/json/parse/pass/object/object_string.json"       , construct_object_string       , one));
    print_test(name, "{\"abc\":123}"                                    , test_serial_json("resources/test/json/serial/object/TESTER_object_int.json"          , "resources/test/json/parse/pass/object/object_int.json"          , construct_object_int          , one));
    print_test(name, "{\"pi\":3.14}"                                    , test_serial_json("resources/test/json/serial/object/TESTER_object_float.json"        , "resources/test/json/parse/pass/object/object_float.json"        , construct_object_float        , one));
    print_test(name, "{\"abc\":false}"                                  , test_serial_json("resources/test/json/serial/object/TESTER_object_false.json"        , "resources/test/json/parse/pass/object/object_false.json"        , construct_object_false        , one));
    print_test(name, "{\"abc\":true}"                                   , test_serial_json("resources/test/json/serial/object/TESTER_object_true.json"         , "resources/test/json/parse/pass/object/object_true.json"         , construct_object_true         , one));
    print_test(name, "{\"abc\":\"def\",\"ghi\":\"jkl\",\"mno\":\"pqr\"}", test_serial_json("resources/test/json/serial/object/TESTER_object_strings.json"      , "resources/test/json/parse/pass/object/object_strings.json"      , construct_object_strings      , one));
    print_test(name, "{\"name\":\"jake\",\"age\":20,\"height\":1.779}"  , test_serial_json("resources/test/json/serial/object/TESTER_object_mixed_values.json" , "resources/test/json/parse/pass/object/object_mixed_values.json" , construct_object_mixed_values , one));
    print_test(name, "{\"abc\":{\"def\":123}}"                          , test_serial_json("resources/test/json/serial/object/TESTER_object_object.json"       , "resources/test/json/parse/pass/object/object_object.json"       , construct_object_object       , one));
    print_test(name, "{\"abc\":{\"def\":{\"ghi\":123}}}"                , test_serial_json("resources/test/json/serial/object/TESTER_object_object_object.json", "resources/test/json/parse/pass/object/object_object_object.json", construct_object_object_object, one));
    print_test(name, "{\"abc\":[1,2,3]}"                                , test_serial_json("resources/test/json/serial/object/TESTER_object_array.json"        , "resources/test/json/parse/pass/object/object_array.json"        , construct_object_array        , one));
    print_test(name, "{\"a\":[{\"a\":1},{\"b\":2},{\"c\":3}]}"          , test_serial_json("resources/test/json/serial/object/TESTER_object_array_objects.json", "resources/test/json/parse/pass/object/object_array_objects.json", construct_object_array_objects, one));
    print_test(name, "{\"a\":[{\"a\":1}]}"                              , test_serial_json("resources/test/json/serial/object/TESTER_object_array_object.json" , "resources/test/json/parse/pass/object/object_array_object.json" , construct_object_array_object , one));
    print_test(name, "{\"a\":{\"b\":{\"c\":{\"d\":{...{\"z\":{ }...}"   , test_serial_json("resources/test/json/serial/object/TESTER_object_recursive.json"    , "resources/test/json/parse/pass/object/object_recursive.json"    , construct_object_recursive    , one));

    // Print the summary of this test
    print_final_summary();

    return 1;
}

int test_serial_array ( char *name )
{

    // Formatting
    log_scenario("%s\n", name);
    
    print_test(name, "[]"                                  , test_serial_json("resources/test/json/serial/array/TESTER_array_empty.json"            , "resources/test/json/parse/pass/array/array_empty.json"            , construct_array_empty            , one));
    print_test(name, "[null]"                              , test_serial_json("resources/test/json/serial/array/TESTER_array_null.json"             , "resources/test/json/parse/pass/array/array_null.json"             , construct_array_null             , one));
    print_test(name, "[null, null, null]"                  , test_serial_json("resources/test/json/serial/array/TESTER_array_nulls.json"            , "resources/test/json/parse/pass/array/array_nulls.json"            , construct_array_nulls            , one));
    print_test(name, "[true]"                              , test_serial_json("resources/test/json/serial/array/TESTER_array_bool.json"             , "resources/test/json/parse/pass/array/array_bool.json"             , construct_array_bool             , one));
    print_test(name, "[true, false, true]"                 , test_serial_json("resources/test/json/serial/array/TESTER_array_bools.json"            , "resources/test/json/parse/pass/array/array_bools.json"            , construct_array_bools            , one));
    print_test(name, "[1]"                                 , test_serial_json("resources/test/json/serial/array/TESTER_array_int.json"              , "resources/test/json/parse/pass/array/array_int.json"              , construct_array_int              , one));
    print_test(name, "[1, 2, 3]"                           , test_serial_json("resources/test/json/serial/array/TESTER_array_ints.json"             , "resources/test/json/parse/pass/array/array_ints.json"             , construct_array_ints             , one));
    print_test(name, "[3.14]"                              , test_serial_json("resources/test/json/serial/array/TESTER_array_float.json"            , "resources/test/json/parse/pass/array/array_float.json"            , construct_array_float            , one));
    print_test(name, "[1.2, 3.4, 5.6]"                     , test_serial_json("resources/test/json/serial/array/TESTER_array_floats.json"           , "resources/test/json/parse/pass/array/array_floats.json"           , construct_array_floats           , one));
    print_test(name, "[\"\"]"                              , test_serial_json("resources/test/json/serial/array/TESTER_array_string_empty.json"     , "resources/test/json/parse/pass/array/array_string_empty.json"     , construct_array_string_empty     , one));
    print_test(name, "[\"abc\"]"                           , test_serial_json("resources/test/json/serial/array/TESTER_array_string.json"           , "resources/test/json/parse/pass/array/array_string.json"           , construct_array_string           , one));
    print_test(name, "[\"abc\", \"def\", \"ghi\"]"         , test_serial_json("resources/test/json/serial/array/TESTER_array_strings.json"          , "resources/test/json/parse/pass/array/array_strings.json"          , construct_array_strings          , one));
    print_test(name, "[{}]"                                , test_serial_json("resources/test/json/serial/array/TESTER_array_object_empty.json"     , "resources/test/json/parse/pass/array/array_object_empty.json"     , construct_array_object_empty     , one));
    print_test(name, "[{\"a\":1}]"                         , test_serial_json("resources/test/json/serial/array/TESTER_array_object.json"           , "resources/test/json/parse/pass/array/array_object.json"           , construct_array_object           , one));
    print_test(name, "[[{\"a\":1}, {\"b\":2}, {\"c\":3}]]" , test_serial_json("resources/test/json/serial/array/TESTER_array_objects.json"          , "resources/test/json/parse/pass/array/array_objects.json"          , construct_array_objects          , one));
    print_test(name, "[[]]"                                , test_serial_json("resources/test/json/serial/array/TESTER_array_array_empty.json"      , "resources/test/json/parse/pass/array/array_array_empty.json"      , construct_array_array_empty      , one));
    print_test(name, "[[[]]]"                              , test_serial_json("resources/test/json/serial/array/TESTER_array_array_array_empty.json", "resources/test/json/parse/pass/array/array_array_array_empty.json", construct_array_array_array_empty, one));
    print_test(name, "[[1, 2, 3],[4, 5, 6],[7, 8, 9]]"     , test_serial_json("resources/test/json/serial/array/TESTER_array_matrix.json"           , "resources/test/json/parse/pass/array/array_matrix.json"           , construct_array_matrix           , one));
    print_test(name, "[[[1, 2], [3, 4]], [[5, 6], [7, 8]]]", test_serial_json("resources/test/json/serial/array/TESTER_array_tensor.json"           , "resources/test/json/parse/pass/array/array_tensor.json"           , construct_array_tensor           , one));

    // Print the summary of this test
    print_final_summary();

    // success
    return 1;
}

result_t load_json ( json_value **pp_value, char *test_file, char **free_me )
{

    // unused
    (void) free_me;

    // initialized data
    size_t    file_len = load_file(test_file, 0, false);
    char     *file_buf = default_allocator(0, file_len + 1);
    result_t  r        = 0; 
    
    // Initialize data
    memset(file_buf, 0, file_len + 1);

    // Load the file
    load_file(test_file, file_buf, false);

    // Parse JSON
    r = (result_t) json_value_parse(file_buf, 0, pp_value);

    // Release memory
    file_buf = default_allocator(file_buf, 0);

    // success
    return r;
}

bool value_equals (json_value *a, json_value *b)
{
    result_t result = 1;

    if ( a == 0 )
    {
        if (b == 0)
            return result;
        else
            return 0;
    }
    
    if ( b == 0 )
    {
        if (a == 0)
            return result;
        else
            return 0;
    }      

    if ( a->type != b->type )
    {
        result = 0;
        goto exit;
    }

    if ( a->type == JSON_VALUE_BOOLEAN )
        if ( a->boolean != b->boolean )
            result = 0;
    
    if ( a->type == JSON_VALUE_NUMBER )
    {

        // Set the last bit of each float's mantissa to zero. 
        // This averts rounding errors
        if (
            (*((unsigned long long *)((&a->number))) & (unsigned long long)0xfffffffffffffffe) !=
            (*((unsigned long long *)((&b->number))) & (unsigned long long)0xfffffffffffffffe) 
        )
            result = 0;
    }

    if ( a->type == JSON_VALUE_INTEGER )
        if ( a->integer != b->integer )
            result = 0;

    if ( a->type == JSON_VALUE_STRING )
        if ( strcmp(a->string, b->string) )
            result = 0;

    if ( a->type == JSON_VALUE_OBJECT )
    {
        dict   *a_dict       = a->object,
               *b_dict       = b->object;
        size_t  a_properties = dict_values(a_dict, 0),
                b_properties = dict_values(b_dict, 0);
        const char **a_keys       = 0,
                   **b_keys       = 0;
        void  **a_values     = 0,
              **b_values     = 0;
               
        if (a_properties != b_properties)
        {
            result = 0;
        }

        a_keys   = calloc(a_properties+1, sizeof(char *)),
        b_keys   = calloc(a_properties+1, sizeof(char *)),
        a_values = calloc(b_properties+1, sizeof(json_value *)),
        b_values = calloc(b_properties+1, sizeof(json_value *));
        
        dict_keys(a_dict, a_keys);
        dict_keys(b_dict, b_keys);
        dict_values(a_dict, a_values);
        dict_values(b_dict, b_values);
        
        for (size_t i = 0; i < a_properties; i++)
        {
            bool has_key   = false,
                 has_value = false;
                 
            for (size_t j = 0; j < b_properties; j++)
            {
                if (strcmp(a_keys[i], b_keys[j])==0)
                {
                    has_key = true;
                }
                if ( value_equals(a_values[i], b_values[j]) )
                {
                    has_value = true;
                }
            }

            if ( ( has_key && has_value ) == 0 )
            {
                result = 0;
            }
        }

        free(a_keys);
        free(b_keys);
        free(a_values);
        free(b_values);
    }

    if ( a->type == JSON_VALUE_ARRAY )
    {
        array        *a_array      = a->list,
                     *b_array      = b->list;
        size_t        a_properties = 0,
                      b_properties = 0;
        json_value **a_values     = 0,
                    **b_values     = 0;
        
        array_get(a_array,0,&a_properties);
        array_get(b_array,0,&b_properties);

        if (a_properties != b_properties)
        {
            result = 0;
        }

        a_values = calloc(b_properties+1, sizeof(json_value *)),
        b_values = calloc(b_properties+1, sizeof(json_value *));
        
        array_get(a_array, (void **) a_values, 0);
        array_get(b_array, (void **) b_values, 0);
        
        for (size_t i = 0; i < a_properties; i++)
        {
            bool has_value = false;
                 
            for (size_t j = 0; j < b_properties; j++)
            {
                if ( value_equals(a_values[i], b_values[j]) )
                {
                    has_value = true;
                }
            }

            if (has_value == false)
                result = 0;
        }

        // Clean up
        free(a_values);
        free(b_values);
    }

    exit:
    return result;
}

int construct_null ( json_value **pp_value )
{

    // null is NULL is (void *) 0
    *pp_value = (void *) 0;

    // success
    return 1;
}

int construct_bool_false ( json_value **pp_value )
{

    // initialized data
    json_value *p_value = calloc(1, sizeof(json_value));
    
    // Populate the json_value
    *p_value = (json_value) 
    {
        .type    = JSON_VALUE_BOOLEAN,
        .boolean = false
    };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_bool_true ( json_value **pp_value )
{

    // initialized data
    json_value *p_value = calloc(1, sizeof(json_value));
    
    // Populate the json_value
    *p_value = (json_value) 
    {
        .type    = JSON_VALUE_BOOLEAN,
        .boolean = true
    };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_int_minus_one ( json_value **pp_value )
{

    // initialized data
    json_value *p_value = calloc(1, sizeof(json_value));
    
    // Type
    *p_value = (json_value)
    {
        .type    = JSON_VALUE_INTEGER,
        .integer = -1
    };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_int_zero ( json_value **pp_value )
{

    // initialized data
    json_value *p_value = calloc(1, sizeof(json_value));
    
    // Type
    *p_value = (json_value)
    {
        .type    = JSON_VALUE_INTEGER,
        .integer = 0
    };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_int_one ( json_value **pp_value )
{

    // initialized data
    json_value *p_value = calloc(1, sizeof(json_value));
    
    // Type
    *p_value = (json_value)
    {
        .type    = JSON_VALUE_INTEGER,
        .integer = 1
    };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_int_max ( json_value **pp_value )
{

    // initialized data
    json_value *p_value = calloc(1, sizeof(json_value));
    
    // Type
    *p_value = (json_value)
    {
        .type    = JSON_VALUE_INTEGER,
        .integer = 9223372036854775807
    };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_int_min ( json_value **pp_value )
{

    // initialized data
    json_value *p_value = calloc(1, sizeof(json_value));
    
    // Type
    *p_value = (json_value)
    {
        .type    = JSON_VALUE_INTEGER,
        .integer = (-9223372036854775807 - 1)
    };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_float_minus_one ( json_value **pp_value )
{

    // initialized data
    json_value *p_value = calloc(1, sizeof(json_value));
    
    // Type
    *p_value = (json_value)
    {
        .type    = JSON_VALUE_NUMBER,
        .number = -1.0
    };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_float_zero ( json_value **pp_value )
{

    // initialized data
    json_value *p_value = calloc(1, sizeof(json_value));
    
    // Type
    *p_value = (json_value)
    {
        .type   = JSON_VALUE_NUMBER,
        .number = 0.0
    };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_float_one ( json_value **pp_value )
{

    // initialized data
    json_value *p_value = calloc(1, sizeof(json_value));
    
    // Type
    *p_value = (json_value)
    {
        .type   = JSON_VALUE_NUMBER,
        .number = 1.0
    };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_float_max ( json_value **pp_value )
{

    // initialized data
    json_value *p_value = calloc(1, sizeof(json_value));
    
    // Type
    *p_value = (json_value)
    {
        .type   = JSON_VALUE_NUMBER,
        .number = DBL_MAX
    };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_float_min ( json_value **pp_value )
{

    // initialized data
    json_value *p_value = calloc(1, sizeof(json_value));
    
    // Type
    *p_value = (json_value)
    {
        .type   = JSON_VALUE_NUMBER,
        .number = -DBL_MAX
    };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_string_empty ( json_value **pp_value )
{

    // initialized data
    json_value *p_value = calloc(1, sizeof(json_value));
    
    char *z = default_allocator(0, 3);
    *z='\0';

    // Type
    *p_value = (json_value)
    {
        .type   = JSON_VALUE_STRING,
        .string = z
    };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_string_a ( json_value **pp_value )
{

    // initialized data
    json_value *p_value = calloc(1, sizeof(json_value));
    char *z = default_allocator(0, 5);

    z[0]='a';
    z[1]='\0';

    // Type
    *p_value = (json_value)
    {
        .type   = JSON_VALUE_STRING,
        .string = z
    };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_string_abc ( json_value **pp_value )
{

    // initialized data
    json_value *p_value = calloc(1, sizeof(json_value));
    char *z = default_allocator(0, 5);

    z[0]='a';
    z[1]='b';
    z[2]='c';
    z[3]='\0';

    // Type
    *p_value = (json_value)
    {
        .type   = JSON_VALUE_STRING,
        .string = z
    };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_string_quote_abc_quote ( json_value **pp_value )
{

    // initialized data
    json_value *p_value = calloc(1, sizeof(json_value));
    char *z = default_allocator(0, 10);

    z[0]='\"';
    z[1]='a';
    z[2]='b';
    z[3]='c';
    z[4]='\"';
    z[5]='\0';

    // Type
    *p_value = (json_value)
    {
        .type   = JSON_VALUE_STRING,
        .string = z
    };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_string_quote ( json_value **pp_value )
{

    // initialized data
    json_value *p_value = calloc(1, sizeof(json_value));
    char *z = default_allocator(0, 3);

    z[0]='\"';
    z[1]='\0';

    // Type
    *p_value = (json_value)
    {
        .type   = JSON_VALUE_STRING,
        .string = z
    };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_string_quote_quote ( json_value **pp_value )
{

    // initialized data
    json_value *p_value = calloc(1, sizeof(json_value));
    char *z = default_allocator(0, 4);

    z[0]='\"';
    z[1]='\"';
    z[2]='\0';
    
    // Type
    *p_value = (json_value)
    {
        .type   = JSON_VALUE_STRING,
        .string = z
    };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_string_whitespaces_abc ( json_value **pp_value )
{

    // initialized data
    json_value *p_value = calloc(1, sizeof(json_value));
    char *z = default_allocator(0, 4);

    z[0]='a';
    z[1]='b';
    z[2]='c';
    z[3]='\0';

    // Type
    *p_value = (json_value)
    {
        .type   = JSON_VALUE_STRING,
        .string = z
    };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_string_reverse_solidus ( json_value **pp_value )
{

    // initialized data
    json_value *p_value = calloc(1, sizeof(json_value));
    char *z = default_allocator(0, 3);

    z[0]='\\';
    z[1]='\0';
    
    // Type
    *p_value = (json_value)
    {
        .type   = JSON_VALUE_STRING,
        .string = z
    };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_string_solidus ( json_value **pp_value )
{

    // initialized data
    json_value *p_value = calloc(1, sizeof(json_value));
    char *z = default_allocator(0, 3);

    z[0]='/';
    z[1]='\0';

    // Type
    *p_value = (json_value)
    {
        .type   = JSON_VALUE_STRING,
        .string = z
    };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_string_backspace ( json_value **pp_value )
{

    // initialized data
    json_value *p_value = calloc(1, sizeof(json_value));
    char *z = default_allocator(0, 3);

    z[0]='\b';
    z[1]='\0';

    // Type
    *p_value = (json_value)
    {
        .type   = JSON_VALUE_STRING,
        .string = z
    };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_string_formfeed ( json_value **pp_value )
{

    // initialized data
    json_value *p_value = calloc(1, sizeof(json_value));
    char *z = default_allocator(0, 3);

    z[0]='\f';
    z[1]='\0';

    // Type
    *p_value = (json_value)
    {
        .type   = JSON_VALUE_STRING,
        .string = z
    };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_string_linefeed ( json_value **pp_value )
{

    // initialized data
    json_value *p_value = calloc(1, sizeof(json_value));
    char *z = default_allocator(0, 3);

    z[0]='\n';
    z[1]='\0';

    // Type
    *p_value = (json_value)
    {
        .type   = JSON_VALUE_STRING,
        .string = z
    };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_string_carriage_return ( json_value **pp_value )
{

    // initialized data
    json_value *p_value = calloc(1, sizeof(json_value));
    char *z = default_allocator(0, 3);

    z[0]='\r';
    z[1]='\0';

    // Type
    *p_value = (json_value)
    {
        .type   = JSON_VALUE_STRING,
        .string = z
    };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_string_horizontal_tab ( json_value **pp_value )
{

    // initialized data
    json_value *p_value = calloc(1, sizeof(json_value));
    char *z = default_allocator(0, 3);

    z[0]='\t';
    z[1]='\0';

    // Type
    *p_value = (json_value)
    {
        .type   = JSON_VALUE_STRING,
        .string = z
    };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_object_empty ( json_value **pp_value )
{

    // initialized data
    json_value *p_value = calloc(1, sizeof(json_value));
    
    // Type
    p_value->type = JSON_VALUE_OBJECT;

    // Value
    dict_construct(&p_value->object, 1, 0);

    // Return
    *pp_value = p_value;

    // success
    return 1;
}

int construct_object_string ( json_value **pp_value )
{

    // initialized data
    json_value *p_value     = calloc(1, sizeof(json_value)),
               *p_abc_value = calloc(1, sizeof(json_value));
    char *z = default_allocator(0, 4);

    z[0]='d';
    z[1]='e';
    z[2]='f';
    z[3]='\0';

    // Type
    p_value->type = JSON_VALUE_OBJECT;
    p_abc_value->type = JSON_VALUE_STRING;
    p_abc_value->string = z;
    
    // Value
    dict_construct(&p_value->object, 1, 0);
    dict_add(p_value->object, "abc", p_abc_value);

    // Return
    *pp_value = p_value;

    // success
    return 1;
}

int construct_object_int  ( json_value **pp_value )
{

    // initialized data
    json_value *p_value     = calloc(1, sizeof(json_value)),
               *p_abc_value = calloc(1, sizeof(json_value));
    
    // Type
    p_value->type = JSON_VALUE_OBJECT;
    p_abc_value->type = JSON_VALUE_INTEGER;
    p_abc_value->integer = 123;
    
    // Value
    dict_construct(&p_value->object, 1, 0);
    dict_add(p_value->object, "abc", p_abc_value);

    // Return
    *pp_value = p_value;

    // success
    return 1;
}

int construct_object_float  ( json_value **pp_value )
{

    // initialized data
    json_value *p_value     = calloc(1, sizeof(json_value)),
               *p_abc_value = calloc(1, sizeof(json_value));
    
    // Type
    p_value->type = JSON_VALUE_OBJECT;
    p_abc_value->type = JSON_VALUE_NUMBER;
    p_abc_value->number = 3.14;
    
    // Value
    dict_construct(&p_value->object, 1, 0);
    dict_add(p_value->object, "pi", p_abc_value);

    // Return
    *pp_value = p_value;

    // success
    return 1;
}

int construct_object_false  ( json_value **pp_value )
{

    // initialized data
    json_value *p_value     = calloc(1, sizeof(json_value)),
               *p_abc_value = calloc(1, sizeof(json_value));
    
    // Type
    p_value->type = JSON_VALUE_OBJECT;
    p_abc_value->type = JSON_VALUE_BOOLEAN;
    p_abc_value->boolean = false;
    
    // Value
    dict_construct(&p_value->object, 1, 0);
    dict_add(p_value->object, "abc", p_abc_value);

    // Return
    *pp_value = p_value;

    // success
    return 1;
}

int construct_object_true  ( json_value **pp_value )
{

    // initialized data
    json_value *p_value     = calloc(1, sizeof(json_value)),
               *p_abc_value = calloc(1, sizeof(json_value));
    
    // Type
    p_value->type = JSON_VALUE_OBJECT;
    p_abc_value->type = JSON_VALUE_BOOLEAN;
    p_abc_value->boolean = true;
    
    // Value
    dict_construct(&p_value->object, 1, 0);
    dict_add(p_value->object, "abc", p_abc_value);

    // Return
    *pp_value = p_value;

    // success
    return 1;
}

int construct_object_strings ( json_value **pp_value )
{

    // initialized data
    json_value *p_value     = calloc(1, sizeof(json_value)),
               *p_abc_value = calloc(1, sizeof(json_value)),
               *p_ghi_value = calloc(1, sizeof(json_value)),
               *p_mno_value = calloc(1, sizeof(json_value));
    char *x = default_allocator(0, 4);
    char *y = default_allocator(0, 4);
    char *z = default_allocator(0, 4);

    x[0] = 'd';
    x[1] = 'e';
    x[2] = 'f';
    x[3] = '\0';

    y[0] = 'j';
    y[1] = 'k';
    y[2] = 'l';
    y[3] = '\0';

    z[0] = 'p';
    z[1] = 'q';
    z[2] = 'r';
    z[3] = '\0';

    // Type
    p_value->type = JSON_VALUE_OBJECT;
    p_abc_value->type = JSON_VALUE_STRING;
    p_abc_value->string = x;
    p_ghi_value->type = JSON_VALUE_STRING;
    p_ghi_value->string = y;
    p_mno_value->type = JSON_VALUE_STRING;
    p_mno_value->string = z;
    
    // Value
    dict_construct(&p_value->object, 3, 0);
    dict_add(p_value->object, "abc", p_abc_value);
    dict_add(p_value->object, "ghi", p_ghi_value);
    dict_add(p_value->object, "mno", p_mno_value);

    // Return
    *pp_value = p_value;

    // success
    return 1;
}

int construct_object_mixed_values ( json_value **pp_value )
{

    // initialized data
    json_value *p_value        = calloc(1, sizeof(json_value)),
               *p_name_value   = calloc(1, sizeof(json_value)),
               *p_age_value    = calloc(1, sizeof(json_value)),
               *p_height_value = calloc(1, sizeof(json_value));

    char *z = default_allocator(0, 5);
    
    z[0] = 'j';
    z[1] = 'a';
    z[2] = 'k';
    z[3] = 'e';
    z[4] = '\0';

    // Type
    p_value->type          = JSON_VALUE_OBJECT;
    p_name_value->type     = JSON_VALUE_STRING;
    p_name_value->string   = z;
    p_age_value->type      = JSON_VALUE_INTEGER;
    p_age_value->integer   = 20;
    p_height_value->type   = JSON_VALUE_NUMBER;
    p_height_value->number = 1.779;
    
    // Value
    dict_construct(&p_value->object, 3, 0);
    dict_add(p_value->object, "name"  , p_name_value);
    dict_add(p_value->object, "age"   , p_age_value);
    dict_add(p_value->object, "height", p_height_value);

    // Return
    *pp_value = p_value;

    // success
    return 1;
}

int construct_object_object ( json_value **pp_value )
{

    // initialized data
    json_value *p_value     = calloc(1, sizeof(json_value)),
               *p_abc_value = calloc(1, sizeof(json_value)),
               *p_def_value = calloc(1, sizeof(json_value));
    
    // Type
    p_value->type     = JSON_VALUE_OBJECT;
    p_abc_value->type = JSON_VALUE_OBJECT;

    p_def_value->type    = JSON_VALUE_INTEGER;
    p_def_value->integer = 123;

    // Construct the object
    dict_construct(&p_value->object, 1, 0);
    dict_construct(&p_abc_value->object, 1, 0);
    dict_add(p_value->object, "abc", p_abc_value);
    dict_add(p_abc_value->object, "def", p_def_value);

    // Return
    *pp_value = p_value;

    // success
    return 1;
}

int construct_object_object_object ( json_value **pp_value )
{

    // initialized data
    json_value *p_value     = calloc(1, sizeof(json_value)),
               *p_abc_value = calloc(1, sizeof(json_value)),
               *p_def_value = calloc(1, sizeof(json_value)),
               *p_ghi_value = calloc(1, sizeof(json_value));
    
    // Type
    p_value->type     = JSON_VALUE_OBJECT;
    p_abc_value->type = JSON_VALUE_OBJECT;
    p_def_value->type = JSON_VALUE_OBJECT;
    p_ghi_value->type = JSON_VALUE_INTEGER;

    // Construct the object
    dict_construct(&p_value->object, 1, 0);
    dict_construct(&p_abc_value->object, 1, 0);
    dict_construct(&p_def_value->object, 1, 0);

    p_ghi_value->integer = 123;
    
    dict_add(p_def_value->object, "ghi", p_ghi_value);
    dict_add(p_abc_value->object, "def", p_def_value);
    dict_add(p_value->object, "abc", p_abc_value);

    // Return
    *pp_value = p_value;

    // success
    return 1;
}

int construct_object_recursive ( json_value **pp_value )
{

    // initialized data
    json_value *p_iter_value = default_allocator(0, sizeof(json_value)),
               *p_last_value = 0;

    p_iter_value->type = JSON_VALUE_OBJECT;

    
    p_iter_value->type = JSON_VALUE_OBJECT;
    dict_construct(&p_iter_value->object, 1, 0);

    p_last_value = p_iter_value;
    
    // Iterate through the alphabet
    for (size_t i = 'z'; i > 'a'-1; i--)
    {

        p_iter_value = default_allocator(0, sizeof(json_value)),

        p_iter_value->type = JSON_VALUE_OBJECT;
        dict_construct(&p_iter_value->object, 1, 0);
        dict_add(p_iter_value->object,alphabet[i-'a'], p_last_value);   
        p_last_value = p_iter_value;
    }
    
    // Return
    *pp_value = p_iter_value;

    // success
    return 1;
}

int construct_object_array ( json_value **pp_value )
{

    // initialized data
    json_value *p_value       = calloc(1, sizeof(json_value)),
                *p_array_value = calloc(1, sizeof(json_value)),
                *p_int_value   = 0;
    
    // Type
    p_value->type        = JSON_VALUE_OBJECT;
    p_array_value->type  = JSON_VALUE_ARRAY;

    // Construct the object
    dict_construct(&p_value->object, 1, 0);
    array_construct(&p_array_value->list, 3);

    p_int_value          = calloc(1, sizeof(json_value));
    p_int_value->type    = JSON_VALUE_INTEGER;
    p_int_value->integer = 1;
    array_add(p_array_value->list, p_int_value);

    p_int_value          = calloc(1, sizeof(json_value));
    p_int_value->type    = JSON_VALUE_INTEGER;
    p_int_value->integer = 2;
    array_add(p_array_value->list, p_int_value);
    
    p_int_value          = calloc(1, sizeof(json_value));
    p_int_value->type    = JSON_VALUE_INTEGER;
    p_int_value->integer = 3;
    array_add(p_array_value->list, p_int_value);

    dict_add(p_value->object, "abc", p_array_value);

    // Return
    *pp_value = p_value;

    // success
    return 1;
}

int construct_object_array_objects ( json_value **pp_value )
{

    // initialized data
    json_value *p_value       = calloc(1, sizeof(json_value)),
                *p_array_value = calloc(1, sizeof(json_value)),
                *p_dict_value  = 0,
                *p_int_value   = 0;
    
    // Type
    p_value->type        = JSON_VALUE_OBJECT;
    p_array_value->type  = JSON_VALUE_ARRAY;

    // Construct the object
    dict_construct(&p_value->object, 1, 0);
    array_construct(&p_array_value->list, 3);

    p_int_value          = calloc(1, sizeof(json_value));
    p_dict_value         = calloc(1, sizeof(json_value));
    p_int_value->type    = JSON_VALUE_INTEGER;
    p_int_value->integer = 1;
    p_dict_value->type   = JSON_VALUE_OBJECT;
    dict_construct(&p_dict_value->object, 1, 0);
    dict_add(p_dict_value->object,"a",p_int_value);
    array_add(p_array_value->list, p_dict_value);

    p_int_value          = calloc(1, sizeof(json_value));
    p_dict_value         = calloc(1, sizeof(json_value));
    p_int_value->type    = JSON_VALUE_INTEGER;
    p_int_value->integer = 2;
    p_dict_value->type   = JSON_VALUE_OBJECT;
    dict_construct(&p_dict_value->object, 1, 0);
    dict_add(p_dict_value->object,"b",p_int_value);
    array_add(p_array_value->list, p_dict_value);
    
    p_int_value          = calloc(1, sizeof(json_value));
    p_dict_value         = calloc(1, sizeof(json_value));
    p_int_value->type    = JSON_VALUE_INTEGER;
    p_int_value->integer = 3;
    p_dict_value->type   = JSON_VALUE_OBJECT;
    dict_construct(&p_dict_value->object, 1, 0);
    dict_add(p_dict_value->object,"c",p_int_value);
    array_add(p_array_value->list, p_dict_value);

    dict_add(p_value->object, "a", p_array_value);

    // Return
    *pp_value = p_value;

    // success
    return 1;
}

int construct_object_array_object ( json_value **pp_value )
{

    // initialized data
    json_value *p_value       = calloc(1, sizeof(json_value)),
                *p_array_value = calloc(1, sizeof(json_value)),
                *p_dict_value  = 0,
                *p_int_value   = 0;
    
    // Type
    p_value->type        = JSON_VALUE_OBJECT;
    p_array_value->type  = JSON_VALUE_ARRAY;

    // Construct the object
    dict_construct(&p_value->object, 1, 0);
    array_construct(&p_array_value->list, 3);

    p_int_value          = calloc(1, sizeof(json_value));
    p_dict_value         = calloc(1, sizeof(json_value));
    p_int_value->type    = JSON_VALUE_INTEGER;
    p_int_value->integer = 1;
    p_dict_value->type   = JSON_VALUE_OBJECT;
    dict_construct(&p_dict_value->object, 1, 0);
    dict_add(p_dict_value->object,"a",p_int_value);
    array_add(p_array_value->list, p_dict_value);

    dict_add(p_value->object, "a", p_array_value);

    // Return
    *pp_value = p_value;

    // success
    return 1;
}

int construct_array_empty ( json_value **pp_value )
{

    // initialized data
    json_value *p_value = calloc(1, sizeof(json_value));
    
    // Type
    p_value->type = JSON_VALUE_ARRAY;

    // Value
    array_construct(&p_value->list, 1);

    // Return
    *pp_value = p_value;

    // success
    return 1;
}

int construct_array_null ( json_value **pp_value )
{

    // initialized data
    json_value *p_value = calloc(1, sizeof(json_value));
    
    // Type
    p_value->type = JSON_VALUE_ARRAY;

    // Value
    array_construct(&p_value->list, 1);
    array_add(p_value->list, 0);

    // Return
    *pp_value = p_value;

    // success
    return 1;
}

int construct_array_nulls ( json_value **pp_value )
{

    // initialized data
    json_value *p_value = calloc(1, sizeof(json_value));
    
    // Type
    p_value->type = JSON_VALUE_ARRAY;

    // Value
    array_construct(&p_value->list, 3);
    array_add(p_value->list, 0);
    array_add(p_value->list, 0);
    array_add(p_value->list, 0);
    
    // Return
    *pp_value = p_value;

    // success
    return 1;
}

int construct_array_bool ( json_value **pp_value )
{

    // initialized data
    json_value *p_value = calloc(1, sizeof(json_value)),
               *p_bool  = 0;

    // Type
    p_value->type = JSON_VALUE_ARRAY;

    // Value
    p_bool = calloc(1, sizeof(json_value));
    p_bool->type = JSON_VALUE_BOOLEAN;
    p_bool->boolean = true;
    array_construct(&p_value->list, 1);
    array_add(p_value->list, p_bool);

    // Return
    *pp_value = p_value;

    // success
    return 1;
}

int construct_array_bools ( json_value **pp_value )
{

    // initialized data
    json_value *p_value = calloc(1, sizeof(json_value)),
               *p_bool  = 0;

    // Type
    p_value->type = JSON_VALUE_ARRAY;
    array_construct(&p_value->list, 3);
    
    // Value 1
    p_bool = calloc(1, sizeof(json_value));
    p_bool->type = JSON_VALUE_BOOLEAN;
    p_bool->boolean = true;
    array_add(p_value->list, p_bool);
    
    // Value 2
    p_bool = calloc(1, sizeof(json_value));
    p_bool->type = JSON_VALUE_BOOLEAN;
    p_bool->boolean = false;
    array_add(p_value->list, p_bool);

    // Value 3
    p_bool = calloc(1, sizeof(json_value));
    p_bool->type = JSON_VALUE_BOOLEAN;
    p_bool->boolean = true;
    array_add(p_value->list, p_bool);

    // Return
    *pp_value = p_value;

    // success
    return 1;
}

int construct_array_int ( json_value **pp_value )
{

    // initialized data
    json_value *p_value = calloc(1, sizeof(json_value)),
               *p_bool  = 0;

    // Type
    p_value->type = JSON_VALUE_ARRAY;

    // Value
    p_bool = calloc(1, sizeof(json_value));
    p_bool->type = JSON_VALUE_INTEGER;
    p_bool->integer = 1;
    array_construct(&p_value->list, 1);
    array_add(p_value->list, p_bool);

    // Return
    *pp_value = p_value;

    // success
    return 1;
}

int construct_array_ints ( json_value **pp_value )
{

    // initialized data
    json_value *p_value = calloc(1, sizeof(json_value)),
                *p_int  = 0;

    // Type
    p_value->type = JSON_VALUE_ARRAY;
    array_construct(&p_value->list, 3);
    
    // Value 1
    p_int = calloc(1, sizeof(json_value));
    p_int->type = JSON_VALUE_INTEGER;
    p_int->integer = 1;
    array_add(p_value->list, p_int);
    
    // Value 2
    p_int = calloc(1, sizeof(json_value));
    p_int->type = JSON_VALUE_INTEGER;
    p_int->integer = 2;
    array_add(p_value->list, p_int);

    // Value 3
    p_int = calloc(1, sizeof(json_value));
    p_int->type = JSON_VALUE_INTEGER;
    p_int->integer = 3;
    array_add(p_value->list, p_int);

    // Return
    *pp_value = p_value;

    // success
    return 1;
}

int construct_array_float ( json_value **pp_value )
{

    // initialized data
    json_value *p_value = calloc(1, sizeof(json_value)),
                *p_float  = 0;

    // Type
    p_value->type = JSON_VALUE_ARRAY;

    // Value
    p_float = calloc(1, sizeof(json_value));
    p_float->type = JSON_VALUE_NUMBER;
    p_float->number = 3.14;
    array_construct(&p_value->list, 1);
    array_add(p_value->list, p_float);

    // Return
    *pp_value = p_value;

    // success
    return 1;
}

int construct_array_floats ( json_value **pp_value )
{

    // initialized data
    json_value *p_value = calloc(1, sizeof(json_value)),
                *p_float  = 0;

    // Type
    p_value->type = JSON_VALUE_ARRAY;
    array_construct(&p_value->list, 3);
    
    // Value 1
    p_float = calloc(1, sizeof(json_value));
    p_float->type = JSON_VALUE_NUMBER;
    p_float->number = 1.2;
    array_add(p_value->list, p_float);
    
    // Value 2
    p_float = calloc(1, sizeof(json_value));
    p_float->type = JSON_VALUE_NUMBER;
    p_float->number = 3.4;
    array_add(p_value->list, p_float);

    // Value 3
    p_float = calloc(1, sizeof(json_value));
    p_float->type = JSON_VALUE_NUMBER;
    p_float->number = 5.6;
    array_add(p_value->list, p_float);

    // Return
    *pp_value = p_value;

    // success
    return 1;
}

int construct_array_string_empty ( json_value **pp_value )
{

    // initialized data
    json_value *p_value = calloc(1, sizeof(json_value)),
                *p_string  = 0;

    // Type
    p_value->type = JSON_VALUE_ARRAY;

    char *z = default_allocator(0, 5);

    z[0]='\0';

    // Value
    p_string = calloc(1, sizeof(json_value));
    p_string->type = JSON_VALUE_STRING;
    p_string->string = z;
    array_construct(&p_value->list, 1);
    array_add(p_value->list, p_string);

    // Return
    *pp_value = p_value;

    // success
    return 1;
}

int construct_array_string ( json_value **pp_value )
{

    // initialized data
    json_value *p_value = calloc(1, sizeof(json_value)),
               *p_string = 0;

    // Type
    p_value->type = JSON_VALUE_ARRAY;
    char *z = default_allocator(0, 5);

    z[0]='a';
    z[1]='b';
    z[2]='c';
    z[3]='\0';

    // Value
    p_string = calloc(1, sizeof(json_value));
    p_string->type = JSON_VALUE_STRING;
    p_string->string = z;
    array_construct(&p_value->list, 1);
    array_add(p_value->list, p_string);

    // Return
    *pp_value = p_value;

    // success
    return 1;
}

int construct_array_strings ( json_value **pp_value )
{

    // initialized data
    json_value *p_value  = calloc(1, sizeof(json_value)),
                *p_string = 0;
    char *x = default_allocator(0, 4);
    char *y = default_allocator(0, 4);
    char *z = default_allocator(0, 4);

    x[0] = 'a';
    x[1] = 'b';
    x[2] = 'c';
    x[3] = '\0';

    y[0] = 'd';
    y[1] = 'e';
    y[2] = 'f';
    y[3] = '\0';

    z[0] = 'g';
    z[1] = 'h';
    z[2] = 'i';
    z[3] = '\0';

    // Type
    p_value->type = JSON_VALUE_ARRAY;
    array_construct(&p_value->list, 3);
    
    // Value 1
    p_string = calloc(1, sizeof(json_value));
    p_string->type = JSON_VALUE_STRING;
    p_string->string = x;
    array_add(p_value->list, p_string);
    
    // Value 2
    p_string = calloc(1, sizeof(json_value));
    p_string->type = JSON_VALUE_STRING;
    p_string->string = y;
    array_add(p_value->list, p_string);

    // Value 3
    p_string = calloc(1, sizeof(json_value));
    p_string->type = JSON_VALUE_STRING;
    p_string->string = z;
    array_add(p_value->list, p_string);

    // Return
    *pp_value = p_value;

    // success
    return 1;
}

int construct_array_object_empty ( json_value **pp_value )
{

    // initialized data
    json_value *p_value  = calloc(1, sizeof(json_value)),
                *p_object = 0;

    // Type
    p_value->type = JSON_VALUE_ARRAY;
    array_construct(&p_value->list, 3);
    
    // Value 1
    p_object = calloc(1, sizeof(json_value));
    p_object->type = JSON_VALUE_OBJECT;
    dict_construct(&p_object->object, 2, 0);
    array_add(p_value->list, p_object);
    
    // Return
    *pp_value = p_value;

    // success
    return 1;
}

int construct_array_object ( json_value **pp_value )
{
    
    // initialized data
    json_value *p_value           = calloc(1, sizeof(json_value)),
                *p_object          = 0,
                *p_object_property = 0;

    // Type
    p_value->type = JSON_VALUE_ARRAY;
    array_construct(&p_value->list, 3);
    
    // Value 1
    p_object_property = calloc(1, sizeof(json_value));
    p_object_property->type = JSON_VALUE_INTEGER;
    p_object_property->integer = 1;

    // Value 2
    p_object = calloc(1, sizeof(json_value));
    p_object->type = JSON_VALUE_OBJECT;
    dict_construct(&p_object->object, 2, 0);
    dict_add(p_object->object, "a", p_object_property);
    array_add(p_value->list, p_object);
    
    // Return
    *pp_value = p_value;

    // success
    return 1;
}

int construct_array_objects ( json_value **pp_value )
{
    
    // initialized data
    json_value *p_value           = calloc(1, sizeof(json_value)),
                *p_object          = 0,
                *p_object_property = 0;

    // Type
    p_value->type = JSON_VALUE_ARRAY;
    array_construct(&p_value->list, 3);
    
    // Value 1
    p_object_property = calloc(1, sizeof(json_value));
    p_object_property->type = JSON_VALUE_INTEGER;
    p_object_property->integer = 1;

    // Value 2
    p_object = calloc(1, sizeof(json_value));
    p_object->type = JSON_VALUE_OBJECT;
    dict_construct(&p_object->object, 2, 0);
    dict_add(p_object->object, "a", p_object_property);
    array_add(p_value->list, p_object);
    
    // Value 1
    p_object_property = calloc(1, sizeof(json_value));
    p_object_property->type = JSON_VALUE_INTEGER;
    p_object_property->integer = 2;

    // Value 2
    p_object = calloc(1, sizeof(json_value));
    p_object->type = JSON_VALUE_OBJECT;
    dict_construct(&p_object->object, 2, 0);
    dict_add(p_object->object, "b", p_object_property);
    array_add(p_value->list, p_object);

    // Value 1
    p_object_property = calloc(1, sizeof(json_value));
    p_object_property->type = JSON_VALUE_INTEGER;
    p_object_property->integer = 3;

    // Value 2
    p_object = calloc(1, sizeof(json_value));
    p_object->type = JSON_VALUE_OBJECT;
    dict_construct(&p_object->object, 2, 0);
    dict_add(p_object->object, "c", p_object_property);
    array_add(p_value->list, p_object);

    // Return
    *pp_value = p_value;

    // success
    return 1;
}

int construct_array_array_empty ( json_value **pp_value )
{
    
    // initialized data
    json_value *p_value = calloc(1, sizeof(json_value)),
               *p_array = 0;

    // Type
    p_value->type = JSON_VALUE_ARRAY;
    array_construct(&p_value->list, 3);
    
    // Value 2
    p_array = calloc(1, sizeof(json_value));
    p_array->type = JSON_VALUE_ARRAY;
    array_construct(&p_array->list, 3);

    // Value 2
    array_add(p_value->list, p_array);
    
    // Return
    *pp_value = p_value;

    // success
    return 1;
}

int construct_array_array_array_empty ( json_value **pp_value )
{
    
    // initialized data
    json_value *p_value           = calloc(1, sizeof(json_value)),
                *p_array           = 0,
                *p_array_array     = 0;

    // Type
    p_value->type = JSON_VALUE_ARRAY;
    array_construct(&p_value->list, 3);
    
    // Value 1
    p_array_array = calloc(1, sizeof(json_value));
    p_array_array->type = JSON_VALUE_ARRAY;
    array_construct(&p_array_array->list, 3);

    // Value 2
    p_array = calloc(1, sizeof(json_value));
    p_array->type = JSON_VALUE_ARRAY;
    array_construct(&p_array->list, 3);
    array_add(p_array->list, p_array_array);

    // Value 2
    array_add(p_value->list, p_array);
    
    // Return
    *pp_value = p_value;

    // success
    return 1;
}

int construct_array_matrix ( json_value **pp_value )
{

    // initialized data
    json_value *p_value           = calloc(1, sizeof(json_value)),
                *p_subarray        = 0,
                *p_object_property = 0;

    p_value->type = JSON_VALUE_ARRAY;
    array_construct(&p_value->list, 3);

    {
        p_subarray = calloc(1, sizeof(json_value));
        p_subarray->type = JSON_VALUE_ARRAY;
        array_construct(&p_subarray->list, 3);

        p_object_property = calloc(1, sizeof(json_value));
        p_object_property->type = JSON_VALUE_INTEGER;
        p_object_property->integer = 1;
        array_add(p_subarray->list, p_object_property);

        p_object_property = calloc(1, sizeof(json_value));
        p_object_property->type = JSON_VALUE_INTEGER;
        p_object_property->integer = 2;
        array_add(p_subarray->list, p_object_property);

        p_object_property = calloc(1, sizeof(json_value));
        p_object_property->type = JSON_VALUE_INTEGER;
        p_object_property->integer = 3;
        array_add(p_subarray->list, p_object_property);

        array_add(p_value->list, p_subarray);
    }

    {
        p_subarray = calloc(1, sizeof(json_value));
        p_subarray->type = JSON_VALUE_ARRAY;
        array_construct(&p_subarray->list, 3);
    
        p_object_property = calloc(1, sizeof(json_value));
        p_object_property->type = JSON_VALUE_INTEGER;
        p_object_property->integer = 4;
        array_add(p_subarray->list, p_object_property);
        
        p_object_property = calloc(1, sizeof(json_value));
        p_object_property->type = JSON_VALUE_INTEGER;
        p_object_property->integer = 5;
        array_add(p_subarray->list, p_object_property);
    
        p_object_property = calloc(1, sizeof(json_value));
        p_object_property->type = JSON_VALUE_INTEGER;
        p_object_property->integer = 6;
        array_add(p_subarray->list, p_object_property);
    
        array_add(p_value->list, p_subarray);
    }

    {
        p_subarray = calloc(1, sizeof(json_value));
        p_subarray->type = JSON_VALUE_ARRAY;
        array_construct(&p_subarray->list, 3);
    
        p_object_property = calloc(1, sizeof(json_value));
        p_object_property->type = JSON_VALUE_INTEGER;
        p_object_property->integer = 7;
        array_add(p_subarray->list, p_object_property);
        
        p_object_property = calloc(1, sizeof(json_value));
        p_object_property->type = JSON_VALUE_INTEGER;
        p_object_property->integer = 8;
        array_add(p_subarray->list, p_object_property);
    
        p_object_property = calloc(1, sizeof(json_value));
        p_object_property->type = JSON_VALUE_INTEGER;
        p_object_property->integer = 9;
        array_add(p_subarray->list, p_object_property);
    
        array_add(p_value->list, p_subarray);
    }

    // Return
    *pp_value = p_value;

    // success
    return 1;
}

int construct_array_tensor ( json_value **pp_value )
{

    // initialized data
    json_value *p_value           = calloc(1, sizeof(json_value)),
                *p_subarray        = 0,
                *p_subsubarray     = 0,
                *p_object_property = 0;

    p_value->type = JSON_VALUE_ARRAY;
    array_construct(&p_value->list, 2);

    {
        p_subarray = calloc(1, sizeof(json_value));
        p_subarray->type = JSON_VALUE_ARRAY;
        array_construct(&p_subarray->list, 2);


        {
            p_subsubarray = calloc(1, sizeof(json_value));
            p_subsubarray->type = JSON_VALUE_ARRAY;
            array_construct(&p_subsubarray->list, 2);

            p_object_property = calloc(1, sizeof(json_value));
            p_object_property->type = JSON_VALUE_INTEGER;
            p_object_property->integer = 1;
            array_add(p_subsubarray->list, p_object_property);

            p_object_property = calloc(1, sizeof(json_value));
            p_object_property->type = JSON_VALUE_INTEGER;
            p_object_property->integer = 2;
            array_add(p_subsubarray->list, p_object_property);

            array_add(p_subarray->list, p_subsubarray);
        }

        {
            p_subsubarray = calloc(1, sizeof(json_value));
            p_subsubarray->type = JSON_VALUE_ARRAY;
            array_construct(&p_subsubarray->list, 2);

            p_object_property = calloc(1, sizeof(json_value));
            p_object_property->type = JSON_VALUE_INTEGER;
            p_object_property->integer = 3;
            array_add(p_subsubarray->list, p_object_property);

            p_object_property = calloc(1, sizeof(json_value));
            p_object_property->type = JSON_VALUE_INTEGER;
            p_object_property->integer = 4;
            array_add(p_subsubarray->list, p_object_property);

            array_add(p_subarray->list, p_subsubarray);
        }

        array_add(p_value->list, p_subarray);
    }

    {
        p_subarray = calloc(1, sizeof(json_value));
        p_subarray->type = JSON_VALUE_ARRAY;
        array_construct(&p_subarray->list, 2);


        {
            p_subsubarray = calloc(1, sizeof(json_value));
            p_subsubarray->type = JSON_VALUE_ARRAY;
            array_construct(&p_subsubarray->list, 2);

            p_object_property = calloc(1, sizeof(json_value));
            p_object_property->type = JSON_VALUE_INTEGER;
            p_object_property->integer = 5;
            array_add(p_subsubarray->list, p_object_property);

            p_object_property = calloc(1, sizeof(json_value));
            p_object_property->type = JSON_VALUE_INTEGER;
            p_object_property->integer = 6;
            array_add(p_subsubarray->list, p_object_property);

            array_add(p_subarray->list, p_subsubarray);
        }

        {
            p_subsubarray = calloc(1, sizeof(json_value));
            p_subsubarray->type = JSON_VALUE_ARRAY;
            array_construct(&p_subsubarray->list, 2);

            p_object_property = calloc(1, sizeof(json_value));
            p_object_property->type = JSON_VALUE_INTEGER;
            p_object_property->integer = 7;
            array_add(p_subsubarray->list, p_object_property);

            p_object_property = calloc(1, sizeof(json_value));
            p_object_property->type = JSON_VALUE_INTEGER;
            p_object_property->integer = 8;
            array_add(p_subsubarray->list, p_object_property);

            array_add(p_subarray->list, p_subsubarray);
        }

        array_add(p_value->list, p_subarray);
    }

    // Return
    *pp_value = p_value;

    // success
    return 1;
}

bool test_parse_json ( char *test_file, int(*expected_value_constructor) (json_value **), result_t expected )
{
    
    // initialized data
    result_t     result   = 0,
                 value_eq = 0;
    char        *free_me = 0;
    json_value  *p_return_value   = 0,
                *p_expected_value = 0;

    // construct the expected json value
    if (expected_value_constructor) expected_value_constructor(&p_expected_value);

    // parse the json value
    result = load_json ( &p_return_value, test_file, NULL );

    // test for equality
    value_eq = value_equals(p_return_value, p_expected_value);

    // free the json value
    if ( p_return_value ) json_value_free(p_return_value, 0);
    if ( p_expected_value ) json_value_free(p_expected_value, 0);

    // release the allocation
    if ( free_me )
        free_me = default_allocator(free_me, 0);

    // success
    return (result == expected && value_eq);
}

bool test_serial_json ( char *test_file, char *expected_file, int(*expected_value_constructor) (json_value **), result_t expected )
{
    
    // Suppress warnings
    (void) expected;

    // initialized data
    json_value  *p_return_value   = 0,
                *p_expected_value = 0;
    bool         ret              = true;
    FILE        *p_f              = fopen(test_file, "w"),
                *p_ef             = 0;
    
    // Construct the expected value
    if ( expected_value_constructor ) expected_value_constructor(&p_expected_value);

    // Write the expected value to a file
    json_value_fprint(p_expected_value, p_f);
    
    // Clean up resources
    json_value_free(p_return_value, 0);
    json_value_free(p_expected_value, 0);

    fclose(p_f);

    // Open the expected and the result
    p_f  = fopen(test_file, "r");
    p_ef = fopen(expected_file, "r");    

    load_file(test_file, 0, false);
    load_file(expected_file, 0, false);
    
    // Compare the files
    {

        // initialized data
        size_t  f_len       = load_file(test_file, 0, false),
                ef_len      = load_file(expected_file, 0, false);
        char   *f_contents  = calloc(f_len+1, sizeof(char)),
               *ef_contents = calloc(ef_len+1, sizeof(char));

        // error checking
        if ( ef_contents == (void *) 0 ) return false;
        
        // error checking
        if ( f_contents == (void *) 0 ) return false;

        // Load each file
        load_file(test_file    , f_contents, false),
        load_file(expected_file, ef_contents, false);

        // For each character ...
        for (size_t i = 0; i < f_len; i++)

            // ... compare the expected to the result ...
            if ( f_contents[i] != ef_contents[i] )

                // ... and fail if there is a difference
                ret = false;


        // Free the file contents
        free(f_contents);
        free(ef_contents);
    }

    fclose(p_f);
    fclose(p_ef);

    // success
    return ret;
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

size_t load_file ( const char *path, void *buffer, bool binary_mode )
{

    // argument checking 
    if ( path == 0 ) goto no_path;

    // initialized data
    size_t  ret = 0;
    FILE   *f   = fopen(path, (binary_mode) ? "rb" : "r");
    
    // Check if file is valid
    if ( f == NULL ) goto invalid_file;

    // Find file size and prep for read
    fseek(f, 0, SEEK_END);
    ret = (size_t) ftell(f);
    fseek(f, 0, SEEK_SET);
    
    // Read to data
    if ( buffer ) 
        ret = fread(buffer, 1, ret, f);

    // The file is no longer needed
    fclose(f);
    
    // success
    return ret;

    // error handling
    {

        // argument errors
        {
            no_path:
                #ifndef NDEBUG
                    log_error("[json] Null path provided to function \"%s\n", __FUNCTION__);
                #endif

            // error
            return 0;
        }

        // File errors
        {
            invalid_file:
                #ifndef NDEBUG
                    printf("[Standard library] Failed to load file \"%s\". %s\n", path, strerror(errno));
                #endif

            // error
            return 0;
        }
    }
}
