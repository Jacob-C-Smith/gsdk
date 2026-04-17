/** !
 * Double ended queue example program
 * 
 * @file src/examples/double_queue_example.c
 * 
 * @author Jacob Smith
 */

// standard library
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// gsdk
/// core
#include <core/log.h>
#include <core/sync.h>
#include <core/pack.h>
#include <core/hash.h>
#include <core/interfaces.h>

/// data
#include <data/double_queue.h>

// enumeration definitions
enum shapes_e
{
    CIRCLE         = 0,
    SQUARE         = 1,
    TRIANGLE       = 2,
    RECTANGLE      = 3,
    PENTAGON       = 4,
    HEXAGON        = 5,
	SHAPE_QUANTITY = 6
};

// forward declarations
/// logs
int checkpoint ( double_queue *p_double_queue, const char *p_event );

// string
fn_comparator string_compare;
fn_fori string_print;
fn_hash64 string_hash;
fn_map string_upper_case;
fn_map string_lower_case;
fn_pack string_pack;
fn_unpack string_unpack;

// data
/// immutable piece strings
const char *_p_shapes[SHAPE_QUANTITY] =
{
    [CIRCLE]    = "Circle",
	[SQUARE]    = "Square",
	[TRIANGLE]  = "Triangle",
	[RECTANGLE] = "Rectangle",
	[PENTAGON]  = "Pentagon",
	[HEXAGON]   = "Hexagon",
};

/// file for reflection
FILE *p_f = NULL;
size_t file_len = 0;

/// hashes
hash64 h1 = 0,
       h2 = 0;

/// working cache
double_queue *p_double_queue = NULL;

// entry point
int main ( int argc, const char* argv[] )
{

    // unused
    (void) argc;
    (void) argv;

    // #0 - start
    checkpoint(p_double_queue, "start");
    
    // #1 - initial
    {
        
        // construct the cache
        double_queue_construct(&p_double_queue);
        
        // checkpoint
        checkpoint(p_double_queue, "after construction");
    }
        
    // #2 - insert 
    {
        
        // push some shapes
        for (enum shapes_e _shape = CIRCLE; _shape < PENTAGON; _shape++)
            double_queue_rear_add(p_double_queue, (void *) _p_shapes[_shape]);
        
        // checkpoint
        checkpoint(p_double_queue,"after adding < Circle, Square, Triangle, Rectangle >");
    }
        
    // #3 - peek front
    {
        
        // initialized data
        void *p_value = NULL;
        
        // peek the last element
        double_queue_front(p_double_queue, &p_value);
        
        // print the top of the cache
        printf("peek() -> %s\n", (char *)p_value);

        // checkpoint
        checkpoint(p_double_queue, "after peek front");
    }

    // #4 - peek rear
    {
        
        // initialized data
        void *p_value = NULL;
        
        // peek the last element
        double_queue_rear(p_double_queue, &p_value);
        
        // print the top of the cache
        printf("peek() -> %s\n", (char *) p_value);

        // checkpoint
        checkpoint(p_double_queue, "after peek rear");
    }

    // #5 - to binary
    {

        // initialized data
        char buf[1024] = { 0 };
        
        // open a file for writing
        p_f = fopen("resources/reflection/double_queue.bin", "wb");

        // reflect the double ended queue to a buffer
        file_len = double_queue_pack(buf, p_double_queue, string_pack),
        
        // write the buffer to a file
        fwrite(buf, file_len, 1, p_f),

        // close the file
        fclose(p_f),

        // checkpoint
        checkpoint(p_double_queue, "after serialize");
    }
    
    // #6 - hash 1
    {

        // initialized data
        h1 = double_queue_hash(p_double_queue, (fn_hash64 *)string_hash);

        // print the hash
        printf("hash 1 -> 0x%llx\n", h1);

        // checkpoint
        checkpoint(p_double_queue, "after hash 1");
    }

    // #7 - remove front and rear
    {

        // pop the first and last elements
        double_queue_front_remove(p_double_queue, NULL),
        double_queue_rear_remove(p_double_queue, NULL);

        // checkpoint
        checkpoint(p_double_queue, "after removing < Circle, Rectangle >");
    }

    // #8 - add front and rear 
    {

        // push some pieces
        double_queue_front_add(p_double_queue, (void *) _p_shapes[PENTAGON]),
        double_queue_rear_add(p_double_queue, (void *) _p_shapes[HEXAGON]);

        // checkpoint
        checkpoint(p_double_queue, "after adding < Pentagon, Hexagon >");
    }

    // #9 - map
    {

        // map to upper case
        double_queue_map(p_double_queue, string_upper_case, NULL);

        // checkpoint
        checkpoint(p_double_queue, "after map upper case");
    }

    // #10 - destroy 
    {

        // destroy the double ended queue
        double_queue_destroy(&p_double_queue, default_allocator);

        // checkpoint
        checkpoint(p_double_queue, "after destroy");
    }
    
    
    // #11 - from binary
    {

        // initialized data
        char buf[1024] = { 0 };
        
        // read a buffer from a file
        p_f = fopen("resources/reflection/double_queue.bin", "rb"),
        fread(buf, file_len, 1, p_f),
        fclose(p_f);

        // reflect a double ended queue from the buffer
        double_queue_unpack(&p_double_queue, buf, string_unpack),

        // checkpoint
        checkpoint(p_double_queue, "after parse");
    }

    // #12 - hash 2
    {

        // initialized data
        h2 = double_queue_hash(p_double_queue, (fn_hash64 *)string_hash);

        // print the hash
        printf("hash 2 -> 0x%llx\n", h2);

        // error check
        if ( h1 != h2 ) log_error("Error: hash 1 != hash 2\n");

        // checkpoint
        checkpoint(p_double_queue, "after hash 2");
    }

    // #13 - destroy
    {

        // destroy the cache
        double_queue_destroy(&p_double_queue, default_allocator);

        // checkpoint
        checkpoint(p_double_queue, "after destroy");
    }

    // #14 - end
    checkpoint(p_double_queue, "end");
   
    // success
    return EXIT_SUCCESS;
}

int checkpoint ( double_queue *p_double_queue, const char *p_event )
{
    
    // static data
    static int step = 0;
     
    // print the cache
    if ( NULL == p_double_queue )
		log_info("#%d - Double queue %s: ", step, p_event),
		printf("NULL\n");
    else
		log_info("#%d - Double queue %s:\n", step, p_event),
		double_queue_fori(p_double_queue, string_print),
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
    printf("%d -> %s\n", i, (char *)p_value);
    
    // done
    return ;
}

int string_compare ( const void *const p_a, const void *const p_b )
{
    char *a = *(char **)p_a,
         *b = *(char **)p_b;

    return strcmp(a, b);
}

int string_pack ( void *p_buffer, const void *const p_value )
{

    // done
    return pack_pack(p_buffer, "%s", p_value);
}

int string_unpack ( void *const p_value, void *p_buffer )
{

    // initialized data
    char       **pp_value        = (char **) p_value;
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

hash64 string_hash ( const void *const string, unsigned long long unused )
{

    // unused
    (void)unused;

    // done
    return hash_crc64(string, strlen(string));
}
