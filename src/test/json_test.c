/** !
 * json tester
 * 
 * @file src/test/json_test.c
 * 
 * @author Jacob Smith
 */

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// gsdk
/// core
#include <core/log.h>
#include <core/sync.h>
#include <core/stream.h>
#include <core/test.h>

/// data
#include <data/array.h>
#include <data/dict.h>

/// reflection
#include <reflection/json.h>

// external declarations
extern fn_key_accessor object_key_accessor;

// function declarations
/// scenario constructors
fn_scenario_constructor construct_null;
fn_scenario_constructor construct_bool_false;
fn_scenario_constructor construct_bool_true;
fn_scenario_constructor construct_int_minus_one;
fn_scenario_constructor construct_int_zero;
fn_scenario_constructor construct_int_one;
fn_scenario_constructor construct_int_max;
fn_scenario_constructor construct_int_min;
fn_scenario_constructor construct_float_minus_one;
fn_scenario_constructor construct_float_zero;
fn_scenario_constructor construct_float_one;
fn_scenario_constructor construct_float_max;
fn_scenario_constructor construct_float_min;
fn_scenario_constructor construct_string_empty;
fn_scenario_constructor construct_string_a;
fn_scenario_constructor construct_string_abc;
fn_scenario_constructor construct_string_quote_abc_quote;
fn_scenario_constructor construct_string_quote;
fn_scenario_constructor construct_string_quote_quote;
fn_scenario_constructor construct_string_reverse_solidus;
fn_scenario_constructor construct_string_solidus;
fn_scenario_constructor construct_string_backspace;
fn_scenario_constructor construct_string_formfeed;
fn_scenario_constructor construct_string_linefeed;
fn_scenario_constructor construct_string_carriage_return;
fn_scenario_constructor construct_string_horizontal_tab;
fn_scenario_constructor construct_object_empty;
fn_scenario_constructor construct_object_string;
fn_scenario_constructor construct_object_int;
fn_scenario_constructor construct_object_float;
fn_scenario_constructor construct_object_false;
fn_scenario_constructor construct_object_true;
fn_scenario_constructor construct_object_mixed_values;
fn_scenario_constructor construct_object_object;
fn_scenario_constructor construct_object_object_object;
fn_scenario_constructor construct_object_recursive;
fn_scenario_constructor construct_object_array;
fn_scenario_constructor construct_object_array_objects;
fn_scenario_constructor construct_object_array_object;
fn_scenario_constructor construct_array_empty;
fn_scenario_constructor construct_array_null;
fn_scenario_constructor construct_array_nulls;
fn_scenario_constructor construct_array_bool;
fn_scenario_constructor construct_array_bools;
fn_scenario_constructor construct_array_int;
fn_scenario_constructor construct_array_ints;
fn_scenario_constructor construct_array_float;
fn_scenario_constructor construct_array_floats;
fn_scenario_constructor construct_array_string_empty;
fn_scenario_constructor construct_array_string;
fn_scenario_constructor construct_array_strings;
fn_scenario_constructor construct_array_object_empty;
fn_scenario_constructor construct_array_object;
fn_scenario_constructor construct_array_objects;
fn_scenario_constructor construct_array_array_empty;
fn_scenario_constructor construct_array_array_array_empty;
fn_scenario_constructor construct_array_matrix;
fn_scenario_constructor construct_array_tensor;

/// test cases
fn_test_case test_parse;
fn_test_case test_serialize;

/// allocators

/// result evaluators
fn_results_match json_results_match;
fn_results_match textual_results_match;

bool value_equals ( json_value *a, json_value *b );

// test
/// cases
test_case _json_test_cases[] = 
{
    TEST_MATCH("parse"    , test_parse    , NULL, json_results_match   ),
    TEST_MATCH("serialize", test_serialize, NULL, textual_results_match),
};

