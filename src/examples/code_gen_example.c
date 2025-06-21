/** !
 * alloc example program
 * 
 * @file main.c
 * 
 * @author Jacob Smith
 */

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// POSIX
#include <sys/wait.h>
#include <unistd.h>

// log module
#include <log/log.h>

// code gen module 
#include <code_gen/code_gen.h>
#include <code_gen/x86_64.h>


// preprocessor definitions
//

// enumeration definitions
enum code_gen_examples_e
{
    CODE_GEN_X86_64_EXAMPLE   = 0,
    CODE_GEN_AARCH32_EXAMPLE  = 1,
    CODE_GEN_EXAMPLE_QUANTITY = 2
};

// forward declarations
/** !
 * Print a usage message to standard out
 * 
 * @param argv0 the name of the program
 * 
 * @return void
 */
void print_usage ( const char *argv0 );

/** !
 * Parse command line arguments
 * 
 * @param argc            the argc parameter of the entry point
 * @param argv            the argv parameter of the entry point
 * @param examples_to_run return
 * 
 * @return void on success, program abort on failure
 */
void parse_command_line_arguments ( int argc, const char *argv[], bool *examples_to_run );

/** !
 * x86_64 code generator example program
 * 
 * @param argc the argc parameter of the entry point
 * @param argv the argv parameter of the entry point
 * 
 * @return 1 on success, 0 on error
 */
int code_gen_x86_64_example ( int argc, const char *argv[] );

/** !
 * Aarch32 code generator example program
 * 
 * @param argc the argc parameter of the entry point
 * @param argv the argv parameter of the entry point
 * 
 * @return 1 on success, 0 on error
 */
int code_gen_aarch32_example ( int argc, const char *argv[] );

// entry point
int main ( int argc, const char *argv[] )
{

    // Supress compiler warnings
    (void) argc;
    (void) argv;

    // initialized data
    bool examples_to_run[CODE_GEN_EXAMPLE_QUANTITY] = { 0 };

    // Parse command line arguments
    parse_command_line_arguments(argc, argv, examples_to_run);

    // Formatting
    log_info(
        "╭──────────────────╮\n"\
        "│ code gen example │\n"\
        "╰──────────────────╯\n"\
        "The code gen library .\n"\
        "code gen supports %d architectures. x86_64, and Aarch32.\n\n"\
        "x86_64 is a 64-bit CISC architecture from AMD\n"\
        "AArch32 is a 32-bit RISC architecture from ARM\n"\
        "\n",
        CODE_GEN_EXAMPLE_QUANTITY
    );

    //////////////////////
    // Run the examples //
    //////////////////////

    // Run the x86_64 example program
    if ( examples_to_run[CODE_GEN_X86_64_EXAMPLE] )

        // error check
        if ( code_gen_x86_64_example(argc, argv) == 0 ) goto failed_to_run_x86_64_example;
    
    // Run the AArch32 example program
    if ( examples_to_run[CODE_GEN_AARCH32_EXAMPLE] )

        // error check
        if ( code_gen_aarch32_example(argc, argv) == 0 ) goto failed_to_run_aarch32_example;
    
    // success
    return EXIT_SUCCESS;

    // error handling
    {
        failed_to_run_x86_64_example:

            // Write an error message to standard out
            log_error("Error: Failed to run x86_64 example!\n");

            // error
            return EXIT_FAILURE;

        failed_to_run_aarch32_example:

            // Write an error message to standard out
            log_error("Error: Failed to run AArch32 example!\n");

            // error
            return EXIT_FAILURE;
    }
}

void print_usage ( const char *argv0 )
{

    // argument check
    if ( argv0 == (void *) 0 ) exit(EXIT_FAILURE);

    // Print a usage message to standard out
    printf("Usage: %s [x86_64] [aarch32]\n", argv0);

    // Done
    return;
}

