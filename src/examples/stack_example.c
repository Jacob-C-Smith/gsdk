/** !
 * Example program for stack module
 * 
 * @file src/examples/stack_example.c
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
#include <core/stream.h>

// data
#include <data/stack.h>

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
int checkpoint ( stack *p_stack, const char *p_event );

/// string
fn_fori   string_print;
fn_hash64 string_hash;
fn_pack   string_pack;
fn_unpack string_unpack;
 
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

/// working stack
stack  *p_stack  = NULL;
size_t  file_len = 0;

// entry point
int main ( int argc, const char* argv[] )
{

    // unused
	(void) argc;
	(void) argv;

    // #0 - start
    checkpoint(p_stack, "start");
    
    // #1 - initial
    {

        // construct the stack
	    stack_construct(&p_stack, 4);
        
        // checkpoint
        checkpoint(p_stack, "after construction");
    }

    // #2 - push 4
    {

        // push some values
	    stack_push(p_stack, (void *) _p_fruits[APPLE]),
	    stack_push(p_stack, (void *) _p_fruits[BANANA]);
        stack_push(p_stack, (void *) _p_fruits[ORANGE]);
        stack_push(p_stack, (void *) _p_fruits[LEMON]);

        // checkpoint
        checkpoint(p_stack, "after push < Apple, Banana, Orange, Lemon >");
    }

    // #3 - peek
    {

        // initialized data
        void *top = NULL;

        // peek the top of the stack
	    stack_peek(p_stack, &top);

        // print the top of the stack
        printf("peek() -> %s\n", (char *) top);

        // checkpoint
        checkpoint(p_stack, "after peek");
    }

    // #4 - push (overflow!)
    {

        // cause a stack overflow
        stack_push(p_stack, (void *) _p_fruits[GRAPE]);
        
        // checkpoint
        checkpoint(p_stack, "after push Grape (overflow)");
    }

    // #5 - to binary
    {

        // initialized data
        stream *p_stream = NULL;
        
        // open a file for writing
        stream_from_path(&p_stream, "resources/reflection/stack.bin");

        // reflect the stack to a buffer
        stack_pack(p_stream, p_stack, string_pack);
        
        // close the file
        stream_destroy(&p_stream);

        // checkpoint
        checkpoint(p_stack, "after serialize");
    }

    // #6 - hash 1
    {

        // initialized data
        h1 = stack_hash(p_stack, string_hash);

        // print the hash
        printf("hash 1 -> 0x%llx\n", h1);

        // checkpoint
        checkpoint(p_stack, "after hash 1");
    }

    // #7 - pop 4
    {

        // pop some values off the stack
	    stack_pop(p_stack, NULL),
	    stack_pop(p_stack, NULL);
	    stack_pop(p_stack, NULL);
	    stack_pop(p_stack, NULL);
        
        // checkpoint
        checkpoint(p_stack, "after pop 4");
    }

    // #8 - pop (underflow!)
    {
        
        // initialized data
        void *first = NULL;

        // cause a stack underflow
	    stack_pop(p_stack, &first);
        
        // checkpoint
        checkpoint(p_stack, "after pop (underflow)");
    }

    // #9 - destroy
    {

        // destroy the stack
	    stack_destroy(&p_stack);
        
        // checkpoint
        checkpoint(p_stack, "after destroy");
    }

    // #10 - from binary
    {
        
        // initialized data
        stream *p_stream = NULL;
        
        // read a buffer from a file
        stream_from_path(&p_stream, "resources/reflection/stack.bin");

        // reflect an stack from the buffer
        stack_unpack(&p_stack, p_stream, string_unpack);

        stream_destroy(&p_stream);

        // checkpoint
        checkpoint(p_stack, "after parse");
    }


    // #11 - hash 2
    {

        // initialized data
        h2 = stack_hash(p_stack, string_hash);

        // print the hash
        printf("hash 2 -> 0x%llx\n", h2);

        // error check
        if ( h1 != h2 ) log_error("Error: hash 1 != hash 2\n");

        // checkpoint
        checkpoint(p_stack, "after hash 2");
    }

    // #12 - end
    checkpoint(p_stack, "end");
    
    // success
    return EXIT_SUCCESS;
}

int checkpoint ( stack *p_stack, const char *p_event )
{

    // static data
    static int step = 0;

    // print the stack
    if ( NULL == p_stack )
        log_info("#%d - Stack %s: ", step, p_event),
        printf("NULL\n");
    else
        log_info("#%d - Stack %s:\n", step, p_event),
        stack_fori(p_stack, string_print),
        putchar('\n');

    // increment counter
    step++;

    // success
    return 1;
}

void string_print ( void *p_value, int i )
{
    
    // print the element
    printf("[%d] - %s\n", i, (char *)p_value);
    
    // done
    return;
}


hash64 string_hash ( const void *const string, unsigned long long unused )
{

    // unused
    (void)unused;

    // done
    return hash_crc64(string, strlen(string));
}

int string_pack ( stream *p_stream, const void *const p_value )
{

    // done
    return pack_pack(p_stream, "%s", p_value);
}

int string_unpack ( void *const p_value, stream *p_stream )
{

    // initialized data
    char       **pp_value        = (char **) p_value;
    int          result          = 0;
    char        *p_string        = NULL;
    const char   _string  [1024] = { 0 };

    // unpack the buffer
    result = pack_unpack(p_stream, "%s", &_string);

    // duplicate the string
    p_string = strdup(_string);

    // return a pointer to the caller
    *pp_value = p_string;

    // done
    return result;
}
