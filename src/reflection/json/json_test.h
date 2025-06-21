/** !
 * Tests JSON reflection
 * 
 * @file JSON tester
 * 
 * @author Jacob Smith
*/

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <errno.h>
#include <float.h>

// data 
#include <data/dict.h>
#include <data/array.h>

// reflection
#include <reflection/json.h>

// Utility
/** ! 
 * Recursively compare a to b. 
 * 
 * @param a a value
 * @param b b value
 * 
 * @return true if a is exactly equal to b else false
 */
bool value_equals ( json_value *a, json_value *b );

// Scenario constructors
/** ! 
 * Returns null as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error 
 */
int construct_null ( json_value **pp_value );

/** ! 
 * Returns false as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error 
 */
int construct_bool_false ( json_value **pp_value );

/** ! 
 * Returns true as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error 
 */
int construct_bool_true ( json_value **pp_value );

/** ! 
 * Returns -1 as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error 
 */
int construct_int_minus_one ( json_value **pp_value ); 

/** ! 
 * Returns 0 as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error 
 */
int construct_int_zero ( json_value **pp_value ); 

/** ! 
 * Returns 1 as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error 
 */
int construct_int_one ( json_value **pp_value ); 

/** ! 
 * Returns 9223372036854775807 as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error 
 */
int construct_int_max ( json_value **pp_value ); 

/** ! 
 * Returns -9223372036854775807 - 1 as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error 
 */
int construct_int_min ( json_value **pp_value ); 

/** ! 
 * Returns -1.0 as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error 
 */
int construct_float_minus_one( json_value **pp_value ); 

/** ! 
 * Returns 0.0 as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error 
 */
int construct_float_zero ( json_value **pp_value ); 

/** ! 
 * Returns 1.0 as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error 
 *
 */
int construct_float_one ( json_value **pp_value ); 

/** ! 
 * Returns DBL_MAX as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error 
 *
 */
int construct_float_max ( json_value **pp_value ); 

/** ! 
 * Returns -DBL_MAX as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error 
 *
 */
int construct_float_min ( json_value **pp_value ); 

/** ! 
 * Returns "" as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error 
 *
 */
int construct_string_empty ( json_value **pp_value );

/** ! 
 * Returns "a" as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error 
 *
 */
int construct_string_a ( json_value **pp_value );

/** ! 
 * Returns "abc" as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error 
 *
 */
int construct_string_abc ( json_value **pp_value );

/** ! 
 * Returns "\"abc\"" as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error 
 *
 */
int construct_string_quote_abc_quote ( json_value **pp_value );

/** ! 
 * Returns "\"" as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error 
 *
 */
int construct_string_quote ( json_value **pp_value );

/** ! 
 * Returns "\"\"" as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error 
 *
 */
int construct_string_quote_quote ( json_value **pp_value );

/** ! 
 * Returns "\"abc\"" as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error 
 *
 */
int construct_string_whitespaces_abc ( json_value **pp_value );

/** ! 
 * Returns "\\" as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error 
 *
 */
int construct_string_reverse_solidus ( json_value **pp_value );

/** ! 
 * Returns "/" as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error 
 *
 */
int construct_string_solidus ( json_value **pp_value );

/** ! 
 * Returns "\\b" as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error 
 *
 */
int construct_string_backspace ( json_value **pp_value );

/** ! 
 * Returns "\\f" as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error 
 *
 */
int construct_string_formfeed ( json_value **pp_value );

/** ! 
 * Returns "\\n" as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error 
 *
 */
int construct_string_linefeed ( json_value **pp_value );

/** ! 
 * Returns "\\r" as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error 
 *
 */
int construct_string_carriage_return ( json_value **pp_value );

/** ! 
 * Returns "\\t" as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error 
 *
 */
int construct_string_horizontal_tab ( json_value **pp_value );

/** ! 
 * Returns { } as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error 
 *
 */
int construct_object_empty ( json_value **pp_value );

/** ! 
 * Returns { "abc" : "def" } as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error 
 *
 */
int construct_object_string ( json_value **pp_value );

/** ! 
 * Returns { "abc" : 123 } as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error 
 *
 */
int construct_object_int ( json_value **pp_value );

