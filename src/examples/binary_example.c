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
    char _string[16];
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
fn_hash64       number_and_string_hash;
fn_allocator    number_and_string_destroy;

int number_and_string_number_print ( void *p_value );
int number_and_string_string_print ( void *p_value );

// data
/// working binary tree
binary_tree *p_binary_tree = 0;

/// file for reflection
FILE *p_f = NULL;
size_t file_len = 0;

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
    
    // #6 - to binary
    {

        // initialized data
        char buf[1024] = { 0 };
        
        // Open a file for writing
        p_f = fopen("resources/reflection/binary_tree.bin", "wb");

        // reflect the binary tree to a buffer
        file_len = binary_tree_pack(buf, p_binary_tree, number_and_string_pack),
        
        // write the buffer to a file
        fwrite(buf, file_len, 1, p_f),

        // close the file
        fclose(p_f);

        // checkpoint
        checkpoint(p_binary_tree, "after serialize"),
        putchar('\n');
    }
 
    // #7 - hash 1
    {
        
        // hash the binary tree
        h1 = binary_tree_hash(p_binary_tree, number_and_string_hash);

        // print the hash
        printf("hash 1 -> 0x%llx\n", h1);

        // checkpoint
        checkpoint(p_binary_tree, "after hash 1"),
        putchar('\n');
    }

    // #8 - remove
    {

        // remove multiples of 4
        for (size_t i = 0; i < sizeof(_values)/sizeof(*_values); i += 4)
            binary_tree_remove(p_binary_tree, (void *)i, NULL);

        // checkpoint
        checkpoint(p_binary_tree, "after remove"),
        putchar('\n');
    }

    // #9 - destroy
    {

        // destroy the binary tree
        binary_tree_destroy(&p_binary_tree, NULL);

        // checkpoint
        checkpoint(p_binary_tree, "after destroy"),
        putchar('\n');
    }

    // #10 - from binary
    {
        
        // initialized data
        char buf[1024] = { 0 };
        
        // read a buffer from a file
        p_f = fopen("resources/reflection/binary_tree.bin", "rb"),
        fread(buf, sizeof(char), file_len, p_f),
        fclose(p_f);
        
        // reflect a binary tree from the buffer
        binary_tree_unpack(
            &p_binary_tree, 
            buf, 
            
            number_and_string_unpack,
            number_and_string_number_comparator,
            number_and_string_number_key_accessor
        );

        // checkpoint
        checkpoint(p_binary_tree, "after unpack"),
        putchar('\n');
    }

    // #11 - hash 2
    {
        
        // hash the binary tree
        h2 = binary_tree_hash(p_binary_tree, number_and_string_hash);

        // print the hash
        printf("hash 2 -> 0x%llx\n", h2);

        // error check
        if ( h1 != h2 ) 

            // abort
            log_error("Error: hash 1 != hash 2\n"), exit(EXIT_FAILURE);

        // checkpoint
        checkpoint(p_binary_tree, "after hash 2");
    }

    // #12 - destroy
    {

        // destroy the binary tree
        binary_tree_destroy(&p_binary_tree, default_allocator);

        // checkpoint
        checkpoint(p_binary_tree, "after destroy"),
        putchar('\n');
    }
    // success
    return EXIT_SUCCESS;
}

int checkpoint ( binary_tree *p_binary_tree, const char *p_event )
{

    // static data
    static int step = 0;

    // print the array
    if ( NULL == p_binary_tree )
        log_info("#%d - Binary tree %s: NULL\n", step, p_event);
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

hash64 number_and_string_hash ( const void *const k, unsigned long long l )
{

    // unused
    (void) l;

    // done
    return default_hash(k, sizeof(number_and_string));
}

int number_and_string_pack ( void *p_buffer, const void *const p_value )
{

    // initialized data 
    number_and_string *p_number_and_string = (number_and_string *)p_value;
    char *p = p_buffer;

    // pack the metadata
    p += pack_pack(p_buffer, "%i32%s", 
        p_number_and_string->number,
        p_number_and_string->_string
    );

    // success
    return sizeof(number_and_string);
}

int number_and_string_unpack ( void *const p_value, void *p_buffer )
{

    // initialized data
    number_and_string **pp_value            = (number_and_string **) p_value;
    number_and_string  *p_number_and_string = NULL;

    int        result        = 0;
    size_t     number        = 0;
    const char _string[1024] = { 0 };

    // unpack the buffer
    pack_unpack(p_buffer, "%i32%s", &number, &_string);

    // allocate memory for the result
    p_number_and_string = default_allocator(0, sizeof(number_and_string));

    // store the values
    p_number_and_string->number = number,
    strncpy(p_number_and_string->_string, _string, sizeof(p_number_and_string->_string));

    // return a pointer to the caller
    *pp_value = p_number_and_string;

    // done
    return sizeof(number_and_string);
}