/// scenarios
test_scenario _scenarios[] = 
{
    TEST_SCENARIO("null"                   , "resources/test/json/null.json"                   , _json_test_cases, construct_null                   , json_allocator),
    TEST_SCENARIO("bool false"             , "resources/test/json/bool/false.json"             , _json_test_cases, construct_bool_false             , json_allocator),
    TEST_SCENARIO("bool true"              , "resources/test/json/bool/true.json"              , _json_test_cases, construct_bool_true              , json_allocator),
    TEST_SCENARIO("int minus one"          , "resources/test/json/int/-1.json"                 , _json_test_cases, construct_int_minus_one          , json_allocator),
    TEST_SCENARIO("int zero"               , "resources/test/json/int/0.json"                  , _json_test_cases, construct_int_zero               , json_allocator),
    TEST_SCENARIO("int one"                , "resources/test/json/int/1.json"                  , _json_test_cases, construct_int_one                , json_allocator),
    TEST_SCENARIO("int max"                , "resources/test/json/int/max.json"                , _json_test_cases, construct_int_max                , json_allocator),
    TEST_SCENARIO("int min"                , "resources/test/json/int/min.json"                , _json_test_cases, construct_int_min                , json_allocator),
    TEST_SCENARIO("float minus one"        , "resources/test/json/float/-1.json"               , _json_test_cases, construct_float_minus_one        , json_allocator),
    TEST_SCENARIO("float zero"             , "resources/test/json/float/0.json"                , _json_test_cases, construct_float_zero             , json_allocator),
    TEST_SCENARIO("float one"              , "resources/test/json/float/1.json"                , _json_test_cases, construct_float_one              , json_allocator),
    TEST_SCENARIO("float max"              , "resources/test/json/float/max.json"              , _json_test_cases, construct_float_max              , json_allocator),
    TEST_SCENARIO("float min"              , "resources/test/json/float/min.json"              , _json_test_cases, construct_float_min              , json_allocator),
    TEST_SCENARIO("string empty"           , "resources/test/json/string/empty.json"           , _json_test_cases, construct_string_empty           , json_allocator),
    TEST_SCENARIO("string a"               , "resources/test/json/string/a.json"               , _json_test_cases, construct_string_a               , json_allocator),
    TEST_SCENARIO("string abc"             , "resources/test/json/string/abc.json"             , _json_test_cases, construct_string_abc             , json_allocator),
    TEST_SCENARIO("string quote abc quote" , "resources/test/json/string/quote_abc_quote.json" , _json_test_cases, construct_string_quote_abc_quote , json_allocator),
    TEST_SCENARIO("string quote"           , "resources/test/json/string/quote.json"           , _json_test_cases, construct_string_quote           , json_allocator),
    TEST_SCENARIO("string quote quote"     , "resources/test/json/string/quote_quote.json"     , _json_test_cases, construct_string_quote_quote     , json_allocator),
    TEST_SCENARIO("string reverse solidus" , "resources/test/json/string/reverse_solidus.json" , _json_test_cases, construct_string_reverse_solidus , json_allocator),
    TEST_SCENARIO("string solidus"         , "resources/test/json/string/solidus.json"         , _json_test_cases, construct_string_solidus         , json_allocator),
    TEST_SCENARIO("string backspace"       , "resources/test/json/string/backspace.json"       , _json_test_cases, construct_string_backspace       , json_allocator),
    TEST_SCENARIO("string formfeed"        , "resources/test/json/string/formfeed.json"        , _json_test_cases, construct_string_formfeed        , json_allocator),
    TEST_SCENARIO("string linefeed"        , "resources/test/json/string/linefeed.json"        , _json_test_cases, construct_string_linefeed        , json_allocator),
    TEST_SCENARIO("string carriage return" , "resources/test/json/string/carriage_return.json" , _json_test_cases, construct_string_carriage_return , json_allocator),
    TEST_SCENARIO("string horizontal tab"  , "resources/test/json/string/horizontal_tab.json"  , _json_test_cases, construct_string_horizontal_tab  , json_allocator),
    TEST_SCENARIO("object empty"           , "resources/test/json/object/empty.json"           , _json_test_cases, construct_object_empty           , json_allocator),
    TEST_SCENARIO("object string"          , "resources/test/json/object/string.json"          , _json_test_cases, construct_object_string          , json_allocator),
    TEST_SCENARIO("object int"             , "resources/test/json/object/int.json"             , _json_test_cases, construct_object_int             , json_allocator),
    TEST_SCENARIO("object float"           , "resources/test/json/object/float.json"           , _json_test_cases, construct_object_float           , json_allocator),
    TEST_SCENARIO("object false"           , "resources/test/json/object/false.json"           , _json_test_cases, construct_object_false           , json_allocator),
    TEST_SCENARIO("object true"            , "resources/test/json/object/true.json"            , _json_test_cases, construct_object_true            , json_allocator),
    TEST_SCENARIO("object mixed values"    , "resources/test/json/object/mixed_values.json"    , _json_test_cases, construct_object_mixed_values    , json_allocator),
    TEST_SCENARIO("object object"          , "resources/test/json/object/object.json"          , _json_test_cases, construct_object_object          , json_allocator),
    TEST_SCENARIO("object object object"   , "resources/test/json/object/object_object.json"   , _json_test_cases, construct_object_object_object   , json_allocator),
    TEST_SCENARIO("object recursive"       , "resources/test/json/object/recursive.json"       , _json_test_cases, construct_object_recursive       , json_allocator),
    TEST_SCENARIO("object array"           , "resources/test/json/object/array.json"           , _json_test_cases, construct_object_array           , json_allocator),
    TEST_SCENARIO("object array objects"   , "resources/test/json/object/array_objects.json"   , _json_test_cases, construct_object_array_objects   , json_allocator),
    TEST_SCENARIO("object array object"    , "resources/test/json/object/array_object.json"    , _json_test_cases, construct_object_array_object    , json_allocator),
    TEST_SCENARIO("array empty"            , "resources/test/json/array/empty.json"            , _json_test_cases, construct_array_empty            , json_allocator),
    TEST_SCENARIO("array null"             , "resources/test/json/array/null.json"             , _json_test_cases, construct_array_null             , json_allocator),
    TEST_SCENARIO("array nulls"            , "resources/test/json/array/nulls.json"            , _json_test_cases, construct_array_nulls            , json_allocator),
    TEST_SCENARIO("array bool"             , "resources/test/json/array/bool.json"             , _json_test_cases, construct_array_bool             , json_allocator),
    TEST_SCENARIO("array bools"            , "resources/test/json/array/bools.json"            , _json_test_cases, construct_array_bools            , json_allocator),
    TEST_SCENARIO("array int"              , "resources/test/json/array/int.json"              , _json_test_cases, construct_array_int              , json_allocator),
    TEST_SCENARIO("array ints"             , "resources/test/json/array/ints.json"             , _json_test_cases, construct_array_ints             , json_allocator),
    TEST_SCENARIO("array float"            , "resources/test/json/array/float.json"            , _json_test_cases, construct_array_float            , json_allocator),
    TEST_SCENARIO("array floats"           , "resources/test/json/array/floats.json"           , _json_test_cases, construct_array_floats           , json_allocator),
    TEST_SCENARIO("array string empty"     , "resources/test/json/array/string_empty.json"     , _json_test_cases, construct_array_string_empty     , json_allocator),
    TEST_SCENARIO("array string"           , "resources/test/json/array/string.json"           , _json_test_cases, construct_array_string           , json_allocator),
    TEST_SCENARIO("array strings"          , "resources/test/json/array/strings.json"          , _json_test_cases, construct_array_strings          , json_allocator),
    TEST_SCENARIO("array object empty"     , "resources/test/json/array/object_empty.json"     , _json_test_cases, construct_array_object_empty     , json_allocator),
    TEST_SCENARIO("array object"           , "resources/test/json/array/object.json"           , _json_test_cases, construct_array_object           , json_allocator),
    TEST_SCENARIO("array objects"          , "resources/test/json/array/objects.json"          , _json_test_cases, construct_array_objects          , json_allocator),
    TEST_SCENARIO("array array empty"      , "resources/test/json/array/array_empty.json"      , _json_test_cases, construct_array_array_empty      , json_allocator),
    TEST_SCENARIO("array array array empty", "resources/test/json/array/array_array_empty.json", _json_test_cases, construct_array_array_array_empty, json_allocator),
    TEST_SCENARIO("array matrix"           , "resources/test/json/array/matrix.json"           , _json_test_cases, construct_array_matrix           , json_allocator),
    TEST_SCENARIO("array tensor"           , "resources/test/json/array/tensor.json"           , _json_test_cases, construct_array_tensor           , json_allocator),
};

/// suites
test_suite _suite = TEST_SUITE("json", _scenarios);