/** ! 
 * Returns { "pi" : 3.14 } as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error 
 *
 */
int construct_object_float ( json_value **pp_value );

/** ! 
 * Returns { "abc" : false } as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error 
 *
 */
int construct_object_false ( json_value **pp_value );

/** ! 
 * Returns { "abc" : true } as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error 
 *
 */
int construct_object_true ( json_value **pp_value );

/** ! 
 * Returns { "abc" : "def", "ghi" : "jkl", "mno" : "pqr" } as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error 
 *
 */
int construct_object_strings ( json_value **pp_value );

/** ! 
 * Returns { "name" : "jake", "age" : 20, "height" : 1.779 } as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error 
 *
 */
int construct_object_mixed_values ( json_value **pp_value );

/** ! 
 * Returns { "abc" : { "def" : 123 } } as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error 
 *
 */
int construct_object_object ( json_value **pp_value );

/** ! 
 * Returns { "abc" : { "def" : { "ghi" : 123 } } } as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error 
 *
 */
int construct_object_object_object ( json_value **pp_value );

/** !
 * Returns { "a" : { "b" : { "c" : {...{ "z" : { } ... } as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error
 */
int construct_object_recursive ( json_value **pp_value );

/** !
 * Returns { "abc" : [ 1, 2, 3 ] } as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error
 */
int construct_object_array ( json_value **pp_value );

/** !
 * Returns { "a" : [ { "a" : 1 }, { "b" : 2 }, { "c" : 3 } ] } as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error
 */
int construct_object_array_objects ( json_value **pp_value );

/** !
 * Returns { "a" : [ { "a" : 1 } ] } as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error
 */
int construct_object_array_object ( json_value **pp_value );

/** !
 * Returns [] as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error
 */
int construct_array_empty ( json_value **pp_value );

/** !
 * Returns [ null ] as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error
 */
int construct_array_null ( json_value **pp_value );

/** !
 * Returns [ null, null, null ] as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error
 */
int construct_array_nulls ( json_value **pp_value );

/** !
 * Returns [ true ] as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error
 */
int construct_array_bool ( json_value **pp_value );

/** !
 * Returns [ true, false, true ] as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error
 */
int construct_array_bools ( json_value **pp_value );

/** !
 * Returns [ 1 ] as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error
 */
int construct_array_int ( json_value **pp_value );

/** !
 * Returns [ 1, 2, 3 ] as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error
 */
int construct_array_ints ( json_value **pp_value );

/** !
 * Returns [ 3.14 ] as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error
 */
int construct_array_float ( json_value **pp_value );

/** !
 * Returns [ 1.2, 3.4, 5.6 ] as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error
 */
int construct_array_floats ( json_value **pp_value );

/** !
 * Returns [ "" ] as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error
 */
int construct_array_string_empty ( json_value **pp_value );

/** !
 * Returns [ "abc" ] as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error
 */
int construct_array_string ( json_value **pp_value );

/** !
 * Returns [ "abc", "def", "ghi" ] as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error
 */
int construct_array_strings ( json_value **pp_value );

/** !
 * Returns [ { } ] as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error
 */
int construct_array_object_empty ( json_value **pp_value );

/** !
 * Returns [ { "a" : 1 } ] as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error
 */
int construct_array_object ( json_value **pp_value );

/** !
 * Returns [ [ { "a" : 1 }, { "b" : 2 }, { "c" : 3 } ] ]  as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error
 */
int construct_array_objects ( json_value **pp_value );

/** !
 * Returns [ [ ] ] as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error
 */
int construct_array_array_empty ( json_value **pp_value );

/** !
 * Returns [ [ [ ] ] ] as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error
 */
int construct_array_array_array_empty ( json_value **pp_value );

/** !
 * Returns [ [ 1, 2, 3 ], [ 4, 5, 6 ], [ 7, 8, 9 ] ] as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error
 */
int construct_array_matrix ( json_value **pp_value );

/** !
 * Returns [ [ [ 1, 2 ], [ 3, 4 ] ], [ [ 5, 6 ], [ 7, 8 ] ] ]  as a json_value
 * 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error
 */
int construct_array_tensor ( json_value **pp_value );
