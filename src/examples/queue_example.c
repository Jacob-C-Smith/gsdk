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
void    string_print ( void *p_value, int i );
hash64  string_hash ( void *p_value );
int     string_pack ( void *p_buffer, const void *const p_value );
int     string_unpack ( void *const p_value, void *p_buffer );
 
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
	    queue_from_contents(&p_queue, _p_people, 3);
        
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
        printf("front() -> %s\n", front);
        printf("rear()  -> %s\n", rear);

        // checkpoint
        checkpoint(p_queue, "after peek");
    }

    // #4 - to binary
    {

        // initialized data
        char buf[1024] = { 0 };
        
        // open a file for writing
        p_f = fopen("resources/reflection/queue.bin", "wb");

        // reflect the queue to a buffer
        file_len = queue_pack(buf, p_queue, string_pack),
        
        // write the buffer to a file
        fwrite(buf, file_len, 1, p_f),

        // close the file
        fclose(p_f),

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
	    queue_enqueue(p_queue, _p_people[EVE]),
	    queue_enqueue(p_queue, _p_people[FRANK]);
        
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
        char buf[1024] = { 0 };
        
        // read a buffer from a file
        p_f = fopen("resources/reflection/queue.bin", "rb"),
        fread(buf, file_len, 1, p_f),
        fclose(p_f),

        // reflect an queue from the buffer
        queue_unpack(&p_queue, buf, string_unpack),

        // checkpoint
        checkpoint(p_queue, "after parse");
    }

    // #10 - hash 2
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

void string_print ( void *p_value, int i )
{
    
    // print the element
    printf("[%d] - %s\n", i, (char *)p_value);
    
    // done
    return ;
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
