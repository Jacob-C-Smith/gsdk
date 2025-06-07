/** !
 * Implementation of pack
 * 
 * @file pack.c
 * 
 * @author Jacob Smith
 */

// Header
#include <pack/pack.h>

// dict module
#include <dict/dict.h>

// Data
static bool initialized = false;
static dict *p_pack_lookup   = (void *) 0,
            *p_unpack_lookup = (void *) 0;

// Function definitions
void pack_init ( void )
{

    // State check
    if ( initialized == true ) return;

    // Initialize the log library
    log_init();

    // Initialize the sync library
    sync_init();

    // Initialize the hash cache library
    hash_cache_init();

    // Initialize the dict library
    dict_init();

    // Construct a lookup for pack
    dict_construct(&p_pack_lookup, 64, 0);
    
    // Construct a lookup for pack
    dict_construct(&p_unpack_lookup, 64, 0);

    // Set the initialized flag
    initialized = true;

    // Done
    return;
}

int pack_register ( const char *const p_name, pack_pair _pack_pair )
{

    // Argument check
    if ( _pack_pair.pfn_pack   == (void *) 0 &&
         _pack_pair.pfn_unpack == (void *) 0    ) goto no_pack_unpack;
    if ( p_name                == (void *) 0    ) goto no_name;

    // Warnings
    if ( _pack_pair.pfn_pack == (void *) 0 )
        log_warning(
            "[pack] Null pointer provided for parameter \"_pack_pair.pfn_pack\" in call to function \"%s\"\n"\
            "\t%s(s) will be parsable, but not serializable\n",
            __FUNCTION__,
            p_name
        );

    if ( _pack_pair.pfn_unpack == (void *) 0 )
        log_warning(
            "[pack] Null pointer provided for parameter \"_pack_pair.pfn_unpack\" in call to function \"%s\"\n"\
            "\t%s(s) will be serializable, but not parsable\n",
            __FUNCTION__,
            p_name
        );

    // Done
    return dict_add(p_pack_lookup, p_name, _pack_pair.pfn_pack)  && 
           dict_add(p_unpack_lookup, p_name, _pack_pair.pfn_unpack);

    // Error handling
    {

        // Argument errors
        {
            no_name:
                #ifndef NDEBUG
                    log_error("[pack] Null pointer provided for parameter \"p_name\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_pack_unpack:
                #ifndef NDEBUG
                    log_error("[pack] Null pointers provided for parameters \"_pack_pair.pfn_pack\" and \"_pack_pair.pfn_unpack\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int pack_unregister ( const char *const p_name )
{

    // Argument check
    if ( p_name == (void *) 0 ) goto no_name;

    // Unregister the pack/unpack functions
    (void) dict_pop(p_pack_lookup, p_name, 0),
    (void) dict_pop(p_unpack_lookup, p_name, 0);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_name:
                #ifndef NDEBUG
                    log_error("[pack] Null pointer provided for parameter \"p_name\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }

}

size_t pack ( char *type, void *p_buffer, void *p_value )
{

    // Argument check
    if ( type     == (void *) 0 ) goto no_type;
    if ( p_buffer == (void *) 0 ) goto no_buffer;
    if ( p_value  == (void *) 0 ) goto no_value;
    
    // Initialized data
    fn_pack *pfn_pack = dict_get(p_pack_lookup, type);

    // TODO: Check if pfn_pack is NULL

    // Done
    return pfn_pack(p_buffer, p_value);

    // Error handling
    {

        // Argument errors
        {
            no_type:
                #ifndef NDEBUG
                    log_error("[pack] Null pointer provided for parameter \"type\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_buffer:
                #ifndef NDEBUG
                    log_error("[pack] Null pointer provided for parameter \"p_buffer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_value:
                #ifndef NDEBUG
                    log_error("[pack] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

size_t unpack ( char *type, void *p_value, void *p_buffer )
{

    // Argument check
    if ( type     == (void *) 0 ) goto no_type;
    if ( p_value  == (void *) 0 ) goto no_value;
    if ( p_buffer == (void *) 0 ) goto no_buffer;
    
    // Initialized data
    fn_unpack *pfn_unpack = dict_get(p_unpack_lookup, type);

    // TODO: Check if pfn_pack is NULL

    // Done
    return pfn_unpack(p_value, p_buffer);

    // Error handling
    {

        // Argument errors
        {
            no_type:
                #ifndef NDEBUG
                    log_error("[pack] Null pointer provided for parameter \"type\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_value:
                #ifndef NDEBUG
                    log_error("[pack] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_buffer:
                #ifndef NDEBUG
                    log_error("[pack] Null pointer provided for parameter \"p_buffer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

size_t pack_pack ( void *p_buffer, const char *restrict format, ... )
{

    // Argument check
    if ( p_buffer == (void *) 0 ) goto no_buffer; 
    if ( format   == (void *) 0 ) goto no_format; 

    // Initialized data
    va_list list;
    size_t read = 0, written = 0, format_specifier_length = 1;

    // Construct the variadic list
    va_start(list, format);
    
    parse_format_specifier:

    // Iterate through the format string
    while ( format[read] )
    {

        // Parse format characters
        if ( format[read] == '%' )
        {

            // Increment the index
            read++;

            continue_parsing_format_specifier:

            // Strategy
            switch ( format[read] )
            {

                // Length
                case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':

                    // Parse the quantity of values
                    goto parse_len;

                    // Continue reading format specifiers
                    done_reading_format_specifier_length: goto continue_parsing_format_specifier;
                
                // Float
                case 'f':

                    // Increment the read index
                    read++;

                    // 32-bit value
                    if ( format[read] == '3' && format[read + 1] == '2')
                        goto write_f32;

                    // 64-bit value
                    else if ( format[read] == '6' && format[read + 1] == '4')
                        goto write_f64;
                    
                    // Default
                    else
                        return 0;

                    // Done
                    break;

                // Integer
                case 'i':

                    // Increment the read index
                    read++;

                    // 8-bit value
                    if ( format[read] == '8' )
                        goto write_8;
                    
                    // 16-bit value
                    else if ( format[read] == '1' && format[read + 1] == '6' )
                        goto write_16;
                    
                    // 32-bit value
                    else if ( format[read] == '3' && format[read + 1] == '2' )
                        goto write_32;

                    // 64-bit value
                    else if ( format[read] == '6' && format[read + 1] == '4' )
                        goto write_64;

                    // Default
                    else
                        return 0;

                    // Done
                    break;

                // String (maximum 65535)
                case 's': 

                    // Increment the read index
                    read++;

                    // Write the string
                    goto write_str;
                default:

                    // Done
                    break;
            }
        }
        
        // Increment the index
        read++;
	}

    // Done with variadic list
    va_end(list);

    // Success
    return written;

    // This branch parses the quantity of values in a format specifier
    parse_len:
    {

        // Store the length of the format specifier
        format_specifier_length = atoi(&format[read]);

        // Update the read index
        while (isdigit(format[++read]));

        // Done
        goto done_reading_format_specifier_length;
    }

    // This branch writes bytes 
    write_8:
    {

        // Iterate through each variadic argument
        for (size_t i = 0; i < format_specifier_length; i++)
        {

            // Initialized data
            char c = va_arg(list, int);

            // Store the value
            *((char *)p_buffer) = c;
            
            // Update the buffer and written byte counter
            p_buffer += sizeof(char), written += sizeof(char);
        }

        // Reset the format specifier length
        format_specifier_length = 1;
        
        // Update the read index
        read++;

        // Continue
        goto parse_format_specifier;
    }

    // This branch writes shorts 
    write_16:
    {

        // Iterate through each variadic argument
        for (size_t i = 0; i < format_specifier_length; i++)
        {

            // Initialized data
            short s = va_arg(list, int);

            // Store the value
            *((short *)p_buffer) = s;
            
            // Update the buffer and written byte counter
            p_buffer += sizeof(short), written += sizeof(short);
        }

        // Reset the format specifier length
        format_specifier_length = 1;

        // Update the read index
        read += 2;

        // Continue
        goto parse_format_specifier;
    }
    
    // This branch writes ints 
    write_32:
    {

        // Iterate through each variadic argument
        for (size_t i = 0; i < format_specifier_length; i++)
        {

            // Initialized data
            int l = va_arg(list, int);

            // Store the value
            *((int *)p_buffer) = l;
            
            // Update the buffer and written byte counter
            p_buffer += sizeof(int), written += sizeof(int);
        }

        // Reset the format specifier length
        format_specifier_length = 1;

        // Update the read index
        read += 2;

        // Continue
        goto parse_format_specifier;
    }

    // This branch writes longs 
    write_64:
    {

        // Iterate through each variadic argument
        for (size_t i = 0; i < format_specifier_length; i++)
        {

            // Initialized data
            long l = va_arg(list, long);

            // Store the value
            *((long *)p_buffer) = l;
            
            // Update the buffer and written byte counter
            p_buffer += sizeof(long), written += sizeof(long);
        }

        // Reset the format specifier length
        format_specifier_length = 1;

        // Update the read index
        read += 2;

        // Continue
        goto parse_format_specifier;
    }

    // This branch writes f32s 
    write_f32:
    {

        // Iterate through each variadic argument
        for (size_t i = 0; i < format_specifier_length; i++)
        {

            // Initialized data
            float f = va_arg(list, double);

            // Store the value
            *((float *)p_buffer) = f;
            
            // Update the buffer and written byte counter
            p_buffer += sizeof(float), written += sizeof(float);
        }

        // Reset the format specifier length
        format_specifier_length = 1;

        // Update the read index
        read += 2;

        // Continue
        goto parse_format_specifier;
    }
    
    // This branch writes f64s
    write_f64:
    {

        // Iterate through each variadic argument
        for (size_t i = 0; i < format_specifier_length; i++)
        {

            // Initialized data
            double d = va_arg(list, double);

            // Store the value
            *((double *)p_buffer) = d;
            
            // Update the buffer and written byte counter
            p_buffer += sizeof(double), written += sizeof(double);
        }

        // Reset the format specifier length
        format_specifier_length = 1;
        
        // Update the read index
        read += 2;

        // Continue
        goto parse_format_specifier;
    }

    // This branch writes strings 
    write_str:
    {

        // Iterate through each variadic argument
        for (size_t i = 0; i < format_specifier_length; i++)
        {

            // Initialized data
            const char *s = va_arg(list, char *);
            size_t len = strlen(s) + 1;

            // Error check
            if ( len > USHRT_MAX - 1 ) return 0;

            // Store the length
            *((unsigned short *)p_buffer) = (unsigned short) len;

            // Update the buffer and written byte counter
            p_buffer += sizeof(unsigned short), written += sizeof(unsigned short);
            
            // Store the value
            memcpy(p_buffer, s, len);

            // Update the buffer and written byte counter
            p_buffer += len, written += len;
        }

        // Reset the format specifier length
        format_specifier_length = 1;

        // Continue
        goto parse_format_specifier;
    }
    
    // Error handling
    {

        // Argument errors
        {
            no_buffer:
                #ifndef NDEBUG 
                    printf("[pack] Null pointer provided for parameter \"p_buffer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_format:
                #ifndef NDEBUG 
                    printf("[pack] Null pointer provided for parameter \"format\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

size_t pack_unpack ( void *p_buffer, const char *restrict format, ... )
{

    // Argument check
    if ( p_buffer == (void *) 0 ) goto no_buffer; 
    if ( format   == (void *) 0 ) goto no_format; 

    // Initialized data
    va_list list;
    size_t read = 0, written = 0, format_specifier_length = 1;

    // Construct the variadic list
    va_start(list, format);
    
    parse_format_specifier:

    // Iterate through the format string
    while ( format[read] )
    {

        // Parse format characters
        if ( format[read] == '%' )
        {

            // Increment the index
            read++;

            continue_parsing_format_specifier:

            // Strategy
            switch ( format[read] )
            {

                // Length
                case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':

                    // Parse the quantity of values
                    goto parse_len;

                    // Continue reading format specifiers
                    done_reading_format_specifier_length: goto continue_parsing_format_specifier;
                
                // Float
                case 'f':

                    // Increment the read index
                    read++;

                    // 32-bit value
                    if ( format[read] == '3' && format[read + 1] == '2')
                        goto read_f32;

                    // 64-bit value
                    else if ( format[read] == '6' && format[read + 1] == '4')
                        goto read_f64;
                    
                    // Default
                    else
                        return 0;

                    // Done
                    break;

                // Integer
                case 'i':

                    // Increment the read index
                    read++;

                    // 8-bit value
                    if ( format[read] == '8' )
                        goto read_8;
                    
                    // 16-bit value
                    else if ( format[read] == '1' && format[read + 1] == '6' )
                        goto read_16;
                    
                    // 32-bit value
                    else if ( format[read] == '3' && format[read + 1] == '2' )
                        goto read_32;

                    // 64-bit value
                    else if ( format[read] == '6' && format[read + 1] == '4' )
                        goto read_64;

                    // Default
                    else
                        return 0;

                    // Done
                    break;

                // String (maximum 65535)
                case 's': 

                    // Increment the read index
                    read++;

                    // Write the string
                    goto read_str;
                default:

                    // Done
                    break;
            }
        }
        
        // Increment the index
        read++;
	}

    // Done with variadic list
    va_end(list);

    // Success
    return written;

    // This branch parses the quantity of values in a format specifier
    parse_len:
    {

        // Store the length of the format specifier
        format_specifier_length = atoi(&format[read]);

        // Update the read index
        while (isdigit(format[++read]));

        // Done
        goto done_reading_format_specifier_length;
    }

    // This branch reads bytes 
    read_8:
    {

        // Iterate through each variadic argument
        for (size_t i = 0; i < format_specifier_length; i++)
        {

            // Initialized data
            char *c = va_arg(list, void *);

            // Store the value
            *c = *(char *)p_buffer;
            
            // Update the buffer and written byte counter
            p_buffer += sizeof(char), written += sizeof(char);
        }

        // Reset the format specifier length
        format_specifier_length = 1;
        
        // Update the read index
        read++;

        // Continue
        goto parse_format_specifier;
    }

    // This branch writes shorts 
    read_16:
    {

        // Iterate through each variadic argument
        for (size_t i = 0; i < format_specifier_length; i++)
        {

            // Initialized data
            short *s = va_arg(list, void *);

            // Store the value
            *s = *(short *)p_buffer;
            
            // Update the buffer and written byte counter
            p_buffer += sizeof(short), written += sizeof(short);
        }

        // Reset the format specifier length
        format_specifier_length = 1;

        // Update the read index
        read += 2;

        // Continue
        goto parse_format_specifier;
    }
    
    // This branch writes ints 
    read_32:
    {

        // Iterate through each variadic argument
        for (size_t i = 0; i < format_specifier_length; i++)
        {

            // Initialized data
            int *l = va_arg(list, void *);

            // Store the value
            *l = *(int *)p_buffer;
            
            // Update the buffer and written byte counter
            p_buffer += sizeof(int), written += sizeof(int);
        }

        // Reset the format specifier length
        format_specifier_length = 1;

        // Update the read index
        read += 2;

        // Continue
        goto parse_format_specifier;
    }

    // This branch writes longs 
    read_64:
    {

        // Iterate through each variadic argument
        for (size_t i = 0; i < format_specifier_length; i++)
        {

            // Initialized data
            long *l = va_arg(list, void *);

            // Store the value
            *l = *(long *)p_buffer;
            
            // Update the buffer and written byte counter
            p_buffer += sizeof(long), written += sizeof(long);
        }

        // Reset the format specifier length
        format_specifier_length = 1;

        // Update the read index
        read += 2;

        // Continue
        goto parse_format_specifier;
    }

    // This branch reads f32s 
    read_f32:
    {

        // Iterate through each variadic argument
        for (size_t i = 0; i < format_specifier_length; i++)
        {

            // Initialized data
            float *f = va_arg(list, void *);

            // Store the value
            *f = *(float *)p_buffer;
            
            // Update the buffer and written byte counter
            p_buffer += sizeof(float), written += sizeof(float);
        }

        // Reset the format specifier length
        format_specifier_length = 1;

        // Update the read index
        read += 2;

        // Continue
        goto parse_format_specifier;
    }
    
    // This branch writes f64s
    read_f64:
    {

        // Iterate through each variadic argument
        for (size_t i = 0; i < format_specifier_length; i++)
        {

            // Initialized data
            double *d = va_arg(list, void *);

            // Store the value
            *d = *(double *)p_buffer;
            
            // Update the buffer and written byte counter
            p_buffer += sizeof(double), written += sizeof(double);
        }

        // Reset the format specifier length
        format_specifier_length = 1;
        
        // Update the read index
        read += 2;

        // Continue
        goto parse_format_specifier;
    }

    // This branch writes strings 
    read_str:
    {

        // Iterate through each variadic argument
        for (size_t i = 0; i < format_specifier_length; i++)
        {

            // Initialized data
            const char *s = va_arg(list, void *);
            size_t len = *((unsigned short *)p_buffer);

            // Store the value
            memcpy(s, p_buffer + 2, len);

            // Update the buffer and written byte counter
            p_buffer += ( len + 2 ),
            written  += ( len + 2 );
        }

        // Reset the format specifier length
        format_specifier_length = 1;
        
        // Update the read index
        read += 1;

        // Continue
        goto parse_format_specifier;
    }
    
    // Error handling
    {

        // Argument errors
        {
            no_buffer:
                #ifndef NDEBUG 
                    printf("[pack] Null pointer provided for parameter \"p_buffer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_format:
                #ifndef NDEBUG 
                    printf("[pack] Null pointer provided for parameter \"format\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

void pack_exit ( void )
{
    
    // State check
    if ( initialized == false ) return;

    // Clean up the log library
    log_exit();

    // Clean up the sync library
    sync_exit();

    // Clean up the hash cache library
    hash_cache_exit();

    // Clean up the dict library
    dict_exit();

    // Destruct the pack lookup 
    dict_destroy(&p_pack_lookup);

    // Destruct the unpack lookup 
    dict_destroy(&p_unpack_lookup);

    // Clear the initialized flag
    initialized = false;

    // Done
    return;
}
