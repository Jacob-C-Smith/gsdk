/** !
 * Binary tree example
 *
 * @file src/examples/binary_example.c
 *
 * @author Jacob Smith
 */

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// gsdk
/// core
#include <core/log.h>

/// data
#include <data/binary.h>

// structure definitions
struct number_and_string_s
{
    char _string[12];
    int  number;
};

// type definitions
typedef struct number_and_string_s number_and_string;

// forward declarations
/// logs
int checkpoint ( binary_tree *p_binary_tree, const char *p_event );

fn_comparator   number_and_string_string_comparator;
fn_key_accessor number_and_string_string_key_accessor;
fn_comparator   number_and_string_number_comparator;
fn_key_accessor number_and_string_number_key_accessor;
fn_pack         number_and_string_pack;
fn_unpack       number_and_string_unpack;

int number_and_string_number_print ( void *p_value );
int number_and_string_string_print ( void *p_value );

// data
/// working binary tree
binary_tree *p_binary_tree = 0;

/// file for reflection
FILE *p_f = NULL;
size_t len = 0;

/// hashes
hash64 h1 = 0,
       h2 = 0;

/// example data
number_and_string _values[] =
{
    { ._string = "eight"   , .number = 8  },
    { ._string = "four"    , .number = 4  },
    { ._string = "twelve"  , .number = 12 },
    { ._string = "two"     , .number = 2  },
    { ._string = "six"     , .number = 6  },
    { ._string = "ten"     , .number = 10 },
    { ._string = "fourteen", .number = 14 },
    { ._string = "one"     , .number = 1  },
    { ._string = "three"   , .number = 3  },
    { ._string = "five"    , .number = 5  },
    { ._string = "seven"   , .number = 7  },
    { ._string = "nine"    , .number = 9  },
    { ._string = "eleven"  , .number = 11 },
    { ._string = "thirteen", .number = 13 },
    { ._string = "fifteen" , .number = 15 }
};

// entry point
int main ( int argc, const char *argv[] )
{
    
    // unused
    (void) argc;
    (void) argv;

    // #0 - start
    checkpoint(p_binary_tree, "start");

    // #1 - construct a binary tree
    {

        // construct a binary tree
        binary_tree_construct(
            &p_binary_tree,             // result
            sizeof(number_and_string),  // size

            (fn_comparator *)  number_and_string_number_comparator,  // comparator
            (fn_key_accessor *)number_and_string_number_key_accessor // key accessor
        );

        // checkpoint
        checkpoint(p_binary_tree, "after construct"),
        putchar('\n');
    }

    // #2 - insert
    {

        // iterate over each property ...
        for (size_t i = 0; i < sizeof(_values) / sizeof(*_values); i++)
            
            // ... store the property in the tree
            binary_tree_insert(p_binary_tree, &_values[i]);

        // checkpoint
        checkpoint(p_binary_tree, "after insert"),
        putchar('\n');

    }

    // #3 - search
    {

        // initialized data
        number_and_string *p_result = NULL;

        // search the binary tree for 11
        binary_tree_search(p_binary_tree, (void *)11, (void **)&p_result);

        // checkpoint
        checkpoint(p_binary_tree, "after search");

        // print the result
        printf("Searching 11 yields \"%s\"\n", p_result->_string);

        // formatting
        putchar('\n');
    }

    // #4 - pre order
    {

        // checkpoint
        checkpoint(NULL, "pre order traversal");

        // perform a pre order traversal on the keys in the binary tree
        binary_tree_traverse_preorder(p_binary_tree, number_and_string_number_print), putchar('\n'),
        binary_tree_traverse_preorder(p_binary_tree, number_and_string_string_print), putchar('\n'),
        putchar('\n');
    }

    // #5 - post order
    {

        // checkpoint
        checkpoint(NULL, "post order traversal");

        // perform a post order traversal on the keys in the binary tree
        binary_tree_traverse_postorder(p_binary_tree, number_and_string_number_print), putchar('\n'),
        binary_tree_traverse_postorder(p_binary_tree, number_and_string_string_print), putchar('\n'),
        putchar('\n');
    }

    // #6 - remove
    {

        // iterate through integer multiples of 4
        for (size_t i = 0; i < sizeof(_values)/sizeof(*_values); i += 4)

            // remove them
            binary_tree_remove(p_binary_tree, (void *)i, NULL);

        // checkpoint
        checkpoint(p_binary_tree, "after remove"),
        putchar('\n');
        
    }

    // // #7 - hash 1
    // {
        
    //     // initialized data
    //     h1 = binary_tree_hash(p_binary_tree, NULL);

    //     // print the hash
    //     printf("hash 1 -> 0x%llx\n", h1);

    //     // checkpoint
    //     checkpoint(p_binary_tree, "after hash 1"),
    //     putchar('\n');
    // }

    // success
    return EXIT_SUCCESS;
}

int checkpoint ( binary_tree *p_binary_tree, const char *p_event )
{

    // static data
    static int step = 0;

    // print the array
    if ( NULL == p_binary_tree )
        log_info("#%d - Binary tree %s: \n", step, p_event);
    else
        log_info("#%d - Binary tree %s:\n", step, p_event),
        
        putchar('('), putchar(' '),
        binary_tree_traverse_inorder(p_binary_tree, number_and_string_number_print),
        putchar(')'),

        printf(" -> "),
        
        putchar('('), putchar(' '),
        binary_tree_traverse_inorder(p_binary_tree, number_and_string_string_print),
        putchar(')'),

        putchar('\n');

    // increment counter
    step++;

    // success
    return 1;
}

int number_and_string_string_comparator ( const void *const p_a, const void *const p_b )
{

    // success
    return strcmp(p_b, p_a);
}

int number_and_string_number_comparator ( const void *const p_a, const void *const p_b )
{

    // success
    return p_b - p_a;
}

void *number_and_string_string_key_accessor ( const void *const p_value )
{

    // initialized data
    const number_and_string *p_number_and_string = p_value;

    // success
    return (void *)p_number_and_string->_string;
}

void *number_and_string_number_key_accessor ( const void *const p_value )
{

    // initialized data
    const number_and_string *p_number_and_string = p_value;

    // success
    return (void *)p_number_and_string->number;
}

int number_and_string_number_print ( void *p_value )
{

    // initialized data
    number_and_string *p_number_and_string = (number_and_string *)p_value;

    // print the property
    printf("%d ", p_number_and_string->number);

    // success
    return 1;
}

int number_and_string_string_print ( void *p_value )
{

    // initialized data
    number_and_string *p_number_and_string = (number_and_string *)p_value;

    // print the property
    printf("%s ", p_number_and_string->_string);

    // success
    return 1;
}

int binary_tree_example_serializer ( FILE *p_file, binary_tree_node *p_binary_tree_node )
{

    // Write the key to the output
    fwrite(p_binary_tree_node->p_value, sizeof(number_and_string), 1, p_file);
    
    // success
    return 1;
}

int binary_tree_example_parser ( FILE *p_file, binary_tree_node *p_binary_tree_node )
{

    // initialized data
    number_and_string *p_number_and_string = malloc(sizeof(number_and_string));

    // Read a string from the input
    fread(&p_number_and_string->_string, 12, 1, p_file);

    // Read a double from the input
    fread(&p_number_and_string->number, 4, 1, p_file);

    // store the value
    p_binary_tree_node->p_value = p_number_and_string;
    
    // success
    return 1;
}
