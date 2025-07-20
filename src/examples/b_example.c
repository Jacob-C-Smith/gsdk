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
#include <data/b.h>

// Preprocessor defines
#define b_TREE_EXAMPLE_LIST_LENGTH 15

// structure definitions
struct number_and_string_s
{
    char _string[12];
    int  number;
};

// type definitions
typedef struct number_and_string_s number_and_string;

// Forward declarations
int number_and_string_compare ( const void *const p_a, const void *const p_b );
void *number_and_string_key_accessor ( const void *const p_value );

/// string
void    string_print ( void *p_value );
int     string_compare ( const void *const p_a, const void *const p_b );
void   *string_upper_case ( void *p_value );
void   *string_lower_case ( void *p_value );
hash64  string_hash ( const void *const string, unsigned long long unused );
int     string_pack ( void *p_buffer, const void *const p_value );
int     string_unpack ( void *const p_value, void *p_buffer );
 
// entry point
int main ( int argc, const char *argv[] )
{
    
    // Unused
    (void) argc;
    (void) argv;

    // Formatting
    log_info("╭────────────────╮\n");
    log_info("│ B tree example │\n");
    log_info("╰────────────────╯\n");
    printf(
        "This example creates a balanced b tree from a sorted list of keys\n"\
        "in linear time. This balanced b tree is serialized to the disk,\n"
        "loaded, and a key is searched for at random.\n\n"
    );

    // initialized data
    b_tree             *p_b_tree = 0;
    number_and_string       *p_result      = 0;
    number_and_string        _values[b_TREE_EXAMPLE_LIST_LENGTH] =
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
    b_tree_construct(
        &p_b_tree,                   // Result
        "resources/example.b_tree", // Path to the tree
        number_and_string_compare,   // Node comparator
        number_and_string_key_accessor, // Key accessor
        3,                             // Degree
        0                                // Calculate sizeof(node) automatically
    );

    // Iterate over each property
    for (size_t i = 0; i < b_TREE_EXAMPLE_LIST_LENGTH; i++)
        
        // Store the property in the tree
        b_tree_insert(p_b_tree, &_values[i]);

    // Serialize the b tree to a file
    // b_tree_serialize(
    //     p_b_tree,                  // The b tree
    //     "resources/output.b_tree", // The path
    //     (fn_pack *)string_pack  // The node serializer
    // );

    // // Destroy the b tree
    // b_tree_destroy(&p_b_tree);

    // // Load the b tree from the file
    // b_tree_parse(&p_b_tree,
    //     "resources/output.b_tree",   // Path to the tree
    //     (fn_comparator *)default_comparator,   // The comparator
    //     (fn_unpack *)string_unpack               // The node parser
    // );
    
    // Query the b tree
    b_tree_search(p_b_tree, "thirteen", (void **)&p_result);

    // // Print the result
    // printf("Searching \"thirteen\" yields \"%d\"\n", p_result->number);

    // success
    return 1;
}

int number_and_string_compare ( const void *const p_a, const void *const p_b )
{
    // argument check
    if ( p_a == (void *) 0 ) return -1;
    if ( p_b == (void *) 0 ) return 1;

    // initialized data
    const number_and_string *a = (const number_and_string *) p_a;
    const number_and_string *b = (const number_and_string *) p_b;

    // Compare by number
    if ( a->number < b->number ) return -1;
    if ( a->number > b->number ) return 1;
    return 0;
}

void *number_and_string_key_accessor ( const void *const p_value )
{
    // argument check
    if ( p_value == (void *) 0 ) return (void *) 0;

    // For simplicity, we'll use the whole structure as the key
    // In a real implementation, you might return &((number_and_string *)p_value)->number
    return (void *) p_value;
}

void *string_upper_case ( void *p_value )
{

    // initialized data
    char *p_string = strdup((char *)p_value),
         *p_result = p_string;
         
    // iterate upto null terminator
    while (*p_string)
    {

        // initialized data
        char c = *p_string;

        // convert offending characters
        if ( islower(c) ) c = toupper(c);
        
        // store the upper case character
        *p_string++ = c;
    }

    // success
    return p_result;
}

void *string_lower_case ( void *p_value )
{

    // initialized data
    char *p_string = strdup((char *)p_value),
         *p_result = p_string;
         
    // iterate upto null terminator
    while (*p_string)
    {

        // initialized data
        char c = *p_string;

        // convert offending characters
        if ( isupper(c) ) c = tolower(c);
        
        // store the lower case character
        *p_string++ = c;
    }

    // success
    return p_result;
}

void string_print ( void *p_value )
{
    
    // print the element
    printf("%s", (char *)p_value);
    
    // done
    return ;
}

int string_compare ( const void *const p_a, const void *const p_b )
{
    char *a = *(char **)p_a,
         *b = *(char **)p_b;

    return strcmp(a, b);
}

hash64 string_hash ( const void *const string, unsigned long long unused )
{

    // unused
    (void)unused;

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
