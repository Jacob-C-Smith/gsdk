/** !
 * Lisp syntax highlighter
 * 
 * @file lisp_syntax_highlighter.c
 * 
 * @author Jacob Smith
 */

// standard library
#include <stdio.h>
#include <stdbool.h>

// core
#include <core/log.h>

// data
#include <data/stack.h>

// entry point
int main ( int argc, const char *argv[] )
{
    
    // initialized data
    stack *p_stack = NULL;
    int state = 1;
    
    // construct a stack
    if ( 0 == stack_construct(&p_stack, 1024) ) goto failed_to_construct_stack;

    // read until EOF
    while ( !feof(stdin) )
    {

        // initialized data
        char c = getchar();

        switch (c)
        {
        
            case EOF:
                goto done;
                break;
            case '(':

                // Preserve the current state
                stack_push(p_stack, (void *) state);

                // Update the state
                state = 1;
                printf("\033[0m(\033[31m");
                continue;
            case ')':

                // Restore the previous state
                stack_pop(p_stack, &state);

                printf("\033[0m)\033[%dm", state > 0 ? 31 : 34);
                continue;
            case ';':
                printf("\033[0m\033[32 m");
                while (c != '\n')
                    putchar(c),
                    c = getchar();
                putchar('\n');
                continue;
            case ' ':
                state = -1;

                printf("\033[34m");
        }

        // print the character
        putchar(c);
    }
    
    done:

    // cleanup
    printf("\033[0m");
    
    // success
    return EXIT_SUCCESS;

    // error handling
    {

        // stack errors
        {
            failed_to_construct_stack:
                #ifndef NDEBUG
                    log_error("[lisp syntax] Failed to construct stack in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return EXIT_FAILURE;
        }
    }
}
