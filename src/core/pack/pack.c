/** !
 * Implementation of pack interface
 * 
 * @file src/core/pack/pack.c
 * 
 * @author Jacob Smith
 */

// header
#include <core/pack.h>

// function definitions
size_t pack_pack ( stream *p_stream, const char *restrict format, ... )
{

    // argument check
    if ( NULL == p_stream ) goto no_stream; 
    if ( NULL ==   format ) goto no_format; 

    // initialized data
    va_list list;
    size_t read = 0, written = 0, format_specifier_length = 1;

    // construct the variadic list
    va_start(list, format);
    
    parse_format_specifier:

    // iterate through the format string
    while ( format[read] )
    {

        // parse format characters
        if ( format[read] == '%' )
        {

            // increment the index
            read++;

            continue_parsing_format_specifier:

            // strategy
            switch ( format[read] )
            {

                // length
                case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':

                    // parse the quantity of values
                    goto parse_len;

                    // continue reading format specifiers
                    done_reading_format_specifier_length: goto continue_parsing_format_specifier;
                
                // float
                case 'f':

                    // increment the read index
                    read++;

                    // 32-bit value
                    if ( format[read] == '3' && format[read + 1] == '2') goto write_f32;

                    // 64-bit value
                    else if ( format[read] == '6' && format[read + 1] == '4') goto write_f64;
                    
                    // default
                    else return 0;

                    // done
                    break;

                // integer
                case 'i':

                    // increment the read index
                    read++;

                    // 8-bit value
                    if ( format[read] == '8' ) goto write_8;
                    
                    // 16-bit value
                    else if ( format[read] == '1' && format[read + 1] == '6' ) goto write_16;
                    
                    // 32-bit value
                    else if ( format[read] == '3' && format[read + 1] == '2' ) goto write_32;

                    // 64-bit value
                    else if ( format[read] == '6' && format[read + 1] == '4' )  goto write_64;

                    // default
                    else
                        return 0;

                    // done
                    break;

                // string (maximum 65535)
                case 's': 

                    // increment the read index
                    read++;

                    // Write the string
                    goto write_str;

                default:

                    // done
                    break;
            }
        }
        
        // increment the index
        read++;
	}

    // done with variadic list
    va_end(list);

    // success
    return written;

    // This branch parses the quantity of values in a format specifier
    parse_len:
    {

        // store the length of the format specifier
        format_specifier_length = atoi(&format[read]);

        // update the read index
        while (isdigit(format[++read]));

        // done
        goto done_reading_format_specifier_length;
    }

    // This branch writes bytes 
    write_8:
    {

        // iterate through each variadic argument
        for (size_t i = 0; i < format_specifier_length; i++)
        {

            // initialized data
            char c = va_arg(list, int);

            // store the value
            stream_write(p_stream, &c, sizeof(char));
            
            // update the written byte counter
            written += sizeof(char);
        }

        // reset the format specifier length
        format_specifier_length = 1;
        
        // update the read index
        read++;

        // continue
        goto parse_format_specifier;
    }

    // This branch writes shorts 
    write_16:
    {

        // iterate through each variadic argument
        for (size_t i = 0; i < format_specifier_length; i++)
        {

            // initialized data
            short s = va_arg(list, int);

            // store the value
            stream_write(p_stream, &s, sizeof(short));
            
            // update the written byte counter
            written += sizeof(short);
        }

        // reset the format specifier length
        format_specifier_length = 1;

        // update the read index
        read += 2;

        // continue
        goto parse_format_specifier;
    }
    
    // This branch writes ints 
    write_32:
    {

        // iterate through each variadic argument
        for (size_t i = 0; i < format_specifier_length; i++)
        {

            // initialized data
            int l = va_arg(list, int);

            // store the value
            stream_write(p_stream, &l, sizeof(int));
            
            // update the written byte counter
            written += sizeof(int);
        }

        // reset the format specifier length
        format_specifier_length = 1;

        // update the read index
        read += 2;

        // continue
        goto parse_format_specifier;
    }

    // This branch writes longs 
    write_64:
    {

        // iterate through each variadic argument
        for (size_t i = 0; i < format_specifier_length; i++)
        {

            // initialized data
            long l = va_arg(list, long);

            // store the value
            stream_write(p_stream, &l, sizeof(long));
            
            // update the written byte counter
            written += sizeof(long);
        }

        // reset the format specifier length
        format_specifier_length = 1;

        // update the read index
        read += 2;

        // continue
        goto parse_format_specifier;
    }

    // This branch writes f32s 
    write_f32:
    {

        // iterate through each variadic argument
        for (size_t i = 0; i < format_specifier_length; i++)
        {

            // initialized data
            float f = va_arg(list, double);

            // store the value
            stream_write(p_stream, &f, sizeof(float));
            
            // update the written byte counter
            written += sizeof(float);
        }

        // reset the format specifier length
        format_specifier_length = 1;

        // update the read index
        read += 2;

        // continue
        goto parse_format_specifier;
    }
    
    // This branch writes f64s
    write_f64:
    {

        // iterate through each variadic argument
        for (size_t i = 0; i < format_specifier_length; i++)
        {

            // initialized data
            double d = va_arg(list, double);

            // store the value
            stream_write(p_stream, &d, sizeof(double));
            
            // update the written byte counter
            written += sizeof(double);
        }

        // reset the format specifier length
        format_specifier_length = 1;
        
        // update the read index
        read += 2;

        // continue
        goto parse_format_specifier;
    }

    // This branch writes strings 
    write_str:
    {

        // iterate through each variadic argument
        for (size_t i = 0; i < format_specifier_length; i++)
        {

            // initialized data
            const char *s = va_arg(list, char *);
            size_t len = strlen(s) + 1;

            // error check
            if ( len > USHRT_MAX - 1 ) return 0;

            // store the length
            stream_write(p_stream, &len, sizeof(unsigned short));

            // update the written byte counter
            written += sizeof(unsigned short);
            
            // store the value
            stream_write(p_stream, s, len);

            // update the written byte counter
            written += len;
        }

        // reset the format specifier length
        format_specifier_length = 1;

        // continue
        goto parse_format_specifier;
    }
    
    // error handling
    {

        // argument errors
        {
            no_stream:
                #ifndef NDEBUG 
                    printf("[pack] Null pointer provided for parameter \"p_stream\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_format:
                #ifndef NDEBUG 
                    printf("[pack] Null pointer provided for parameter \"format\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

size_t pack_unpack ( stream *p_stream, const char *restrict format, ... )
{

    // argument check
    if ( NULL == p_stream ) goto no_stream; 
    if ( NULL ==   format ) goto no_format; 

    // initialized data
    va_list list;
    size_t read = 0, written = 0, format_specifier_length = 1;

    // construct the variadic list
    va_start(list, format);
    
    parse_format_specifier:

    // iterate through the format string
    while ( format[read] )
    {

        // parse format characters
        if ( format[read] == '%' )
        {

            // increment the index
            read++;

            continue_parsing_format_specifier:

            // Strategy
            switch ( format[read] )
            {

                // Length
                case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':

                    // parse the quantity of values
                    goto parse_len;

                    // continue reading format specifiers
                    done_reading_format_specifier_length: goto continue_parsing_format_specifier;
                
                // Float
                case 'f':

                    // increment the read index
                    read++;

                    // 32-bit value
                    if ( format[read] == '3' && format[read + 1] == '2') goto read_f32;

                    // 64-bit value
                    else if ( format[read] == '6' && format[read + 1] == '4') goto read_f64;
                    
                    // default
                    else return 0;

                    // done
                    break;

                // Integer
                case 'i':

                    // increment the read index
                    read++;

                    // 8-bit value
                    if ( format[read] == '8' ) goto read_8;
                    
                    // 16-bit value
                    else if ( format[read] == '1' && format[read + 1] == '6' ) goto read_16;
                    
                    // 32-bit value
                    else if ( format[read] == '3' && format[read + 1] == '2' ) goto read_32;

                    // 64-bit value
                    else if ( format[read] == '6' && format[read + 1] == '4' ) goto read_64;

                    // default
                    else return 0;

                    // done
                    break;

                // string (maximum 65535)
                case 's': 

                    // increment the read index
                    read++;

                    // write the string
                    goto read_str;
                default:

                    // done
                    break;
            }
        }
        
        // increment the index
        read++;
	}

    // done with variadic list
    va_end(list);

    // success
    return written;

    // This branch parses the quantity of values in a format specifier
    parse_len:
    {

        // store the length of the format specifier
        format_specifier_length = atoi(&format[read]);

        // update the read index
        while (isdigit(format[++read]));

        // done
        goto done_reading_format_specifier_length;
    }

    // This branch reads bytes 
    read_8:
    {

        // iterate through each variadic argument
        for (size_t i = 0; i < format_specifier_length; i++)
        {

            // initialized data
            char *c = va_arg(list, void *);

            // store the value
            stream_read(p_stream, c, sizeof(char));
            
            // update written byte counter
            written += sizeof(char);
        }

        // reset the format specifier length
        format_specifier_length = 1;
        
        // update the read index
        read++;

        // continue
        goto parse_format_specifier;
    }

    // This branch reads shorts 
    read_16:
    {

        // iterate through each variadic argument
        for (size_t i = 0; i < format_specifier_length; i++)
        {

            // initialized data
            short *s = va_arg(list, void *);

            // store the value
            stream_read(p_stream, s, sizeof(short));
            
            // update the written byte counter
            written += sizeof(short);
        }

        // reset the format specifier length
        format_specifier_length = 1;

        // update the read index
        read += 2;

        // continue
        goto parse_format_specifier;
    }
    
    // This branch reads ints 
    read_32:
    {

        // iterate through each variadic argument
        for (size_t i = 0; i < format_specifier_length; i++)
        {

            // initialized data
            int *l = va_arg(list, void *);

            // store the value
            stream_read(p_stream, l, sizeof(int));
            
            // update the written byte counter
            written += sizeof(int);
        }

        // reset the format specifier length
        format_specifier_length = 1;

        // update the read index
        read += 2;

        // continue
        goto parse_format_specifier;
    }

    // This branch reads longs 
    read_64:
    {

        // iterate through each variadic argument
        for (size_t i = 0; i < format_specifier_length; i++)
        {

            // initialized data
            long *l = va_arg(list, void *);

            // store the value
            stream_read(p_stream, l, sizeof(long));
            
            // update the written byte counter
            written += sizeof(long);
        }

        // reset the format specifier length
        format_specifier_length = 1;

        // update the read index
        read += 2;

        // continue
        goto parse_format_specifier;
    }

    // This branch reads f32s 
    read_f32:
    {

        // iterate through each variadic argument
        for (size_t i = 0; i < format_specifier_length; i++)
        {

            // initialized data
            float *f = va_arg(list, void *);

            // store the value
            stream_read(p_stream, f, sizeof(float));
            
            // update the written byte counter
            written += sizeof(float);
        }

        // reset the format specifier length
        format_specifier_length = 1;

        // update the read index
        read += 1;

        // continue
        goto parse_format_specifier;
    }
    
    // This branch reads f64s
    read_f64:
    {

        // iterate through each variadic argument
        for (size_t i = 0; i < format_specifier_length; i++)
        {

            // initialized data
            double *d = va_arg(list, void *);

            // store the value
            stream_read(p_stream, d, sizeof(double));
            
            // update the written byte counter
            written += sizeof(double);
        }

        // reset the format specifier length
        format_specifier_length = 1;
        
        // update the read index
        read += 2;

        // continue
        goto parse_format_specifier;
    }

    // This branch reads strings 
    read_str:
    {

        // iterate through each variadic argument
        for (size_t i = 0; i < format_specifier_length; i++)
        {

            // initialized data
            char *s = va_arg(list, void *);
            size_t len = 0;

            // store the value
            stream_read(p_stream, &len, sizeof(unsigned short)),
            stream_read(p_stream, s, len);

            // update the written byte counter
            written  += ( len + 2 );
        }

        // reset the format specifier length
        format_specifier_length = 1;

        // continue
        goto parse_format_specifier;
    }
    
    // error handling
    {

        // argument errors
        {
            no_stream:
                #ifndef NDEBUG 
                    printf("[pack] Null pointer provided for parameter \"p_stream\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_format:
                #ifndef NDEBUG 
                    printf("[pack] Null pointer provided for parameter \"format\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}
