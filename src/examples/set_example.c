/** ! 
 * Example program for set module 
 * 
 * @file src/examples/set_example.c
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
#include <core/pack.h>

// data
#include <data/set.h>

// enumeration definitions
enum planet_e
{
    MERCURY         = 0,
    VENUS           = 1,
    EARTH           = 2,
    MARS            = 3,
    JUPITER         = 4,
    SATURN          = 5,
    URANUS          = 6,
    NEPTUNE         = 7,
    PLANET_QUANTITY = 8
};

// forward declarations
/// logs
int checkpoint ( set *p_set, const char *name, const char *p_event );

/// string
void    string_print(void *p_string, size_t unused);
int     string_compare ( const void *const p_a, const void *const p_b );
void   *string_upper_case ( void *p_value );
void   *string_lower_case ( void *p_value );
hash64  string_hash ( const void *const string, unsigned long long unused );
int     string_pack ( void *p_buffer, const void *const p_value );
int     string_unpack ( void *const p_value, void *p_buffer );
 
// data
/// immutable planet strings
const char *_p_planets[PLANET_QUANTITY] =
{
    [MERCURY] = "Mercury",
    [VENUS]   = "Venus",
    [EARTH]   = "Earth",
    [MARS]    = "Mars",
    [JUPITER] = "Jupiter",
    [SATURN]  = "Saturn",
    [URANUS]  = "Uranus",
    [NEPTUNE] = "Neptune"
};

/// file for reflection
FILE   *p_f      = NULL;
size_t  file_len = 0;

/// hashes
hash64 h1 = 0,
       h2 = 0,
       h3 = 0;

/// working sets
set *p_a            = NULL,
    *p_b            = NULL,
    *p_union        = NULL,
    *p_difference   = NULL,
    *p_intersection = NULL;

// entry point
int main ( int argc, const char* argv[] )
{
    // unused
    (void) argc;
    (void) argv;

    // #0 - start
    checkpoint(p_a, "set", "start");
    
    // #1 - construct A
    {

        // construct A
        set_construct(&p_a, 8, string_compare);
        
        // checkpoint
        checkpoint(p_a, "A", "after construction");
    }

    // #2 - construct B
    {

        // construct B
        set_construct(&p_b, 8, string_compare);
        
        // checkpoint
        checkpoint(p_b, "B", "after construction");
    }

    // #3 - add inner planets to set A
    {

        // add some planets
        for (enum planet_e _planet = MERCURY; _planet < MARS; _planet++)
            set_add(p_a, (void *)_p_planets[_planet]);

        // checkpoint
        checkpoint(p_a, "A", "after adding 3 planets");
    }

    // #4 - add outer planets to set B
    {

        // add some planets
        for (enum planet_e _planet = MARS; _planet < PLANET_QUANTITY; _planet++)
            set_add(p_b, (void *)_p_planets[_planet]);

        // checkpoint
        checkpoint(p_b, "B", "after adding 4 planets");
    }

    // #5 - add mars to inner planets
    {

        // add Mars
        set_add(p_a, (void *)_p_planets[MARS]);

        // checkpoint
        checkpoint(p_a, "A", "after adding 'Mars'");
    }

    // #6 - union
    {

        // compute the union of sets A and B
        set_union(&p_union, p_a, p_b);

        // checkpoint
        checkpoint(p_union, "A ∪ B", "after union");
    }
    
    // #7 - intersection
    {

        // compute the intersection of sets A and B
        set_intersection(&p_intersection, p_a, p_b);

        // checkpoint
        checkpoint(p_intersection, "A ∩ B", "after intersection");
    }

    // #8 - difference
    {

        // compute the difference of sets A and B
        set_difference(&p_difference, p_a, p_b);

        // checkpoint
        checkpoint(p_difference, "A Δ B", "after difference");
    }

    // #9 - to binary
    {

        // initialized data
        char buf[1024] = { 0 };
        
        // Open a file for writing
        p_f = fopen("resources/reflection/set.bin", "wb");

        // reflect the set to a buffer
        file_len = set_pack(buf, p_difference, string_pack),
        
        // write the buffer to a file
        fwrite(buf, file_len, 1, p_f),

        // close the file
        fclose(p_f);

        // checkpoint
        checkpoint(p_difference, "A Δ B", "after serialize");
    }

    // #10 - hash 1
    {

        // initialized data
        h1 = set_hash(p_difference, (fn_hash64 *)string_hash);

        // print the hash
        printf("hash 1 -> 0x%llx\n", h1);

        // checkpoint
        checkpoint(p_difference, "A Δ B", "after hash 1");
    }

    // #11 - destroy
    {

        // destroy
        set_destroy(&p_difference);

        // checkpoint
        checkpoint(p_difference, "A Δ B", "after destroy");
    }

    // #12 - remove
    {

        // remove Mars from solar system
        set_remove(p_union, (void *)_p_planets[MARS]);

        // checkpoint
        checkpoint(p_union, "A ∪ B", "after removing 'Mars'");
    }

    // #13 - hash 2
    {

        // hash the array
        h2 = set_hash(p_union, (fn_hash64 *)string_hash);

        // print the hash
        printf("hash 2 -> 0x%llx\n", h2);

        // error check
        if ( h1 != h2 ) 

            // abort
            log_error("Error: hash 1 != hash 2\n"), exit(EXIT_FAILURE);

        // checkpoint
        checkpoint(p_union, "A ∪ B", "after hash 2");
    }

    // #14 - pop
    {

        // initialized data
        void *p_popped = NULL;

        // remove an outer planet
        set_pop(p_b, &p_popped);
        if (p_popped) {
            printf("Popped value: %s\n", (char *)p_popped);
        }

        // checkpoint
        checkpoint(p_b, "B", "after pop");
    }
    
    // #15 - from binary
    {
        
        // initialized data
        char buf[1024] = { 0 };
        
        // read a buffer from a file
        p_f = fopen("resources/reflection/set.bin", "rb"),
        fread(buf, sizeof(char), file_len, p_f),
        
        // reflect an array from the buffer
        set_unpack(&p_difference, buf, string_unpack, string_compare),

        // checkpoint
        checkpoint(p_difference, "A Δ B", "after parse");
    }

    // #16 - hash 3
    {

        // hash the array
        h3 = set_hash(p_difference, (fn_hash64 *)string_hash);

        // print the hash
        printf("hash 3 -> 0x%llx\n", h3);

        // error check
        if ( h1 != h3 ) 

            // abort
            log_error("Error: hash 1 != hash 3\n"), exit(EXIT_FAILURE);

        // checkpoint
        checkpoint(p_difference, "A Δ B", "after hash 3");
    }

    // #17 - destroy A
    {

        // destroy
        set_destroy(&p_a);
        
        // checkpoint
        checkpoint(p_a, "A", "after destroy");
    }

    // #18 - destroy B
    {

        // destroy
        set_destroy(&p_b);
        
        // checkpoint
        checkpoint(p_b, "B", "after destroy");
    }

    // #19 - destroy union
    {

        // destroy
        set_destroy(&p_union);

        // checkpoint
        checkpoint(p_union, "A ∪ B", "after destroy");
    }

    // #20 - destroy intersection
    {

        // destroy
        set_destroy(&p_intersection);

        // checkpoint
        checkpoint(p_intersection, "A ∩ B", "after destroy");
    }
    
    // #21 - destroy difference
    {

        // destroy
        set_destroy(&p_difference);

        // checkpoint
        checkpoint(p_difference, "A Δ B", "after destroy");
    }

    // #22 - end
    checkpoint(p_a, "set", "end");
    
    // success
    return EXIT_SUCCESS;
}

int checkpoint ( set *p_set, const char *name, const char *p_event )
{

    // static data
    static int step = 0;

    // print the array
    if (p_set == NULL)
        log_info("#%d - %s %s: ", step, name, p_event),
        printf("NULL\n");
    else
        log_info("#%d - %s %s:\n", step, name, p_event),
        set_foreach_i(p_set, string_print),
        putchar('\n');

    // increment counter
    step++;

    // success
    return 1;
}

void string_print ( void *const p_string, size_t unused )
{

    // unused
    (void) unused;

    // log
    printf("%s\n", (char *)p_string);
}

void *string_upper_case ( void *p_value )
{

    // initialized data
    char *p_string = strdup((char *)p_value),
         *p_result = p_string;
         
    // iterate upto null terminator
    while (*p_string)
    {

        // initialized data
        char c = *p_string;

        // convert offending characters
        if ( islower(c) ) c = toupper(c);
        
        // store the upper case character
        *p_string++ = c;
    }

    // success
    return p_result;
}

void *string_lower_case ( void *p_value )
{

    // initialized data
    char *p_string = strdup((char *)p_value),
         *p_result = p_string;
         
    // iterate upto null terminator
    while (*p_string)
    {

        // initialized data
        char c = *p_string;

        // convert offending characters
        if ( isupper(c) ) c = tolower(c);
        
        // store the lower case character
        *p_string++ = c;
    }

    // success
    return p_result;
}

int string_compare ( const void *const p_a, const void *const p_b )
{
    // done
    return strcmp(p_a, p_b);
}

hash64 string_hash ( const void *const string, unsigned long long unused )
{

    // unused
    (void)unused;

    // done
    return hash_crc64(string, strlen(string));
}

int string_pack ( void *p_buffer, const void *const p_value )
{

    // done
    return pack_pack(p_buffer, "%s", p_value);
}

int string_unpack ( void *const p_value, void *p_buffer )
{

    // done
    return pack_unpack(p_buffer, "%s", p_value);
}
