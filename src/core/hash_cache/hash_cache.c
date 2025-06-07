/** !
 * Hashing functions and caching abstractions
 * 
 * @file hash_cache.c 
 * 
 * @author Jacob Smith
 */

// Header
#include <core/hash_cache.h>

// Data
static bool initialized = false;
unsigned long long crc64_table[256] = { 0 };

void hash_cache_init ( void )
{

    // State check
    if ( initialized == true ) return;

    // Initialize the log library
    log_init();

    // Initialize the sync library
    sync_init();

    // Initialize the CRC64 table
    for (unsigned long long i = 0; i < 256; i++)
    {
        
        // Initialized data
        unsigned long long crc = i;

        for (size_t j = 0; j < 8; j++)

            if ( crc & 1 ) crc = (crc >> 1) ^ 0xC96C5795D7870F42;
            else           crc >>= 1;
        
        // Store the value
        crc64_table[i] = crc;        
    }
    
    // Set the initialized flag
    initialized = true;

    // Done
    return;
}

int hash_cache_equals ( const void *const p_a, const void *p_b )
{

    // Done
    return ( p_a == p_b ) ? 0 : 1;
}

void *hash_cache_key_accessor ( const void *const p_value )
{

    // Done
    return (void *)p_value;
}

void hash_cache_exit ( void )
{

    // State check
    if ( initialized == false ) return;

    // Clean up the log library
    log_exit();

    // Clean up the sync library
    sync_exit();

    // Clear the initialized flag
    initialized = false;

    // Done
    return;
}
