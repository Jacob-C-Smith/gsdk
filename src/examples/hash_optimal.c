/** !
 * A tool for creating optimally small 
 * hash tables without collisions
 * 
 * @file hash_optimizer.c
 * 
 * @author Jacob Smith
 */

// standard library
#include <stdio.h>
#include <stdlib.h>

// log
#include <core/log.h>

// hash cache
#include <core/hash.h>

// Preprocessor definitions
#define HASH_TABLE_OPTIMIZER_BUFFER_LENGTH_MAX 4095+1
//#define HASH_TABLE_OPTIMIZER_DISPLAY_MODE

// Enumeration definitions
enum hash_table_optimizer_hash_functions_e
{
    HASH_TABLE_OPTIMIZER_CRC64    = 0,
    HASH_TABLE_OPTIMIZER_FNV64    = 1,
    HASH_TABLE_OPTIMIZER_MMH64    = 2,
    HASH_TABLE_OPTIMIZER_QUANTITY = 3
};

// Structure definitions
struct quasi_hash_table_entry_s
{
    bool  occupied;
    char *value;
};

struct quasi_hash_table_s
{
    size_t len;
    struct quasi_hash_table_entry_s data[];
};

struct quasi_hash_table_property_s 
{
    size_t  len;
    hash64  hash;
    char    _text[];
};

// type definitions
typedef struct quasi_hash_table_s          quasi_hash_table;
typedef struct quasi_hash_table_property_s quasi_hash_table_property;

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
 * @param argc               the argc parameter of the entry point
 * @param argv               the argv parameter of the entry point
 * @param ppfn_hash_function result
 * 
 * @return void on success, program abort on failure
 */
void parse_command_line_arguments ( int argc, const char *argv[], fn_hash64 **ppfn_hash_function );

