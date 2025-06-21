/** !
 * Example hash program
 * 
 * @file hash_example.c
 * 
 * @author Jacob Smith
 */

// standard library
#include <stdio.h>
#include <stdlib.h>

// core
#include <core/log.h>
#include <core/hash.h>

int main ( int argc, const char *argv[] )
{
    
    // Supress warnings
    (void) argc;
    (void) argv;

    // Formatting
    log_info("╭──────────────╮\n");
    log_info("│ hash example │\n");
    log_info("╰──────────────╯\n\n");
    printf(
        "This example hashes a string using many hashing functions. Each hash is\n"\
        "printed to standard out.\n\n"
    );

    // initialized data
    const char _string[] = "Hi mom!";
    hash64 crc64 = hash_crc64(_string, sizeof(_string));
    hash64 mmh64 = hash_mmh64(_string, sizeof(_string));
    hash64 fvn64 = hash_fnv64(_string, sizeof(_string));
    hash64 xxh64 = hash_xxh64(_string, sizeof(_string));

    // Print each hash
    printf("crc64(\"%s\") = 0x%016llX\n", _string, crc64);
    printf("fvn64(\"%s\") = 0x%016llX\n", _string, fvn64);
    printf("mmh64(\"%s\") = 0x%016llX\n", _string, mmh64);
    printf("xxh64(\"%s\") = 0x%016llX\n", _string, xxh64);

    // Formatting
    putchar('\n');

    // success
    return EXIT_SUCCESS;
}
