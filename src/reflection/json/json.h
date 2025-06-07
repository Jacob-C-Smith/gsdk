/** !
 * Include header for json library
 * 
 * @file json/json.h 
 * 
 * @author Jacob Smith
 */

// Include guard
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

// sync module
#include <core/sync.h>

// dict module
#include <data/dict.h>

// array module
#include <data/array.h>

// Forward declared functions
void free_token ( void *ptr );

// Platform dependent macros

// This macro is used to free json_value_t *'s 
#define FREE_VALUE( value ) json_value_free(value)

// Set the reallocator for the dict submodule
#ifdef DICT_REALLOC
    #undef DICT_REALLOC
    #define DICT_REALLOC(p, sz) realloc(p, sz)
#endif

// Set the reallocator for the array submodule
#ifdef ARRAY_REALLOC
    #undef ARRAY_REALLOC
    #define ARRAY_REALLOC(p, sz) realloc(p, sz)
#endif

// Memory management macro
#ifndef JSON_REALLOC
#define JSON_REALLOC(p, sz) realloc(p,sz)
#endif

// Enumerations
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

// Structures
struct json_value_s
{
    enum json_value_type_e type;
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

// Function declarations
// Initializer
/** !
 * This gets called at runtime before main. 
 * 
 * @param void
 * 
 * @return void
 */
void json_init ( void ) __attribute__((constructor));

// Parser
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

// Serializer
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

// Destructor
/** ! 
 * Free a json value, and its contents
 * 
 * @param p_value pointer to json_value
 *  
 * @return void
 */
void json_value_free ( json_value *p_value );

// Cleanup
/** !
 * This gets called at runtime after main
 * 
 * @param void
 * 
 * @return void
 */
void json_exit ( void ) __attribute__((destructor));