// entry point
int main ( int argc, const char *argv[] ) 
{

    // unused
    (void) argc;
    (void) argv;
     
    // run the tests
    test_suite_test(&_suite); 
    
    // done
    return (_suite.counters.total.fails == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int construct_null ( void **pp_result )
{

    // null is NULL
    *pp_result = NULL;

    // success
    return 1;
}

int construct_bool_false ( void **pp_result )
{

    // initialized data
    json_value **pp_value = (json_value **) pp_result;
    json_value  *p_value  = default_allocator(NULL, sizeof(json_value));
    
    // populate the json value
    *p_value = (json_value) { .type = JSON_VALUE_BOOLEAN, .boolean = false };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_bool_true ( void **pp_result )
{

    // initialized data
    json_value **pp_value = (json_value **) pp_result;
    json_value  *p_value  = default_allocator(NULL, sizeof(json_value));
    
    // populate the json value
    *p_value = (json_value) { .type = JSON_VALUE_BOOLEAN, .boolean = true };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_int_minus_one ( void **pp_result )
{

    // initialized data
    json_value **pp_value = (json_value **) pp_result;
    json_value  *p_value  = default_allocator(NULL, sizeof(json_value));
    
    // populate the json value
    *p_value = (json_value) { .type = JSON_VALUE_INTEGER, .integer = -1 };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_int_zero ( void **pp_result )
{

    // initialized data
    json_value **pp_value = (json_value **) pp_result;
    json_value  *p_value  = default_allocator(NULL, sizeof(json_value));
    
    // populate the json value
    *p_value = (json_value) { .type = JSON_VALUE_INTEGER, .integer = 0 };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_int_one ( void **pp_result )
{

    // initialized data
    json_value **pp_value = (json_value **) pp_result;
    json_value  *p_value  = default_allocator(NULL, sizeof(json_value));
    
    // populate the json value
    *p_value = (json_value) { .type = JSON_VALUE_INTEGER, .integer = 1 };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_int_max ( void **pp_result )
{

    // initialized data
    json_value **pp_value = (json_value **) pp_result;
    json_value  *p_value  = default_allocator(NULL, sizeof(json_value));
    
    // populate the json value
    *p_value = (json_value) { .type = JSON_VALUE_INTEGER, .integer = 9223372036854775807 };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_int_min ( void **pp_result )
{

    // initialized data
    json_value **pp_value = (json_value **) pp_result;
    json_value  *p_value  = default_allocator(NULL, sizeof(json_value));
    
    // populate the json value
    *p_value = (json_value) { .type = JSON_VALUE_INTEGER, .integer = -9223372036854775807 };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_float_minus_one ( void **pp_result )
{

    // initialized data
    json_value **pp_value = (json_value **) pp_result;
    json_value  *p_value  = default_allocator(NULL, sizeof(json_value));
    
    // populate the json value
    *p_value = (json_value) { .type = JSON_VALUE_NUMBER, .number = -1.0 };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_float_zero ( void **pp_result )
{

    // initialized data
    json_value **pp_value = (json_value **) pp_result;
    json_value  *p_value  = default_allocator(NULL, sizeof(json_value));
    
    // populate the json value
    *p_value = (json_value) { .type = JSON_VALUE_NUMBER, .number = 0.0 };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_float_one ( void **pp_result )
{

    // initialized data
    json_value **pp_value = (json_value **) pp_result;
    json_value  *p_value  = default_allocator(NULL, sizeof(json_value));
    
    // populate the json value
    *p_value = (json_value) { .type = JSON_VALUE_NUMBER, .number = 1.0 };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_float_max ( void **pp_result )
{

    // initialized data
    json_value **pp_value = (json_value **) pp_result;
    json_value  *p_value  = default_allocator(NULL, sizeof(json_value));
    
    // populate the json value
    *p_value = (json_value) { .type = JSON_VALUE_NUMBER, .number = DBL_MAX };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_float_min ( void **pp_result )
{

    // initialized data
    json_value **pp_value = (json_value **) pp_result;
    json_value  *p_value  = default_allocator(NULL, sizeof(json_value));
    
    // populate the json value
    *p_value = (json_value) { .type = JSON_VALUE_NUMBER, .number = -DBL_MAX };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_string_empty ( void **pp_result )
{

    // initialized data
    json_value **pp_value = (json_value **) pp_result;
    json_value  *p_value  = default_allocator(NULL, sizeof(json_value));
    char        *p_string = default_allocator(NULL, 32);

    // store the string
    strncpy(p_string, "", 1);

    // populate the json value
    *p_value = (json_value) { .type = JSON_VALUE_STRING, .string = p_string };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_string_a ( void **pp_result )
{

    // initialized data
    json_value **pp_value = (json_value **) pp_result;
    json_value  *p_value  = default_allocator(NULL, sizeof(json_value));
    char        *p_string = default_allocator(NULL, 32);

    // store the string
    strncpy(p_string, "a", 2);

    // populate the json value
    *p_value = (json_value) { .type = JSON_VALUE_STRING, .string = p_string };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_string_abc ( void **pp_result )
{

    // initialized data
    json_value **pp_value = (json_value **) pp_result;
    json_value  *p_value  = default_allocator(NULL, sizeof(json_value));
    char        *p_string = default_allocator(NULL, 32);

    // store the string
    strncpy(p_string, "abc", 4);

    // populate the json value
    *p_value = (json_value) { .type = JSON_VALUE_STRING, .string = p_string };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_string_quote_abc_quote ( void **pp_result )
{

    // initialized data
    json_value **pp_value = (json_value **) pp_result;
    json_value  *p_value  = default_allocator(NULL, sizeof(json_value));
    char        *p_string = default_allocator(NULL, 32);

    // store the string
    strncpy(p_string, "\"abc\"", 6);

    // populate the json value
    *p_value = (json_value) { .type = JSON_VALUE_STRING, .string = p_string };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_string_quote ( void **pp_result )
{

    // initialized data
    json_value **pp_value = (json_value **) pp_result;
    json_value  *p_value  = default_allocator(NULL, sizeof(json_value));
    char        *p_string = default_allocator(NULL, 32);

    // store the string
    strncpy(p_string, "\"", 2);

    // populate the json value
    *p_value = (json_value) { .type = JSON_VALUE_STRING, .string = p_string };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_string_quote_quote ( void **pp_result )
{

    // initialized data
    json_value **pp_value = (json_value **) pp_result;
    json_value  *p_value  = default_allocator(NULL, sizeof(json_value));
    char        *p_string = default_allocator(NULL, 32);

    // store the string
    strncpy(p_string, "\"\"", 3);

    // populate the json value
    *p_value = (json_value) { .type = JSON_VALUE_STRING, .string = p_string };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_string_reverse_solidus ( void **pp_result )
{

    // initialized data
    json_value **pp_value = (json_value **) pp_result;
    json_value  *p_value  = default_allocator(NULL, sizeof(json_value));
    char        *p_string = default_allocator(NULL, 32);

    // store the string
    strncpy(p_string, "\\", 2);

    // populate the json value
    *p_value = (json_value) { .type = JSON_VALUE_STRING, .string = p_string };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_string_solidus ( void **pp_result )
{

    // initialized data
    json_value **pp_value = (json_value **) pp_result;
    json_value  *p_value  = default_allocator(NULL, sizeof(json_value));
    char        *p_string = default_allocator(NULL, 32);

    // store the string
    strncpy(p_string, "/", 2);

    // populate the json value
    *p_value = (json_value) { .type = JSON_VALUE_STRING, .string = p_string };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_string_backspace ( void **pp_result )
{

    // initialized data
    json_value **pp_value = (json_value **) pp_result;
    json_value  *p_value  = default_allocator(NULL, sizeof(json_value));
    char        *p_string = default_allocator(NULL, 32);

    // store the string
    strncpy(p_string, "\b", 2);

    // populate the json value
    *p_value = (json_value) { .type = JSON_VALUE_STRING, .string = p_string };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_string_formfeed ( void **pp_result )
{

    // initialized data
    json_value **pp_value = (json_value **) pp_result;
    json_value  *p_value  = default_allocator(NULL, sizeof(json_value));
    char        *p_string = default_allocator(NULL, 32);

    // store the string
    strncpy(p_string, "\f", 2);

    // populate the json value
    *p_value = (json_value) { .type = JSON_VALUE_STRING, .string = p_string };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_string_linefeed ( void **pp_result )
{

    // initialized data
    json_value **pp_value = (json_value **) pp_result;
    json_value  *p_value  = default_allocator(NULL, sizeof(json_value));
    char        *p_string = default_allocator(NULL, 32);

    // store the string
    strncpy(p_string, "\n", 2);

    // populate the json value
    *p_value = (json_value) { .type = JSON_VALUE_STRING, .string = p_string };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_string_carriage_return ( void **pp_result )
{

    // initialized data
    json_value **pp_value = (json_value **) pp_result;
    json_value  *p_value  = default_allocator(NULL, sizeof(json_value));
    char        *p_string = default_allocator(NULL, 32);

    // store the string
    strncpy(p_string, "\r", 2);

    // populate the json value
    *p_value = (json_value) { .type = JSON_VALUE_STRING, .string = p_string };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_string_horizontal_tab ( void **pp_result )
{

    // initialized data
    json_value **pp_value = (json_value **) pp_result;
    json_value  *p_value  = default_allocator(NULL, sizeof(json_value));
    char        *p_string = default_allocator(NULL, 32);

    // store the string
    strncpy(p_string, "\t", 2);

    // populate the json value
    *p_value = (json_value) { .type = JSON_VALUE_STRING, .string = p_string };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_object_empty ( void **pp_result )
{

    // initialized data
    json_value **pp_value = (json_value **) pp_result;
    json_value  *p_value  = default_allocator(NULL, sizeof(json_value));
    dict        *p_dict   = NULL;

    // construct a dictionary
    dict_construct(&p_dict, 16, NULL, object_key_accessor, NULL);

    // populate the json value
    *p_value = (json_value) { .type = JSON_VALUE_OBJECT, .object = p_dict };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_object_string ( void **pp_result )
{

    // initialized data
    json_value **pp_value = (json_value **) pp_result;
    json_value  *p_value  = default_allocator(NULL, sizeof(json_value));
    json_value  *p_1      = default_allocator(NULL, sizeof(json_value));
    dict        *p_dict   = NULL;

    // construct a dictionary
    dict_construct(&p_dict, 16, NULL, object_key_accessor, NULL);

    // populate 1
    *p_1 = (json_value) { .type = JSON_VALUE_STRING, .p_key = strdup("abc"), .string = strdup("def"), };
    
    // populate the dictionary
    dict_add(p_dict, p_1);

    // populate the json value
    *p_value = (json_value) { .type = JSON_VALUE_OBJECT, .object = p_dict };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_object_int ( void **pp_result )
{

    // initialized data
    json_value **pp_value = (json_value **) pp_result;
    json_value  *p_value  = default_allocator(NULL, sizeof(json_value));
    json_value  *p_1      = default_allocator(NULL, sizeof(json_value));
    dict        *p_dict   = NULL;

    // construct a dictionary
    dict_construct(&p_dict, 16, NULL, object_key_accessor, NULL);

    // populate 1
    *p_1 = (json_value) { .type = JSON_VALUE_INTEGER, .p_key = strdup("abc"), .integer = 123, };
    
    // populate the dictionary
    dict_add(p_dict, p_1);

    // populate the json value
    *p_value = (json_value) { .type = JSON_VALUE_OBJECT, .object = p_dict };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_object_float ( void **pp_result )
{

    // initialized data
    json_value **pp_value = (json_value **) pp_result;
    json_value  *p_value  = default_allocator(NULL, sizeof(json_value));
    json_value  *p_1      = default_allocator(NULL, sizeof(json_value));
    dict        *p_dict   = NULL;

    // construct a dictionary
    dict_construct(&p_dict, 16, NULL, object_key_accessor, NULL);

    // populate 1
    *p_1 = (json_value) { .type = JSON_VALUE_NUMBER, .p_key = strdup("pi"), .number = 3.14, };
    
    // populate the dictionary
    dict_add(p_dict, p_1);

    // populate the json value
    *p_value = (json_value) { .type = JSON_VALUE_OBJECT, .object = p_dict };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_object_false ( void **pp_result )
{

    // initialized data
    json_value **pp_value = (json_value **) pp_result;
    json_value  *p_value  = default_allocator(NULL, sizeof(json_value));
    json_value  *p_1      = default_allocator(NULL, sizeof(json_value));
    dict        *p_dict   = NULL;

    // construct a dictionary
    dict_construct(&p_dict, 16, NULL, object_key_accessor, NULL);

    // populate 1
    *p_1 = (json_value) { .type = JSON_VALUE_BOOLEAN, .p_key = strdup("abc"), .boolean = false, };
    
    // populate the dictionary
    dict_add(p_dict, p_1);

    // populate the json value
    *p_value = (json_value) { .type = JSON_VALUE_OBJECT, .object = p_dict };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_object_true ( void **pp_result )
{

    // initialized data
    json_value **pp_value = (json_value **) pp_result;
    json_value  *p_value  = default_allocator(NULL, sizeof(json_value));
    json_value  *p_1      = default_allocator(NULL, sizeof(json_value));
    dict        *p_dict   = NULL;

    // construct a dictionary
    dict_construct(&p_dict, 16, NULL, object_key_accessor, NULL);

    // populate 1
    *p_1 = (json_value) { .type = JSON_VALUE_BOOLEAN, .p_key = strdup("abc"), .boolean = true, };
    
    // populate the dictionary
    dict_add(p_dict, p_1);

    // populate the json value
    *p_value = (json_value) { .type = JSON_VALUE_OBJECT, .object = p_dict };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_object_mixed_values ( void **pp_result )
{

    // initialized data
    json_value **pp_value = (json_value **) pp_result;
    json_value  *p_value  = default_allocator(NULL, sizeof(json_value));
    json_value  *p_1      = default_allocator(NULL, sizeof(json_value));
    json_value  *p_2      = default_allocator(NULL, sizeof(json_value));
    json_value  *p_3      = default_allocator(NULL, sizeof(json_value));
    dict        *p_dict   = NULL;

    // construct a dictionary
    dict_construct(&p_dict, 16, NULL, object_key_accessor, NULL);

    // populate 1
    *p_1 = (json_value) { .type = JSON_VALUE_STRING, .p_key = strdup("name"), .string = strdup("Jacob"), };
    
    // populate 2
    *p_2 = (json_value) { .type = JSON_VALUE_INTEGER, .p_key = strdup("age"), .integer = 23, };
    
    // populate 3
    *p_3 = (json_value) { .type = JSON_VALUE_NUMBER, .p_key = strdup("height"), .number = 1.83, };
    
    // populate the dictionary
    dict_add(p_dict, p_1);
    dict_add(p_dict, p_2);
    dict_add(p_dict, p_3);

    // populate the json value
    *p_value = (json_value) { .type = JSON_VALUE_OBJECT, .object = p_dict };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_object_object ( void **pp_result )
{

    // initialized data
    json_value **pp_value = (json_value **) pp_result;
    json_value  *p_value  = default_allocator(NULL, sizeof(json_value));
    json_value  *p_1      = default_allocator(NULL, sizeof(json_value));
    json_value  *p_2      = default_allocator(NULL, sizeof(json_value));
    dict        *p_dict   = NULL;
    dict        *p_dict1  = NULL;

    // construct a dictionary
    dict_construct(&p_dict, 16, NULL, object_key_accessor, NULL);

    // construct another dictionary
    dict_construct(&p_dict1, 16, NULL, object_key_accessor, NULL);
    
    // populate 2
    *p_2 = (json_value) { .type = JSON_VALUE_INTEGER, .p_key = strdup("def"), .integer = 123, };

    // populate dictionary 1
    dict_add(p_dict1, p_2);
    
    // populate 1
    *p_1 = (json_value) { .type = JSON_VALUE_OBJECT, .p_key = strdup("abc"), .object = p_dict1, };

    // populate the dictionary
    dict_add(p_dict, p_1);

    // populate the json value
    *p_value = (json_value) { .type = JSON_VALUE_OBJECT, .object = p_dict };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_object_object_object ( void **pp_result )
{

    // initialized data
    json_value **pp_value = (json_value **) pp_result;
    json_value  *p_value  = default_allocator(NULL, sizeof(json_value));
    json_value  *p_1      = default_allocator(NULL, sizeof(json_value));
    json_value  *p_2      = default_allocator(NULL, sizeof(json_value));
    json_value  *p_3      = default_allocator(NULL, sizeof(json_value));
    dict        *p_dict   = NULL;
    dict        *p_dict1  = NULL;
    dict        *p_dict2  = NULL;

    // construct a dictionary
    dict_construct(&p_dict, 16, NULL, object_key_accessor, NULL);

    // construct another dictionary
    dict_construct(&p_dict1, 16, NULL, object_key_accessor, NULL);
    
    // construct yet another dictionary
    dict_construct(&p_dict2, 16, NULL, object_key_accessor, NULL);
    
    // populate 3
    *p_3 = (json_value) { .type = JSON_VALUE_INTEGER, .p_key = strdup("ghi"), .integer = 123, };

    // populate dictionary 2
    dict_add(p_dict2, p_3);

    // populate 2
    *p_2 = (json_value) { .type = JSON_VALUE_OBJECT, .p_key = strdup("def"), .object = p_dict2, };

    // populate dictionary 1
    dict_add(p_dict1, p_2);
    
    // populate 1
    *p_1 = (json_value) { .type = JSON_VALUE_OBJECT, .p_key = strdup("abc"), .object = p_dict1, };

    // populate the dictionary
    dict_add(p_dict, p_1);

    // populate the json value
    *p_value = (json_value) { .type = JSON_VALUE_OBJECT, .object = p_dict };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_array_empty ( void **pp_result )
{

    // initialized data
    json_value **pp_value = (json_value **) pp_result;
    json_value  *p_value  = default_allocator(NULL, sizeof(json_value));
    array       *p_array  = NULL;

    // construct an array
    array_construct(&p_array, 1);

    // populate the json value
    *p_value = (json_value) { .type = JSON_VALUE_ARRAY, .list = p_array };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_array_null ( void **pp_result )
{

    // initialized data
    json_value **pp_value = (json_value **) pp_result;
    json_value  *p_value  = default_allocator(NULL, sizeof(json_value));
    array       *p_array  = NULL;

    // construct an array 
    array_construct(&p_array, 1);
    
    // populate the array
    array_add(p_array, NULL);

    // populate the json value
    *p_value = (json_value) { .type = JSON_VALUE_ARRAY, .list = p_array };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_array_nulls ( void **pp_result )
{

    // initialized data
    json_value **pp_value = (json_value **) pp_result;
    json_value  *p_value  = default_allocator(NULL, sizeof(json_value));
    array       *p_array  = NULL;

    // construct an array 
    array_construct(&p_array, 1);
    
    // populate the array
    array_add(p_array, NULL);
    array_add(p_array, NULL);
    array_add(p_array, NULL);

    // populate the json value
    *p_value = (json_value) { .type = JSON_VALUE_ARRAY, .list = p_array };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_array_bool ( void **pp_result )
{

    // initialized data
    json_value **pp_value = (json_value **) pp_result;
    json_value  *p_value  = default_allocator(NULL, sizeof(json_value));
    json_value  *p_1      = default_allocator(NULL, sizeof(json_value));
    array       *p_array  = NULL;

    // construct an array 
    array_construct(&p_array, 1);
    
    // populate 1
    *p_1 = (json_value) { .type = JSON_VALUE_BOOLEAN, .boolean = true, };
    
    // populate the array
    array_add(p_array, p_1);

    // populate the json value
    *p_value = (json_value) { .type = JSON_VALUE_ARRAY, .list = p_array };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_array_bools ( void **pp_result )
{

    // initialized data
    json_value **pp_value = (json_value **) pp_result;
    json_value  *p_value  = default_allocator(NULL, sizeof(json_value));
    json_value  *p_1      = default_allocator(NULL, sizeof(json_value));
    json_value  *p_2      = default_allocator(NULL, sizeof(json_value));
    json_value  *p_3      = default_allocator(NULL, sizeof(json_value));
    array       *p_array  = NULL;

    // construct an array 
    array_construct(&p_array, 1);
    
    // populate 1
    *p_1 = (json_value) { .type = JSON_VALUE_BOOLEAN, .boolean = true, };
    
    // populate 2
    *p_2 = (json_value) { .type = JSON_VALUE_BOOLEAN, .boolean = false, };
    
    // populate 3
    *p_3 = (json_value) { .type = JSON_VALUE_BOOLEAN, .boolean = true, };
    
    // populate the array
    array_add(p_array, p_1),
    array_add(p_array, p_2),
    array_add(p_array, p_3);

    // populate the json value
    *p_value = (json_value) { .type = JSON_VALUE_ARRAY, .list = p_array };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_array_int ( void **pp_result )
{

    // initialized data
    json_value **pp_value = (json_value **) pp_result;
    json_value  *p_value  = default_allocator(NULL, sizeof(json_value));
    json_value  *p_1      = default_allocator(NULL, sizeof(json_value));
    array       *p_array  = NULL;

    // construct an array 
    array_construct(&p_array, 1);
    
    // populate 1
    *p_1 = (json_value) { .type = JSON_VALUE_INTEGER, .integer = 1, };
    
    // populate the array
    array_add(p_array, p_1);

    // populate the json value
    *p_value = (json_value) { .type = JSON_VALUE_ARRAY, .list = p_array };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_array_ints ( void **pp_result )
{

    // initialized data
    json_value **pp_value = (json_value **) pp_result;
    json_value  *p_value  = default_allocator(NULL, sizeof(json_value));
    json_value  *p_1      = default_allocator(NULL, sizeof(json_value));
    json_value  *p_2      = default_allocator(NULL, sizeof(json_value));
    json_value  *p_3      = default_allocator(NULL, sizeof(json_value));
    array       *p_array  = NULL;

    // construct an array 
    array_construct(&p_array, 1);
    
    // populate 1
    *p_1 = (json_value) { .type = JSON_VALUE_INTEGER, .integer = 1, };
    
    // populate 2
    *p_2 = (json_value) { .type = JSON_VALUE_INTEGER, .integer = 2, };
    
    // populate 3
    *p_3 = (json_value) { .type = JSON_VALUE_INTEGER, .integer = 3, };
    
    // populate the array
    array_add(p_array, p_1),
    array_add(p_array, p_2),
    array_add(p_array, p_3);

    // populate the json value
    *p_value = (json_value) { .type = JSON_VALUE_ARRAY, .list = p_array };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_array_float ( void **pp_result )
{

    // initialized data
    json_value **pp_value = (json_value **) pp_result;
    json_value  *p_value  = default_allocator(NULL, sizeof(json_value));
    json_value  *p_1      = default_allocator(NULL, sizeof(json_value));
    array       *p_array  = NULL;

    // construct an array 
    array_construct(&p_array, 1);
    
    // populate 1
    *p_1 = (json_value) { .type = JSON_VALUE_NUMBER, .number = 3.14, };
    
    // populate the array
    array_add(p_array, p_1);

    // populate the json value
    *p_value = (json_value) { .type = JSON_VALUE_ARRAY, .list = p_array };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_array_floats ( void **pp_result )
{

    // initialized data
    json_value **pp_value = (json_value **) pp_result;
    json_value  *p_value  = default_allocator(NULL, sizeof(json_value));
    json_value  *p_1      = default_allocator(NULL, sizeof(json_value));
    json_value  *p_2      = default_allocator(NULL, sizeof(json_value));
    json_value  *p_3      = default_allocator(NULL, sizeof(json_value));
    array       *p_array  = NULL;

    // construct an array 
    array_construct(&p_array, 1);
    
    // populate 1
    *p_1 = (json_value) { .type = JSON_VALUE_NUMBER, .number = 1.2, };
    
    // populate 2
    *p_2 = (json_value) { .type = JSON_VALUE_NUMBER, .number = 3.4, };
    
    // populate 3
    *p_3 = (json_value) { .type = JSON_VALUE_NUMBER, .number = 5.6, };
    
    // populate the array
    array_add(p_array, p_1),
    array_add(p_array, p_2),
    array_add(p_array, p_3);

    // populate the json value
    *p_value = (json_value) { .type = JSON_VALUE_ARRAY, .list = p_array };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_object_recursive ( void **pp_result )
{
    
    // initialized data
    json_value **pp_value  = (json_value **) pp_result;
    json_value  *p_value   = default_allocator(NULL, sizeof(json_value));
    dict        *p_dict    = NULL;
    dict        *p_current = NULL;

    // construct a dictionary
    dict_construct(&p_dict, 16, NULL, object_key_accessor, NULL);

    // populate the value
    *p_value = (json_value) { .type = JSON_VALUE_OBJECT, .object = p_dict };

    // store the root
    p_current = p_dict;

    for (char c = 'a'; c <= 'z'; c++) 
    {

        // initialized data
        json_value *p_value = default_allocator(NULL, sizeof(json_value));
        dict       *p_next  = NULL;
        char        key[2]  = { c, '\0' };

        // construct a dictionary
        dict_construct(&p_next, 16, NULL, object_key_accessor, NULL);

        // populate the child
        *p_value = (json_value) { .type = JSON_VALUE_OBJECT, .p_key = strdup(key), .object = p_next };

        // populate the dictionary
        dict_add(p_current, p_value);

        // store the next dictionary
        p_current = p_next;
    }

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_object_array ( void **pp_result )
{

    // initialized data
    json_value **pp_value = (json_value **) pp_result;
    json_value  *p_value  = default_allocator(NULL, sizeof(json_value));
    json_value  *p_1      = default_allocator(NULL, sizeof(json_value));
    dict        *p_dict   = NULL;
    array       *p_array  = NULL;

    // construct a dictionary
    dict_construct(&p_dict, 16, NULL, object_key_accessor, NULL);

    // construct an array
    array_construct(&p_array, 1);

    // populate the array
    for (int i = 1; i <= 3; i++) 
    {

        // initialized data
        json_value *p_2 = default_allocator(NULL, sizeof(json_value));

        // populate the value
        *p_2 = (json_value) { .type = JSON_VALUE_INTEGER, .integer = i };

        // populate the array
        array_add(p_array, p_2);
    }

    // populate the value
    *p_1 = (json_value) { .type = JSON_VALUE_ARRAY, .p_key = strdup("abc"), .list = p_array };

    // populate the dictionary
    dict_add(p_dict, p_1);

    // populate the value
    *p_value = (json_value) { .type = JSON_VALUE_OBJECT, .object = p_dict };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_object_array_objects ( void **pp_result )
{

    // initialized data
    json_value **pp_value = (json_value **) pp_result;
    json_value  *p_value  = default_allocator(NULL, sizeof(json_value));
    json_value  *p_1      = default_allocator(NULL, sizeof(json_value));
    dict        *p_dict   = NULL;
    array       *p_array  = NULL;
    char        *keys[]   = {"a", "b", "c"};

    // construct a dictionary
    dict_construct(&p_dict, 16, NULL, object_key_accessor, NULL);

    // construct an array
    array_construct(&p_array, 1);

    // populate the values
    for (int i = 0; i < 3; i++) 
    {

        // initialized data
        json_value *p_2    = default_allocator(NULL, sizeof(json_value));
        json_value *p_3    = default_allocator(NULL, sizeof(json_value));
        dict       *p_dict = NULL;

        // construct a dictionary
        dict_construct(&p_dict, 16, NULL, object_key_accessor, NULL);

        // populate the value
        *p_2 = (json_value) { .type = JSON_VALUE_INTEGER, .p_key = strdup(keys[i]), .integer = i + 1 };

        // populate the dictionary
        dict_add(p_dict, p_2);

        // populate the value
        *p_3 = (json_value) { .type = JSON_VALUE_OBJECT, .object = p_dict };

        // populate the array
        array_add(p_array, p_3);
    }

    // populate the value
    *p_1 = (json_value) { .type = JSON_VALUE_ARRAY, .p_key = strdup("a"), .list = p_array };

    // populate the dictionary
    dict_add(p_dict, p_1);

    // populate the value
    *p_value = (json_value) { .type = JSON_VALUE_OBJECT, .object = p_dict };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_object_array_object ( void **pp_result )
{

    // initialized data
    json_value **pp_value    = (json_value **) pp_result;
    json_value  *p_value     = default_allocator(NULL, sizeof(json_value));
    json_value  *p_1         = default_allocator(NULL, sizeof(json_value));
    json_value  *p_2         = default_allocator(NULL, sizeof(json_value));
    json_value  *p_3         = default_allocator(NULL, sizeof(json_value));
    dict        *p_dict      = NULL;
    array       *p_array     = NULL;
    dict        *p_item_dict = NULL;

    // construct a dictionary
    dict_construct(&p_dict, 16, NULL, object_key_accessor, NULL);

    // construct an array
    array_construct(&p_array, 1);
    
    // construct a dictionary
    dict_construct(&p_item_dict, 16, NULL, object_key_accessor, NULL);

    // populate the value
    *p_1 = (json_value) { .type = JSON_VALUE_INTEGER, .p_key = strdup("a"), .integer = 1 };

    // populate the dictionary
    dict_add(p_item_dict, p_1);

    // populate the value
    *p_2 = (json_value) { .type = JSON_VALUE_OBJECT, .object = p_item_dict };

    // populate the array
    array_add(p_array, p_2);

    // populate the value
    *p_3 = (json_value) { .type = JSON_VALUE_ARRAY, .p_key = strdup("a"), .list = p_array };

    // populate the dictionary
    dict_add(p_dict, p_3);

    // populate the value
    *p_value = (json_value) { .type = JSON_VALUE_OBJECT, .object = p_dict };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_array_string_empty ( void **pp_result )
{

    // initialized data
    json_value **pp_value = (json_value **) pp_result;
    json_value  *p_value  = default_allocator(NULL, sizeof(json_value));
    json_value  *p_1      = default_allocator(NULL, sizeof(json_value));
    array       *p_array  = NULL;
    char        *p_str    = default_allocator(NULL, 1);

    // construct an array
    array_construct(&p_array, 1);

    // store a null terminator
    p_str[0] = '\0';

    // populate the value
    *p_1 = (json_value) { .type = JSON_VALUE_STRING, .string = p_str };

    // populate the array
    array_add(p_array, p_1);

    // populate the value
    *p_value = (json_value) { .type = JSON_VALUE_ARRAY, .list = p_array };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_array_string ( void **pp_result )
{

    // initialized data
    json_value **pp_value = (json_value **) pp_result;
    json_value  *p_value  = default_allocator(NULL, sizeof(json_value));
    json_value  *p_1      = default_allocator(NULL, sizeof(json_value));
    array       *p_array  = NULL;
    char        *p_str    = default_allocator(NULL, 4);

    // construct an array
    array_construct(&p_array, 1);

    // populate the string
    strncpy(p_str, "abc", 4);

    // populate the value
    *p_1 = (json_value) { .type = JSON_VALUE_STRING, .string = p_str };

    // populate the array
    array_add(p_array, p_1);

    // populate the value
    *p_value = (json_value) { .type = JSON_VALUE_ARRAY, .list = p_array };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_array_strings ( void **pp_result )
{

    // initialized data
    json_value **pp_value = (json_value **) pp_result;
    json_value  *p_value  = default_allocator(NULL, sizeof(json_value));
    array       *p_array  = NULL;
    char        *strs[]   = {"abc", "def", "ghi"};

    // construct an array
    array_construct(&p_array, 1);

    // populate the value
    for (int i = 0; i < 3; i++) 
    {

        // initialized data
        json_value *p_item = default_allocator(NULL, sizeof(json_value));
        char       *p_str  = default_allocator(NULL, 4);

        // populate the string
        strncpy(p_str, strs[i], 4);

        // populate the value
        *p_item = (json_value) { .type = JSON_VALUE_STRING, .string = p_str };

        // populate the array
        array_add(p_array, p_item);
    }

    // populate the value
    *p_value = (json_value) { .type = JSON_VALUE_ARRAY, .list = p_array };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_array_object_empty ( void **pp_result )
{

    // initialized data
    json_value **pp_value = (json_value **) pp_result;
    json_value  *p_value  = default_allocator(NULL, sizeof(json_value));
    json_value  *p_1      = default_allocator(NULL, sizeof(json_value));
    array       *p_array  = NULL;
    dict        *p_dict   = NULL;

    // construct an array
    array_construct(&p_array, 1);

    // construct a dictionary
    dict_construct(&p_dict, 16, NULL, object_key_accessor, NULL);

    // populate the value
    *p_1 = (json_value) { .type = JSON_VALUE_OBJECT, .object = p_dict };

    // populate the array
    array_add(p_array, p_1);

    // populate the value
    *p_value = (json_value) { .type = JSON_VALUE_ARRAY, .list = p_array };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_array_object ( void **pp_result )
{

    // initialized data
    json_value **pp_value = (json_value **) pp_result;
    json_value  *p_value  = default_allocator(NULL, sizeof(json_value));
    json_value  *p_1      = default_allocator(NULL, sizeof(json_value));
    json_value  *p_2      = default_allocator(NULL, sizeof(json_value));
    array       *p_array  = NULL;
    dict        *p_dict   = NULL;
    
    // construct an array
    array_construct(&p_array, 1);

    // construct a dictionary
    dict_construct(&p_dict, 16, NULL, object_key_accessor, NULL);

    // populate the value
    *p_1 = (json_value) { .type = JSON_VALUE_INTEGER, .p_key = strdup("a"), .integer = 1 };

    // populate the dictionary
    dict_add(p_dict, p_1);

    // populate the value
    *p_2 = (json_value) { .type = JSON_VALUE_OBJECT, .object = p_dict };

    // populate the array
    array_add(p_array, p_2);

    // populate the value
    *p_value = (json_value) { .type = JSON_VALUE_ARRAY, .list = p_array };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_array_objects ( void **pp_result )
{

    // initialized data
    json_value **pp_value = (json_value **) pp_result;
    json_value  *p_value  = default_allocator(NULL, sizeof(json_value));
    array       *p_array  = NULL;
    char        *keys[]   = {"a", "b", "c"};

    // construct an array
    array_construct(&p_array, 1);

    // populate the value
    for (int i = 0; i < 3; i++) 
    {

        // initialized data
        json_value *p_1    = default_allocator(NULL, sizeof(json_value));
        json_value *p_2    = default_allocator(NULL, sizeof(json_value));
        dict       *p_dict = NULL;

        // construct a dictionary
        dict_construct(&p_dict, 16, NULL, object_key_accessor, NULL);
        
        // populate the value
        *p_2 = (json_value) { .type = JSON_VALUE_INTEGER, .p_key = strdup(keys[i]), .integer = i + 1 };
        
        // populate the dictionary
        dict_add(p_dict, p_2);
        
        // populate the value
        *p_1 = (json_value) { .type = JSON_VALUE_OBJECT, .object = p_dict };

        // populate the array
        array_add(p_array, p_1);
    }

    // populate the value
    *p_value = (json_value) { .type = JSON_VALUE_ARRAY, .list = p_array };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_array_array_empty ( void **pp_result )
{

    // initialized data
    json_value **pp_value = (json_value **) pp_result;
    json_value  *p_value  = default_allocator(NULL, sizeof(json_value));
    json_value  *p_item   = default_allocator(NULL, sizeof(json_value));
    array       *p_array  = NULL;
    array       *p_inner  = NULL;

    // construct arrays
    array_construct(&p_array, 1),
    array_construct(&p_inner, 1);

    // populate the value
    *p_item = (json_value) { .type = JSON_VALUE_ARRAY, .list = p_inner };

    // populate the array
    array_add(p_array, p_item);

    // populate the value
    *p_value = (json_value) { .type = JSON_VALUE_ARRAY, .list = p_array };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_array_array_array_empty ( void **pp_result )
{

    // initialized data
    json_value **pp_value = (json_value **) pp_result;
    json_value  *p_value  = default_allocator(NULL, sizeof(json_value));
    json_value  *p_1      = default_allocator(NULL, sizeof(json_value));
    json_value  *p_2      = default_allocator(NULL, sizeof(json_value));
    array       *p_array  = NULL;
    array       *p_inner1 = NULL;
    array       *p_inner2 = NULL;

    // construct arrays
    array_construct(&p_array, 1),
    array_construct(&p_inner1, 1),
    array_construct(&p_inner2, 1);

    // popualte the value
    *p_2 = (json_value) { .type = JSON_VALUE_ARRAY, .list = p_inner2 };

    // populate the array
    array_add(p_inner1, p_2);

    // popualte the value
    *p_1 = (json_value) { .type = JSON_VALUE_ARRAY, .list = p_inner1 };

    // populate the array
    array_add(p_array, p_1);

    // popualte the value
    *p_value = (json_value) { .type = JSON_VALUE_ARRAY, .list = p_array };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_array_matrix ( void **pp_result )
{

    // initialized data
    json_value **pp_value = (json_value **) pp_result;
    json_value  *p_value  = default_allocator(NULL, sizeof(json_value));
    array       *p_array  = NULL;
    int          val      = 1;

    // construct an array
    array_construct(&p_array, 1);

    // populate the matrix
    for (int i = 0; i < 3; i++) 
    {

        // initialized data
        json_value *p_row   = default_allocator(NULL, sizeof(json_value));
        array      *p_inner = NULL;

        // construct an array
        array_construct(&p_inner, 1);

        // populate the columns
        for (int j = 0; j < 3; j++) 
        {

            // initialized data
            json_value *p_item = default_allocator(NULL, sizeof(json_value));

            // populate the value
            *p_item = (json_value) { .type = JSON_VALUE_INTEGER, .integer = val++ };

            // populate the array
            array_add(p_inner, p_item);
        }

        // populate the value
        *p_row = (json_value) { .type = JSON_VALUE_ARRAY, .list = p_inner };
        
        // populate the array
        array_add(p_array, p_row);
    }

    // populate the value
    *p_value = (json_value) { .type = JSON_VALUE_ARRAY, .list = p_array };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

int construct_array_tensor ( void **pp_result )
{

    // initialized data
    json_value **pp_value = (json_value **) pp_result;
    json_value  *p_value  = default_allocator(NULL, sizeof(json_value));
    array       *p_array  = NULL;
    int          val      = 1;

    // construct an array
    array_construct(&p_array, 1);

    // populate the tensor
    for (int i = 0; i < 2; i++) 
    {

        // initialized data
        json_value *p_matrix = default_allocator(NULL, sizeof(json_value));
        array      *p_1      = NULL;

        // construct an array
        array_construct(&p_1, 1);

        // populate the row
        for (int j = 0; j < 2; j++) 
        {

            // initialized data
            json_value *p_row = default_allocator(NULL, sizeof(json_value));
            array      *p_2   = NULL;
            
            // construct an array
            array_construct(&p_2, 1);

            // populate the columns
            for (int k = 0; k < 2; k++) 
            {
                
                // initialized data
                json_value *p_column = default_allocator(NULL, sizeof(json_value));

                // populate the value
                *p_column = (json_value) { .type = JSON_VALUE_INTEGER, .integer = val++ };

                // populate the array
                array_add(p_2, p_column);
            }
            
            // populate the value
            *p_row = (json_value) { .type = JSON_VALUE_ARRAY, .list = p_2 };
            
            // populate the array
            array_add(p_1, p_row);
        }
        
        // populate the value
        *p_matrix = (json_value) { .type = JSON_VALUE_ARRAY, .list = p_1 };
       
        // populate the array
        array_add(p_array, p_matrix);
    }

    // populate the value
    *p_value = (json_value) { .type = JSON_VALUE_ARRAY, .list = p_array };

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;
}

void *test_parse ( test_case *p_test_case, void *p_subject )
{

    // unused
    (void) p_subject;

    // initialized data
    json_value        *p_result   = NULL;
    stream            *p_stream   = NULL;
    const char *const  p_path     = (const char *const) p_test_case->p_data;

    // construct a stream
    stream_from_path(&p_stream, p_path);

    // parse json
    json_parse(&p_result, p_stream, NULL);

    // done
    return (void *) p_result;
}

void *test_serialize ( test_case *p_test_case, void *p_subject )
{

    // unused
    (void) p_test_case;

    // initialized data
    stream *p_result = NULL;

    // construct another stream
    stream_from_dynamic_buffer(&p_result);

    // serialize json
    json_serialize(p_result, p_subject);

    // done
    return (void *) p_result;
}

bool json_results_match ( test_scenario *p_scenario, test_case *p_case, void *p_subject, void *p_result )
{

    // unused
    (void) p_scenario;
    (void) p_case;

    // done
    return value_equals((json_value *)p_subject, (json_value *)p_result);
}

bool textual_results_match ( test_scenario *p_scenario, test_case *p_case, void *p_subject, void *p_result )
{

    // unused
    (void) p_scenario;
    (void) p_subject;

    // initialized data
    stream *p_a = NULL;
    stream *p_b = p_result;

    // construct a stream
    stream_from_path(&p_a, (const char *)p_case->p_data);

    // seek start
    stream_seek(p_b, 0, SEEK_SET);

    // equal?
    while( !stream_eof(p_a) )
    {

        // initialized data
        char a = '\0';
        char b = '\0';

        // read
        stream_read(p_a, &a, 1),
        stream_read(p_b, &b, 1);

        // equal?
        if ( a != b ) return false;
    }

    // done
    return true; 
}

bool value_equals ( json_value *a, json_value *b )
{

    // null?
    if ( NULL == a ) return ( NULL == b );
    if ( NULL == b ) return ( NULL == a );    

    // type?
    if ( a->type != b->type ) return false;

    // strategy
    switch (a->type)
    {

        // boolean equality
        case JSON_VALUE_BOOLEAN: return ( a->boolean == b->boolean );

        // integer equality
        case JSON_VALUE_INTEGER: return ( a->integer == b->integer );

        // float equality
        // NOTE: The least significant bit of the mantissa is cleared to avoid
        //       rounding errors
        case JSON_VALUE_NUMBER: 
            return 
            (
                (*((unsigned long long *)(&a->number)) & (unsigned long long)0xfffffffffffffffe) ==
                (*((unsigned long long *)(&b->number)) & (unsigned long long)0xfffffffffffffffe) 
            );

        // string equality
        case JSON_VALUE_STRING: return ( 0 == strcmp(a->string, b->string) );

        // object equality
        case JSON_VALUE_OBJECT:
        {

            // initialized data
            dict     *p_a          = a->object;
            dict     *p_b          = b->object;
            size_t    a_properties = 0;
            size_t    b_properties = 0;
            iterator  it_a         = { 0 };
            iterator  it_b         = { 0 };
            
            // store the size of a
            dict_size(p_a, &a_properties);

            // store the size of b
            dict_size(p_b, &b_properties);

            // size?
            if ( a_properties != b_properties ) return false;

            // construct an iterator on a
            it_a = dict_iterator(p_a);

            // iterate through each property of a
            for (size_t i = 0; i < a_properties; i++)
            {

                // initialized data
                bool        found = false;
                json_value *p_i   = it_a.item(&it_a);
                
                // construct an iterator on b
                it_b = dict_iterator(p_b);

                // iterate through each property of b
                for (size_t j = 0; j < b_properties; j++)
                {

                    // initialized data
                    json_value *p_j = it_b.item(&it_b);

                    // key?
                    if ( 0 == strcmp(p_i->p_key, p_j->p_key) ) 
                    {

                        // value?
                        found = value_equals(p_i, p_j);

                        // done
                        break;
                    }
                }

                // found?
                if ( false == found ) return false;
            }

            // done
            return true;
        }

        case JSON_VALUE_ARRAY:
        {

            // initialized data
            array *p_a = a->list;
            array *p_b = b->list;
            size_t    a_length = array_size(p_a);
            size_t    b_length = array_size(p_b);
            
            // size?
            if ( a_length != b_length ) return false;

            // iterate through each element of each arary
            for 
            ( 
                iterator it_a = array_iterator(p_a), it_b = array_iterator(p_b); 
                !it_a.done(&it_a) && !it_b.done(&it_b);
                it_a.next(&it_a), it_b.next(&it_b)
            )

                // equal?
                if ( false == value_equals (
                        (json_value *)it_a.item(&it_a),
                        (json_value *)it_b.item(&it_b)
                ) ) 
                    return false;

            // done
            return true;
        }
    }

    // error
    return false;
}
