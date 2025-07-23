/** !
 * Code generator
 * 
 * @file code_gen.c 
 * 
 * @author Jacob Smith
 */

// header file 
#include <code_gen/code_gen.h>

// Data
static bool initialized = false;

void code_gen_init ( void ) 
{

    // State check
    if ( initialized == true ) return;

    // Initialize the log library
    log_init();

    // Set the initialized flag
    initialized = true;

    // done
    return;
}

void code_gen_exit ( void ) 
{

    // State check
    if ( initialized == false ) return;

    // Clean up log
    log_exit();

    // Clear the initialized flag
    initialized = false;

    // done
    return;
}
