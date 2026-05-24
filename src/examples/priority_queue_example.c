/** !
 * Example of a priority queue
 * 
 * @file src/examples/priority_queue_example.c
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
#include <core/interfaces.h>

/// data
#include <data/priority_queue.h>

// forward declarations
/// logs
int checkpoint ( priority_queue *p_priority_queue, const char *p_event );

// string
void string_print ( void *p_value, int i );

// data
const char *_p_values[] =
{
    "Pawn",
    "Rook",
    "Knight",
    "Bishop",
    "Queen",
    "King"
};

// entry point
int main ( int argc, const char *argv[] )
{

    // unused
    (void) argc;
    (void) argv;

    // initialized data
    priority_queue *p_priority_queue = (void *) 0;

    // #0 - start
    checkpoint(p_priority_queue, "start");

    // #1 - construct
    {
        // Construct a priority queue
        priority_queue_construct(&p_priority_queue, 8, (priority_queue_equal_fn *)strcmp);

        // checkpoint
        checkpoint(p_priority_queue, "after construction");
    }

    // #2 - enqueue
    {
        // enqueue some elements
        for ( int i = 0; i < 6; i++ )
            priority_queue_enqueue(p_priority_queue, (void *)_p_values[i]);

        // checkpoint
        checkpoint(p_priority_queue, "after enqueueing < Pawn, Rook, Knight, Bishop, Queen, King >");
    }

    // #3 - dequeue
    {
        // dequeue the max element
        void *p_max = NULL;
        priority_queue_dequeue(p_priority_queue, &p_max);

        // print the max element
        printf("dequeue() -> %s\n", (char *)p_max);

        // checkpoint
        checkpoint(p_priority_queue, "after dequeue");
    }

    // #4 - destroy
    {
        // destroy the priority queue
        priority_queue_destroy(&p_priority_queue);

        // checkpoint
        checkpoint(p_priority_queue, "after destroy");
    }

    // #5 - end
    checkpoint(p_priority_queue, "end");

    // success
    return EXIT_SUCCESS;
}

int checkpoint ( priority_queue *p_priority_queue, const char *p_event )
{
    
    // static data
    static int step = 0;
    
    // print the cache
    if ( NULL == p_priority_queue )
        log_info("#%d - Priority queue %s: ", step, p_event),
        printf("NULL\n");
    else
    {
        // initialized data
        int i = 0;

        // logs
        log_info("#%d - Priority queue %s:\n", step, p_event);

        // iterate through the priority queue
        for ( iterator it = priority_queue_iterator(p_priority_queue); !it.done(&it); it.next(&it) )
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
    return ;
}
