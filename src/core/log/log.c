/** !
 * log library
 *
 * @file src/core/log/log.c
 *
 * @author Jacob Smith
 */

// headers
#include <core/log.h>

// data
static FILE *log_file = NULL;
static bool  log_with_ansi_color = false;
static bool  initialized = false;

void log_init ( void )
{

    // state check
    if ( initialized == true ) return;

    // log to standard out
    log_file = stdout;
    
    // ANSI color flag
    log_with_ansi_color = true;
    
    // flush standard out
    fflush(stdout);

    // set the initialized flag
    initialized = true;

    // done
    return;
}

int log_update ( FILE *p_f, bool ansi_color )
{

    // argument check
    if ( NULL == p_f ) goto log_on_standard_out;

    // update the log file
    log_file = p_f;

    // success
    return 1;

    log_on_standard_out:
        
        // log to standard out
        log_file = stdout;
        
        // ANSI color flag
        log_with_ansi_color = ansi_color;
        
        // flush standard out
        fflush(stdout);

        // success
        return 1;
}

int log_error ( const char *const format, ... )
{

    // argument check
    if ( format == (void *) 0 ) goto no_format;

    // initialized data
    va_list list;

    // Use the varadic argument list in vprintf call
    va_start(list, format);

    // Uses ANSI terminal escapes to set the color to red
    if ( log_with_ansi_color ) printf("\033[91m");
    
    // Print the error
    vfprintf(log_file, format, list);

    // Restore the color.
    if ( log_with_ansi_color ) printf("\033[0m");

    // done with variadic list
    va_end(list);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_format:
                #ifndef NDEBUG
                    printf("[log] Null pointer provided for parameter \"format\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int log_warning ( const char *const format, ... )
{

    // argument check
    if ( format == (void *) 0 ) goto no_format;

    // initialized data
    va_list list;

    // Use the varadic argument list in vprintf call
    va_start(list, format);

    // Uses ANSI terminal escapes to set the color to yellow
    if ( log_with_ansi_color ) printf("\033[93m");
    
    // Print the warning
    vfprintf(log_file, format, list);

    // Restore the color.
    if ( log_with_ansi_color ) printf("\033[0m");

    // done with variadic list
    va_end(list);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_format:
                #ifndef NDEBUG
                    printf("[log] Null pointer provided for parameter \"format\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int log_info ( const char *const format, ... )
{

    // argument check
    if ( format == (void *) 0 ) goto no_format;

    // initialized data
    va_list list;

    // Use the varadic argument list in vprintf call
    va_start(list, format);

    // Uses ANSI terminal escapes to set the color to light blue,
    if ( log_with_ansi_color ) printf("\033[94m");
    
    // Print the info
    vfprintf(log_file, format, list);

    // Restore the color.
    if ( log_with_ansi_color ) printf("\033[0m");

    // done with variadic list
    va_end(list);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_format:
                #ifndef NDEBUG
                    printf("[log] Null pointer provided for parameter \"format\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int log_pass ( const char *const format, ... )
{

    // argument check
    if ( format == (void *) 0 ) goto no_format;

    // initialized data
    va_list list;

    // Use the varadic argument list in vprintf call
    va_start(list, format);

    // Uses ANSI terminal escapes to set the color to green
    if ( log_with_ansi_color ) printf("\033[42m\033[1m");

    fprintf(log_file, "[PASS]");

    // Restore the color.
    if ( log_with_ansi_color ) printf("\033[0m");

    // Uses ANSI terminal escapes to set the color to green
    if ( log_with_ansi_color ) printf(" \033[92m");
    
    // Print the info
    vfprintf(log_file, format, list);

    // Restore the color.
    if ( log_with_ansi_color ) printf("\033[0m");

    // done with variadic list
    va_end(list);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_format:
                #ifndef NDEBUG
                    printf("[log] Null pointer provided for parameter \"format\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int log_fail ( const char *const format, ... )
{

    // argument check
    if ( format == (void *) 0 ) goto no_format;

    // initialized data
    va_list list;

    // Use the varadic argument list in vprintf call
    va_start(list, format);

    // Uses ANSI terminal escapes to set the background color to red 
    if ( log_with_ansi_color ) printf("\033[41m\033[1m");

    fprintf(log_file, "[FAIL]");

    // Restore the color.
    if ( log_with_ansi_color ) printf("\033[0m");

    // Uses ANSI terminal escapes to set the foreground color to red
    if ( log_with_ansi_color ) printf(" \033[91m");

    // Print the info
    vfprintf(log_file, format, list);

    // Restore the color.
    if ( log_with_ansi_color ) printf("\033[0m");

    // done with variadic list
    va_end(list);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_format:
                #ifndef NDEBUG
                    printf("[log] Null pointer provided for parameter \"format\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int log_scenario ( const char *const format, ... )
{

    // argument check
    if ( format == (void *) 0 ) goto no_format;

    // initialized data
    va_list list;

    // Use the varadic argument list in vprintf call
    va_start(list, format);

    // Uses ANSI terminal escapes to set the color to cyan
    if ( log_with_ansi_color ) printf("\033[96m\033[1m\033[4m");

    fprintf(log_file, "Scenario: ");

    // Print the info
    vfprintf(log_file, format, list);

    // Restore the color.
    if ( log_with_ansi_color ) printf("\033[0m");

    // done with variadic list
    va_end(list);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_format:
                #ifndef NDEBUG
                    printf("[log] Null pointer provided for parameter \"format\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int log_colorful ( enum log_color_e color, const char *const format, ... )
{
    
    // argument check
    if ( format == (void *) 0 ) goto no_format;

    // initialized data
    va_list list;

    // Use the varadic argument list in vprintf call
    va_start(list, format);

    // Uses ANSI terminal escapes to set the color ,
    if ( log_with_ansi_color ) printf("\033[%dm", color);

    // Print the info
    vfprintf(log_file, format, list);

    // Restore the color.
    if ( log_with_ansi_color ) printf("\033[0m");

    // done with variadic list
    va_end(list);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_format:
                #ifndef NDEBUG
                    printf("[log] Null pointer provided for parameter \"format\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}


void log_exit ( void )
{

    // state check
    if ( initialized == false ) return;

    // Close the log file
    fclose(log_file);
    
    // Clear the initialized flag
    initialized = false;

    // done
    return;
}
