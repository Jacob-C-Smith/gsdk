/** !
 * Example hash cache program
 * 
 * @file main.c
 * 
 * @author Jacob Smith
 */

// standard library
#include <stdio.h>
#include <stdlib.h>

// log
#include <core/log.h>

// hash cache
#include <core/hash_cache.h>
#include <core/hash.h>
#include <core/cache.h>
#include <core/hash_table.h>

// Enumeration definitions
enum hash_cache_examples_e
{
    HASH_CACHE_HASH_EXAMPLE       = 0,
    HASH_CACHE_CACHE_EXAMPLE      = 1,
    HASH_CACHE_HASH_TABLE_EXAMPLE = 2,
    HASH_CACHE_EXAMPLES_QUANTITY  = 3
};

// Structure definitions
struct word_frequency_s
{
    char   _word[16];
    size_t frequency;
};

// type definitions
typedef struct word_frequency_s word_frequency;

// Forward declarations
/** !
 * Print a usage message to standard out
 * 
 * @param argv0 the name of the program
 * 
 * @return void
 */
void print_usage ( const char *argv0 );

/** !
 * Parse command line arguments
 * 
 * @param argc            the argc parameter of the entry point
 * @param argv            the argv parameter of the entry point
 * @param examples_to_run return
 * 
 * @return void on success, program abort on failure
 */
void parse_command_line_arguments ( int argc, const char *argv[], bool *examples_to_run );

/** !
 * Hash example program
 * 
 * @param argc the argc parameter of the entry point
 * @param argv the argv parameter of the entry point
 * 
 * @return 1 on success, 0 on error
 */
int hash_cache_hash_example ( int argc, const char *argv[] );

/** !
 * Cache example program
 * 
 * @param argc the argc parameter of the entry point
 * @param argv the argv parameter of the entry point
 * 
 * @return 1 on success, 0 on error
 */
int hash_cache_cache_example ( int argc, const char *argv[] );

/** !
 * Hash table example program
 * 
 * @param argc the argc parameter of the entry point
 * @param argv the argv parameter of the entry point
 * 
 * @return 1 on success, 0 on error
 */
int hash_cache_hash_table_example ( int argc, const char *argv[] );

/** !
 * Get the key from a word frequency struct
 * 
 * @param p_word_frequency pointer to the word frequency struct
 * 
 * @return 1 on success, 0 on error
 */
char *hash_cache_word_frequency_key_get ( const word_frequency *const p_word_frequency );

/** !
 * Print a word frequency struct to standard out
 * 
 * @param p_word_frequency pointer to word frequency struct
 * @param i                control variable
 * 
 * @return 1 on success, 0 on error
 */
int hash_cache_word_frequency_print_i ( const word_frequency *const p_word_frequency, size_t i );

// Data
size_t total_words = 0;

// Entry point
int main ( int argc, const char *argv[] )
{
    
    // Initialized data
    bool examples_to_run[HASH_CACHE_EXAMPLES_QUANTITY] = { 0 };

    // Parse command line arguments
    parse_command_line_arguments(argc, argv, examples_to_run);

    // Formatting
    log_info("╔════════════════════╗\n");
    log_info("║ hash cache example ║\n");
    log_info("╚════════════════════╝\n");
    printf(
        "The hash cache library provides hashing functions and caching abstractions.\n"\
        "Hash cache provides %d abstractions. Hashing functions, the cache, and the\n"\
        "hash table.\n\n"\
        "Hashing functions take some length of data, and produce a unique integer value.\n"\
        "A cache stores objects for quick access using a most recently used scheme.\n"
        "A hash table is used to access properties in constant time.\n\n",
        HASH_CACHE_EXAMPLES_QUANTITY
    );

    //////////////////////
    // Run the examples //
    //////////////////////

    // Run the hashing example program
    if ( examples_to_run[HASH_CACHE_HASH_EXAMPLE] )

        // Error check
        if ( hash_cache_hash_example(argc, argv) == EXIT_FAILURE ) goto failed_to_run_hash_example;

    // Run the cache example program
    if ( examples_to_run[HASH_CACHE_CACHE_EXAMPLE] )

        // Error check
        if ( hash_cache_cache_example(argc, argv) == EXIT_FAILURE ) goto failed_to_run_cache_example;

    // Run the hash table program
    if ( examples_to_run[HASH_CACHE_HASH_TABLE_EXAMPLE] )

        // Error check
        if ( hash_cache_hash_table_example(argc, argv) == EXIT_FAILURE ) goto failed_to_run_hash_table_example;

    // Success
    return EXIT_SUCCESS;

    // Error handling
    {
        failed_to_run_hash_example:

            // Print an error message
            log_error("Failed to run hash example!\n");

            // Error
            return EXIT_FAILURE;

        failed_to_run_cache_example:

            // Print an error message
            log_error("Failed to run cache example!\n");

            // Error
            return EXIT_FAILURE;

        failed_to_run_hash_table_example:

            // Print an error message
            log_error("Failed to run hash table example!\n");

            // Error
            return EXIT_FAILURE;
    }
}

