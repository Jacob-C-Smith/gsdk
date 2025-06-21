/** !
 * Implementation of functions in main tree header
 * 
 * @file tree.c
 * 
 * @author Jacob Smith
 */

#include <data/tree.h>

int tree_init ( void )
{

    // success
    return 1;
}

int tree_compare_function ( const void *const a, const void *const b )
{
    
    // Return
    return ( a == b ) ? 0 : ( a < b ) ? 1 : -1;
}

const void *tree_key_is_value ( const void *p_value )
{

    // success
    return p_value;
}