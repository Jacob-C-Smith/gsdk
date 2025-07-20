/** !
 * Example tree program
 *
 * @file main.c
 *
 * @author Jacob Smith
 */

// standard library
#include <stdio.h>
#include <stdlib.h>

// core
#include <core/log.h>

// data
#include <data/binary.h>

// Preprocessor defines
#define BINARY_TREE_EXAMPLE_LIST_LENGTH 15

// structure definitions
struct number_and_string_s
{
    char _string[12];
    int  number;
};

// type definitions
typedef struct number_and_string_s number_and_string;

/** !
 * Example binary tree comparator
 * 
 * @param p_a pointer to a
 * @param p_b pointer to b 
 * 
 * @return 1 on success, 0 on error
*/
int binary_tree_example_comparator ( const void *const p_a, const void *const p_b );

/** !
 * Example binary tree key accessor
 * 
 * @param pp_result result
 * @param p_value   the value to access 
 * 
 * @return 1 on success, 0 on error
*/
const void *binary_tree_example_key_accessor ( const void *const p_value );

/** !
 * Example binary tree node serializer
 * 
 * @param p_file             the file to write to
 * @param p_binary_tree_node the binary tree node to write 
 * 
 * @return 1 on success, 0 on error
*/
int binary_tree_example_serializer ( FILE *p_file, binary_tree_node *p_binary_tree_node );

/** !
 * Example binary tree node parser
 * 
 * @param p_file             the file to read from
 * @param p_binary_tree_node the binary tree node to populate
 * 
 * @return 1 on success, 0 on error
*/
int binary_tree_example_parser ( FILE *p_file, binary_tree_node *p_binary_tree_node );

/** !
 * Print a binary tree node to standard out
 * 
 * @param p_value the value
 * 
 * @return 1 on success, 0 on error
 */
int binary_tree_print_node ( void *p_value );

// entry point
int main ( int argc, const char *argv[] )
{
    
    // Unused
    (void) argc;
    (void) argv;

    // Formatting
    log_info("╭─────────────────────╮\n");
    log_info("│ Binary tree example │\n");
    log_info("╰─────────────────────╯\n");
    printf(
        "This example creates a balanced binary tree from a sorted list of keys\n"\
        "in linear time. This balanced binary tree is serialized to the disk,\n"
        "loaded, and a key is searched for at random.\n\n"
    );

    // initialized data
    binary_tree             *p_binary_tree = 0;
    number_and_string       *p_result      = 0;
    number_and_string        _values[BINARY_TREE_EXAMPLE_LIST_LENGTH] =
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

    // Construct a tree
    binary_tree_construct(
        &p_binary_tree,                   // Result
        (fn_comparator *)binary_tree_example_comparator,   // Node comparator
        (fn_key_accessor *)binary_tree_example_key_accessor, // Key accessor
        16                                // sizeof(node)
    );

    // Iterate over each property
    for (size_t i = 0; i < BINARY_TREE_EXAMPLE_LIST_LENGTH; i++)
        
        // Store the property in the tree
        binary_tree_insert(p_binary_tree, &_values[i]);

    // Serialize the binary tree to a file
    binary_tree_serialize(
        p_binary_tree,                  // The binary tree
        "resources/output.binary_tree", // The path
        binary_tree_example_serializer  // The node serializer
    );

    // Destroy the binary tree
    binary_tree_destroy(&p_binary_tree);

    // Load the binary tree from the file
    binary_tree_parse(
        &p_binary_tree,                   // Result
        "resources/output.binary_tree",   // Path to the tree
        (fn_comparator *)binary_tree_example_comparator,   // The comparator
        (fn_key_accessor *)binary_tree_example_key_accessor, // The key accessor
        (fn_binary_tree_parse *)binary_tree_example_parser        // The node parser
    );
    
    // Traverse the binary tree using the in order technique
    binary_tree_traverse_inorder(p_binary_tree, binary_tree_print_node);

    // Query the binary tree
    binary_tree_search(p_binary_tree, "thirteen", (void **)&p_result);

    // Print the result
    printf("Searching \"thirteen\" yields \"%d\"\n", p_result->number);

    // success
    return 1;
}
int binary_tree_example_comparator ( const void *const p_a, const void *const p_b )
{

    // success
    return strcmp(p_b, p_a);
}

const void *binary_tree_example_key_accessor ( const void *const p_value )
{

    // initialized data
    const number_and_string *p_number_and_string = p_value;

    // success
    return (void *)p_number_and_string->_string;
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

    // Store the value
    p_binary_tree_node->p_value = p_number_and_string;
    
    // success
    return 1;
}

int binary_tree_print_node ( void *p_value )
{

    // Print the property
    log_info("%s\n", ((number_and_string *) p_value)->_string);

    // success
    return 1;
}