void print_usage ( const char *argv0 )
{

    // Argument check
    if ( argv0 == (void *) 0 ) exit(EXIT_FAILURE);

    // Print a usage message to standard out
    printf("Usage: %s [hash] [cache] [hash-table]\n", argv0);

    // Done
    return;
}

void parse_command_line_arguments ( int argc, const char *argv[], bool *examples_to_run )
{

    // If no command line arguments are supplied, run all the examples
    if ( argc == 1 ) goto all_examples;

    // Error check
    if ( argc > HASH_CACHE_EXAMPLES_QUANTITY + 1 ) goto invalid_arguments;

    // Iterate through each command line argument
    for (int i = 1; i < argc; i++)
    {
        
        // Hash example?
        if ( strcmp(argv[i], "hash") == 0 )
            
            // Set the thread pool example flag
            examples_to_run[HASH_CACHE_HASH_EXAMPLE] = true;

        // Cache example?
        else if ( strcmp(argv[i], "cache") == 0 )

            // Set the schedule example flag
            examples_to_run[HASH_CACHE_CACHE_EXAMPLE] = true;
        
        // Hash table example?
        else if ( strcmp(argv[i], "hash-table") == 0 )

            // Set the schedule example flag
            examples_to_run[HASH_CACHE_HASH_TABLE_EXAMPLE] = true;

        // Default
        else goto invalid_arguments;
    }
    
    // Success
    return;

    // Set each example flag
    all_examples:
    {

        // For each example ...
        for (size_t i = 0; i < HASH_CACHE_EXAMPLES_QUANTITY; i++)
        
            // ... set the example flag
            examples_to_run[i] = true;
        
        // Success
        return;
    }

    // Error handling
    {

        // Argument errors
        {
            invalid_arguments:
                
                // Print a usage message to standard out
                print_usage(argv[0]);

                // Abort
                exit(EXIT_FAILURE);
        }
    }
}

int hash_cache_hash_example ( int argc, const char *argv[] )
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

    // Initialized data
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

    // Success
    return EXIT_SUCCESS;

    // Error handling
    { }
}

