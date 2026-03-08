/** !
 * Example program for hash table module
 * 
 * @file src/examples/hash_table_example.c
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
    INDIGO         = 5,
    VIOLET         = 6,
    COLOR_QUANTITY = 7
};

// structure definitions
struct color_s
{
    char   _string[32];
    int    hex_code;
    size_t counter;
};

// type definitions
typedef struct color_s color;
typedef void *(fn_get_sample)(void);

// forward declarations
/// logs
int checkpoint ( hash_table *p_hash_table, const char *p_event );

/// color
fn_equality     color_equality;
fn_key_accessor color_key_accessor;
fn_hash64       color_hash;
fn_get_sample   color_get_sample;
fn_foreach      color_print;
fn_foreach      accumulate_probes;
fn_fori         color_slot_print;

// data
/// working hash table
hash_table *p_hash_table = NULL;

/// file for reflection
FILE *p_f = NULL;
size_t file_len = 0;

/// total probes
size_t total_probes = 0;

/// example data
color _values[] =
{
    [RED]    = { ._string = "Red"   , .hex_code = 0xFF0000 },
    [ORANGE] = { ._string = "Orange", .hex_code = 0xFFA500 },
    [YELLOW] = { ._string = "Yellow", .hex_code = 0xFFFF00 },
    [GREEN]  = { ._string = "Green" , .hex_code = 0x00FF00 },
    [BLUE]   = { ._string = "Blue"  , .hex_code = 0x0000FF },
    [INDIGO] = { ._string = "Indigo", .hex_code = 0x4B0082 },
    [VIOLET] = { ._string = "Purple", .hex_code = 0x7F00FF }
};

// entry point
int main ( int argc, const char* argv[] )
{

    // unused
    (void) argc;
    (void) argv;

    // #0 - start
    checkpoint(p_hash_table, "start");
    
    // #1 - construct
    {

        // construct the hash table
        hash_table_construct(
            &p_hash_table, 
            7,
            LINEAR_PROBE,
            
            color_equality, 
            color_key_accessor, 
            color_hash
        );

        // checkpoint
        checkpoint(p_hash_table, "after construction");
    }

    // #2 - add
    {
        
        // add some colors
        for (enum color_e _color = RED; _color < COLOR_QUANTITY; _color++)
            if ( 0 == hash_table_insert(p_hash_table, &_values[_color]) )
                log_error("Error: Failed to insert \"%s\"\n", _values[_color]._string);

        // checkpoint
        checkpoint(p_hash_table, "after adding < Red, Orange, Yellow, Green, Blue, Indigo, Purple>");
    }

    // #3 - search
    {

        // initialized data
        color *p_color = NULL;

        // search for a value
        hash_table_search(p_hash_table, "Green", &p_color);

        // print results
        printf("Searching for \"Green\" yields #%06x\n", p_color->hex_code);

        // checkpoint
        checkpoint(p_hash_table, "after search");
    }

    // #5 - destroy
    {
        
        // destroy the hash table
        hash_table_destroy(&p_hash_table, NULL);

        // checkpoint
        checkpoint(p_hash_table, "after destroy");
    }

    // #X - end
    checkpoint(p_hash_table, "end");
    
    // success
    return EXIT_SUCCESS;
}

int checkpoint ( hash_table *p_hash_table, const char *p_event )
{

    // static data
    static int step = 0;

    // print the array
    if ( NULL == p_hash_table )
        log_info("#%d - Hash table %s: NULL\n", step, p_event);
    else
        log_info("#%d - Hash table %s:\n", step, p_event),

        // summate probes
        total_probes = 0, hash_table_foreach(p_hash_table, accumulate_probes),

        // print load factor and average probes
        printf("α: %%%lf, avg. probes: %lf\n", 
            100.00 * hash_table_load_factor(p_hash_table),
            (double)total_probes / (double)COLOR_QUANTITY
        ),

        // print the contents of the hash table
        hash_table_fori(p_hash_table, color_slot_print),
        putchar('\n');

    // increment counter
    step++;

    // success
    return 1;
}

int color_equality ( const void *const p_a, const void *const p_b )
{

    // done
    return strcmp
    (
        ((color *)p_a)->_string,
        ((color *)p_b)->_string
    );
}

void color_print ( void *p_element )
{

    // initialized data
    color *p_color = p_element;

    // print the element
    printf("%d : ( %-7s -> #%06lx )\n", p_color->counter, p_color->_string, p_color->hex_code);

    // done
    return;
}

void accumulate_probes ( void *p_element )
{

    // initialized data
    color *p_color = p_element;

    // accumulate
    total_probes += p_color->counter;

    // done
    return;
}

hash64 color_hash ( const void *const k, unsigned long long unused )
{

    // initialized data
    char *p_key = k;

    // done
    return hash_fnv64(p_key, strlen(p_key));
}

void color_slot_print ( void *p_element, int i )
{
    
    // initialized data
    color *p_color = p_element;

    // fast fail
    if ( NULL == p_color ) 
    {
        printf("[%d] -> NULL\n", i);
        return;
    }

    // print the element
    printf("[%d] : ( %s -> #%06lx ), %d probes\n", i, p_color->_string, p_color->hex_code, p_color->counter);

    // done
    return;
}

void *color_key_accessor ( const void *const p_property )
{

    // initialized data
    color *p_color = p_property;

    // increment the probe counter
    p_color->counter++;

    // done
    return p_color->_string;
}
