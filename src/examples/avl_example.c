/** !
 * AVL tree example
 *
 * @file src/examples/avl_example.c
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
#include <data/avl.h>

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
int checkpoint ( avl_tree *p_avl_tree, const char *p_event );

fn_allocator    number_and_string_destroy;
fn_comparator   number_and_string_number_comparator;
fn_comparator   number_and_string_string_comparator;
fn_forcontext   number_and_string_accumulate;
fn_foreach      number_and_string_number_print;
fn_foreach      number_and_string_string_print;
fn_hash64       number_and_string_hash;
fn_key_accessor number_and_string_string_key_accessor;
fn_key_accessor number_and_string_number_key_accessor;
fn_pack         number_and_string_pack;
fn_unpack       number_and_string_unpack;

// data
/// working avl tree
avl_tree *p_avl_tree = 0;

/// file for reflection
FILE *p_f = NULL;
size_t file_len = 0;

/// hashes
hash64 h1 = 0,
       h2 = 0;

/// example data
number_and_string _values[] =
{
    { ._string = "one"     , .number = 1  },
    { ._string = "two"     , .number = 2  },
    { ._string = "three"   , .number = 3  },
    { ._string = "four"    , .number = 4  },
    { ._string = "five"    , .number = 5  },
    { ._string = "six"     , .number = 6  },
    { ._string = "seven"   , .number = 7  },
    { ._string = "eight"   , .number = 8  },
    { ._string = "nine"    , .number = 9  },
    { ._string = "ten"     , .number = 10 },
    { ._string = "eleven"  , .number = 11 },
    { ._string = "twelve"  , .number = 12 },
    { ._string = "thirteen", .number = 13 },
    { ._string = "fourteen", .number = 14 },
    { ._string = "fifteen" , .number = 15 }
};

// entry point
int main ( int argc, const char *argv[] )
{
    
    // unused
    (void) argc;
    (void) argv;

    // #0 - start
    checkpoint(p_avl_tree, "start");

    // #1 - construct a avl tree
    {

        // construct a avl tree
        avl_tree_construct(
            &p_avl_tree,             // result
            sizeof(number_and_string),  // size

            (fn_comparator *)  number_and_string_number_comparator,  // comparator
            (fn_key_accessor *)number_and_string_number_key_accessor // key accessor
        );

        // checkpoint
        checkpoint(p_avl_tree, "after construct"),
        putchar('\n');
    }

    // #2 - insert
    {

        // iterate over each property ...
        for (size_t i = 0; i < sizeof(_values) / sizeof(*_values); i++)
            
            // ... store the property in the tree
            avl_tree_insert(p_avl_tree, &_values[i]);

        // checkpoint
        checkpoint(p_avl_tree, "after insert"),
        putchar('\n');

    }

    // #3 - search
    {

        // initialized data
        number_and_string *p_result = NULL;

        // search the avl tree for 11
        avl_tree_search(p_avl_tree, (void *)11, (void **)&p_result);

        // checkpoint
        checkpoint(p_avl_tree, "after search");

        // print the result
        printf("Searching 11 yields \"%s\"\n", p_result->_string);

        // formatting
        putchar('\n');
    }

    // #4 - pre order
    {

        // checkpoint
        checkpoint(NULL, "pre order traversal");

        // perform a pre order traversal on the keys in the avl tree
        avl_tree_traverse_preorder(p_avl_tree, number_and_string_number_print), putchar('\n'),
        avl_tree_traverse_preorder(p_avl_tree, number_and_string_string_print), putchar('\n'),
        putchar('\n');
    }

    // #5 - post order
    {

        // checkpoint
        checkpoint(NULL, "post order traversal");

        // perform a post order traversal on the keys in the avl tree
        avl_tree_traverse_postorder(p_avl_tree, number_and_string_number_print), putchar('\n'),
        avl_tree_traverse_postorder(p_avl_tree, number_and_string_string_print), putchar('\n'),
        putchar('\n');
    }
    
    // #6 - to binary
    {

        // initialized data
        stream *p_stream = NULL;
        
        // construct a stream
        stream_from_path(&p_stream, "resources/reflection/avl_tree.bin");

        // reflect the avl tree to a stream
        avl_tree_pack(p_stream, p_avl_tree, number_and_string_pack),

        // destroy the stream
        stream_destroy(&p_stream);

        // checkpoint
        checkpoint(p_avl_tree, "after serialize"),
        putchar('\n');
    }
 
    // #7 - hash 1
    {
        
        // hash the avl tree
        h1 = avl_tree_hash(p_avl_tree, number_and_string_hash);

        // print the hash
        printf("hash 1 -> 0x%llx\n", h1);

        // checkpoint
        checkpoint(p_avl_tree, "after hash 1"),
        putchar('\n');
    }

    // #8 - remove
    {

        // remove multiples of 4
        for (size_t i = 4; i < sizeof(_values)/sizeof(*_values); i += 4)
            avl_tree_remove(p_avl_tree, (void *)i, NULL);

        // checkpoint
        checkpoint(p_avl_tree, "after remove"),
        putchar('\n');
    }

    // #9 - destroy
    {

        // destroy the avl tree
        avl_tree_destroy(&p_avl_tree, NULL);

        // checkpoint
        checkpoint(p_avl_tree, "after destroy"),
        putchar('\n');
    }

    // #10 - from binary
    {
        
        // initialized data
        stream *p_stream = NULL;
        
        // read a buffer from a file
        stream_from_path(&p_stream, "resources/reflection/avl_tree.bin");
        
        // reflect a avl tree from the buffer
        avl_tree_unpack(
            &p_avl_tree, 
            p_stream, 
            
            number_and_string_unpack,
            number_and_string_number_comparator,
            number_and_string_number_key_accessor
        );

        // destroy the stream
        stream_destroy(&p_stream);

        // checkpoint
        checkpoint(p_avl_tree, "after unpack"),
        putchar('\n');
    }

    // #11 - hash 2
    {
        
        // hash the avl tree
        h2 = avl_tree_hash(p_avl_tree, number_and_string_hash);

        // print the hash
        printf("hash 2 -> 0x%llx\n", h2);

        // error check
        if ( h1 != h2 ) 

            // abort
            log_error("Error: hash 1 != hash 2\n"), exit(EXIT_FAILURE);

        // checkpoint
        checkpoint(p_avl_tree, "after hash 2");
    }

    // #12 - for context
    {

        // initialized data
        int sum = 0;

        // checkpoint
        checkpoint(NULL, "for context (accumulate)");

        // perform a pre order traversal on the keys in the avl tree
        avl_tree_forcontext(p_avl_tree, number_and_string_accumulate, (void *) &sum);

        // log the sum
        printf("sum: %d\n", sum);
    }

    // #13 - destroy
    {

        // destroy the avl tree
        avl_tree_destroy(&p_avl_tree, default_allocator);

        // checkpoint
        checkpoint(p_avl_tree, "after destroy"),
        putchar('\n');
    }

    // success
    return EXIT_SUCCESS;
}

int checkpoint ( avl_tree *p_avl_tree, const char *p_event )
{

    // static data
    static int step = 0;

    // print the array
    if ( NULL == p_avl_tree )
        log_info("#%d - AVL tree %s: NULL\n", step, p_event);
    else
        log_info("#%d - AVL tree %s:\n", step, p_event),
        
        putchar('('), putchar(' '),
        avl_tree_traverse_inorder(p_avl_tree, number_and_string_number_print),
        putchar(')'),

        printf(" -> "),
        
        putchar('('), putchar(' '),
        avl_tree_traverse_inorder(p_avl_tree, number_and_string_string_print),
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
    return strcmp(p_a, p_b);
}

int number_and_string_number_comparator ( const void *const p_a, const void *const p_b )
{

    // success
    return p_a - p_b;
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
    return (void *)((size_t)p_number_and_string->number);
}

void number_and_string_number_print ( void *p_value )
{

    // initialized data
    number_and_string *p_number_and_string = (number_and_string *)p_value;

    // print the property
    printf("%d ", p_number_and_string->number);

    // done
    return;
}

void number_and_string_string_print ( void *p_value )
{

    // initialized data
    number_and_string *p_number_and_string = (number_and_string *)p_value;

    // print the property
    printf("%s ", p_number_and_string->_string);

    // done
    return;
}

void number_and_string_accumulate ( void *p_value, void *p_context )
{

    // initialized data
    number_and_string *p_number_and_string = (number_and_string *)p_value;
    int               *p_sum               = (int *)p_context;

    // accumulate
    *p_sum += p_number_and_string->number;

    // done
    return;
}


hash64 number_and_string_hash ( const void *const k, unsigned long long l )
{

    // unused
    (void) l;

    // done
    return default_hash(k, sizeof(number_and_string));
}

int number_and_string_pack ( stream *p_stream, const void *const p_value )
{

    // initialized data 
    number_and_string *p_number_and_string = (number_and_string *)p_value;

    // pack the metadata
    pack_pack(p_stream, "%i32%s", 
        p_number_and_string->number,
        p_number_and_string->_string
    );

    // success
    return sizeof(number_and_string);
}

int number_and_string_unpack ( void *const p_value, stream *p_stream )
{

    // initialized data
    number_and_string **pp_value            = (number_and_string **) p_value;
    number_and_string  *p_number_and_string = NULL;

    size_t     number        = 0;
    const char _string[1024] = { 0 };

    // unpack the buffer
    pack_unpack(p_stream, "%i32%s", &number, &_string);

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