// Entry point
int main ( int argc, const char *argv[] )
{
    
    // Initialized data
    size_t entry_quantity = 0,
           entry_max = 1,
           hash_table_test_size = 0;
    char _buffer[HASH_TABLE_OPTIMIZER_BUFFER_LENGTH_MAX] = { 0 };
    quasi_hash_table *p_quasi_hash_table = (void *) 0;
    fn_hash64 *pfn_hashing_function = (void *) 0;
    quasi_hash_table_property **pp_properties = HASH_CACHE_REALLOC(0, sizeof(void *) * entry_max);
    
    // Error check
    if ( pp_properties == (void *) 0 ) goto failed_to_realloc;

    // Parse command line arguments
    parse_command_line_arguments(argc, argv, &pfn_hashing_function);

    // Read until EOF
    while ( !feof(stdin) )
    {
        
        // Initialized data
        size_t                     len      = 0;
        quasi_hash_table_property *p_string = (void *) 0;

        // Read a string from standard in
        if ( fgets(_buffer, HASH_TABLE_OPTIMIZER_BUFFER_LENGTH_MAX, stdin) == 0 ) break;

        // Resize?
        if ( entry_max == entry_quantity ) 
        {

            // Double the maximum
            entry_max *= 2;

            // Grow the allocation
            pp_properties = HASH_CACHE_REALLOC(pp_properties, sizeof(quasi_hash_table_property *) * entry_max);

            // Error check 
            if ( pp_properties == (void *) 0 ) goto failed_to_realloc; 
        }

        // Compute the length of the entry
        len = strlen(_buffer);

        // Allocate memory for the entry
        p_string = HASH_CACHE_REALLOC(0, sizeof(quasi_hash_table_property) + len * sizeof(char));

        // Error check
        if ( p_string == (void *) 0 ) goto failed_to_realloc;

        // Copy the string from the stack to the heap
        strncpy((char *)p_string->_text, (char *)_buffer, len + 1);

        // Store a null terminator
        p_string->_text[len - 1] = '\0';

        // Store the length of the string on the heap
        p_string->len = len - 1;

        // Compute and store the hash
        p_string->hash = pfn_hashing_function(&p_string->_text, p_string->len);

        // Add the entry to the buffer
        pp_properties[entry_quantity] = p_string;

        // Increment the quantity of elements
        entry_quantity++;
    }
    
    // Store the entry quantity
    hash_table_test_size = entry_quantity;
    
    // Initial allocation
    p_quasi_hash_table = HASH_CACHE_REALLOC(0, sizeof(struct quasi_hash_table_s));

    #ifdef HASH_TABLE_OPTIMIZER_DISPLAY_MODE
        printf("\r%d lines read\n\n", entry_quantity);
    #endif

    // Until all the properties fit without collisions ...
    while ( true )
    {

        // Grow the allocation
        p_quasi_hash_table = HASH_CACHE_REALLOC(p_quasi_hash_table, sizeof(struct quasi_hash_table_s) + sizeof(struct quasi_hash_table_entry_s) * hash_table_test_size);

        // Clear out old results
        memset(p_quasi_hash_table, 0, sizeof(struct quasi_hash_table_s) + sizeof(struct quasi_hash_table_entry_s) * hash_table_test_size);

        // Store the length of the hash table on this iteration
        p_quasi_hash_table->len = hash_table_test_size;

        for (size_t i = 0; i < entry_quantity; i++)
        {
            
            // Initialized data
            size_t index = pp_properties[i]->hash % hash_table_test_size;

            // Test if the bit is occupied
            if ( p_quasi_hash_table->data[index].occupied )
                
                // Try again ...
                goto try_again;
            
            // Set the occupied flag
            p_quasi_hash_table->data[index].occupied = true;
            
            // Store a pointer to this property
            p_quasi_hash_table->data[index].value = (char *) &pp_properties[i]->_text;
        }

        // Display mode 
        #ifdef HASH_TABLE_OPTIMIZER_DISPLAY_MODE
            printf("\rNo collisions at size %d\n\n", hash_table_test_size);
        #endif

        // Done
        break;

        try_again:

            // ... with a larger table
            hash_table_test_size++;

            // Display mode 
            #ifdef HASH_TABLE_OPTIMIZER_DISPLAY_MODE
                printf("\rCollisions at size %d", hash_table_test_size - 1);
                fflush(stdout);
                system("sleep 0.25");
            #endif 

            continue;
    }
    
    // Write the hash table to standard out as an array of C strings 
    {

        // Formatting
        printf("[%zu] = {", p_quasi_hash_table->len);

        // If this entry contains a value ...
        if ( p_quasi_hash_table->data[0].occupied )

            // ... print the value as a string ...
            printf("\"%s\"", p_quasi_hash_table->data[0].value);

        // ... otherwise ...
        else
            
            // ... null pointer
            printf("(const char *) 0");

        // Iterate through the rest of the hash table
        for (size_t i = 1; i < p_quasi_hash_table->len; i++)
        {

            // If this entry contains a value ...
            if ( p_quasi_hash_table->data[i].occupied )
            
                // ... print the value as a string ...
                printf(",\"%s\"", p_quasi_hash_table->data[i].value);
            
            // ... otherwise ...
            else

                // ... null pointer
                printf(",(const char *) 0");
        }

        // Formatting
        printf("};\n");
    }

    // Clean up each property
    for (size_t i = 0; i < entry_quantity; i++)

        // Release each property 
        pp_properties[i] = HASH_CACHE_REALLOC(pp_properties[i], 0);
    
    // Release the property list
    pp_properties = HASH_CACHE_REALLOC(pp_properties, 0);

    // Release the hash table
    p_quasi_hash_table = HASH_CACHE_REALLOC(p_quasi_hash_table, 0);

    // Success
    return EXIT_SUCCESS;

    // Error handling
    {

        // standard library errors
        {
            failed_to_realloc:
                #ifndef NDEBUG
                    log_error("[hash-cache] [hash-optimal] Call to \"realloc\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return EXIT_FAILURE;
        }
    }
}

void print_usage ( const char *argv0 )
{

    // Argument check
    if ( argv0 == (void *) 0 ) exit(EXIT_FAILURE);

    // Print a usage message to standard out
    printf("Usage: %s [ crc | fnv | mmh ]\n\n", argv0);
    printf("    crc   Hash values with cyclic redundancy check\n");
    printf("    fnv   Hash values with Fowler-Noll-Vo hash\n");
    printf("    mmh   Hash values with MurMur hash\n");

    // Done
    return;
}

void parse_command_line_arguments ( int argc, const char *argv[], fn_hash64 **ppfn_hash_function )
{

    // If no command line arguments are supplied, run all the examples
    if ( argc != 2 ) goto invalid_arguments;

    // Cyclic Redundancy Check
    if ( strcmp(argv[1], "crc") == 0 )
        
        // Set the thread pool example flag
        *ppfn_hash_function = hash_crc64;

    // Fowler Noll Vo
    else if ( strcmp(argv[1], "fnv") == 0 )

        // Fowler-Noll-Vo
        *ppfn_hash_function = hash_fnv64;
    
    // MurMur
    else if ( strcmp(argv[1], "mmh") == 0 )

        // MurMur
        *ppfn_hash_function = hash_mmh64;

    // Default
    else goto invalid_arguments;
    
    // Success
    return;
    
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
