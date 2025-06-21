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

// core
#include <core/log.h>

// reflection
#include <reflection/pack.h>

// Preprocessor macros
#define HEX_DUMP_COLUMNS 4
#define RESET() (i = 0, p_f = NULL, memset(_buf, 0, sizeof(_buf)))

// Forward declarations
/** !
 * Print raw data
 * 
 * @param p_buf the data
 * @param len   the quantity of bytes to print
 * 
 * @return 1 on success, 0 on error
 */
int print_raw ( void *p_buf, size_t len );

// Entry point
int main ( int argc, const char *argv[] )
{

    // Supress compiler warnings
    (void) argc;
    (void) argv;

    // Formatting
    log_info("╭──────────────╮\n");
    log_info("│ pack example │\n");
    log_info("╰──────────────╯\n");
    printf("The pack library provides abstractions for packing data into a binary format\n");
    printf("Pack provides 2 abstractions. Pack, and unpack.\n");
    printf("Pack is used to serialize data into a binary format.\n");
    printf("Unpack is used to parse binary data into usable data.\n\n");

    // Initialized data
    char _buf[512] = { 0 };
    FILE *p_f = NULL;
    size_t i = 0;

    // Formatting
    log_info("╭─────────────────╮\n");
    log_info("│ general example │\n");
    log_info("╰─────────────────╯\n");

    // General use
    {

        // Initialized data
        char      __strings[3][16] = { 0 };
        char      _chars[3] = { 0 };
        short     s = 0;
        int       z = 0;
        long long l = 0;
        float     _f32x2[2] = { 0 };
        double    _f64x2[2] = { 0 };

        // Pack some data
        i += pack_pack(&_buf[i], "%3s"           , "Hello, World!", "Hi Mom!", "Hi Dad");
        i += pack_pack(&_buf[i], "%3i8"          , 'a', 'b', 'c');
        i += pack_pack(&_buf[i], "%i16 %i32 %i64", 31415, 2718281, 1618033988749);
        i += pack_pack(&_buf[i], "%2f32"         , 3.1415927f, 6.2831855f);
        i += pack_pack(&_buf[i], "%2f64"         , 3.141592653589793, 6.283185307179586);
        
        // Unpack the data
        pack_unpack(_buf, "%3s %3i8 %i16 %i32 %i64 %2f32 %2f64",
            &__strings[0], &__strings[1], &__strings[2],
            &_chars[0]   , &_chars[1]   , &_chars[2],
            &s, &z, &l,
            &_f32x2[0], &_f32x2[1], 
            &_f64x2[0], &_f64x2[1]
        );

        // Print the data
        printf("strings: \"%s\", \"%s\", \"%s\"\n", __strings[0], __strings[1], __strings[2]);
        printf("chars  : '%c', '%c', '%c'\n", _chars[0]   , _chars[1]   , _chars[2]);
        printf("ints   : %hi, %i, %lli\n", s, z, l);
        printf("f32x2  : %.5f, %.5f\n", _f32x2[0], _f32x2[1]);
        printf("f64x2  : %.10lf, %.10lf\n\n", _f64x2[0], _f64x2[1]);

    }

    // Reset
    RESET();

    // // Formatting
    // log_info("╭─────────────────╮\n");
    // log_info("│ packing example │\n");
    // log_info("╰─────────────────╯\n");

    // // Pack a struct 
    // {
        
    //     // Initialized data
    //     vec3 _vec3 = { .x = 4.f, .y = 3.f, .z = 5.f };
 
    //     // Formatting
    //     printf("pack( vec3(");
    //     vec3_print(_vec3);
    //     printf(") )");

    //     // Open a file
    //     p_f = fopen("../vec3.bin", "w+");

    //     // Pack
    //     i = pack("vec3", &_buf, &_vec3);

    //     // Write the binary data to the output file
    //     fwrite(&_buf, 1, i, p_f);

    //     // Close the file
    //     fclose(p_f);

    //     // Formatting
    //     printf(" ->\n");
    //     print_raw(_buf, i);
    // }

    // // Reset
    // RESET();

    // // Pack a nested struct
    // {

    //     // Initialized data
    //     transform _transform = 
    //     {
    //         .loc = { .x = 4.f , .y =   3.f, .z = 5.f },
    //         .rot = { .x = 90.f, .y = -30.f, .z = -45.f },
    //         .sca = { .x = 1.f , .y =   1.f, .z = 1.f }
    //     };
        
    //     // Formatting
    //     printf("pack( transform(");
    //     transform_print(_transform);
    //     printf(") )");

    //     // Open a file
    //     p_f = fopen("../transform.bin", "w+");

    //     // Pack
    //     i = pack("transform", &_buf, &_transform);

    //     // Write the binary data to the output file
    //     fwrite(&_buf, 1, i, p_f);

    //     // Close the file
    //     fclose(p_f);

    //     // Formatting
    //     printf(" ->\n");
    //     print_raw(_buf, i);
    // }

    // // Reset
    // RESET();

    // // Formatting
    // log_info("╭───────────────────╮\n");
    // log_info("│ unpacking example │\n");
    // log_info("╰───────────────────╯\n");

    // // Unpack a struct 
    // {
        
    //     // Initialized data
    //     vec3 _vec3 = { 0 };

    //     // Open a file
    //     p_f = fopen("../vec3.bin", "rw");

    //     // Read the binary data into a buffer
    //     fread(&_buf, 1, sizeof(_buf), p_f);
        
    //     // Unpack
    //     (void) unpack("vec3", &_vec3, &_buf);

    //     // Format
    //     printf ("unpack( \"vec3.bin\" ) ->\n");
    //     vec3_print(_vec3);
    //     putchar('\n');
    //     putchar('\n');

    //     // Close the file
    //     fclose(p_f);
    // }

    // // Reset
    // RESET();

    // // Unpack a nested struct 
    // {

    //     // Initialized data
    //     transform _transform = { 0 };
        
    //     // Open a file
    //     p_f = fopen("../transform.bin", "rw");

    //     // Read the binary data into a buffer
    //     fread(&_buf, 1, sizeof(_buf), p_f);

    //     // Unpack
    //     int r = unpack("transform", &_transform, &_buf);

    //     // Format
    //     printf ("unpack( \"transform.bin\" ) ->\n");
    //     transform_print(_transform);
    //     putchar('\n');

    //     // Close the file
    //     fclose(p_f);
    // }

    // Success
    return EXIT_SUCCESS;

}

int print_raw ( void *p_buf, size_t len )
{

    // Argument check
    if ( p_buf == (void *) 0 ) goto no_buffer;

    // Initialized data
    unsigned char *p_data = p_buf;

    // Print the data
    for (size_t i = 0; i < (len << 1); i++)
    {

        // Initialized data
        bool          lo_hi = ( i % 2 );
        unsigned char _data = (*p_data) >> (lo_hi * 4);

        // Column spacing
        if ( i % (HEX_DUMP_COLUMNS << 1) == 0 && i != 0 ) 
            putchar('\n');

        // Print the character
        switch ( _data & 0xF )
        {
            case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9:
                putchar('0' + ( _data & 0xF ));
                break;

            case 0xA: case 0xB: case 0xC: case 0xD: case 0xE: case 0xF:
                putchar('A' - 10 + ( _data & 0xF )); 
                break;
        }

        // Next byte
        if ( lo_hi )

            // Increment
            p_data++, putchar(' ');
    }
    
    // Formatting
    putchar('\n');
    putchar('\n');

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_buffer:
                #ifndef NDEBUG
                    log_error("Null pointer provided for parameter \"p_buf\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}
