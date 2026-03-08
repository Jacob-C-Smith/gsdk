/** !
 * Example program for cache module
 * 
 * @file src/examples/cache_example.c
 * 
 * @author Jacob Smith
 */

// standard library
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// gsdk
/// core
#include <core/hash.h>
#include <core/interfaces.h>
#include <core/log.h>
#include <core/pack.h>
#include <core/sync.h>

/// data
#include <data/cache.h>

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

// forward declarations
/// logs
int checkpoint ( cache *p_cache, const char *p_event );

// string
fn_equality string_equality;
fn_fori     string_print;
fn_hash64   string_hash;
fn_map      string_upper_case;
fn_map      string_lower_case;
fn_pack     string_pack;
fn_unpack   string_unpack;

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
    [VIOLET] = "Violet"
};

/// file for reflection
FILE *p_f = NULL;

/// hashes
hash64 h1 = 0,
       h2 = 0;

/// working cache
cache *p_cache = NULL;
size_t file_len = 0;

// entry point
int main ( int argc, const char* argv[] )
{

    // unused
    (void) argc;
    (void) argv;

    // #0 - start
    checkpoint(p_cache, "start");
    
    // #1 - initial
    {
        
        // construct the cache
        cache_construct(
            &p_cache,
            5,

            string_equality,
            NULL,
            NULL
        );
        
        // checkpoint
        checkpoint(p_cache, "after construction");
    }
        
    // #2 - insert 
    {
        
        // add some colors
        for (enum color_e _color = RED; _color < COLOR_QUANTITY; _color++)
            cache_insert(p_cache, _p_colors[_color], NULL);
        
        // checkpoint
        checkpoint(p_cache, "after adding < Red, Orange, Yellow, Green, Blue, Indigo, Violet >");
    }
        
    // #3 - find
    {
        
        // initialized data
        void *p_value = NULL;
        
        // find yellow
        cache_find(p_cache, "Blue", &p_value);

        // checkpoint
        checkpoint(p_cache, "after find \"Blue\"");
    }

    // #4 - more inserts
    {
        
        // add some colors
        for (enum color_e _color = RED; _color < GREEN; _color++)
            cache_insert(p_cache, _p_colors[_color], NULL);
        
        // checkpoint
        checkpoint(p_cache, "after adding < Red, Orange, Yellow >");
    }

    // #5 - find (miss)
    {
        
        // initialized data
        void *p_value = NULL;
        
        // find some colors
        for (enum color_e _color = BLUE; RED < _color; --_color)
            if ( 0 == cache_find(p_cache, _p_colors[_color], &p_value) )
                log_error("\"%s\" not found\n", _p_colors[_color]);

        // checkpoint
        checkpoint(p_cache, "after find < Blue -> Green -> Yellow -> ... >");
    }

    // #6 - peek
    {

        // initialized data
        void *p_value = NULL;
        
        // peek violet
        cache_peek(p_cache, "Violet", &p_value);

        // log the value
        printf("peek(\"Violet\") -> %s\n", (char *)p_value);

        // checkpoint
        checkpoint(p_cache, "after peek \"Violet\"");
    }

    // #7 - remove some
    {
        
        // remove some colors
        for (int i = 1; i < COLOR_QUANTITY; i += 3)
            cache_remove(p_cache, _p_colors[i], NULL);

        // checkpoint
        checkpoint(p_cache, "after removing < Orange, Blue >");
    }

    // #8 - to binary
    {

        // initialized data
        char buf[1024] = { 0 };
        
        // Open a file for writing
        p_f = fopen("resources/reflection/cache.bin", "wb");

        // reflect the cache to a buffer
        file_len = cache_pack(buf, p_cache, string_pack),
        
        // write the buffer to a file
        fwrite(buf, file_len, 1, p_f),

        // close the file
        fclose(p_f);

        // checkpoint
        checkpoint(p_cache, "after serialize");
    }

    // #9 - hash 1
    {

        // compute the hash
        h1 = cache_hash(p_cache, string_hash);

        // print the hash
        printf("hash 1 -> 0x%llx\n", h1);

        // checkpoint
        checkpoint(p_cache, "after hash 1");
    }

    // #10 - destroy
    {

        // destroy the cache
        cache_destroy(&p_cache);

        // checkpoint
        checkpoint(p_cache, "after destroy");
    }

    // #11 - from binary
    {
        
        // initialized data
        char buf[1024] = { 0 };
        
        // read a buffer from a file
        p_f = fopen("resources/reflection/cache.bin", "rb"),
        fread(buf, file_len, 1, p_f),
        
        // reflect a cache from the buffer
        cache_unpack(
            &p_cache,
            buf,
            string_unpack,
            
            string_equality,
            NULL,
            default_allocator
        ),

        // close the file
        fclose(p_f);

        // checkpoint
        checkpoint(p_cache, "after parse");
    }
    
    // #12 - hash 2
    {

        // compute the hash
        h2 = cache_hash(p_cache, string_hash);

        // print the hash
        printf("hash 2 -> 0x%llx\n", h2);

        // error check
        if ( h1 != h2 ) 

            // abort
            log_error("Error: hash 1 != hash 2\n"), exit(EXIT_FAILURE);

        // checkpoint
        checkpoint(p_cache, "after hash 2");
    }

    // #13 - find
    {
        
        // initialized data
        void *p_value = NULL;
        
        // find yellow
        cache_find(p_cache, "Blue", &p_value);

        // checkpoint
        checkpoint(p_cache, "after find \"Blue\"");
    }

    // #14 - map upper case
    {

        // convert the cache elements to upper case
        cache_map(p_cache, string_upper_case, default_allocator);

        // checkpoint
        checkpoint(p_cache, "after upper case map");
    }

    // #15 - insert 
    {
        
        // add some colors
        for (enum color_e _color = RED; _color < COLOR_QUANTITY; _color++)
            cache_insert(p_cache, strdup(_p_colors[_color]), default_allocator);
        
        // checkpoint
        checkpoint(p_cache, "after adding < Red, Orange, Yellow, ... >");
    }

    // #16 - destroy
    {

        // destroy the cache
        cache_destroy(&p_cache);

        // checkpoint
        checkpoint(p_cache, "after destroy");
    }

    // #17 - end
    checkpoint(p_cache, "end");
   
    // success
    return EXIT_SUCCESS;
}

int checkpoint ( cache *p_cache, const char *p_event )
{
    
    // static data
    static int step = 0;
    
    // print the cache
    if ( NULL == p_cache )
        log_info("#%d - Cache %s: ", step, p_event),
        printf("NULL\n");
    else
        log_info("#%d - Cache %s:\n", step, p_event),
        cache_fori(p_cache, string_print),
        putchar('\n');
    
    // increment counter
    step++;
    
    // success
    return 1;
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

void string_print ( void *p_value, int i )
{
    
    // print the element
    printf("[%d] - %s\n", i, (char *)p_value);
    
    // done
    return ;
}

int string_equality ( const void *const p_a, const void *const p_b )
{

    // initialized data
    char *a = (char *)p_a,
         *b = (char *)p_b;

    // done
    return ( 0 == strcmp(a, b) );
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

    // initialized data
    char       **pp_value        = (const char **) p_value;
    int          result          = 0;
    char        *p_string        = NULL;
    const char   _string  [1024] = { 0 };

    // unpack the buffer
    result = pack_unpack(p_buffer, "%s", &_string);

    // duplicate the string
    p_string = strdup(_string);

    // return a pointer to the caller
    *pp_value = p_string;

    // done
    return result;
}
