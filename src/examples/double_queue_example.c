/** !
 * Double queue example
 * 
 * @file main.c
 * 
 * @author Jacob Smith
 */

// standard library
#include <stdio.h>

// gsdk
/// core
#include <core/log.h>

/// data
#include <data/double_queue.h>

// entry point
int main ( int argc, const char *argv[] )
{

	// unused
	(void) argc;
	(void) argv;

	// initialized data
	double_queue *p_double_queue = 0;
	void         *value          = 0;
	char         *pp_contents[]  = { "First!", "Second!", "Third!", "Fourth!", "Fifth!", "Sixth!", 0x0 };

	// Construct a queue
	double_queue_construct(&p_double_queue);
	
	// Add three elements to the queue
	double_queue_rear_add(p_double_queue, "Third!");
	double_queue_rear_add(p_double_queue, "Fourth!");
	double_queue_front_add(p_double_queue, "Second!");
	double_queue_front_add(p_double_queue, "First!");

	// Empty the queue and print each element
	while ( double_queue_empty(p_double_queue) == false )
	{

		// initialized data
		void *v = 0;

		// Dequeue an item
		double_queue_front_remove(p_double_queue, &v);

		// Print the element
		printf("%s\n", (char *) v);
	}
	
	printf("\n");

	// Fill up the queue
	double_queue_from_contents(&p_double_queue, (void **)pp_contents, 6);

	log_info(" - Ascending -\n");

	// Empty the full queue
	while ( double_queue_empty(p_double_queue) == false )
	{

		// initialized data
		void *v = 0;

		// Dequeue an item
		double_queue_front_remove(p_double_queue,&v);

		// Print the element
		printf("%s\n", (char *) v);
	}

	// Destroy the double queue
	// double_queue_destroy(&p_double_queue);
	
	// Fill up the queue
	double_queue_from_contents(&p_double_queue, (void **)pp_contents, 6);

	// Formatting
	log_info("\n - Descending -\n");

	// Empty the full queue
	while ( double_queue_empty(p_double_queue) == false )
	{

		// initialized data
		void *v = 0;

		// Dequeue an item
		double_queue_rear_remove(p_double_queue,&v);

		// Print the element
		printf("%s\n", (char *) v);
	}
	
	// Cause an underflow
	if ( double_queue_front_remove(p_double_queue, &value) == 0 )
		log_warning("\nDetected double queue underflow!\n");

	// Destroy the double queue
	// double_queue_destroy(&p_double_queue);

	// success
	return EXIT_SUCCESS;
}