/** !
 * A tool for creating optimally small hash tables without collisions
 * 
 * @file hash_optimizer.c
 * 
 * @author Jacob Smith
 */

// standard library
#include <stdio.h>
#include <stdlib.h>

// core
#include <core/log.h>
#include <core/hash.h>
#include <core/sha.h>

// preprocessor definitions
// #define HASH_TABLE_OPTIMIZER_DISPLAY_MODE
#define HASH_TABLE_OPTIMIZER_BUFFER_LENGTH_MAX 4095+1

// structure definitions
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

// forward declarations
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

// entry point
int main ( int argc, const char *argv[] )
{
    
    // initialized data
    size_t entry_quantity = 0,
           entry_max = 1,
           hash_table_test_size = 0;
    char _buffer[HASH_TABLE_OPTIMIZER_BUFFER_LENGTH_MAX] = { 0 };
    quasi_hash_table *p_quasi_hash_table = (void *) 0;
    fn_hash64 *pfn_hashing_function = (void *) 0;
    quasi_hash_table_property **pp_properties = default_allocator(0, sizeof(void *) * entry_max);
    
    // error check
    if ( pp_properties == (void *) 0 ) goto failed_to_default_allocator;

    // parse command line arguments
    parse_command_line_arguments(argc, argv, &pfn_hashing_function);

    // read until EOF
    while ( !feof(stdin) )
    {
        
        // initialized data
        size_t                     len      = 0;
        quasi_hash_table_property *p_string = (void *) 0;

        // read a string from standard in
        if ( fgets(_buffer, HASH_TABLE_OPTIMIZER_BUFFER_LENGTH_MAX, stdin) == 0 ) break;

        // resize?
        if ( entry_max == entry_quantity ) 
        {

            // double the maximum
            entry_max *= 2;

            // grow the allocation
            pp_properties = default_allocator(pp_properties, sizeof(quasi_hash_table_property *) * entry_max);

            // error check 
            if ( pp_properties == (void *) 0 ) goto failed_to_default_allocator; 
        }

        // compute the length of the entry
        len = strlen(_buffer);

        // allocate memory for the entry
        p_string = default_allocator(0, sizeof(quasi_hash_table_property) + len * sizeof(char) + 1);

        // error check
        if ( p_string == (void *) 0 ) goto failed_to_default_allocator;

        // copy the string from the stack to the heap
        strncpy((char *)p_string->_text, (char *)_buffer, len + 1);

        // store a null terminator
        p_string->_text[len - 1] = '\0';

        // store the length of the string on the heap
        p_string->len = len - 1;

        // compute and store the hash
        p_string->hash = pfn_hashing_function(&p_string->_text, p_string->len);

        // add the entry to the buffer
        pp_properties[entry_quantity] = p_string;

        // increment the quantity of elements
        entry_quantity++;
    }
    
    // store the entry quantity
    hash_table_test_size = entry_quantity;
    
    // initial allocation
    p_quasi_hash_table = default_allocator(0, sizeof(struct quasi_hash_table_s));

    #ifdef HASH_TABLE_OPTIMIZER_DISPLAY_MODE
        printf("\r%zu lines read\n\n", entry_quantity);
    #endif

    // until all the properties fit without collisions ...
    while ( true )
    {

        // grow the allocation
        p_quasi_hash_table = default_allocator(p_quasi_hash_table, sizeof(struct quasi_hash_table_s) + sizeof(struct quasi_hash_table_entry_s) * hash_table_test_size);

        // clear out old results
        memset(p_quasi_hash_table, 0, sizeof(struct quasi_hash_table_s) + sizeof(struct quasi_hash_table_entry_s) * hash_table_test_size);

        // store the length of the hash table on this iteration
        p_quasi_hash_table->len = hash_table_test_size;

        for (size_t i = 0; i < entry_quantity; i++)
        {
            
            // initialized data
            size_t index = pp_properties[i]->hash % hash_table_test_size;

            // test if the bit is occupied
            if ( p_quasi_hash_table->data[index].occupied )
                
                // try again ...
                goto try_again;
            
            // set the occupied flag
            p_quasi_hash_table->data[index].occupied = true;
            
            // store a pointer to this property
            p_quasi_hash_table->data[index].value = (char *) &pp_properties[i]->_text;
        }

        // display mode 
        #ifdef HASH_TABLE_OPTIMIZER_DISPLAY_MODE
            printf("\rNo collisions at size %zu\n\n", hash_table_test_size);
        #endif

        // done
        break;

        try_again:

            // ... with a larger table
            hash_table_test_size++;

            // display mode 
            #ifdef HASH_TABLE_OPTIMIZER_DISPLAY_MODE
                printf("\rCollisions at size %zu", hash_table_test_size - 1);
                fflush(stdout);
                system("sleep 0.25");
            #endif 

            continue;
    }
    
    // write the hash table to standard out as an array of C strings 
    {

        // formatting
        printf("[%zu] = {", p_quasi_hash_table->len);

        // if this entry contains a value ...
        if ( p_quasi_hash_table->data[0].occupied )

            // ... print the value as a string ...
            printf("\"%s\"", p_quasi_hash_table->data[0].value);

        // ... otherwise ...
        else
            
            // ... null pointer
            printf("(const char *) 0");

        // iterate through the rest of the hash table
        for (size_t i = 1; i < p_quasi_hash_table->len; i++)
        {

            // if this entry contains a value ...
            if ( p_quasi_hash_table->data[i].occupied )
            
                // ... print the value as a string ...
                printf(",\"%s\"", p_quasi_hash_table->data[i].value);
            
            // ... otherwise ...
            else

                // ... null pointer
                printf(",(const char *) 0");
        }

        // formatting
        printf("};\n");
    }

    // clean up each property
    for (size_t i = 0; i < entry_quantity; i++)

        // release each property 
        pp_properties[i] = default_allocator(pp_properties[i], 0);
    
    // release the property list
    pp_properties = default_allocator(pp_properties, 0);

    // release the hash table
    p_quasi_hash_table = default_allocator(p_quasi_hash_table, 0);

    // success
    return EXIT_SUCCESS;

    // error handling
    {

        // standard library errors
        {
            failed_to_default_allocator:
                #ifndef NDEBUG
                    log_error("[hash-cache] [hash-optimal] Call to \"default_allocator\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return EXIT_FAILURE;
        }
    }
}

void print_usage ( const char *argv0 )
{

    // argument check
    if ( argv0 == (void *) 0 ) exit(EXIT_FAILURE);

    // print a usage message to standard out
    printf("Usage: %s [ crc | fnv | mmh | sha ]\n\n", argv0);
    printf("    crc   Hash values with cyclic redundancy check\n");
    printf("    fnv   Hash values with Fowler-Noll-Vo hash\n");
    printf("    mmh   Hash values with MurMur hash\n");
    printf("    sha   Hash values with SHA-256 hash (truncates to 64-bit hash)\n");

    // done
    return;
}

void parse_command_line_arguments ( int argc, const char *argv[], fn_hash64 **ppfn_hash_function )
{

    // if no command line arguments are supplied, run all the examples
    if ( argc != 2 ) goto invalid_arguments;

    // cyclic Redundancy Check
    if ( strcmp(argv[1], "crc") == 0 )
        
        // set the thread pool example flag
        *ppfn_hash_function = hash_crc64;

    // fowler noll vo
    else if ( strcmp(argv[1], "fnv") == 0 )

        // fowler-noll-vo
        *ppfn_hash_function = hash_fnv64;
    
    // murmur
    else if ( strcmp(argv[1], "mmh") == 0 )

        // murmur
        *ppfn_hash_function = hash_mmh64;

    // sha
    else if ( strcmp(argv[1], "sha") == 0 )

        // sha
        *ppfn_hash_function = sha256_hash64;

    // default
    else goto invalid_arguments;
    
    // success
    return;
    
    // error handling
    {

        // argument errors
        {
            invalid_arguments:
                
                // print a usage message to standard out
                print_usage(argv[0]);

                // abort
                exit(EXIT_FAILURE);
        }
    }
}
