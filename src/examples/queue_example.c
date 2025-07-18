/** !
 * Example program for queue module
 * 
 * @file src/examples/queue_example.c
 * 
 * @author Jacob Smith
 */

// standard library
#include <stdio.h>
#include <stdlib.h>

// core
#include <core/log.h>
#include <core/hash.h>
#include <core/pack.h>

// data
#include <data/queue.h>

// enumeration definitions
enum fruit_type_e
{
    APPLE,
    BANANA,
    ORANGE,
    LEMON,
    GRAPE,
    PEACH,
    FRUIT_QUANTITY
};

// forward declarations
/// logs
int checkpoint ( queue *p_queue, const char *p_event );

/// string
void    string_print ( void *p_value, int i );
int     string_compare ( const void *const p_a, const void *const p_b );
void   *string_upper_case ( void *p_value );
void   *string_lower_case ( void *p_value );
hash64  string_hash ( void *p_value );
int     string_pack ( void *p_buffer, const void *const p_value );
int     string_unpack ( void *const p_value, void *p_buffer );
 
// data
/// immutable color strings
const char *_p_fruits[FRUIT_QUANTITY] =
{
    [APPLE]  = "Apple",
    [BANANA] = "Banana",
    [ORANGE] = "Orange",
    [LEMON]  = "Lemon",
    [GRAPE]  = "Grape",
    [PEACH]  = "Peach"
};

/// file for reflection
FILE *p_f = NULL;

/// hashes
hash64 h1 = 0,
       h2 = 0;

/// working queue
queue  *p_queue  = NULL;
size_t  file_len = 0;

// entry point
int main ( int argc, const char* argv[] )
{

    // unused
	(void) argc;
	(void) argv;

    // #0 - start
    checkpoint(p_queue, "start");
    
    // #1 - initial
    {

        // construct the queue
	    queue_construct(&p_queue);
        
        // checkpoint
        checkpoint(p_queue, "after construction");
    }

    // #2 - push 4
    {

        // push some values
	    queue_enqueue(p_queue, (void *) _p_fruits[APPLE]),
	    queue_enqueue(p_queue, (void *) _p_fruits[BANANA]);
        queue_enqueue(p_queue, (void *) _p_fruits[ORANGE]);
        queue_enqueue(p_queue, (void *) _p_fruits[LEMON]);

        // checkpoint
        checkpoint(p_queue, "after push < Apple, Banana, Orange, Lemon >");
    }

    // #3 - peek
    {

        // initialized data
        void *top = NULL;

        // peek the head of the queue
	    queue_front(p_queue, &top);

        // print the top of the queue
        printf("peek() -> %s\n", top);

        // checkpoint
        checkpoint(p_queue, "after peek");
    }

    // #4 - push (overflow!)
    {

        // cause a queue overflow
        queue_enqueue(p_queue, (void *) _p_fruits[GRAPE]);
        
        // checkpoint
        checkpoint(p_queue, "after push Grape (overflow)");
    }

    // #5 - to binary
    {

        // initialized data
        char buf[1024] = { 0 };
        
        // open a file for writing
        printf("Writing resources/reflection/queue.bin "),
        p_f = fopen("resources/reflection/queue.bin", "wb");

        // reflect the queue to a buffer
        file_len = queue_pack(buf, p_queue, string_pack),
        
        // write the buffer to a file
        fwrite(buf, file_len, 1, p_f),

        // close the file
        fclose(p_f),
        printf("[DONE]\n");

        // checkpoint
        checkpoint(p_queue, "after serialize");
    }

    // #6 - hash 1
    {

        // initialized data
        h1 = queue_hash(p_queue, (fn_hash64 *)string_hash);

        // print the hash
        printf("hash 1 -> 0x%llx\n", h1);

        // checkpoint
        checkpoint(p_queue, "after hash 1");
    }

    // #7 - pop 4
    {

        // pop some values off the queue
	    queue_dequeue(p_queue, NULL),
	    queue_dequeue(p_queue, NULL);
	    queue_dequeue(p_queue, NULL);
	    queue_dequeue(p_queue, NULL);
        
        // checkpoint
        checkpoint(p_queue, "after pop 4");
    }

    // #8 - pop (underflow!)
    {
        
        // initialized data
        void *first = NULL;

        // cause a queue underflow
	    queue_dequeue(p_queue, &first);
        
        // checkpoint
        checkpoint(p_queue, "after pop (underflow)");
    }

    // #9 - destroy
    {

        // destroy the queue
	    queue_destroy(&p_queue);
        
        // checkpoint
        checkpoint(p_queue, "after destroy");
    }

    // #10 - from binary
    {
        
        // initialized data
        char buf[1024] = { 0 };
        
        // read a buffer from a file
        printf("Reading resources/reflection/queue.bin"),
        p_f = fopen("resources/reflection/queue.bin", "rb"),
        fread(buf, file_len, 1, p_f),
        fclose(p_f),
        printf("[DONE]\n");

        // reflect an queue from the buffer
        queue_unpack(&p_queue, buf, string_unpack),

        // checkpoint
        checkpoint(p_queue, "after parse");
    }


    // #11 - hash 2
    {

        // initialized data
        h2 = queue_hash(p_queue, (fn_hash64 *)string_hash);

        // print the hash
        printf("hash 2 -> 0x%llx\n", h2);

        // error check
        if ( h1 != h2 ) log_error("Error: hash 1 != hash 2\n");

        // checkpoint
        checkpoint(p_queue, "after hash 2");
    }

    // #12 - end
    checkpoint(p_queue, "end");
    
    // success
    return EXIT_SUCCESS;
}

int checkpoint ( queue *p_queue, const char *p_event )
{

    // static data
    static int step = 0;

    // print the queue
    if ( NULL == p_queue )
        log_info("#%d - Queue %s: ", step, p_event),
        printf("NULL\n");
    else
        log_info("#%d - Queue %s:\n", step, p_event),
        queue_fori(p_queue, string_print),
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

int string_compare ( const void *const p_a, const void *const p_b )
{
    char *a = *(char **)p_a,
         *b = *(char **)p_b;

    return strcmp(a, b);
}

hash64 string_hash ( void *string )
{

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
