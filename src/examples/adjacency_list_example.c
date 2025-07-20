/** !
 * Example program for adjacency list module
 * 
 * @file main.c
 * 
 * @author Jacob Smith
 */

// standard library
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// core
#include <core/log.h>
#include <core/sync.h>
#include <core/hash.h>
#include <core/pack.h>

// data
#include <data/adjacency_list.h>

// forward declarations
/// logs
int checkpoint ( adjacency_list *p_adjacency_list, const char *p_event );

/// string
void    string_print ( void *p_value );
int     string_compare ( const void *const p_a, const void *const p_b );
void   *string_upper_case ( void *p_value );
void   *string_lower_case ( void *p_value );
hash64  string_hash ( const void *const string, unsigned long long unused );
int     string_pack ( void *p_buffer, const void *const p_value );
int     string_unpack ( void *const p_value, void *p_buffer );
 
// data
/// file for reflection
FILE *p_f = NULL;

/// hashes
hash64 h1 = 0,
       h2 = 0;

/// working adjacency_list
adjacency_list *p_adjacency_list = NULL;
size_t file_len = 0;

// entry point
int main ( int argc, const char* argv[] )
{

    // unused
    (void) argc;
    (void) argv;

    // #0 - start
    checkpoint(p_adjacency_list, "start");
    
    // #1 - initial
    {

        // construct the adjacency list
        adjacency_list_construct(&p_adjacency_list, GRAPH_TYPE_UNWEIGHTED_UNDIRECTED);

        // checkpoint
        checkpoint(p_adjacency_list, "after construction");
    }

    // #2 - add vertices
    {

        // Add vertices to the adjacency list
        adjacency_list_vertex_add(p_adjacency_list, "A");
        adjacency_list_vertex_add(p_adjacency_list, "B");
        adjacency_list_vertex_add(p_adjacency_list, "C");
        
        // checkpoint
        checkpoint(p_adjacency_list, "after adding vertices");
    }

    // #3 - add edges
    {

        // Add edges between vertices
        adjacency_list_edge_add(p_adjacency_list, "A", "B", 0.0);
        adjacency_list_edge_add(p_adjacency_list, "B", "C", 0.0);
        adjacency_list_edge_add(p_adjacency_list, "A", "C", 0.0);

        // checkpoint
        checkpoint(p_adjacency_list, "after adding edges");
    }
    
    // #14 - end
    checkpoint(p_adjacency_list, "end");
    
    // success
    return EXIT_SUCCESS;
}

int checkpoint ( adjacency_list *p_adjacency_list, const char *p_event )
{

    // static data
    static int step = 0;

    // print the adjacency list
    adjacency_list_print(p_adjacency_list, string_print);
    
    // increment counter
    step++;

    // success
    return 1;
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