void parse_command_line_arguments ( int argc, const char *argv[], bool *examples_to_run )
{

    // If no command line arguments are supplied, run all the examples
    if ( argc == 1 ) goto all_examples;

    // error check
    if ( argc > CODE_GEN_EXAMPLE_QUANTITY + 1 ) goto invalid_arguments;

    // Iterate through each command line argument
    for (int i = 1; i < argc; i++)
    {
        
        // x86_64 example?
        if ( strcmp(argv[i], "x86_64") == 0 )

            // Set the linear allocator example flag
            examples_to_run[CODE_GEN_X86_64_EXAMPLE] = true;

        // Aarch32 example?
        else if ( strcmp(argv[i], "aarch32") == 0 )
            
            // Set the slab allocator example flag
            examples_to_run[CODE_GEN_AARCH32_EXAMPLE] = true;

        // Default
        else goto invalid_arguments;
    }
    
    // success
    return;

    // Set each example flag
    all_examples:
    {

        // For each example ...
        for (size_t i = 0; i < CODE_GEN_EXAMPLE_QUANTITY; i++)
        
            // ... set the example flag
            examples_to_run[i] = true;
        
        // success
        return;
    }

    // error handling
    {

        // argument errors
        {
            invalid_arguments:
                
                // Print a usage message to standard out
                print_usage(argv[0]);

                // Abort
                exit(EXIT_FAILURE);
        }
    }
}

int code_gen_x86_64_example ( int argc, const char *argv[] )
{

    // Suppress warnings
    (void) argc;
    (void) argv;

    // Formatting
    log_info(
        "╭─────────────────────────╮\n"\
        "│ x86_64 code gen example │\n"\
        "╰─────────────────────────╯\n"\
        "\n"
        //"\n\n"
    );
    
    // initialized data
    x86_64_code_gen *p_code_gen = (void *) 0;
    char _output[4096] = { 0 };
    int (*add) ( int i, int j ) = (void *) 0;
    int _pipe[2] = { 0 };
    int ndisasm_pid = -1;
    char *const ndisasm_argv[] = { "ndisasm", "-b", "64", "-", NULL };

    // Construct a code generator
    x86_64_code_gen_construct(&p_code_gen, _output, 4096);

    // Construct a pipe
    if ( -1 == pipe(_pipe) )
        goto failed_to_open_pipe;

    // Write the machine code to the pipe
    if ( (ssize_t) -1 == write(_pipe[1], p_code_gen->p_base, p_code_gen->size) )
        goto failed_to_write;

    // Done with write end
    close(_pipe[1]);

    // Fork ndisasm
    ndisasm_pid = fork();

    // error check
    if ( ndisasm_pid == -1 )
        goto failed_to_disassemble;

    // ndisasm
    if ( ndisasm_pid == 0 ) 
    {

        // Replace stdin with read end of pipe
        dup2(_pipe[0], STDIN_FILENO);
        
        // Close unused fd
        close(_pipe[0]);
        
        // Formatting
        printf(
            "                 <<< ndisasm >>>                 \n"\
            "-------------------------------------------------\n"
        );

        // Execute ndisasm
        execvp(ndisasm_argv[0], ndisasm_argv);

        // Abort
        exit(EXIT_FAILURE);
    }
    
    // Close read end
    close(_pipe[0]);

    // Wait on ndisasm
    waitpid(ndisasm_pid, NULL, 0);
    
    // Formatting
    printf("-------------------------------------------------\n");

    // No input
    if ( p_code_gen->size == 0 ) goto done;

    // // Store the base
    // add = p_code_gen->p_base;
    
    // // Call the function
    // int r = add("Hello, World", 1);

    // // Print the result
    // printf("%s(%d,%d) -> %d", "fun", 1, 2, r);

    done:
    // Format
    putchar('\n');
    
    // success
    return 1;

    failed_to_disassemble:
    failed_to_open_pipe:
    failed_to_write:
        return 0;
}

int code_gen_aarch32_example ( int argc, const char *argv[] )
{

    // Suppress warnings
    (void) argc;
    (void) argv;

    // Formatting
    log_info(
        "╭──────────────────────────╮\n"\
        "│ AArch32 code gen example │\n"\
        "╰──────────────────────────╯\n"\
        "[TODO]\n\n"
    );

    // Format
    putchar('\n');

    // success
    return 1;
}
