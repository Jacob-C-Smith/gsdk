// standard library
#include <stdio.h>
#include <stdlib.h>

// Include
#include <data/dict.h>

// Forward declaration
int  print_all_keys         ( dict *d );
void print_value_as_integer ( const void *const p_value, size_t i );

// Entry point
int main ( int argc, const char* argv[] )
{

    // Suppress warnings
    (void) argc;
    (void) argv;

    // Initialized data
    dict *p_dict = (void *) 0;

    // Make a dictionary with 4 hash table items
    dict_construct(&p_dict, 4, 0);

    // Insert some items
    dict_add(p_dict, "Red"  , (void *)0xff0000);
    dict_add(p_dict, "Green", (void *)0x00ff00);
    dict_add(p_dict, "Blue" , (void *)0x0000ff);

    // Print the dictionarys' keys
    print_all_keys(p_dict);

    // Destroy the dictionary
    dict_destroy(&p_dict);

    // Success
    return EXIT_SUCCESS;
}

// Print the dictionarys' keys
int print_all_keys(dict* p_dict)
{

    if (p_dict == 0)
        return 0;

    // Get the dictionarys' keys
    size_t key_count = dict_keys(p_dict, 0);
    const char** keys = calloc(key_count, sizeof(char*));

    dict_keys(p_dict, keys);

    // Print each key
    for (size_t i = 0; i < key_count; i++)
        printf("%s\n", keys[i]);
    
    // Formatting
    putchar('\n');

    // Clean up
    free(keys);

    // Success
    return 1;
}

void print_value_as_integer ( const void *const p_value, size_t i )
{

    // Suppress warnings
    (void) i;

    // Print the value as a hexidecimal number
    printf("0x%zx\n",(size_t) p_value);

    // Return
    return;
}