/** !
 * Implementation of pack interface
 * 
 * @file pack.c
 * 
 * @author Jacob Smith
 */

// Header
#include <reflection/pack.h>

// Function definitions
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
                    if ( format[read] == '3' && format[read + 1] == '2') goto write_f32;

                    // 64-bit value
                    else if ( format[read] == '6' && format[read + 1] == '4') goto write_f64;
                    
                    // Default
                    else return 0;

                    // Done
                    break;

                // Integer
                case 'i':

                    // Increment the read index
                    read++;

                    // 8-bit value
                    if ( format[read] == '8' ) goto write_8;
                    
                    // 16-bit value
                    else if ( format[read] == '1' && format[read + 1] == '6' ) goto write_16;
                    
                    // 32-bit value
                    else if ( format[read] == '3' && format[read + 1] == '2' ) goto write_32;

                    // 64-bit value
                    else if ( format[read] == '6' && format[read + 1] == '4' )  goto write_64;

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
                    if ( format[read] == '3' && format[read + 1] == '2') goto read_f32;

                    // 64-bit value
                    else if ( format[read] == '6' && format[read + 1] == '4') goto read_f64;
                    
                    // Default
                    else return 0;

                    // Done
                    break;

                // Integer
                case 'i':

                    // Increment the read index
                    read++;

                    // 8-bit value
                    if ( format[read] == '8' ) goto read_8;
                    
                    // 16-bit value
                    else if ( format[read] == '1' && format[read + 1] == '6' ) goto read_16;
                    
                    // 32-bit value
                    else if ( format[read] == '3' && format[read + 1] == '2' ) goto read_32;

                    // 64-bit value
                    else if ( format[read] == '6' && format[read + 1] == '4' ) goto read_64;

                    // Default
                    else return 0;

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
        read += 1;

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
            char *s = va_arg(list, void *);
            size_t len = *((unsigned short *)p_buffer);

            // Store the value
            memcpy(s, p_buffer + 2, len);

            // Update the buffer and written byte counter
            p_buffer += ( len + 2 ),
            written  += ( len + 2 );
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
