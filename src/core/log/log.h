/** !
 * Include header for log library
 * 
 * @file src/core/log/log.h 
 * 
 * @author Jacob Smith
 */

// header guard
#pragma once

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

// enumeration definitions
enum log_color_e
{
    black   = 30,
    red     = 31,
    green   = 32,
    yellow  = 33,
    blue    = 34,
    magenta = 35,
    cyan    = 36,
    gray    = 37,
};

// function declarations
/// initializer
/** !
 * This gets called at runtime before main. 
 * By default, log to standard out with ANSI color coding 
 * 
 * @param void
 * 
 * @return void
 */
void log_init ( void ) __attribute__((constructor));

/// state
/** !
 *  Update the log file and color coding flag
 *
 * @param p_f        pointer to FILE
 * @param ansi_color color coded logs if true else plain
 *
 * @return 1 on success, 0 on error
 */
int log_update ( FILE *p_f, bool ansi_color );

/// logging
/** !
 *  Log an error
 *
 * @param format printf format parameter
 * @param ...    Additional arguments
 *
 * @return 1 on success, 0 on error
 */
int log_error ( const char *const format, ... );

/** !
 *  Log a warning
 *
 * @param format printf format parameter
 * @param ...    Additional arguments
 *
 * @return 1 on success, 0 on error
 */
int log_warning ( const char *const format, ... );

/** !
 *  Log some information
 *
 * @param format printf format parameter
 * @param ...    Additional arguments
 *
 * @return 1 on success, 0 on error
 */
int log_info ( const char *const format, ... );

/// tests
/** !
 *  Log a passing test
 *
 * @param format printf format parameter
 * @param ...    Additional arguments
 *
 * @return 1 on success, 0 on error
 */
int log_pass ( const char *const format, ... );

/** !
 *  Log a failing test
 *
 * @param format printf format parameter
 * @param ...    Additional arguments
 *
 * @return 1 on success, 0 on error
 */
int log_fail ( const char *const format, ... );

/** !
 *  Log a test scenario
 *
 * @param format printf format parameter
 * @param ...    Additional arguments
 *
 * @return 1 on success, 0 on error
 */
int log_scenario ( const char *const format, ... );

/// general
/** !
 *  Log with user defined colors
 *
 * @param format printf format parameter
 * @param ...    Additional arguments
 *
 * @return 1 on success, 0 on error
 */
int log_colorful ( enum log_color_e color, const char *const format, ... );
