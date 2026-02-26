/** !
 * JSON parser / serializer
 * 
 * @file json.c
 * 
 * @author Jacob Smith
 */

// header 
#include <reflection/json.h>

// Data
const size_t DICT_SIZE = 16;

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

int json_whitespace_parse ( char *pointer, char **return_pointer )
{

    // argument check
    if (  pointer == (void *) 0 ) goto no_pointer;
    if ( *pointer ==       '\0' ) goto done;

    // Skip past spaces, line feed, carriage return, horizontal tab
    while (
        *pointer == ' '  ||
        *pointer == '\n' ||
        *pointer == '\r' ||
        *pointer == '\t'         
    )
    { if (*pointer == '\0') break; pointer++; };

    // error checking
    if ( *pointer == '\0' ) return 0;
    
    done:

    // Update the pointer
    *return_pointer = pointer;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_pointer:
                #ifndef NDEBUG
                    log_error("[json] Null pointer provided for parameter \"pointer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

        }
    }
}

int json_string_parse ( char *const pointer, char **return_pointer )
{

    // argument check
    if ( pointer == (void *) 0 ) goto no_pointer;

    // initialized data
    size_t i = 0,
           j = 1;

    // error check
    if ( pointer[i] != '\"' ) return 0;
    
    // Check for an empty string
    if ( pointer[0] == '\0' ) goto exit;

    // Walk the string
    while ( pointer[j] )
    {

        // Continuation condition
        if ( pointer[j] == '\"' ) goto exit;
        
        // Escape sequence
        else if ( pointer[j] == '\\' )
        {

            // Escape sequence strategy
            switch ( pointer[j+1] )
            {

                // Single quote
                case '\"':

                    // Write the carriage return 
                    pointer[i] = '\"';

                    // done
                    break;

                // Backslash
                case '\\':

                    // Write the backslash 
                    pointer[i] = '\\';

                    // done
                    break;

                // Forward slash
                case '/':

                    // Write the forward slash 
                    pointer[i] = '/';

                    // done
                    break;

                // Backspace
                case 'b':

                    // Write the backspace 
                    pointer[i] = '\b';

                    // done
                    break;

                // Form feed
                case 'f':

                    // Write the form feed  
                    pointer[i] = '\f';

                    // done 
                    break;

                // Line feed
                case 'n':

                    // Write the line feed 
                    pointer[i] = '\n';

                    // done
                    break;
                
                // Carriage return
                case 'r':

                    // Write the carriage return 
                    pointer[i] = '\r';

                    break;

                // Horizontal tab
                case 't':

                    // Write the horizontal tab 
                    pointer[i]='\t';

                    // done
                    break;
                
                // TODO: Unicode
                case 'u':

                    // TODO:

                    // done
                    break;

                // Bad string
                case '\0':
                default:

                    // error handling
                    goto unexpected_escape_sequence;
            }
            
            // Increment the cursor
            j++;
        }

        // Copy the text
        else
            pointer[i] = pointer[j];
        
        // Increment the cursors
        i++, j++;
    }
    
    exit:

    // Insert a null terminator
    pointer[i] = '\0';

    // return the updated pointer
    *return_pointer = &pointer[j];

    // success
    return 1;
    
    // error handling
    {

        // argument errors
        {
            no_pointer:
                #ifndef NDEBUG
                    log_error("[json] Null pointer provided for parameter \"pointer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // json errors
        {
            unexpected_escape_sequence:
                #ifndef NDEBUG
                    log_error("[json] Unexpected escape sequence while parsing string in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}
 
int json_object_parse ( char *pointer, char **return_pointer, dict **const pp_dict )
{
    
    // initialized data
    dict *p_dict = (void *)0;

    // Construct a dict
    if ( dict_construct(&p_dict, DICT_SIZE, 0) == 0 ) goto failed_to_construct_dict;

    // error checking
    if ( *pointer != '{' ) return 0;

    // Increment the cursor
    pointer++;

    parse_property:

    // Parse whitespaces
    json_whitespace_parse(pointer, &pointer);

    // Parse the property key
    if ( *pointer == '\"' )
    {

        // initialized data
        char       *key   = pointer;
        json_value *value = 0;

        // Parse the property key
        json_string_parse(pointer, &pointer);

        // Increment the cursor
        pointer++;

        // Parse any whitespace
        json_whitespace_parse(pointer, &pointer);

        // Check for a valid property
        if ( *pointer == ':' ) pointer++;

        // Invalid property
        else goto expected_value;

        // Parse a JSON value
        if ( json_value_parse(pointer, &pointer, &value) == 0 ) goto failed_to_parse_json_value;

        // Add the value to the dictionary
        dict_add(p_dict, key, value);

        // Search for the next property ...
        if ( *pointer == ',' )
        {

            // Increment the property counter and the pointer 
            pointer++;

            // Parse another property
            goto parse_property;
        }

        // ... or the end of the object
        else if ( *pointer == '}' );
        
        // Default
        else

            // error
            return 0;
    }
    
    // error checking
    if ( *pointer != '}' ) return 0;

    // Increment the cursor
    pointer++;

    // return the cursor to the caller
    if ( return_pointer ) *return_pointer = pointer;

    // return a pointer to the caller
    *pp_dict = p_dict;

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

        // json errors
        {
            failed_to_parse_json_value:
                #ifndef NDEBUG
                    log_error("[json] Failed to parse json value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            expected_value:
                #ifndef NDEBUG
                    log_error("[json] Expected colon delimiter while parsing json text in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int json_array_parse ( char *pointer, char **return_pointer, array **const pp_array )
{

    // initialized data
    size_t  i       = 0;
    array  *p_array = 0;

    // Construct an array
    if ( array_construct(&p_array, DICT_SIZE) == 0 ) goto failed_to_construct_array;

    // error checking
    if ( pointer[0] != '[' ) return 0;

    // Increment the cursor
    pointer++;

    parse_property:
    
    // Parse whitespaces
    json_whitespace_parse(pointer, &pointer);

    // Walk the text
    if ( pointer[0] != ']' )
    {

        // initialized data
        json_value *value = 0;

        // Parse the text into a value
        json_value_parse(&pointer[i], &pointer, &value);

        // Add the value to the array
        array_add(p_array, value);

        // Check for another property
        if ( *pointer == ',' )
        {

            // Increment the property counter and the cursor
            pointer++;

            // Parse another property
            goto parse_property;
        }

        // Terminate
        else if ( *pointer == ']' );

        // Default
        else

            // error
            return 0;
    }
    
    // error checking
    if ( *pointer != ']' ) return 0;
        
    // Increment the cursor
    pointer++;

    // return the cursor to the caller
    if ( return_pointer ) *return_pointer = pointer;

    // return a pointer to the caller
    *pp_array = p_array;

    // success
    return 1;

    // error check
    {
        
        // dict errors
        {
            failed_to_construct_array:
                #ifndef NDEBUG
                    log_error("[array] Array constructor returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int json_value_parse ( char *text, char **return_pointer, json_value **const pp_value )
{

    // argument check
    if ( text     == (void *) 0 ) goto no_text;
    if ( pp_value == (void *) 0 ) goto no_value;

    // initialized data
    json_value *p_value = (void *) 0;

    if ( return_pointer  ) 
    {

        // allocate memory
        p_value = default_allocator(0, sizeof(json_value));
        
        // error check
        if ( p_value == (void *) 0 ) goto no_mem;

        // Initialize data
        memset(p_value, 0, sizeof(json_value));
    }
    else
    {

        // initialized data
        size_t len = strlen(text);

        // allocate memory
        p_value = default_allocator(0, sizeof(json_value) + len + 1);

        // error check
        if ( p_value == (void *) 0 ) goto no_mem;

        // Initialize data
        memset(p_value, 0, sizeof(json_value) + len + 1);

        // store the length
        p_value->len = len;

        // Copy the json text
        strncpy(p_value->_text, text, len);

        // Update the text pointer
        text = p_value->_text;
    }

    // Parse whitespace
    (void) json_whitespace_parse(text, &text);

    // Parse a value
    switch ( *text )
    {

        // This branch parses a string
        case '\"':
        {

            // initialized data
            char *last_text = text;
            size_t string_len = 0;
            
            // Parse the string
            if ( json_string_parse(text, &text) == 0 ) goto failed_to_parse_json_string;
            
            // Increment the cursor
            text++;

            // Compute the length of the string
            string_len = strlen(last_text); 

            // allocate memory for the string
            p_value->string = default_allocator(0, ( (string_len + 1 + 8) & 0xFFFFFFFFFFFFFFF8) * sizeof(char));
        
            // error check
            if ( p_value->string == (void *) 0 ) goto no_mem;

            // Initialize memory
            memset(p_value->string, 0, ( (string_len + 1 + 8) & 0xFFFFFFFFFFFFFFF8) * sizeof(char));

            // Copy the string 
            (void) strncpy(p_value->string, last_text, string_len+1);

            // Set the return type
            p_value->type = JSON_VALUE_STRING;
            
            // done
            break;
        }
        
        // This branch parses an object
        case '{':
            
            // Parse the JSON text as an object
            if ( json_object_parse(text, &text, &p_value->object) == 0 ) goto failed_to_parse_json_object;

            // Set the value type
            p_value->type = JSON_VALUE_OBJECT;
        
            // done
            break;
        
        
        // This branch parses an array
        case '[':

            // Parse the JSON text as an array
            if ( json_array_parse(text, &text, &p_value->list) == 0 ) goto failed_to_parse_json_array;

            // Set the value type
            p_value->type = JSON_VALUE_ARRAY;

            // done
            break;

        // This branch parses the 'true' keyword
        case 't':

            // Check for the correct keyworkd
            if ( strncmp(text, "true", 4) ) goto failed_to_parse_keyword;

            // Store a true value
            p_value->type = JSON_VALUE_BOOLEAN;
            p_value->boolean = true;

            // Skip the cursor
            text += 4;

            // done
            goto done;

        // This branch parses the 'false' keyword
        case 'f':
            
            // Check for the 'false' keyword
            if ( strncmp(text, "false", 5) ) goto failed_to_parse_keyword;

            // Store a true value
            p_value->type = JSON_VALUE_BOOLEAN;
            p_value->boolean = false;
            
            // Skip the cursor
            text += 5;
            
            // done
            goto done;
        
        // This branch parses the 'null' keyword
        case 'n':

            // error check
            if ( strncmp(text, "null", 4) ) goto failed_to_parse_keyword;
        
            // Free the JSON value
            p_value = default_allocator(p_value, 0);
            p_value = (void *) 0;
            
            // Skip the cursor
            text += 4;

            // done
            goto done;

        // Parse a number
        default:

            // done
            break;
    }

    // Parse a number
    if ( *text == '-' || (*text >= '0' && *text <= '9') ) 
    {
        // initialized data
        size_t i = 0,
               d = 0;
        bool   f = false;
        bool   n = false;
        
        // Check the number's sign
        if ( text[0] == '-' )

            // Set the negative bit, and skip past the sign
            i++, n = true;

        // Skip digits and decimal places
        while ( ( text[i] >= '0' && text[i] <= '9' ) || text[i] == '.' ) 
        { 

            // Check for a float
            f |= ( text[i] == '.' );
            
            // Increment
            i++, d++;
        };

        // Clear errors
        errno = 0;

        // Parse a floating point
        if ( f )
        {

            // Clear error
            errno = 0;

            // Store a number value
            p_value->type = JSON_VALUE_NUMBER;
            p_value->number = strtod(text, NULL);

            // Bounds check
            if ( errno == ERANGE )
            {
                
                // free the JSON value
                p_value = default_allocator(p_value, 0);

                // error handling
                goto float_bounds_exceeded;
            }
        }

        // Parse an integer
        else
        {
 
            // Clear error
            errno = 0;

            // Set the type of the value
            p_value->type = JSON_VALUE_INTEGER;
            p_value->integer = strtoll(text, NULL, 10);

            // Bounds check
            if ( errno == ERANGE )
            {

                // Free the value
                p_value = default_allocator(p_value, 0);
                
                // error handling
                goto integer_bounds_exceeded;
            }
        }

        // Decrement the decimal places
        d--;

        // Skip N decimal places
        text+=d; 

        // Increment the cursor
        text++;

        // Increment the cursor again if the number has a - sign
        text += n ? 1 : 0;
    }

    done:
    
    // Parse whitespace
    if ( p_value ) (void) json_whitespace_parse(text, &text);

    // Write the result
    *pp_value = p_value;

    // Update the cursor
    if ( return_pointer ) *return_pointer = text;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_text:
                #ifndef NDEBUG
                    log_error("[json] Null pointer provided for parameter \"text\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_value:
                #ifndef NDEBUG
                    log_error("[json] Null pointer provided for parameter \"pp_value\" in call to function \"%s\"\n", __FUNCTION__);
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

            failed_to_parse_keyword:
                #ifndef NDEBUG
                    log_error("[json] Failed to parse json keyword in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Clean up
                p_value = default_allocator(p_value, 0);

                // error
                return 0;

            integer_bounds_exceeded:
                #ifndef NDEBUG

                    // For some reason, printing this on one line caused a segfault. 
                    // Maybe I should report it?
                    log_error("[json] Integer must be between [%lld, ", -9223372036854775807LL);
                    log_error("%lld] in call to function \"%s\"\n",9223372036854775807LL, __FUNCTION__);
                #endif


                // error
                return 0;
            
            float_bounds_exceeded:
                #ifndef NDEBUG

                    // For some reason, printing this on one line caused a segfault. 
                    // Maybe I should report it?
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

int json_value_serialize ( const json_value *const p_value, char *_buffer )
{

    size_t written_characters = 0;

    // Null case
    if ( p_value == 0 ) written_characters += (size_t) sprintf(&_buffer[written_characters], "null");

    // Everything else
    else
    {

        // Print the value
        switch ( p_value->type )
        {
            
            // Print a boolean value
            case JSON_VALUE_BOOLEAN:
                written_characters += (size_t) sprintf(&_buffer[written_characters],"%s",p_value->boolean ? "true" : "false");
                break;
            
            // Print an integer value
            case JSON_VALUE_INTEGER:
                written_characters += (size_t) sprintf(&_buffer[written_characters],"%lld", p_value->integer);
                break;

            // Print a floating point value
            case JSON_VALUE_NUMBER:
            {
            
                // initialized data
                int precision = double_precision(p_value->number);

                // Print the value with the current precision
                if ( precision > 18 )
                    written_characters += (size_t) sprintf(&_buffer[written_characters],"%.*le", 16, p_value->number);
                else
                    written_characters += (size_t) sprintf(&_buffer[written_characters],"%.*lf", precision, p_value->number);
                
                // done
                break;
            }

            // Print a string
            case JSON_VALUE_STRING:
            {

                // initialized data
                size_t len = strlen(p_value->string);

                // Formatting
                written_characters += (size_t) sprintf(&_buffer[written_characters], "\"");
                
                // iterate over each character
                for (size_t i = 0; i < len; i++)
                {

                    // Check for an escape sequence
                    switch(p_value->string[i])
                    {

                        // Double quote
                        case '\"':
                            _buffer[written_characters++] = '\\';
                            _buffer[written_characters++] = '\"';

                            break;

                        // Backslash
                        case '\\':
                            _buffer[written_characters++] = '\\';
                            _buffer[written_characters++] = '\\';

                            break;

                        // Backspace
                        case '\b':
                            _buffer[written_characters++] = '\\';
                            _buffer[written_characters++] = 'b';

                            break;
                        
                        // Form feed
                        case '\f':
                            _buffer[written_characters++] = '\\';
                            _buffer[written_characters++] = 'f';

                            break;
                        
                        // Line feed
                        case '\n':
                            _buffer[written_characters++] = '\\';
                            _buffer[written_characters++] = 'n';

                            break;
                        
                        // Carriage return
                        case '\r':
                            _buffer[written_characters++] = '\\';
                            _buffer[written_characters++] = 'r';

                            break;
                        
                        // Horizontal tab
                        case '\t':
                            _buffer[written_characters++] = '\\';
                            _buffer[written_characters++] = 't';

                            break;

                        // TODO: Unicode
                        // TODO:case '\u':
                            // TODO:
                            // TODO:break;
                        
                        // Default
                        default:
                            _buffer[written_characters++] = p_value->string[i];
                    }
                }
            
                // Formatting
                written_characters += (size_t) sprintf(&_buffer[written_characters], "\"");
                
                // done
                break;
            }
            
            // Print an object
            case JSON_VALUE_OBJECT:
            {
                if ( p_value->object )
                {
                    // initialized data
                    size_t        property_count = dict_values(p_value->object, 0);
                    const char **keys            = 0;
                    json_value **values          = 0;

                    written_characters += (size_t) sprintf(&_buffer[written_characters],"{");

                    if ( property_count == 0 )
                        goto doned;

                    keys   = default_allocator(0, property_count * sizeof(char*));
                    values = default_allocator(0, property_count * sizeof(json_value*));

                    dict_keys(p_value->object, keys);
                    dict_values(p_value->object, (void **)values);
                    for (size_t i = 0; i < property_count-1; i++)
                    {
                        written_characters += (size_t) sprintf(&_buffer[written_characters],"\"%s\":",keys[i]);
                        written_characters += json_value_serialize(values[i],&_buffer[written_characters]);
                        written_characters += (size_t) sprintf(&_buffer[written_characters],",");
                    }
                    written_characters += (size_t) sprintf(&_buffer[written_characters],"\"%s\":",keys[property_count-1]);
                    written_characters +=  json_value_serialize(values[property_count-1], &_buffer[written_characters]);

                    keys = default_allocator(keys, 0);
                    values = default_allocator(values, 0);
                    doned:
                    written_characters += (size_t) sprintf(&_buffer[written_characters],"}");
                }
                break;
            }
            
            // Print an array
            case JSON_VALUE_ARRAY:
            {
                
                // initialized data
                size_t       element_count = 0;
                json_value **elements      = 0;

                // error check
                if ( p_value->list == (void *) 0 ) goto no_list;

                // Formatting
                written_characters += (size_t) sprintf(&_buffer[written_characters],"[");

                // Get the quantity of elements                    
                array_get(p_value->list, 0, &element_count);

                // edge case
                if ( 0 == element_count ) goto donea;
                
                // Get the contents of the array
                {

                    // allocate memory for the elements
                    elements = default_allocator(0, element_count * sizeof(json_value*));

                    // error check
                    if ( elements == (void *) 0 ) goto no_mem;

                    // Get the contents of the array
                    array_get(p_value->list, (void **)elements, 0);
                }
                
                // Print the first element
                if ( element_count ) written_characters += json_value_serialize(elements[0], &_buffer[written_characters]);
                
                // iterate over each element
                for (size_t i = 1; i < element_count; i++)
                {

                    // Formatting
                    written_characters += (size_t) sprintf(&_buffer[written_characters], ",");

                    // Print the value
                    written_characters += json_value_serialize(elements[i], &_buffer[written_characters]);
                }

                donea:

                // Free the element
                elements = default_allocator(elements, 0);

                // Formatting
                written_characters += (size_t) sprintf(&_buffer[written_characters], "]");
            }
                break;
            
            default:

                // error
                return 0;
            }
    }

    // success
    return written_characters;

    no_list:
    no_mem:
        return 0;

    // error handling
    {

    }
}

int json_value_print ( const json_value *const p_value )
{
    
    // done
    return json_value_fprint(p_value, stdout);
}

int json_value_fprint ( const json_value *const p_value, FILE *p_f )
{
    
    // initialized data
    size_t written_characters = 0;

    // Null case
    if ( p_value == 0 ) written_characters += fprintf(p_f,"null");

    // Everything else
    else
    {

        // Print the value
        switch (p_value->type)
        {
            // Print a boolean value
            case JSON_VALUE_BOOLEAN:
                written_characters += fprintf(p_f,"%s",p_value->boolean ? "true" : "false");
                break;
            
            // Print an integer value
            case JSON_VALUE_INTEGER:
                written_characters += fprintf(p_f,"%lld", p_value->integer);
                break;

            // Print a floating point value
            case JSON_VALUE_NUMBER:
            {

                // initialized data
                int precision = double_precision(p_value->number);

                // Print the value with the current precision
                if ( precision > 18 )
                    written_characters += (size_t) fprintf(p_f, "%.*le", 16, p_value->number);
                else
                    written_characters += (size_t) fprintf(p_f, "%.*lf", precision, p_value->number);
                
                // done
                break;
            }

            // Print a string
            case JSON_VALUE_STRING:
            {

                // initialized data
                size_t len = strlen(p_value->string);

                // Formatting
                written_characters += fprintf(p_f, "\"");
                
                // iterate over each character
                for (size_t i = 0; i < len; i++)
                {

                    // Check for an escape sequence
                    switch(p_value->string[i])
                    {

                        // Double quote
                        case '\"':
                            putc('\\', p_f); 
                            putc('\"', p_f);

                            break;

                        // Backslash
                        case '\\':
                            putc('\\', p_f);
                            putc('\\', p_f);

                            break;

                        // Forward slash
                        case '/':
                            putc('/', p_f);

                            break;
                        
                        // Backspace
                        case '\b':
                            putc('\\', p_f);
                            putc('b', p_f);

                            break;
                        
                        // Form feed
                        case '\f':
                            putc('\\', p_f);
                            putc('f', p_f);

                            break;
                        
                        // Line feed
                        case '\n':
                            putc('\\', p_f);
                            putc('n', p_f);

                            break;
                        
                        // Carriage return
                        case '\r':
                            putc('\\', p_f);
                            putc('r', p_f);

                            break;
                        
                        // Horizontal tab
                        case '\t':
                            putc('\\', p_f);
                            putc('t', p_f);

                            break;

                        // TODO: Unicode
                        // TODO:case '\u':
                            // TODO:
                            // TODO:break;
                        
                        // Default
                        default:
                            written_characters--;
                            putc(p_value->string[i], p_f);
                    }

                    written_characters += 2;
                }
            
                // Formatting
                written_characters += fprintf(p_f, "\"");

                // done
                break;
            }
            
            // Print an object
            case JSON_VALUE_OBJECT:
            {
                if ( p_value->object )
                {
                    // initialized data
                    size_t        property_count = dict_values(p_value->object, 0);
                    const char **keys            = 0;
                    json_value **values          = 0;

                    written_characters += fprintf(p_f,"{");

                    if ( property_count == 0 ) goto doned;

                    keys   = default_allocator(0, property_count * sizeof(char*));
                    values = default_allocator(0, property_count * sizeof(json_value*));

                    dict_keys(p_value->object, keys);
                    dict_values(p_value->object, (void **)values);
                    for (size_t i = 0; i < property_count-1; i++)
                    {
                        written_characters += fprintf(p_f,"\"%s\":",keys[i]);
                        written_characters += json_value_fprint(values[i],p_f);
                        written_characters += fprintf(p_f,",");
                    }
                    written_characters += fprintf(p_f,"\"%s\":",keys[property_count-1]);
                    written_characters += json_value_fprint(values[property_count-1],p_f);

                    keys = default_allocator(keys, 0);
                    values = default_allocator(values, 0);
                    doned:
                    written_characters += fprintf(p_f,"}");
                    
                    
                }
                
                // done
                break;
            }
            
            // Print an array
            case JSON_VALUE_ARRAY:
            {
                
                // initialized data
                size_t       element_count = 0;
                json_value **elements      = 0;

                // error check
                if ( p_value->list == (void *) 0 ) goto no_list;
                
                // Formatting
                written_characters += fprintf(p_f,"[");

                // Get the quantity of elements                    
                array_get(p_value->list, 0, &element_count);
                
                // edge case
                if ( 0 == element_count ) goto donea;

                // Get the contents of the array
                {

                    // allocate memory for the elements
                    elements = default_allocator(0, element_count * sizeof(json_value*));

                    // error check
                    if ( elements == (void *) 0 ) goto no_mem;

                    // Get the contents of the array
                    array_get(p_value->list, (void **)elements, 0);
                }
                
                // Print the first element
                if ( element_count ) written_characters += json_value_fprint(elements[0],p_f);
                
                // iterate over each element
                for (size_t i = 1; i < element_count; i++)
                {

                    // Formatting
                    written_characters += fprintf(p_f, ",");

                    // Print the value
                    written_characters += json_value_fprint(elements[i],p_f);
                }

                // Free the element
                if ( 0 < element_count )
                    elements = default_allocator(elements, 0);
                
                donea:

                // Formatting
                written_characters += fprintf(p_f, "]");

                // done
                break;
            }
            
            default:

                // error
                return 0;
            }
    }

    // success
    return written_characters;
    
    no_list:
        return 0;

    // error handling
    {
        
        // argument check
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[standard library] Failed to allocate memory in call to function \"%s\"\n",__FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

void *json_value_free ( json_value *p_value, unsigned long long unused )
{
    
    // unused
    (void) unused;

    // argument errors
    if ( p_value == (void *)0 ) return NULL;
    
    // strategy
    switch ( p_value->type )
    {
        case JSON_VALUE_INVALID:
        case JSON_VALUE_BOOLEAN:
        case JSON_VALUE_INTEGER:
        case JSON_VALUE_NUMBER:
        
            // done
            break;
            
        case JSON_VALUE_STRING:

            // Free the string
            p_value->string = default_allocator(p_value->string, 0);
            
            // done
            break;

        case JSON_VALUE_OBJECT:

            // destroy the dictionary
            dict_destroy(&p_value->object, (fn_allocator *) json_value_free);

            // done
            break;

        case JSON_VALUE_ARRAY:

            // destroy the array
            array_destroy(&p_value->list, (fn_allocator *) json_value_free);

            // done
            break;
        
        default:

            // done
            break;
    }

    // done
    return default_allocator(p_value, 0);
}
