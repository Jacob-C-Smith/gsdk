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

// enumeration definitions
enum json_value_type_e
{
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
};

// type definitions
typedef struct json_value_s json_value;

// function declarations
/// parse
/** !
 * Parse a json value from a stream
 * 
 * @param pp_value result
 * @param p_stream the stream
 * @param p_next   result IF NOT NULL ELSE unused
 * 
 * @return 1 on success, 0 on error
 */
int json_parse ( json_value **const pp_value, stream *p_stream, char *p_next );

/// serialize
/** ! 
 * Serialize a json value to a stream
 * 
 * @param p_stream the stream
 * @param p_value result
 * 
 * @return 1 on success, 0 on error
*/
int json_serialize ( stream *p_stream, const json_value *const p_value );

/// destructors
fn_allocator json_allocator;