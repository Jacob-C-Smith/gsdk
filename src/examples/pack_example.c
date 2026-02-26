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

// preprocessor macros
#define HEX_DUMP_COLUMNS 16
#define HEX_DUMP_SPACING 8

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
int pack_checkpoint   ( char (*p_buffer)[64], size_t written, const char *p_event );
int unpack_checkpoint ( size_t read, const char *p_event );

/// person
int person_pack   ( void *p_buffer, person *p_person );
int person_unpack ( person *p_person, void *p_buffer );
int person_print  ( person *p_person );

// static data
static int step = 0;

// data
/// buffers
char _buf[64] = { 0 };
char _person_buffer[1024] = { 0 };

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

    // #0 - start
    checkpoint("start");
    
    // #1 - pack bytes
    {
        
        // pack
        i = pack_pack(&_buf[i], "%3i8", 'a', 'b', 'c');
        
        // checkpoint
        pack_checkpoint(&_buf, i, "pack > bytes");
    }
    
    // #2 - pack shorts
    {
        
        // pack
        i = pack_pack(&_buf[i], "%2i16", 31415, 27182);
        
        // checkpoint
        pack_checkpoint(&_buf, i, "pack > shorts");
    }
    
    // #3 - pack integers
    {
        
        // pack
        i = pack_pack(&_buf[i], "%3i32", 3141592, 2718281, 0x5555AAAA);
        
        // checkpoint
        pack_checkpoint(&_buf, i, "pack > integers");
    }
    
    // #4 - pack longs
    {
        
        // pack
        i = pack_pack(&_buf[i], "%2i64", 1618033988749, 0x0123456789ABCDEF);
        
        // checkpoint
        pack_checkpoint(&_buf, i, "pack > longs");
    }

    // #5 - pack floats
    {
        
        // pack
        i = pack_pack(&_buf[i], "%3f32", 0.125f, 0.25f, 0.5f);
        
        // checkpoint
        pack_checkpoint(&_buf, i, "pack > floats");
    }
    
    // #6 - pack double
    {
        
        // pack
        i = pack_pack(&_buf[i], "%f64", 0.000000000066743);
        
        // checkpoint
        pack_checkpoint(&_buf, i, "pack > double");
    }
    
    // #7 - pack strings
    {

        // pack
        i = pack_pack(&_buf[i], "%3s", "Hello, World!", "Hi Mom!", "Hi Dad");

        // checkpoint
        pack_checkpoint(&_buf, i, "pack > strings");
    }

    // #8 - unpack bytes
    {

        // initialized data
        char _bytes[] = { 'c', 'b', 'a' };
        char a = 0, b = 0, c = 0;

        // unpack
        i = pack_unpack(_bytes, "%3i8", 
            &c, &b, &a
        );

        // checkpoint
        unpack_checkpoint(i, "unpack > bytes");

        // print the reflected data
        printf("%c, %c, %c\n", a, b, c);

        // formatting
        putchar('\n');
    }

    // #9 - unpack shorts
    {

        // initialized data
        short _shorts[] = { 31415, 27182 };
        short s = 0, t = 0;

        // unpack
        i = pack_unpack(_shorts, "%2i16", 
            &s, &t
        );

        // checkpoint
        unpack_checkpoint(i, "unpack > shorts");

        // print the reflected data
        printf("%hi, %hi\n", s, t);

        // formatting
        putchar('\n');
    }

    // #10 - unpack ints
    {

        // initialized data
        int _ints[] = { 3141592, 2718281, 0x5555AAAA };
        int n = 0, o = 0, p = 0;

        // unpack
        i = pack_unpack(_ints, "%3i32", 
            &n, &o, &p
        );

        // checkpoint
        unpack_checkpoint(i, "unpack > ints");

        // print the reflected data
        printf("%i, %i, %i\n", n, o, p);

        // formatting
        putchar('\n');
    }

    // #10 - unpack longs
    {

        // initialized data
        long _longs[] = { 1618033988749, 0x0123456789ABCDEF };
        int l = 0, m = 0;

        // unpack
        i = pack_unpack(_longs, "%2i64", 
            &l, &m
        );

        // checkpoint
        unpack_checkpoint(i, "unpack > longs");

        // print the reflected data
        printf("%lli, %lli\n", l, m);

        // formatting
        putchar('\n');
    }
    
    // #11 - unpack floats
    {

        // initialized data
        float _floats[] = { 0.125f, 0.25f, 0.5f };
        float f = 0, g = 0, h = 0;

        // unpack
        i = pack_unpack(_floats, "%3i32", 
            &f, &g, &h
        );

        // checkpoint
        unpack_checkpoint(i, "unpack > floats");

        // print the reflected data
        printf("%g, %g, %g\n", f, g, h);

        // formatting
        putchar('\n');
    }

    // #12 - unpack double
    {

        // initialized data
        double d = 0.000000000066743;
        double e = 0;

        // unpack
        i = pack_unpack(&d, "%i64", &e);

        // checkpoint
        unpack_checkpoint(i, "unpack > double");

        // print the reflected data
        printf("%lg\n", e);

        // formatting
        putchar('\n');
    }

    // #13 - unpack strings
    {

        // initialized data
        char _b[] = { 5, 0, 'E', 'd', 'd', 'y', 0, 5, 0, 'N', 'a', 'm', 'i', 0 };
        char _eddy[5] = { 0 };
        char _nami[5] = { 0 };

        // unpack
        i = pack_unpack(_b, "%2s", _eddy, _nami);

        // checkpoint
        unpack_checkpoint(i, "unpack > strings");

        // print the reflected data
        printf("%s %s\n", _eddy, _nami);

        // formatting
        putchar('\n');
    }

    // #14 - pack struct
    {

        // pack
        i = person_pack(_person_buffer, &_person);

        // checkpoint
        pack_checkpoint(_person_buffer, i, "pack > person");
    }

    // #15 - unpack struct
    {
        
        // clear the person
        _person = (person){ 0 };

        // unpack
        i = person_unpack(&_person, &_person_buffer);

        // checkpoint
        unpack_checkpoint(i, "unpack > person");

        // print the person
        person_print(&_person);
    }

    // #X - done
    checkpoint("done");
    
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

