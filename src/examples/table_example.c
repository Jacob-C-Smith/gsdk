/** !
 * Include header for table library
 *
 * @file table/table.h 
 * 
 * @author Jacob Smith
 */

// standard library
#include <stdio.h>
#include <stdlib.h>

// table submodule
#include <table/table.h>

// Preprocessor definitions
#define TABLE_EXAMPLE_ROWS    5
#define TABLE_EXAMPLE_COLUMNS 3

/** ! 
 * Print the contents of a table to standard out
 * 
 * @param p_table the table
 * 
 * @return void
*/
void print_table ( const table *const p_table );

/** !
 * Populate a table with some example data
 * 
 * @param p_table the table
 * 
 * @return void
*/
void populate_table ( table *const p_table );

// Entry point
int main ( int argc, const char* argv[] )
{

    // Initialized data
    table *p_table = (void *) 0;

    // Construct a table
    if ( table_construct(&p_table, TABLE_EXAMPLE_COLUMNS, TABLE_EXAMPLE_ROWS) == 0 ) goto failed_to_construct_table;

    // Print the table
    print_table(p_table);

    // Add some data
    populate_table(p_table);

    // Print the table
    print_table(p_table);

    // Swap row 0 and row 3
    table_swap_rows(p_table, 0, 3);

    // Print the table
    print_table(p_table);

    // Success
    return EXIT_SUCCESS;

    // Error handling
    {

        // Argument errors
        {
            failed_to_construct_table:

                // Write an error to standard out
                printf("Error: Failed to construct table\n");

                // Error
                return EXIT_FAILURE;
        }
    }
}

void print_table ( const table *const p_table )
{

    // Iterate over each row
    for (size_t i = 0; i < TABLE_EXAMPLE_ROWS; i++)
    {

        // Formatting
        printf("[%zu] : [ ", i);

        // Iterate over each column
        for (size_t j = 0; j < TABLE_EXAMPLE_COLUMNS; j++)
        {

            // Initialized data
            char *p_str = 0;

            // Get the table cell
            table_get_cell(p_table, j, i, (void **)&p_str);

            // Write the table cell to standard out
            printf("\"%s\", ", (p_str == 0) ? "" : p_str);
        }

        // Formatting
        printf(" ]\n");
    }

    // Formatting
    printf("\n");

    // Done
    return;
}

void populate_table ( table *const p_table )
{

    // Add some data
    table_set_cell(p_table, 0, 0, "1");
    table_set_cell(p_table, 1, 0, "Jake");
    table_set_cell(p_table, 2, 0, "21");

    table_set_cell(p_table, 0, 1, "2");
    table_set_cell(p_table, 1, 1, "Seth");
    table_set_cell(p_table, 2, 1, "21");

    table_set_cell(p_table, 0, 2, "3");
    table_set_cell(p_table, 1, 2, "Adam");
    table_set_cell(p_table, 2, 2, "19");

    table_set_cell(p_table, 0, 3, "4");
    table_set_cell(p_table, 1, 3, "Daniel");
    table_set_cell(p_table, 2, 3, "21");

    // Done
    return;
}