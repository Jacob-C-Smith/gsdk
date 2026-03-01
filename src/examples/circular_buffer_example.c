/** !
 * Circular buffer example program
 * 
 * @file src/examples/circular_buffer_example.c
 * 
 * @author Jacob Smith
 */

// standard library
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// gsdk
/// core
#include <core/log.h>
#include <core/sync.h>
#include <core/pack.h>
#include <core/hash.h>
#include <core/interfaces.h>

/// data
#include <data/circular_buffer.h>

// enumeration definitions
enum pieces_e
{
    PAWN           = 0,
    ROOK           = 1,
    KNIGHT         = 2,
	BISHOP         = 3,
	QUEEN          = 4,
	KING           = 5,
	PIECE_QUANTITY = 6
};

// forward declarations
/// logs
int checkpoint ( circular_buffer *p_circular_buffer, const char *p_event );

// string
void   string_print      ( void *p_value, int unused );
int    string_compare    ( const void *const p_a, const void *const p_b );
void  *string_upper_case ( void *p_value );
void  *string_lower_case ( void *p_value );
int    string_pack       ( void *p_buffer, const void *const p_value );
int    string_unpack     ( void *const p_value, void *p_buffer );
hash64 string_hash       ( const void *const string, unsigned long long unused );

// data
/// immutable piece strings
const char *_p_pieces[PIECE_QUANTITY] =
{
    [PAWN]   = "Pawn",
	[ROOK]   = "Rook",
	[KNIGHT] = "Knight",
	[BISHOP] = "Bishop",
	[QUEEN]  = "Queen",
	[KING]   = "King",
};

/// file for reflection
FILE *p_f = NULL;
size_t file_len = 0;

/// hashes
hash64 h1 = 0,
       h2 = 0;

/// working cache
circular_buffer *p_circular_buffer = NULL;

// entry point
int main ( int argc, const char* argv[] )
{

    // unused
    (void) argc;
    (void) argv;

    // #0 - start
    checkpoint(p_circular_buffer, "start");
    
    // #1 - initial
    {
        
        // construct the cache
        circular_buffer_construct(&p_circular_buffer, 3);
        
        // checkpoint
        checkpoint(p_circular_buffer, "after construction");
    }
        
    // #2 - insert 
    {
        
        // push some pieces
        for (enum pieces_e _piece = PAWN; _piece < QUEEN; _piece++)
            circular_buffer_push(p_circular_buffer, _p_pieces[_piece]);
        
        // checkpoint
        checkpoint(p_circular_buffer,"after adding < Pawn, Rook, Knight, Bishop >");
    }
        
    // #3 - peek
    {
        
        // initialized data
        void *p_value = NULL;
        
        // peek the last element
        circular_buffer_peek(p_circular_buffer, &p_value);
        
        // print the top of the cache
        printf("peek() -> %s\n", p_value);

        // checkpoint
        checkpoint(p_circular_buffer, "after peek");
    }

    // #4 - to binary
    {

        // initialized data
        char buf[1024] = { 0 };
        
        // open a file for writing
        p_f = fopen("resources/reflection/circular_buffer.bin", "wb");

        // reflect the circular buffer to a buffer
        file_len = circular_buffer_pack(buf, p_circular_buffer, string_pack),
        
        // write the buffer to a file
        fwrite(buf, file_len, 1, p_f),

        // close the file
        fclose(p_f),

        // checkpoint
        checkpoint(p_circular_buffer, "after serialize");
    }
    
    // #5 - hash 1
    {

        // initialized data
        h1 = circular_buffer_hash(p_circular_buffer, (fn_hash64 *)string_hash);

        // print the hash
        printf("hash 1 -> 0x%llx\n", h1);

        // checkpoint
        checkpoint(p_circular_buffer, "after hash 1");
    }

    // #6 - pop first 2
    {

        // pop the first 2 elements
        circular_buffer_pop(p_circular_buffer, NULL),
        circular_buffer_pop(p_circular_buffer, NULL);

        // checkpoint
        checkpoint(p_circular_buffer, "after removing < Rook, Knight >");
    }

    // #7 - push 
    {

        // push some pieces
        circular_buffer_push(p_circular_buffer, _p_pieces[QUEEN]),
        circular_buffer_push(p_circular_buffer, _p_pieces[KING]);
        circular_buffer_push(p_circular_buffer, _p_pieces[PAWN]);

        // checkpoint
        checkpoint(p_circular_buffer,"after adding < Queen, King, Pawn >");
    }

    // #8 - destroy 
    {

        // destroy the circular buffer
        circular_buffer_destroy(&p_circular_buffer, NULL);

        // checkpoint
        checkpoint(p_circular_buffer, "after destroy");
    }
    
    // #9 - from binary
    {

        // initialized data
        char buf[1024] = { 0 };
        
        // read a buffer from a file
        p_f = fopen("resources/reflection/circular_buffer.bin", "rb"),
        fread(buf, file_len, 1, p_f),
        fclose(p_f);

        // reflect a circular buffer from the buffer
        circular_buffer_unpack(&p_circular_buffer, buf, string_unpack),

        // checkpoint
        checkpoint(p_circular_buffer, "after parse");
    }

    // #10 - hash 2
    {

        // initialized data
        h2 = circular_buffer_hash(p_circular_buffer, (fn_hash64 *)string_hash);

        // print the hash
        printf("hash 2 -> 0x%llx\n", h2);

        // error check
        if ( h1 != h2 ) log_error("Error: hash 1 != hash 2\n");

        // checkpoint
        checkpoint(p_circular_buffer, "after hash 2");
    }

    // #12 - destroy
    {

        // destroy the cache
        circular_buffer_destroy(&p_circular_buffer, (fn_allocator *)default_allocator);

        // checkpoint
        checkpoint(p_circular_buffer, "after destroy");
    }

    // #12 - end
    checkpoint(p_circular_buffer, "end");
   
    // success
    return EXIT_SUCCESS;
}

int checkpoint ( circular_buffer *p_circular_buffer, const char *p_event )
{
    
    // static data
    static int step = 0;
    
    // print the cache
    if ( NULL == p_circular_buffer )
		log_info("#%d - Circular buffer %s: ", step, p_event),
		printf("NULL\n");
    else
		log_info("#%d - Circular buffer %s:\n", step, p_event),
		circular_buffer_foreach(p_circular_buffer, (fn_foreach *) string_print),
		putchar('\n');
    
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

void string_print ( void *p_value, int unused )
{
    
	// unused
	(void) unused;

    // print the element
    printf("%s\n", (char *)p_value);
    
    // done
    return ;
}

int string_compare ( const void *const p_a, const void *const p_b )
{
    char *a = *(char **)p_a,
         *b = *(char **)p_b;

    return strcmp(a, b);
}

int string_pack ( void *p_buffer, const void *const p_value )
{

    // done
    return pack_pack(p_buffer, "%s", p_value);
}

int string_unpack ( void *const p_value, void *p_buffer )
{

    // initialized data
    char       **pp_value        = (const char **) p_value;
    int          result          = 0;
    char        *p_string        = NULL;
    const char   _string  [1024] = { 0 };

    // unpack the buffer
    result = pack_unpack(p_buffer, "%s", &_string);

    // duplicate the string
    p_string = strdup(_string);

    // return a pointer to the caller
    *pp_value = p_string;

    // done
    return result;
}

hash64 string_hash ( const void *const string, unsigned long long unused )
{

    // unused
    (void)unused;

    // done
    return hash_crc64(string, strlen(string));
}
