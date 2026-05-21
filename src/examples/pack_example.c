/** !
 * Pack example
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

// gsdk
/// core
#include <core/log.h>
#include <core/pack.h>
#include <core/stream.h>

// preprocessor macros
#define HEX_DUMP_COLUMNS 16
#define HEX_DUMP_SPACING 8

// enumeration definitions
enum pack_example_e
{
    PACK_BYTES,
    PACK_SHORTS,
    PACK_INTS,
    PACK_FLOATS,
    PACK_LONGS,
    PACK_DOUBLES,
    PACK_STRINGS,
    PACK_STRUCT,
    PACK_QUANTITY,
};

// type definitions
typedef int (fn_unpack_log)(void *p_value);
typedef struct
{
    char _name[15+1];
    unsigned char age;
    unsigned short height_mm;
    char __interests[3][32];
} person;

// forward declarations
/// logs
int checkpoint        ( const char *p_event );
int pack_checkpoint   ( stream *p_stream, size_t written, const char *p_event );
int unpack_checkpoint ( size_t read, const char *p_event );

/// person
fn_pack person_pack;
fn_unpack person_unpack;
int person_print  ( person *p_person );

// static data
static int step = 0;

// data
/// stream
stream *_p_streams[PACK_QUANTITY] = 
{
    [PACK_BYTES]   = NULL,
    [PACK_SHORTS]  = NULL,
    [PACK_INTS]    = NULL,
    [PACK_FLOATS]  = NULL,
    [PACK_LONGS]   = NULL,
    [PACK_DOUBLES] = NULL,
    [PACK_STRINGS] = NULL,
    [PACK_STRUCT]  = NULL,
};

/// counters
size_t i = 0;

/// person
person _person = 
{
    ._name = "Jacob",
    .age = 23,
    .height_mm = 1829,
    .__interests = 
    {
        [0] = "programming",
        [1] = "organic chemistry",
        [2] = "sociology"
    }
};

// entry point
int main ( int argc, const char *argv[] )
{

    // unused
    (void) argc;
    (void) argv;

    // construct streams
    for (size_t i = 0; i < PACK_QUANTITY; i++)

        // construct a stream
        stream_from_dynamic_buffer(&_p_streams[i]);

    // #0 - start
    checkpoint("start");

    // #1 - pack bytes
    {
        
        // pack
        i = pack_pack(_p_streams[PACK_BYTES], "%3i8", 'a', 'b', 'c');
        
        // checkpoint
        pack_checkpoint(_p_streams[PACK_BYTES], i, "pack > bytes");
    }

    // #2 - pack shorts
    {
        
        // pack
        i = pack_pack(_p_streams[PACK_SHORTS], "%2i16", 31415, 27182);
        
        // checkpoint
        pack_checkpoint(_p_streams[PACK_SHORTS], i, "pack > shorts");
    }
    
    // #3 - pack integers
    {
        
        // pack
        i = pack_pack(_p_streams[PACK_INTS], "%2i32", 0x5555AAAA, 0xDEADBEEF);
        
        // checkpoint
        pack_checkpoint(_p_streams[PACK_INTS], i, "pack > integers");
    }
    
    // #4 - pack floats
    {
        
        // pack
        i = pack_pack(_p_streams[PACK_FLOATS], "%3f32", 0.125f, 0.25f, 0.5f);
        
        // checkpoint
        pack_checkpoint(_p_streams[PACK_FLOATS], i, "pack > floats");
    }
    
    // #5 - pack longs
    {
        
        // pack
        i = pack_pack(_p_streams[PACK_LONGS], "%2i64", 1618033988749, 0x0123456789ABCDEF);
        
        // checkpoint
        pack_checkpoint(_p_streams[PACK_LONGS], i, "pack > longs");
    }

    // #6 - pack doubles
    {
        
        // pack
        i = pack_pack(_p_streams[PACK_DOUBLES], "%2f64", 0.000000000066743, 123456789.9876543);
        
        // checkpoint
        pack_checkpoint(_p_streams[PACK_DOUBLES], i, "pack > double");
    }
    
    // #7 - pack strings
    {

        // pack
        i = pack_pack(_p_streams[PACK_STRINGS], "%3s", "Hello, World!", "Hi Mom!", "Hi Dad!");

        // checkpoint
        pack_checkpoint(_p_streams[PACK_STRINGS], i, "pack > strings");
    }

    // #8 - pack struct
    {

        // pack
        i = person_pack(_p_streams[PACK_STRUCT], &_person);

        // checkpoint
        pack_checkpoint(_p_streams[PACK_STRUCT], i, "pack > person");
    }

    // #9 - seek start
    {

        // iterate through each stream
        for (size_t i = 0; i < PACK_QUANTITY; i++)
            
            // seek start 
            stream_seek(_p_streams[i], 0, STREAM_SEEK_SET);

        // checkpoint
        checkpoint("seek > start");
    }

    // #10 - unpack bytes
    {

        // initialized data
        char a = 0, b = 0, c = 0;

        // unpack
        i = pack_unpack(_p_streams[PACK_BYTES], "%3i8", 
            &a, &b, &c
        );

        // checkpoint
        unpack_checkpoint(i, "unpack > bytes");

        // print the reflected data
        printf("%c, %c, %c\n", a, b, c);

        // formatting
        putchar('\n');
    }

    // #11 - unpack shorts
    {

        // initialized data
        short s = 0, t = 0;

        // unpack
        i = pack_unpack(_p_streams[PACK_SHORTS], "%2i16", 
            &s, &t
        );

        // checkpoint
        unpack_checkpoint(i, "unpack > shorts");

        // print the reflected data
        printf("%hi, %hi\n", s, t);

        // formatting
        putchar('\n');
    }

    // #12 - unpack ints
    {

        // initialized data
        int n = 0, o = 0;

        // unpack
        i = pack_unpack(_p_streams[PACK_INTS], "%2i32", 
            &n, &o
        );

        // checkpoint
        unpack_checkpoint(i, "unpack > ints");

        // print the reflected data
        printf("0x%x, 0x%x\n", n, o);

        // formatting
        putchar('\n');
    }

    // #13 - unpack longs
    {

        // initialized data
        long l = 0, m = 0;

        // unpack
        i = pack_unpack(_p_streams[PACK_LONGS], "%2i64", 
            &l, &m
        );

        // checkpoint
        unpack_checkpoint(i, "unpack > longs");

        // print the reflected data
        printf("%ld, %ld\n", l, m);

        // formatting
        putchar('\n');
    }
    
    // #14 - unpack floats
    {

        // initialized data
        float f = 0, g = 0, h = 0;

        // unpack
        i = pack_unpack(_p_streams[PACK_FLOATS], "%3i32", 
            &f, &g, &h
        );

        // checkpoint
        unpack_checkpoint(i, "unpack > floats");

        // print the reflected data
        printf("%g, %g, %g\n", f, g, h);

        // formatting
        putchar('\n');
    }

    // #15 - unpack double
    {

        // initialized data
        double d = 0;
        double e = 0;

        // unpack
        i = pack_unpack(_p_streams[PACK_DOUBLES], "%2i64", &d, &e);

        // checkpoint
        unpack_checkpoint(i, "unpack > double");

        // print the reflected data
        printf("%lg, %lg\n", d, e);

        // formatting
        putchar('\n');
    }

    // #16 - unpack strings
    {

        // initialized data
        char _hello_world[14] = { 0 };
        char _hi_mom[8] = { 0 };
        char _hi_dad[8] = { 0 };

        // unpack
        i = pack_unpack(_p_streams[PACK_STRINGS], "%3s", _hello_world, _hi_mom, _hi_dad);

        // checkpoint
        unpack_checkpoint(i, "unpack > strings");

        // print the reflected data
        printf("\"%s\", \"%s\", \"%s\"\n", _hello_world, _hi_mom, _hi_dad);

        // formatting
        putchar('\n');
    }

    // #17 - unpack struct
    {
        
        // clear the person
        _person = (person){ 0 };

        // unpack
        i = person_unpack(&_person, _p_streams[PACK_STRUCT]);

        // checkpoint
        unpack_checkpoint(i, "unpack > person");

        // print the person
        person_print(&_person);
    }

    // #18 - done
    checkpoint("done");
    
    // release streams
    for (size_t i = 0; i < PACK_QUANTITY; i++)

        // release the stream
        stream_destroy(&_p_streams[i]);

    // success
    return EXIT_SUCCESS;
}

int checkpoint ( const char *p_event )
{
    
    // print the event
    log_info("#%d - %s\n", step, p_event);

    // increment counter
    step++;
    
    // success
    return 1;
}

int pack_checkpoint ( stream *p_stream, size_t written, const char *p_event )
{
    
    // initialized data
    int rows = (written % 16) ? 1 + (written / HEX_DUMP_COLUMNS) : (written / HEX_DUMP_COLUMNS);
    
    // seek start
    stream_seek(p_stream, 0, STREAM_SEEK_SET);

    // print the event
    log_info("#%d - %s (%d bytes)\n", step, p_event, written);

    // print the buffer
    for (int i = 0; i < rows; i++)
    {

        // initialized data
        char _buf[HEX_DUMP_COLUMNS] = { 0 };

        stream_read(p_stream, _buf, sizeof(_buf));

        // print the offset
        printf("%08x: ", i * HEX_DUMP_COLUMNS);

        // print the buffer as hexadecimal
        for (int j = 0; j < HEX_DUMP_COLUMNS; j++)
        {
            
            // initialized data
            char c = _buf[j];
            
            // color output
            if ( isalnum(c) || ispunct(c) ) printf("\033[32m%hhx", c);
            else printf("%02hhx", c);

            // reset
            printf("\033[0m");

        }

        // formatting
        putchar(' ');

        // print the buffer textually
        for (int j = 0; j < HEX_DUMP_COLUMNS; j++)
        {

            // initialized data
            char c = _buf[j];

            // color output
            if ( isalnum(c) || ispunct(c) ) printf("\033[32m"), putchar(c);
            else if ( isprint(c) ) putchar(c);
            else putchar('.');

            // reset
            printf("\033[0m");
        }        

        // formatting
        putchar('\n');
    }
    
    // formatting
    putchar('\n');

    // reset
    i = 0;

    // seek back
    stream_seek(p_stream, 0, STREAM_SEEK_SET);

    // increment counter
    step++;
    
    // success
    return 1;
}

int unpack_checkpoint ( size_t read, const char *p_event )
{
        
    // print the event
    log_info("#%d - %s (%d bytes)\n", step, p_event, read);

    // increment counter
    step++;
    
    // success
    return 1;
}

int person_pack ( stream *p_stream, const void *const p_value )
{

    // argument check
    if ( NULL == p_value ) goto no_person;

    // initialized data
    size_t written = 0;
    const person *const p_person = (const person *const)p_value;
    
    // pack the person
    /// pack the name
    written += pack_pack(p_stream, "%s", p_person->_name);

    /// pack the age and the height
    written += pack_pack(p_stream, "%i8%i16", p_person->age, p_person->height_mm);
    
    // pack the interests
    for (size_t i = 0; i < sizeof(p_person->__interests) / sizeof(*p_person->__interests); i++)
        written += pack_pack(p_stream, "%s", p_person->__interests[i]);

    // success
    return written;

    // error handling
    no_person:

        // log the error
        log_error("Error: Null pointer provided for \"p_person\" in call to function \"%s\"\n", __FUNCTION__);

        // error
        return 0;
}

int person_unpack ( void *p_value, stream *p_stream )
{

    // argument check
    if ( NULL ==  p_value ) goto no_value;
    if ( NULL == p_stream ) goto no_stream;

    // initialized data
    size_t  read     = 0;
    person *p_person = (person *)p_value;

    // pack the person
    /// pack the name
    read += pack_unpack(p_stream, "%s", &p_person->_name);

    /// pack the age and the height
    read += pack_unpack(p_stream, "%i8%i16", &p_person->age, &p_person->height_mm);
    
    // pack the interests
    for (size_t i = 0; i < sizeof(p_person->__interests) / sizeof(*p_person->__interests); i++)
        read += pack_unpack(p_stream, "%s", &p_person->__interests[i]);

    // success
    return read;

    // error handling
    no_value:

        // log the error
        log_error("Error: Null pointer provided for \"p_value\" in call to function \"%s\"\n", __FUNCTION__);

        // error
        return 0;
        
    no_stream:
        
        // log the error
        log_error("Error: Null pointer provided for \"p_buffer\" in call to function \"%s\"\n", __FUNCTION__);

        // error
        return 0;
        
}

int person_print ( person *p_person )
{
    
    // argument check
    if ( NULL == p_person ) goto no_person;

    // print the person
    /// print the name
    printf("Name: %s\n", p_person->_name);

    /// print the age and height
    printf("Age: %hhi\nHeight: %himm\n", p_person->age, p_person->height_mm);

    /// print the interests
    for (int i = 0; i < (int)(sizeof(p_person->__interests)/sizeof(*p_person->__interests)); i++)
        printf("Interest #%i: %s\n", i, p_person->__interests[i]);

    // formatting
    putchar('\n');

    // success
    return 1;

    // error handling
    no_person:

        // log the error
        log_error("Error: Null pointer provided for \"p_person\" in call to function \"%s\"\n", __FUNCTION__);

        // error
        return 0;
}