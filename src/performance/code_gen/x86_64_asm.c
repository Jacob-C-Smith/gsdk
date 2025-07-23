/** !
 * g assembler (x86_64)
 *
 * @file x86_64_asm.c
 * 
 * @author Jacob Smith
 */

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

// code gen module
#include <code_gen/code_gen.h>
#include <code_gen/x86_64.h>

// Preprocessor macros
#define NASM_DISASSEMBLY

// Data
x86_64_code_gen *p_x86_64_code_gen = (void *) 0;

/** !
 * Evaluate an assembly instruction
 * 
 * @param p_input A line of assembly
 * 
 * @return 1 on success, 0 on error
 */
int x86_64_asm_eval ( const char *p_input );

// entry point
int main ( int argc, const char *argv[] )
{

    // Unused 
    (void) argc;
    (void) argv;

    // Construct an x86_64 code generator
    x86_64_code_gen_construct(&p_x86_64_code_gen, 0, 8192);

    // Formatting
    printf("                 <<<  STDIN  >>>                 \n");
    
    // REPL
    while ( !feof(stdin) )
    {

        // initialized data
        char _in_buf[1024] = { 0 };

        // Wipe the tokens
        memset(_in_buf, 0, 1024);

        // Read a line from standard in
        if ( NULL == fgets(_in_buf, 1024, stdin) ) break;

        // Evaluate
        x86_64_asm_eval(_in_buf);

    }
    
    // Formatting
    printf("                 <<<   EOF   >>>                 \n");
    
    // Disassemble
    #ifdef NASM_DISASSEMBLY
    {

        // initialized data
        int _pipe[2] = { 0 };
        int ndisasm_pid = -1;
        char *const ndisasm_argv[] = { "ndisasm", "-b", "64", "-", NULL };

        // Construct a pipe
        if ( -1 == pipe(_pipe) )
            goto failed_to_open_pipe;

        // Write the machine code to the pipe
        if ( (ssize_t) -1 == write(_pipe[1], p_x86_64_code_gen->p_base, ((size_t)p_x86_64_code_gen->p_offset-(size_t)p_x86_64_code_gen->p_base)) )
            goto failed_to_write;

        // done with write end
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
        
        // done with read end, done with pipe
        close(_pipe[0]);

        // Wait on ndisasm
        waitpid(ndisasm_pid, NULL, 0);
        
        // Formatting
        printf("-------------------------------------------------\n");
    }
    #endif

    // Invocation
    {

        // initialized data
        int    r  = 0;
        void  *p1 = "Hello, World!";
        void  *p2 = strlen(p1);
        void  *p3 = NULL;
        void  *p4 = NULL;
        int  (*fun)
        (
            void *i,
            void *j,
            void *k,
            void *l
        ) = (void *) 0;

        // Store the base
        fun = p_x86_64_code_gen->p_base;
        
        // Print the result
        printf("fun(\"%s\",%d)\n", p1, p2);

        // Call the function
        r = fun(p1, p2, p3, p4);

        // Formatting
        putchar('\n');
    }

    // success
    return EXIT_SUCCESS;

    // error handling
    {
        failed_to_open_pipe:
        failed_to_write:
        failed_to_disassemble:
            return 0;
    }
}

int x86_64_asm_eval ( const char *p_input )
{

    // initialized data
    char  _token[1024]  = { 0 };
    char *p_token       = _token,
         *p_instruction = (void *) 0,
         *p_op1         = (void *) 0,
         *p_op2         = (void *) 0,
         *p_op3         = (void *) 0;

    // Copy the token
    memcpy(_token, p_input, 1024);

    // Transform the string to upper case
    for (size_t i = 0; i < strlen(_token) - 1; i++)
        _token[i] = (char) toupper(p_token[i]);
    
    // Tokenize
    p_instruction = strtok(p_token, " ,\n"), p_instruction = p_token;

    // Tokenize
    p_op1 = strtok(NULL, " ,\n");

    // Tokenize
    p_op2 = strtok(NULL, " ,\n");

    // Tokenize
    p_op3 = strtok(NULL, " ,\n");

    // Assemble
    x86_64_code_gen_node_construct(p_x86_64_code_gen,
        p_instruction,
        p_op1,
        p_op2,
        p_op3);

    // success
    return 1;
}
