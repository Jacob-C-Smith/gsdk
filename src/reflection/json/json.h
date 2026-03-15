/** !
 * json interface
 * 
 * @file src/reflection/json/json.h 
 * 
 * @author Jacob Smith
 */

// header guard
#pragma once

// standard library
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <float.h>
#include <errno.h>
#include <ctype.h>
#include <math.h>

// gsdk
/// core
#include <core/log.h>
#include <core/sync.h>
#include <core/hash.h>
#include <core/pack.h>
#include <core/interfaces.h>

/// data
#include <data/array.h>
#include <data/dict.h>

/// reflection
#include <reflection/json.h>

// preprocessor macros
#define FREE_VALUE( value ) json_value_free(value)

// function declarations
void free_token ( void *ptr );

// enumeration definitions
enum json_value_type_e
{
    JSON_VALUE_INVALID,
    JSON_VALUE_OBJECT,
    JSON_VALUE_ARRAY,
    JSON_VALUE_STRING,
    JSON_VALUE_BOOLEAN,
    JSON_VALUE_INTEGER,
    JSON_VALUE_NUMBER
};

// structure definitions
struct json_value_s
{
    enum json_value_type_e type;
    char *p_key;
    union
    {
        char             *string;
        signed long long  integer;
        double            number;
        dict             *object;
        array            *list;
        bool              boolean;
    };
    size_t len;
    char _text[];
};

// type definitions
typedef struct json_value_s json_value;

// function declarations
/// parse
/** !
 * Parse json text into a json_value
 * 
 * @param text pointer to json text
 * @param return_pointer null or pointer to end of json value
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error
 */
int json_value_parse ( char *text, char **return_pointer, json_value **const pp_value );

/// serialize
/** ! 
 * Serialize a json_value to a buffer
 * 
 * @param p_value pointer to json_value
 * @param _buffer pointer to text buffer
 * 
 * @return 1 on success, 0 on error
*/
int json_value_serialize ( const json_value *const p_value, char *_buffer );

/** !
 * Serialize a json value to standard out
 * 
 * @param p_value the json value
 * 
 * @return 1 on success, 0 on error
 */
int json_value_print ( const json_value *const p_value );

/** !
 * Serialize a json value to a file
 * 
 * @param p_value the json value
 * @param p_f     the file
 * 
 * @return 1 on success, 0 on error
 */
int json_value_fprint ( const json_value *const p_value, FILE *p_f );

/// destructors
/** ! 
 * Free a json value, and its contents
 * 
 * @param p_value pointer to json_value
 *  
 * @return void
 */
void *json_value_free ( json_value *p_value, unsigned long long unused );

// cleanup
/** !
 * This gets called at runtime after main
 * 
 * @param void
 * 
 * @return void
 */
void json_exit ( void ) __attribute__((destructor));
