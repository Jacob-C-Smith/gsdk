/** !
 * json implementation
 * 
 * @file src/reflection/json/json.c
 * 
 * @author Jacob Smith
 */

// header 
#include <reflection/json.h>

// function declarations
fn_key_accessor object_key_accessor;

// function definitions
int double_precision ( double value )
{

    // initialized data
    int    ret           = 18;
    double rounded_value = 0.0;

    // Test precisions downto 1
    for (; ret >= 1; ret--)
    {
        
        // Round
        rounded_value = round(value * pow(10, ret)) / pow(10, ret);

        // Compare
        if ( rounded_value != value ) break;
    }

    // done
    return ret + 1;
};

int json_whitespace_parse ( stream *p_stream, char *p_next )
{

    // argument check
    if ( NULL == p_stream ) goto no_stream;

    // initialized data
    char c = '\0';

    // read a character
    if ( 0 == stream_read(p_stream, &c, 1) ) return 0;

    // eat spaces, line feed, carriage return, horizontal tab
    while 
    (
        ' '  == c ||
        '\n' == c ||
        '\r' == c ||
        '\t' == c         
    )
    { 

        // end of json
        if ( '\0' == c ) 
            break; 
        
        // read the next character
        stream_read(p_stream, &c, 1);
    };

    // return the next character to the caller
    if ( p_next ) *p_next = c;
    
    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_stream:
                #ifndef NDEBUG
                    log_error("[json] Null pointer provided for parameter \"p_stream\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int json_string_parse ( stream *p_stream, char **pp_result, char *p_next )
{

    // argument check
    if ( NULL ==  p_stream ) goto no_stream;
    if ( NULL == pp_result ) goto no_result;

    // initialized data
    char   *p_string = NULL;
    size_t  len      = 16,
            read     = 0;
    char    c        = '\0';

    // allocate memory for the string
    p_string = default_allocator(NULL, len);
    if ( NULL == p_string ) goto no_mem;

    // read a character
    stream_read(p_stream, &c, 1);

    // walk the string
    while ( '\"' != c )
    {

        // grow the buffer
        if ( read >= len - 1 )
        {

            // double the size
            len *= 2;

            // resize
            p_string = default_allocator(p_string, len);

            // error check
            if ( NULL == p_string ) goto no_mem;
        }

        // escape sequence
        if ( '\\' == c )
        {

            // read a character
            stream_read(p_stream, &c, 1);

            // strategy
            switch ( c )
            {

                // double quote
                case '\"':
                    c = '\"';
                    break;

                // back slash
                case '\\':
                    c = '\\';
                    break;

                // forward slash
                case '/':
                    c = '/';
                    break;

                // backspace
                case 'b':
                    c = '\b';
                    break;

                // form feed
                case 'f':
                    c = '\f';
                    break;
                
                // line feed
                case 'n':
                    c = '\n';
                    break;

                // carriage return
                case 'r':
                    c = '\r';
                    break;

                // horizontal tab
                case 't':
                    c = '\t';
                    break;
            }
        }
        
        // store the character
        p_string[read++] = c;

        // read a character
        stream_read(p_stream, &c, 1);
    }

    // eat a character
    stream_read(p_stream, &c, 1);

    // resize
    p_string = default_allocator(p_string, read + 1);
    if ( NULL == p_string ) goto no_mem;

    // null terminate
    p_string[read] = '\0';

    // return a pointer to the caller
    *pp_result = p_string;

    // return the next character to the caller
    if ( p_next ) 
        *p_next = c;

    // success
    return 1;
    
    // error handling
    {

        // argument errors
        {
            no_stream:
                #ifndef NDEBUG
                    log_error("[json] Null pointer provided for parameter \"p_stream\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_result:
                #ifndef NDEBUG
                    log_error("[json] Null pointer provided for parameter \"pp_result\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
					printf("[standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// error
				return 0;
        }
    }
}
 
int json_object_parse ( stream *p_stream, dict **const pp_dict, char *p_next )
{

    // initialized data
    dict *p_dict = NULL;
    char  c      = '\0';

    // construct a dictionary
    if ( 0 == dict_construct(&p_dict, 16, NULL, object_key_accessor, NULL) ) goto failed_to_construct_dict;
    
    parse_property:
    
    // walk the object
    if ( c != '}' )
    {

        // initialized data
        json_value *p_value = NULL;
        char       *p_key = NULL;

        // eat whitespace
        json_whitespace_parse(p_stream, &c);

        // edge case
        if ( '}' == c ) goto end_of_object;

        // error check
        if ( '\"' != c ) return 0;

        // parse the key
        json_string_parse(p_stream, &p_key, &c);

        // eat whitespace
        while ( ' ' == c || '\n' == c || '\r' == c || '\t' == c )
            if ( 0 == stream_read(p_stream, &c, 1) ) break;

        // error check
        if ( ':' != c ) return 0;
        
        // parse the value
        if ( json_parse(&p_value, p_stream, &c) )
        {

            // store the key
            if ( p_value ) 
                p_value->p_key = p_key;
        
            // add the value to the object
            dict_add(p_dict, p_value);
        }

        // eat whitespace
        while ( ' ' == c || '\n' == c || '\r' == c || '\t' == c )
            if ( 0 == stream_read(p_stream, &c, 1) ) break;

        // check for another element
        if ( c == ',' ) goto parse_property;
    }

    end_of_object:

    // error checking
    if ( c != '}' ) return 0;

    // read
    stream_read(p_stream, &c, 1);

    // return a pointer to the caller
    *pp_dict = p_dict;

    // return the next character to the caller
    if ( p_next ) *p_next = c;
    
    // success
    return 1;
    
    // error check
    {
        
        // dict errors
        {
            failed_to_construct_dict:
                #ifndef NDEBUG
                    log_error("[dict] Dictionary constructor returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int json_array_parse ( stream *p_stream, array **const pp_array, char *p_next )
{

    // initialized data
    array *p_array = 0;
    char   c       = '\0';

    // construct an array
    if ( 0 == array_construct(&p_array, 1) ) goto failed_to_construct_array;

    parse_element:
    
    // walk the array
    if ( c != ']' )
    {

        // initialized data
        json_value *p_value = NULL;

        // parse the value
        if ( json_parse(&p_value, p_stream, &c) )
        
            // add the value to the array
            array_add(p_array, p_value);

        // eat whitespace
        while ( ' ' == c || '\n' == c || '\r' == c || '\t' == c )
            if ( 0 == stream_read(p_stream, &c, 1) ) break;

        // check for another element
        if ( c == ',' ) goto parse_element;
    }

    // error checking
    if ( c != ']' ) return 0;

    // read
    stream_read(p_stream, &c, 1);

    // return a pointer to the caller
    *pp_array = p_array;

    // return the next character to the caller
    if ( p_next ) *p_next = c;

    // success
    return 1;

    // error check
    {
        
        // array errors
        {
            failed_to_construct_array:
                #ifndef NDEBUG
                    log_error("[json] Array constructor returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int json_parse ( json_value **const pp_value, stream *p_stream, char *p_next )
{

    // argument check
    if ( NULL == pp_value ) goto no_value;
    if ( NULL == p_stream ) goto no_stream;

    // initialized data
    json_value *p_value = (void *) 0;
    char        c       = '\0';

    // eat whitespace
    if ( 0 == json_whitespace_parse(p_stream, &c) ) return 0;

    // parse a value
    switch ( c )
    {

        // parse a string
        case '\"':
        {

            // initialized data
            char *p_string = NULL;
            
            // parse the string
            if ( 0 == json_string_parse(p_stream, &p_string, &c) ) goto failed_to_parse_json_string;

            // allocate memory for the value
            p_value = default_allocator(NULL, sizeof(json_value));
            if ( NULL == p_value ) goto no_mem;

            // populate the value
            *p_value = (json_value)
            {
                .type   = JSON_VALUE_STRING,
                .string = p_string,
            };

            // done
            goto done;
        }
        
        // parse an object
        case '{':
        {

            // initialized data
            dict *p_dict = NULL;

            // parse an array
            if ( 0 == json_object_parse(p_stream, &p_dict, &c) ) goto failed_to_parse_json_object;

            // allocate memory for the value
            p_value = default_allocator(NULL, sizeof(json_value));
            if ( NULL == p_value ) goto no_mem;

            // populate the value
            *p_value = (json_value)
            {
                .type = JSON_VALUE_OBJECT,
                .object = p_dict
            };

            // done
            goto done;
        }
        
        // parse an array
        case '[':
        {

            // initialized data
            array *p_array = NULL;

            // parse an array
            if ( 0 == json_array_parse(p_stream, &p_array, &c) ) goto failed_to_parse_json_array;

            // allocate memory for the value
            p_value = default_allocator(NULL, sizeof(json_value));
            if ( NULL == p_value ) goto no_mem;

            // populate the value
            *p_value = (json_value)
            {
                .type = JSON_VALUE_ARRAY,
                .list = p_array
            };

            // done
            goto done;
        }

        // parse the 'true' keyword
        case 't':
        {

            // initialized data
            char _maybe_true[5] = { c };

            // read
            stream_read(p_stream, &_maybe_true[1], 3);

            // Check for the correct keyworkd
            if ( strncmp(_maybe_true, "true", 4) ) goto failed_to_parse_keyword;

            // allocate memory for the value
            p_value = default_allocator(NULL, sizeof(json_value));
            if ( NULL == p_value ) goto no_mem;

            // populate the value
            *p_value = (json_value)
            {
                .type = JSON_VALUE_BOOLEAN,
                .boolean = true
            };

            // read
            stream_read(p_stream, &c, 1);

            // return a pointer to the caller
            if ( p_next ) *p_next = c;

            // done
            goto done;
        }

        // parse the 'false' keyword
        case 'f':
        {

            // initialized data
            char _maybe_false[6] = { c };

            // read
            stream_read(p_stream, &_maybe_false[1], 4);

            // Check for the correct keyworkd
            if ( strncmp(_maybe_false, "false", 5) ) goto failed_to_parse_keyword;

            // allocate memory for the value
            p_value = default_allocator(NULL, sizeof(json_value));
            if ( NULL == p_value ) goto no_mem;

            // populate the value
            *p_value = (json_value)
            {
                .type = JSON_VALUE_BOOLEAN,
                .boolean = false
            };

            // read
            stream_read(p_stream, &c, 1);

            // return a pointer to the caller
            if ( p_next ) *p_next = c;

            // done
            goto done;
        }
        
        // parse the 'null' keyword
        case 'n':
        {

            // initialized data
            char _maybe_null[5] = { c };

            // read
            stream_read(p_stream, &_maybe_null[1], 3);

            // Check for the correct keyworkd
            if ( strncmp(_maybe_null, "null", 4) ) goto failed_to_parse_keyword;

            // allocate memory for the value
            p_value = default_allocator(NULL, sizeof(json_value));
            if ( NULL == p_value ) goto no_mem;

            // store a null value
            p_value = NULL;

            // read
            stream_read(p_stream, &c, 1);

            // return a pointer to the caller
            if ( p_next ) *p_next = c;

            // done
            goto done;
        }

        // default
        default: break;
    }

    // parse a number
    if ( '-' == c || ('0' <= c && '9' >= c) ) 
    {

        // // initialized data
        bool   f          = false;
        char   _text[310] = { 0 };
        size_t read       = 0;

        // sign?
        if ( '-' == c ) 

            // store the sign
            _text[read++] = c,

            // read
            stream_read(p_stream, &c, 1);

        // parse integer
        while ( c >= '0' && c <= '9' ) 
        { 

            // store the digit
            _text[read++] = c;

            // read
            if ( 0 == stream_read(p_stream, &c, 1) ) break;

            // error check
            if ( read >= sizeof(_text) - 1 ) goto integer_bounds_exceeded;
        };

        // float?
        if ( c == '.' )
        {

            // set the float bit
            f = true;

            // store the decimal point
            _text[read++] = c;

            // read
            stream_read(p_stream, &c, 1);
        }

        // parse a float
        if ( f )
        {

            // parse fraction
            while ( c >= '0' && c <= '9' ) 
            { 

                // store the digit
                _text[read++] = c;

                // read
                if ( 0 == stream_read(p_stream, &c, 1) ) break;

                // exponent?
                if ( 'E' == c || 'e' == c )
                {

                    // store the exponent
                    _text[read++] = c;

                    // read
                    if ( 0 == stream_read(p_stream, &c, 1) ) break;

                    // sign?
                    if ( '+' == c || '-' == c )
                    {
                        
                        // store the sign
                        _text[read++] = c;

                        // read
                        if ( 0 == stream_read(p_stream, &c, 1) ) break;
                    }
                }

                // error check
                if ( read >= sizeof(_text) - 1 ) 

                    // eat
                    while ( c >= '0' && c <= '9' ) 
                        if ( 0 == stream_read(p_stream, &c, 1) ) break;
            };

            // error check
            if ( '.' == c ) goto failed_to_parse_float;

            // allocate memory for the value
            p_value = default_allocator(NULL, sizeof(json_value));
            if ( NULL == p_value ) goto no_mem;

            // clear error
            errno = 0;

            // populate the value
            *p_value = (json_value)
            {
                .type   = JSON_VALUE_NUMBER,
                .number = strtod(_text, NULL),
            };

            // bounds check
            if ( errno == ERANGE )
            {
                
                // release the json value
                p_value = default_allocator(p_value, 0);

                // error handling
                goto float_bounds_exceeded;
            }
        }

        // parse an integer
        else
        {

            // allocate memory for the value
            p_value = default_allocator(NULL, sizeof(json_value));
            if ( NULL == p_value ) goto no_mem;

            // clear error
            errno = 0;

            // populate the value
            *p_value = (json_value)
            {
                .type = JSON_VALUE_INTEGER,
                .integer = strtoll(_text, NULL, 10)
            };

            // bounds check
            if ( errno == ERANGE )
            {
                
                // release the json value
                p_value = default_allocator(p_value, 0);

                // error handling
                goto integer_bounds_exceeded;
            }
        }
    }
    
    // default
    else 
    {

        // return a pointer to the caller
        if ( p_next ) *p_next = c;

        // error
        return 0;
    }

    done:
    
    // return the next character to the caller
    if ( p_next ) 
        *p_next = c;

    // return a pointer to the caller
    *pp_value = p_value;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_value:
                #ifndef NDEBUG
                    log_error("[json] Null pointer provided for parameter \"pp_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_stream:
                #ifndef NDEBUG
                    log_error("[json] Null pointer provided for parameter \"p_stream\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // json errors
        {
            failed_to_parse_json_string:
                #ifndef NDEBUG
                    log_error("[json] Failed to parse json string in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error 
                return 0;
                
            failed_to_parse_json_array:
                #ifndef NDEBUG
                    log_error("[json] Failed to parse json array in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error 
                return 0;
                
            failed_to_parse_json_object:
                #ifndef NDEBUG
                    log_error("[json] Failed to parse json object in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error 
                return 0;

            failed_to_parse_float:
                    #ifndef NDEBUG
                        log_error("[json] Failed to parse json number in call to function \"%s\"\n", __FUNCTION__);
                    #endif
    
                    // release the value
                    p_value = default_allocator(p_value, 0);
    
                    // error
                    return 0;
            
            failed_to_parse_keyword:
                #ifndef NDEBUG
                    log_error("[json] Failed to parse json keyword in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // release the value
                p_value = default_allocator(p_value, 0);

                // error
                return 0;

            integer_bounds_exceeded:
                #ifndef NDEBUG
                    log_error("[json] Integer must be between [%lld, ", -9223372036854775807LL);
                    log_error("%lld] in call to function \"%s\"\n",9223372036854775807LL, __FUNCTION__);
                #endif

                // error
                return 0;
            
            float_bounds_exceeded:
                #ifndef NDEBUG
                    log_error("[json] Float must be between [%.17lg, ", -DBL_MAX);
                    log_error("%.17lg] in call to function \"%s\"\n",DBL_MAX, __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
					printf("[standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// error
				return 0;
        }
    }
}

int json_serialize ( stream *p_stream, const json_value *const p_value )
{

    // initialized data
    int written = 0;

    // null?
    if ( NULL == p_value ) 

        // write null
        written += stream_write(p_stream, "null", 4);

    // strategy
    else switch ( p_value->type )
    {
            
        // write a boolean 
        case JSON_VALUE_BOOLEAN:
        {
            
            // write the boolean 
            written += stream_write(p_stream, p_value->boolean ? "true" : "false", p_value->boolean ? 4 : 5);

            // done
            break;
        }

        // write a string
        case JSON_VALUE_STRING:
        {

            // write the opening quote
            written += stream_write(p_stream, "\"", 1);
            
            // iterate over each character
            for (size_t i = 0; i < strlen(p_value->string); i++)
            {
                
                // strategy
                switch ( p_value->string[i] )
                {

                    // double quote
                    case '\"':
                        written += stream_write(p_stream, "\\\"", 2);
                        break;

                    // back slash
                    case '\\':
                        written += stream_write(p_stream, "\\\\", 2);
                        break;

                    // backspace
                    case '\b':
                        written += stream_write(p_stream, "\\b", 2);
                        break;
                    
                    // form feed
                    case '\f':
                        written += stream_write(p_stream, "\\f", 2);
                        break;
                    
                    // line feed
                    case '\n':
                        written += stream_write(p_stream, "\\n", 2);
                        break;
                    
                    // carriage return
                    case '\r':
                        written += stream_write(p_stream, "\\r", 2);
                        break;
                    
                    // horizontal tab
                    case '\t':
                        written += stream_write(p_stream, "\\t", 2);
                        break;
                    
                    // default
                    default:
                        written += stream_write(p_stream, &p_value->string[i], 1);
                        break;
                }
            }
            
            // write the closing quote
            written += stream_write(p_stream, "\"", 1);

            // done
            break;
        }

        // write an integer
        case JSON_VALUE_INTEGER:
        {

            // initialized data
            char _buffer[32] = { 0 };

            // write the integer to a buffer
            snprintf(_buffer, 32, "%lld", p_value->integer);

            // write
            written += stream_write(p_stream, _buffer, strlen(_buffer));

            // done
            break;
        }

        // write a number
        case JSON_VALUE_NUMBER:
        {
        
            // initialized data
            int precision = double_precision(p_value->number);
            char _buffer[32] = { 0 };

            // write the value to a buffer
            if ( precision > 18 )
                sprintf(_buffer,"%.*le", 16, p_value->number);
            else
                sprintf(_buffer,"%.*lf", precision, p_value->number);
            
            // write
            written += stream_write(p_stream, _buffer, strlen(_buffer));

            // done
            break;
        }

        // write an array
        case JSON_VALUE_ARRAY:
        {

            // initialized data
            size_t   len = array_size(p_value->list);
            iterator it  = { 0 };

            // edge case
            if ( 0 == len )
            {

                // write empty array
                written += stream_write(p_stream, "[]", 2);

                // done
                break;
            }

            // write the opening bracket
            written += stream_write(p_stream, "[", 1);

            // construct an iterator
            it = array_iterator(p_value->list);

            // iterate through the array
            for (size_t i = 0; i < len - 1; it.next(&it), i++)
            {

                // write the element
                written += json_serialize(p_stream, (json_value *) it.item(&it));

                // write a comma
                written += stream_write(p_stream, ",", 1);
            }

            // write the last element
            written += json_serialize(p_stream, (json_value *) it.item(&it));

            // write the closing bracket
            written += stream_write(p_stream, "]", 1);

            // done
            break;
        }

        // write an object
        case JSON_VALUE_OBJECT:
        {

            // initialized data
            size_t   len = 0;
            iterator it  = { 0 };

            // store the size of the dictionary
            dict_size(p_value->object, &len);

            // edge case
            if ( 0 == len )
            {
                
                // write empty object
                written += stream_write(p_stream, "{}", 2);

                // done
                break;
            }

            // write the opening brace
            written += stream_write(p_stream, "{", 1);

            // construct an iterator
            it = dict_iterator(p_value->object);

            // iterate through the array
            for (size_t i = 0; i < len - 1; it.next(&it), i++)
            {

                // initialized data
                json_value        *p_value = it.item(&it);
                const char *const  p_key   = object_key_accessor(p_value);

                // write the key
                written += stream_write(p_stream, "\"", 1);
                written += stream_write(p_stream, (void *)p_key, strlen(p_key));
                written += stream_write(p_stream, "\"", 1);

                // write the separator
                written += stream_write(p_stream, ":", 1);

                // write the element
                written += json_serialize(p_stream, p_value);

                // write a comma
                written += stream_write(p_stream, ",", 1);
            }

            // write the last element
            {

                // initialized data
                json_value        *p_value = it.item(&it);
                const char *const  p_key   = object_key_accessor(p_value);

                // write the key
                written += stream_write(p_stream, "\"", 1);
                written += stream_write(p_stream, (void *)p_key, strlen(p_key));
                written += stream_write(p_stream, "\"", 1);

                // write the separator
                written += stream_write(p_stream, ":", 1);

                // write the element
                written += json_serialize(p_stream, p_value);
            }

            // write the closing brace
            written += stream_write(p_stream, "}", 1);

            // done
            break;
        }
    }

    // success
    return written;
}

void *json_allocator ( void *p_pointer, unsigned long long unused )
{
    
    // argument check
    if ( NULL == p_pointer ) return NULL;
    
    // unused
    (void) unused;

    // initialized data
    json_value *p_value = (json_value *) p_pointer;
    
    // release the key
    if ( p_value->p_key ) 
        p_value->p_key = default_allocator(p_value->p_key, 0);

    // strategy
    switch ( p_value->type )
    {
        case JSON_VALUE_BOOLEAN:
        case JSON_VALUE_INTEGER:
        case JSON_VALUE_NUMBER:
        
            // done
            break;
            
        case JSON_VALUE_STRING:

            // release the string
            p_value->string = default_allocator(p_value->string, 0);
            
            // done
            break;

        case JSON_VALUE_OBJECT:

            // release the dictionary
            dict_destroy(&p_value->object, json_allocator);

            // done
            break;

        case JSON_VALUE_ARRAY:

            // release the array
            array_destroy(&p_value->list, json_allocator);

            // done
            break;
        
        default:

            // done
            break;
    }

    // done
    return default_allocator(p_value, 0);
}

void *object_key_accessor ( const void *const p_value )
{

    // initialized data
    json_value *p_json = (json_value *) p_value;

    // done
    return p_json->p_key;
}