int hash_cache_cache_example ( int argc, const char *argv[] )
{

    // Supress warnings
    (void) argc;
    (void) argv;

    // Formatting
    log_info("╭───────────────╮\n");
    log_info("│ cache example │\n");
    log_info("╰───────────────╯\n\n");
    printf(
        "In this example, a plain text file is read word by word. Each word is inserted \n"\
        "into a cache. The cache can hold 100 entries total. The file contains 169 unique\n"\
        "words, and 1,000,000 total words. After each word has been inserted, some cache\n"\
        "statistics are printed to standard out.\n\n"
    );

    // Initialized data
    cache *p_cache = (void *) 0;
    FILE  *p_file  = fopen("resources/core/500k.txt", "r");

    // Error check
    if ( p_file == (void *) 0 ) goto failed_to_open_lorem_ipsum;

    // Construct the cache
    if ( cache_construct(&p_cache, 160, (fn_equality *)strcmp, (fn_key_accessor *)hash_cache_word_frequency_key_get) == 0 ) goto failed_to_construct_cache;

    // Read each word
    while ( feof(p_file) == false )
    {

        // Initialized data
        char _word[16] = { 0 };
        word_frequency *p_word_frequency = (void *) 0;
        int r = 0;

        // Read the word
        r = fscanf(p_file, "%s ", (char *)&_word);

        // Done?
        if ( r == EOF ) continue;

        // Search the cache
        cache_get(p_cache, _word, (void **)&p_word_frequency);

        // Hit
        if ( p_word_frequency )
            p_word_frequency->frequency++;
        
        // Miss
        else
        {
            
            // Construct a cache entry
            p_word_frequency            = malloc(sizeof(word_frequency)),
            p_word_frequency->frequency = 1,
            strncpy(p_word_frequency->_word, _word, 16);

            // Insert the word into the cache
            cache_insert(p_cache, p_word_frequency->_word, p_word_frequency);
        }
    }

    for (size_t i = 0; i < p_cache->properties.max; i++)
    {
        word_frequency *p_wf = p_cache->properties.pp_data[i];
        if ( p_cache->properties.pp_data[i] ) printf("[%zu] %zu:%s\n",i , p_wf->frequency, p_wf->_word);
    }
    
    // Success
    return EXIT_SUCCESS;
    
    // Error handling
    {

        // Hash cache errors
        {
            failed_to_construct_cache:
                #ifndef NDEBUG
                    log_error("[hash cache] [cache] Failed to construct cache in call to function \"%s\"\n", __FUNCTION__);
                #endif
            
                // Error
                return 0;
        }

        // standard library errors
        {
            failed_to_open_lorem_ipsum:
                #ifndef NDEBUG
                    log_error("[hash cache] [cache] Failed to open \"lorem_ipsum.txt\" in call to function \"%s\"\n", __FUNCTION__);
                    log_info("Have you extracted \"lorem_ipsum.txt\" to the \"build\" directory?\n");
                #endif
            
                // Error
                return 0;
        }
    }
}

int hash_cache_hash_table_example ( int argc, const char *argv[] )
{
    
    // Supress warnings
    (void) argc;
    (void) argv;

    // Formatting
    log_info("╭────────────────────╮\n");
    log_info("│ hash table example │\n");
    log_info("╰────────────────────╯\n\n");
    printf(
        "In this example [TODO] \n\n"\
    );

    // Initialized data
    //hash_table *p_hash_table = (void *) 0;

    // Construct the hash table
    //

    // 

    // Success
    return EXIT_SUCCESS;
    
    // Error handling
    {

        // Hash cache errors
        {
            /*
            failed_to_construct_hash_table:
                #ifndef NDEBUG
                    log_error("[hash cache] [hash table] Failed to construct hash table in call to function \"%s\"\n", __FUNCTION__);
                #endif
            
                // Error
                return 0;
            */
        }
    }
}

int hash_cache_word_frequency_print_i ( const word_frequency *const p_word_frequency, size_t i )
{

    // Unused
    (void) i;

    // Print the word frequency struct to standard out
    printf("| %-12s | %5zu |\n", p_word_frequency->_word, p_word_frequency->frequency);

    // Accumulate total words
    total_words += p_word_frequency->frequency;

    // Success
    return 1;
}

char *hash_cache_word_frequency_key_get ( const word_frequency *const p_word_frequency )
{

    // Argument check
    if ( p_word_frequency == (void *) 0 ) goto no_word_frequency;

    // Success
    return ((word_frequency *)p_word_frequency)->_word;

    // Error handling
    {

        // Argument errors
        {
            no_word_frequency:
                #ifndef NDEBUG
                    log_error("[hash cache] [cache] Null pointer provided for parameter \"p_word_frequency\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}
