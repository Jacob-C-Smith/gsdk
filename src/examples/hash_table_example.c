/** !
* Example program for hash_table module
 * 
 * @file hash_table_example.c
 * 
 * @author Jacob Smith
 */

// standard library
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// core
#include <core/log.h>
#include <core/sync.h>
#include <core/hash.h>

// data
#include <data/hash_table.h>

// enumeration definitions
enum color_e
{
    RED            = 0,
    ORANGE         = 1,
    YELLOW         = 2,
    GREEN          = 3,
    BLUE           = 4,
    PURPLE         = 5,
    COLOR_QUANTITY = 6
};

// forward declarations
/// logs
int checkpoint ( hash_table *p_hash_table, const char *p_event );

/// string
int string_equality ( const void *const p_a, const void *const p_b );
void *string_key_accessor ( void *p_property );
hash64 string_hash(const void *const string, size_t table_size);
 
// data
/// immutable color strings
const char *_p_colors[COLOR_QUANTITY] =
{
    [RED]    = "Red",
    [ORANGE] = "Orange",
    [YELLOW] = "Yellow",
    [GREEN]  = "Green",
    [BLUE]   = "Blue",
    [PURPLE] = "Purple"
};

/// working hash_table
hash_table *p_hash_table = NULL;

// entry point
int main ( int argc, const char* argv[] )
{

    // unused
    (void) argc;
    (void) argv;

    // #0 - start
    checkpoint(p_hash_table, "start");
    
    // #1 - initial
    {

        // construct the hash_table
        hash_table_construct(
            &p_hash_table, 
            16, 
            
            (fn_equality *)     string_equality, 
            (fn_key_accessor *) string_key_accessor, 
            (fn_hash64 *)       string_hash
        );

        // checkpoint
        checkpoint(p_hash_table, "after construction");
    }

    // #2 - add 
    {

        // add some colors
        for (enum color_e _color = RED; _color < COLOR_QUANTITY; _color++)
            hash_table_insert(p_hash_table, (void *)_p_colors[_color]);

        // checkpoint
        checkpoint(p_hash_table, "after adding colors");
    }

    // #3 - search
    {
        void *p_value = NULL;
        const char *p_key = "Blue";

        log_info("Searching for key \"%s\"...", p_key);
        if (hash_table_search(p_hash_table, (void *)p_key, &p_value) && p_value)
            log_info("Found: %s\n", (char *)p_value);
        else
            log_error("Could not find key \"%s\"\n", p_key);

    }

    // #4 - destroy
    {

        // destroy the hash_table
        hash_table_destroy(&p_hash_table);

        // checkpoint
        checkpoint(p_hash_table, "after destroy");
    }

    // #5 - end
    checkpoint(p_hash_table, "end");
    
    // success
    return EXIT_SUCCESS;
}

int checkpoint ( hash_table *p_hash_table, const char *p_event )
{
    // static data
    static int step = 0;

    // print the hash_table
    if ( NULL == p_hash_table )
    {
        log_info("#%d - Hash Table %s: ", step, p_event);
        printf("NULL\n");
    }
    else
    {
        log_info("#%d - Hash Table %s:\n", step, p_event);
        printf("  count: %zu, max: %zu, length: %zu\n", p_hash_table->properties.count, p_hash_table->properties.max, p_hash_table->properties.length);
        printf("  elements:\n");
        for (size_t i = 0; i < p_hash_table->properties.max; i++)
        {
            if (p_hash_table->properties.pp_data[i] != NULL)
            {
                printf("    [%zu]: %s\n", i, (char *)p_hash_table->properties.pp_data[i]);
            }
        }
        putchar('\n');
    }

    // increment counter
    step++;

    // success
    return 1;
}

int string_equality ( const void *const p_a, const void *const p_b )
{
    return strcmp((const char *)p_a, (const char *)p_b);
}

void *string_key_accessor ( void *p_property )
{
    return p_property;
}

hash64 string_hash(const void *const string, size_t table_size)
{
    return hash_crc64(string, strlen(string)) % table_size;
}