int pack_checkpoint ( char (*p_buffer)[64], size_t written, const char *p_event )
{
    
    // initialized data
    size_t rows = sizeof(*p_buffer) / HEX_DUMP_COLUMNS;
    
    // print the event
    log_info("#%d - %s (%d bytes)\n", step, p_event, written);

    // print the buffer
    for (size_t i = 0; i < rows; i++)
    {
        
        // print the offset
        printf("%08x: ", i * HEX_DUMP_COLUMNS);

        // print the buffer as hexadecimal
        for (size_t j = 0; j < HEX_DUMP_COLUMNS; j++)
        {
            
            // initialized data
            char c = (*p_buffer)[i*HEX_DUMP_COLUMNS+j];
            
            // color output
            if ( isalnum(c) || ispunct(c) ) printf("\033[32m%hhx", c);
            else printf("%02hhx", c);

            // reset
            printf("\033[0m");

        }

        // formatting
        putchar(' ');

        // print the buffer textually
        for (size_t j = 0; j < HEX_DUMP_COLUMNS; j++)
        {

            // initialized data
            char c = (*p_buffer)[i*HEX_DUMP_COLUMNS+j];

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
    i = 0, memset(_buf, 0, sizeof(*_buf));

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

int person_pack ( void *p_buffer, person *p_person )
{

    // argument check
    if ( NULL == p_person ) goto no_person;

    // initialized data
    char *p = (char *)p_buffer;

    // pack the person
    /// pack the name
    p += pack_pack(p, "%s", p_person->_name);

    /// pack the age and the height
    p += pack_pack(p, "%i8%i16", p_person->age, p_person->height_mm);
    
    // pack the interests
    for (size_t i = 0; i < sizeof(p_person->__interests) / sizeof(*p_person->__interests); i++)
        p += pack_pack(p, "%s", p_person->__interests[i]);

    // success
    return p - (char *)p_buffer;

    // error handling
    no_person:

        // log the error
        log_error("Error: Null pointer provided for \"p_person\" in call to function \"%s\"\n", __FUNCTION__);

        // error
        return 0;
}

int person_unpack ( person *p_person, void *p_buffer )
{

    // argument check
    if ( NULL == p_person ) goto no_person;
    if ( NULL == p_buffer ) goto no_buffer;

    // initialized data
    char *p = (char *)p_buffer;

    // pack the person
    /// pack the name
    p += pack_unpack(p, "%s", &p_person->_name);

    /// pack the age and the height
    p += pack_unpack(p, "%i8%i16", &p_person->age, &p_person->height_mm);
    
    // pack the interests
    for (size_t i = 0; i < sizeof(p_person->__interests) / sizeof(*p_person->__interests); i++)
        p += pack_unpack(p, "%s", &p_person->__interests[i]);

    // success
    return p - (char *)p_buffer;

    // error handling
    no_person:

        // log the error
        log_error("Error: Null pointer provided for \"p_person\" in call to function \"%s\"\n", __FUNCTION__);

        // error
        return 0;
        
    no_buffer:
        
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
    for (size_t i = 0; i < sizeof(p_person->__interests)/sizeof(*p_person->__interests); i++)
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