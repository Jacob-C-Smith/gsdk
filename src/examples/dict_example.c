/** !
 * Example program for dictionary module
 * 
 * @file src/examples/dict_example.c
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
#include <core/hash.h>
#include <core/pack.h>
#include <core/stream.h>
#include <core/interfaces.h>

/// data
#include <data/dict.h>

// enumeration definitions
enum people_e
{
    ALEX            = 0,
    BRAD            = 1,
    CELESTE         = 2,
    DANIEL          = 3,
    ETHAN           = 4,
    JACOB           = 5,
    LISA            = 6,
    NATHAN          = 7,
    PIPER           = 8,
    WYATT           = 9,
    PEOPLE_QUANTITY = 10,
};

// structure definitions
struct person_s
{
    char _name[16];
    char _hair_color[16];
    bool glasses;
};

// type definitions
typedef struct person_s person;

// forward declarations
/// logs
int checkpoint ( dict *p_dict, const char *p_event );

/// person
fn_foreach      person_print;
fn_hash64       person_hash_key;
fn_hash64       person_hash;
fn_key_accessor person_key_accessor;
fn_pack         person_pack;
fn_unpack       person_unpack;

// data
/// working dictionary
dict *p_dict = NULL;

/// file for reflection
FILE   *p_f      = NULL;
size_t  file_len = 0;

/// hashes
hash64 h1 = 0,
       h2 = 0;

/// example data
person _people[PEOPLE_QUANTITY] =
{
    [ALEX]    = { ._name = "Alex",    ._hair_color = "blonde", .glasses = true  },
    [BRAD]    = { ._name = "Brad",    ._hair_color = "blonde", .glasses = true  },
    [CELESTE] = { ._name = "Celeste", ._hair_color = "white",  .glasses = false },
    [DANIEL]  = { ._name = "Daniel",  ._hair_color = "black",  .glasses = false },
    [ETHAN]   = { ._name = "Ethan",   ._hair_color = "brown",  .glasses = true  },
    [JACOB]   = { ._name = "Jacob",   ._hair_color = "brown",  .glasses = false },
    [LISA]    = { ._name = "Lisa",    ._hair_color = "brown",  .glasses = true  },
    [NATHAN]  = { ._name = "Nathan",  ._hair_color = "brown",  .glasses = false },
    [PIPER]   = { ._name = "Piper",   ._hair_color = "blonde", .glasses = true  },
    [WYATT]   = { ._name = "Wyatt",   ._hair_color = "blonde", .glasses = true  },
};

// entry point
int main ( int argc, const char* argv[] )
{

    // unused
    (void) argc;
    (void) argv;

    // #0 - start
    checkpoint(p_dict, "start");
    
    
    // #1 - construct
    {

        // construct the dictionary
        dict_construct(
            &p_dict, 
            7,

            NULL,
            person_key_accessor,
            person_hash_key
        );

        // checkpoint
        checkpoint(p_dict, "after construction");
    }
    
    // #2 - insert
    {
        
        // add some people
        for (enum people_e _person = ALEX; _person < PEOPLE_QUANTITY; _person+=2)
            if ( 0 == dict_add(p_dict, &_people[_person]) )
                log_error("Error: Failed to insert \"%s\"\n", _people[_person]._name);
            
        // checkpoint
        checkpoint(p_dict, "after adding < Alex, Celeste, Ethan, Lisa, Piper >");
    }
    
    // #3 - search
    {
        
        // initialized data
        person *p_person = NULL;
        
        // search for a value
        dict_get(p_dict, "Lisa", (void **)&p_person);

        // checkpoint
        checkpoint(p_dict, "after search");

        // print results
        log_info("Searching for \"Lisa\" yields\n"),
        person_print(p_person),
        putchar('\n');
    }

    // #4 - insert more
    {

        // add some more people
        for (enum people_e _person = BRAD; _person < PEOPLE_QUANTITY; _person+=2)
            if ( 0 == dict_add(p_dict, &_people[_person]) )
                log_error("Error: Failed to insert \"%s\"\n", _people[_person]._name);
            
        // checkpoint
        checkpoint(p_dict, "after adding < Brad, Daniel, Jacob, Nathan, Wyatt >");
    }

    // #6 - to binary
    {

        // initialized data
        stream *p_stream = NULL;
        
        // open a file for writing
        stream_from_path(&p_stream, "resources/reflection/dict.bin");

        // reflect the dictionary to a buffer
        dict_pack(p_stream, p_dict, person_pack);
        
        // close the file
        stream_destroy(&p_stream);

        // checkpoint
        checkpoint(p_dict, "after serialize");
    }

    // #7 - hash 1
    {

        // compute the hash of the dictionary
        h1 = dict_hash(p_dict, person_hash);

        // checkpoint
        checkpoint(p_dict, "after hash 1");

        // print the hash
        printf("hash 1 -> 0x%llx\n", h1);
    }

    // #8 - remove 
    {

        // remove some people
        for (enum people_e _person = ALEX; _person < PEOPLE_QUANTITY; _person+=3)
        {

            // initialized data
            void *p_value = NULL; 
            
            // remove one
            if ( 0 == dict_pop(p_dict, _people[_person]._name, (const void **)&p_value) )
                log_error("Error: Failed to remove \"%s\"\n", _people[_person]._name);
        }

        // checkpoint
        checkpoint(p_dict, "after remove < Alex, Daniel, Lisa, Wyatt >");
    }

    // #9 - destroy
    {
        
        // destroy the dictionary
        dict_destroy(&p_dict, NULL);

        // checkpoint
        checkpoint(p_dict, "after destroy");
    }

    // #10 - from binary
    {

        // initialized data
        stream *p_stream = NULL;
        
        // read a buffer from a file
        stream_from_path(&p_stream, "resources/reflection/dict.bin");

        // reflect a dictionary from the buffer
        dict_unpack
        (
            &p_dict,
            p_stream,
            
            person_unpack,
            NULL,
            person_key_accessor,
            person_hash_key
        );

        // close the file
        stream_destroy(&p_stream);

        // checkpoint
        checkpoint(p_dict, "after parse");
    }

    // #11 - hash 2
    {

        // hash the dictionary
        h2 = dict_hash(p_dict, person_hash);

        // checkpoint
        checkpoint(p_dict, "after hash 2");

        // print the hash
        printf("hash 2 -> 0x%llx\n", h2);

        // error check
        if ( h1 != h2 ) 

            // abort
            log_error("Error: hash 1 != hash 2\n"), exit(EXIT_FAILURE);
    }
    
    // #12 - destroy
    {
        
        // destroy the hash table
        dict_destroy(&p_dict, default_allocator);

        // checkpoint
        checkpoint(p_dict, "after destroy");
    }

    // #13 - end
    checkpoint(p_dict, "end");

    // success
    return EXIT_SUCCESS;
}

int checkpoint ( dict *p_dict, const char *p_event )
{

    // static data
    static int step = 0;

    // print the dictionary
    if ( NULL == p_dict )
        log_info("#%d - Dictionary %s: NULL\n", step, p_event);
    else
    {

        // logs
        log_info("#%d - Dictionary %s:\n", step, p_event);

        // iterate through the dictionary
        for ( iterator it = dict_iterator(p_dict); !it.done(&it); it.next(&it) )
            person_print(it.item(&it));
    }
    
    // formatting
    putchar('\n');

    // increment counter
    step++;

    // success
    return 1;
}

void person_print ( void *p_element )
{

    // initialized data
    person *p_person = p_element;

    // print the person
    printf("%-7s -> (%-6s, %s)\n", p_person->_name, p_person->_hair_color, p_person->glasses ? "glasses" : "no glasses");

    // done
    return;
}

void *person_key_accessor ( const void *const p_value )
{

    // initialized data
    const person *p_person = p_value;

    // success
    return (void *)p_person->_name;
}

hash64 person_hash_key ( const void *const k, unsigned long long l )
{

    // initialized data
    char *key = (char *)k;

    // done
    return default_hash(key, l);
}

hash64 person_hash ( const void *const k, unsigned long long l )
{

    // unused
    (void) l;

    // initialized data
    person *p_person = (person *)k;

    // done
    return hash_crc64(p_person, sizeof(person));
}

int person_pack ( stream *p_stream, const void *const p_value )
{

    // initialized data
    person *p_person = (person *)p_value;
    int written = 0;

    // pack the person
    written += pack_pack(p_stream, "%2s%i8", 
        p_person->_name,
        p_person->_hair_color,
        p_person->glasses
    );
    
    // success
    return written;
}

int person_unpack ( void *p_value, stream *p_stream )
{

    // initialized data
    person **pp_person = (person **)p_value;
    person  *p_person  = NULL;
    int written = 0;

    char _name       [16] = { 0 };
    char _hair_color [16] = { 0 };
    bool glasses          = false;

    // unpack the person
    written += pack_unpack(p_stream, "%2s%i8", 
        _name,
        _hair_color,
        &glasses
    );

    // allocate memory for a person
    p_person = default_allocator(0, sizeof(person));

    // copy the name 
    strncpy(p_person->_name, _name, sizeof(p_person->_name));

    // copy the hair color
    strncpy(p_person->_hair_color, _hair_color, sizeof(p_person->_hair_color));

    // store the glasses boolean
    p_person->glasses = glasses;

    // return a pointer to the caller
    *pp_person = p_person;

    // success
    return written;
}
