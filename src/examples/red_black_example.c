/** !
 * Example red black tree program
 * 
 * @file red_black_example.c
 * 
 * @author Jacob Smith
 */

// Standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// data
#include <data/red_black.h>

// Entry point
int main ( int argc, const char* argv[] )
{
    
    // Supress warnings
    (void) argc;
    (void) argv;
    
    // Initialized data
    red_black_tree *p_red_black_tree = (void *) 0;

    // Construct the red black tree
    red_black_tree_construct(&p_red_black_tree, 0, 0, 0);

    // Test data
    int values[] = {10, 5, 15, 3, 7, 12, 18, 1, 4, 6, 8, 11, 13, 16, 20};
    size_t value_count = sizeof(values) / sizeof(values[0]);

    printf("=== Red-Black Tree Example ===\n\n");

    // Insert values
    printf("Inserting values: ");
    for (size_t i = 0; i < value_count; i++)
    {
        printf("%d ", values[i]);
        if (red_black_tree_insert(p_red_black_tree, &values[i]) == 0)
        {
            printf("\nError: Failed to insert %d\n", values[i]);
            goto exit;
        }
    }
    printf("\n\n");

    // Test search
    printf("Testing search:\n");
    for (size_t i = 0; i < value_count; i += 3)
    {
        int *found_value = (void *) 0;
        if (red_black_tree_search(p_red_black_tree, &values[i], (void **)&found_value))
        {
            printf("Found: %d\n", *found_value);
        }
        else
        {
            printf("Not found: %d\n", values[i]);
        }
    }
    printf("\n");

    // Test search for non-existent value
    int non_existent = 999;
    int *found_value = (void *) 0;
    if (red_black_tree_search(p_red_black_tree, &non_existent, (void **)&found_value))
    {
        printf("Unexpected: Found non-existent value %d\n", *found_value);
    }
    else
    {
        printf("Correctly did not find non-existent value: %d\n", non_existent);
    }
    printf("\n");

    // Test removal
    printf("Removing some values:\n");
    const int values_to_remove[] = {5, 15, 3};
    const size_t remove_count = sizeof(values_to_remove) / sizeof(values_to_remove[0]);
    
    for (size_t i = 0; i < remove_count; i++)
    {
        const void *removed_value = (void *) 0;
        if (red_black_tree_remove(p_red_black_tree, &values_to_remove[i], &removed_value))
        {
            printf("Removed: %d\n", *(const int*)removed_value);
        }
        else
        {
            printf("Failed to remove: %d\n", values_to_remove[i]);
        }
    }
    printf("\n");

    printf("Red-Black Tree example completed successfully!\n");

    exit:

    // Clean up
    red_black_tree_destroy(&p_red_black_tree);

    // Success
    return EXIT_SUCCESS;
}
