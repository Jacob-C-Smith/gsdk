/** !
* Example program for hash_table module
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
    PURPLE         = 6,
    COLOR_QUANTITY = 7
};

enum random_source_e
{
    NUMBERS         = 0,
    DATES           = 1,
    WORDS           = 2,
    RANDOM_QUANTITY = 3
};

// structure definitions
struct string_counter_s
{
    char   _string[32];
    size_t counter;
};

// type definitions
typedef struct string_counter_s string_counter;
typedef void *(fn_get_sample)(void);

// forward declarations
/// logs
int checkpoint ( hash_table *p_hash_table, const char *p_event );

/// string
int string_equality ( const void *const p_a, const void *const p_b );
void *string_key_accessor ( void *p_property );
hash64 string_hash ( const void *const string, size_t i );
void *string_get_sample ( void );

// number
int number_equality ( const void *const p_a, const void *const p_b );
void *number_key_accessor ( void *p_property );
hash64 number_hash (const void *const string, size_t i );

// data
/// immutable color strings
const char *_p_colors[COLOR_QUANTITY] =
{
    [RED]    = "Red",
    [ORANGE] = "Orange",
    [YELLOW] = "Yellow",
    [GREEN]  = "Green",
    [BLUE]   = "Blue",
    [INDIGO] = "Indigo",
    [PURPLE] = "Purple"
};

struct 
{
    char *name;
    fn_equality *pfn_equality;
    fn_key_accessor *pfn_key_accessor;
    fn_hash64 *pfn_hash64;
    fn_get_sample *pfn_get_sample;
}
_sources[RANDOM_QUANTITY] = 
{
    [NUMBERS] = 
    {
        .name = "numbers",
        // .pfn_equality = number_equality,
        // .pfn_key_accessor = number_key_accessor,
        // .pfn_hash64 = number_hash
    },
    [DATES] = { 0 },
    [WORDS] = 
    {
        .name = "words",
        .pfn_equality     = (fn_equality *)     string_equality,
        .pfn_key_accessor = (fn_key_accessor *) string_key_accessor,
        .pfn_hash64       = (fn_hash64 *)       string_hash,
        .pfn_get_sample   = (fn_get_sample *)   string_get_sample,
    }
};

/// working hash table
hash_table *p_hash_table = NULL;
enum random_source_e _random = WORDS;

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

        // construct the hash table
        hash_table_construct(
            &p_hash_table, 
            128549, // 8863, 22157, 54499,
            LINEAR_PROBE,
            
            _sources[_random].pfn_equality, 
            _sources[_random].pfn_key_accessor, 
            _sources[_random].pfn_hash64
        );

        // checkpoint
        checkpoint(p_hash_table, "after construction");
    }

    // #2 - add
    {
        for (size_t i = 0;; i++)
        {   
            string_counter *s = string_get_sample();
            void *p_result = NULL;

            if (s == NULL) break;

            hash_table_search(p_hash_table, s->_string, &p_result);

            if ( p_result == NULL ) 
                hash_table_insert(p_hash_table, s);
            else
                free(s);
        }

        // checkpoint
        checkpoint(p_hash_table, "after alice");
    }

    // #2 - add 
    {

        // add some colors
        // for (enum color_e _color = RED; _color < COLOR_QUANTITY; _color++)
        //     hash_table_insert(p_hash_table, (void *)_p_colors[_color]),
        //     checkpoint(p_hash_table, (void *)_p_colors[_color]);

        // // checkpoint
        // checkpoint(p_hash_table, "after adding colors");
    }

    // #3 - search
    {
        void *p_value = NULL;
        const char *p_key = "Green";

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
        
        for (size_t i = 0; i < p_hash_table->properties.max; i++)
        {
            if (p_hash_table->properties.pp_data[i] != NULL)
            {
                printf(
                    "%d %s\n",
                    // "    [%zu]: %d : %s\n",
                    // i, 
                    ((string_counter *)p_hash_table->properties.pp_data[i])->counter + 1,
                    ((string_counter *)p_hash_table->properties.pp_data[i])->_string
                );
            }
        }
        putchar('\n');
        printf(
            "  α: %%%5.2lf, count: %zu, max: %zu\n",

            100 * (double) p_hash_table->properties.count / (double) p_hash_table->properties.max,
            p_hash_table->properties.count,
            p_hash_table->properties.max
        );
        printf("  elements:\n");
    }

    // increment counter
    step++;

    // success
    return 1;
}

int string_equality ( const void *const p_a, const void *const p_b )
{

    // done
    return strcmp
    (
        ((string_counter *)p_a)->_string,
        ((string_counter *)p_b)->_string
    );
}

void *string_key_accessor ( void *p_property )
{

    // initialized data
    string_counter *p_string_counter = p_property;

    // increment the counter
    p_string_counter->counter++;

    // done
    return p_string_counter->_string;
}

hash64 string_hash(const void *const string, size_t i)
{
    return i + hash_crc64(string, strlen(string));
}

void *string_get_sample ( void )
{
    static FILE *p_f = NULL;

    string_counter *s = malloc(sizeof(string_counter));

    if ( p_f == NULL )
        p_f = fopen("include/test/word-list.txt", "r");
    
    if ( feof(p_f) ) return NULL;
    fscanf(p_f, "%[^\n] ", s->_string);

    s->_string;
    s->counter = 0;

    return s;
}