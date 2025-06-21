// standard library
#include <stdio.h>

// data
#include <data/stack.h>

// Entry point
int main ( int argc, const char *argv[] )
{

	// Supress compiler warnings
	(void) argc;
	(void) argv;

	// Initialized data
	stack *p_stack = 0;
	const void  *first   = 0;

	// Construct a stack
	stack_construct(&p_stack, 2);// TOP - null  - null  - BOTTOM

	// Push some values onto the stack
	stack_push(p_stack, (void *) 0x123);// TOP - null  - 0x123 - BOTTOM
	stack_push(p_stack, (void *) 0x456);// TOP - 0x456 - 0x123 - BOTTOM

	// Cause a stack overflow
	stack_push(p_stack, (void *) 0x789);// TOP - 0x456 - 0x123 - BOTTOM

	// Pop some values off the stack
	stack_pop(p_stack, &first);// TOP - null  - 0x123 - BOTTOM	
	stack_pop(p_stack, &first);// TOP - null  - null  - BOTTOM

	// Cause a stack underflow
	stack_pop(p_stack, &first);// TOP - null  - null  - BOTTOM

	// Destroy the stack
	stack_destroy(&p_stack);

	// Success
	return EXIT_SUCCESS;
}
