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
#include <core/stream.h>
#include <core/interfaces.h>

// data
#include <data/queue.h>

// enumeration definitions
enum person_name_e
{
    ALICE,
    BOB,
    CAROL,
    DAVE,
    EVE,
    FRANK,
    PERSON_QUANTITY
};

// forward declarations
/// logs
int checkpoint ( queue *p_queue, const char *p_event );

/// string
fn_fori string_print;
fn_hash64 string_hash;
fn_pack string_pack;
fn_unpack string_unpack;

 
// data
/// immutable people strings
const char *_p_people[PERSON_QUANTITY] =
{
    [ALICE]  = "Alice",
    [BOB]    = "Bob",
    [CAROL]  = "Carol",
    [DAVE]   = "Dave",
    [EVE]    = "Eve",
    [FRANK]  = "Frank"
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

        // construct a queue from the first 3 people
	    queue_from_contents(&p_queue, (void *const *const)_p_people, 3);
        
        // checkpoint
        checkpoint(p_queue, "after construction");
    }

    // #2 - enqueue dave
    {

        // enqueue dave
        queue_enqueue(p_queue, (void *) _p_people[DAVE]);

        // checkpoint
        checkpoint(p_queue, "after enqueue < Dave >");
    }

    // #3 - peek
    {

        // initialized data
        void *front = NULL,
			 *rear  = NULL;

        // peek the front and rear of the queue
	    queue_front(p_queue, &front),
	    queue_rear(p_queue, &rear);

        // print the top of the queue
        printf("front() -> %s\n", (char *) front);
        printf("rear()  -> %s\n", (char *) rear);

        // checkpoint
        checkpoint(p_queue, "after peek");
    }

    // #4 - to binary
    {

        // initialized data
        stream *p_stream = NULL;
        
        // open a file for writing
        stream_from_path(&p_stream, "resources/reflection/queue.bin");

        // reflect the queue to a buffer
        queue_pack(p_stream, p_queue, string_pack);
        
        // close the file
        stream_destroy(&p_stream);

        // checkpoint
        checkpoint(p_queue, "after serialize");
    }

    // #5 - hash 1
    {

        // initialized data
        h1 = queue_hash(p_queue, (fn_hash64 *)string_hash);

        // print the hash
        printf("hash 1 -> 0x%llx\n", h1);

        // checkpoint
        checkpoint(p_queue, "after hash 1");
    }

	// #6 - dequeue first 2
	{

		// dequeue alice and bob
		queue_dequeue(p_queue, NULL),
		queue_dequeue(p_queue, NULL);

        // checkpoint
        checkpoint(p_queue, "after dequeue < Alice, Bob >");
	}

    // #7 - enqueue eve and frank
    {

        // enqueue eve and frank the queue
	    queue_enqueue(p_queue, (void *) _p_people[EVE]),
	    queue_enqueue(p_queue, (void *) _p_people[FRANK]);
        
        // checkpoint
        checkpoint(p_queue, "after enqueue < Eve, Frank >");
    }

    // #8 - destroy
    {

        // destroy the queue
	    queue_destroy(&p_queue);
        
        // checkpoint
        checkpoint(p_queue, "after destroy");
    }

    // #9 - from binary
    {
        
        // initialized data
        stream *p_stream = NULL;
        
        // read a buffer from a file
        stream_from_path(&p_stream, "resources/reflection/queue.bin");

        // reflect an queue from the buffer
        queue_unpack(&p_queue, p_stream, string_unpack);

        stream_destroy(&p_stream);

        // checkpoint
        checkpoint(p_queue, "after parse");
    }

    // #10 - hash 2
    {

        // initialized data
        h2 = queue_hash(p_queue, string_hash);

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
    {

        // initialized data
        size_t i = 0;

        // logs
        log_info("#%d - Queue %s:\n", step, p_event);

        // iterate through the queue
        for ( iterator it = queue_iterator(p_queue); !it.done(&it); it.next(&it) )
            string_print(it.item(&it), i),
            i++;

        // formatting
        putchar('\n');
    }

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
