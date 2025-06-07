/** !
 * Example of a priority queue
 * 
 * @file main.c
 * @author Jacob Smith
 * @date Feb 14, 2024
 */

// Headers
#include <stdio.h>

// data
#include <data/priority_queue.h>

// Entry point
int main ( int argc, const char *argv[] )
{

    // Supress compiler warnings
    (void) argc;
    (void) argv;

    // Initialized data
    priority_queue *p_priority_queue = (void *) 0;

    // Construct a priority queue
    priority_queue_construct(&p_priority_queue, 8, 0);



    // Success
    return EXIT_SUCCESS;
}
