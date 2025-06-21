/** !
 * table library
 *
 * @file table.c
 *
 * @author Jacob Smith
 */

// headers
#include <table/table.h>

// structure definitions
struct table_s
{
    size_t   rows,
             columns;
    void   **swap;
    tuple  **pp_tuples;
    mutex   *p_locks;
};

int table_create ( table **const pp_table )
{

    // argument check
    if ( pp_table == (void *) 0 ) goto no_table;

    // Allocate memory for a table
    table *p_table = realloc(0, sizeof(table));

    // error checking
    if ( p_table == (void *) 0 ) goto no_mem;

    // Zero set
    memset(p_table, 0, sizeof(table));

    // Return the allocated memory
    *pp_table = p_table;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_table:
                #ifndef NDEBUG
                    printf("[table] Null pointer provided for parameter \"pp_table\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    printf("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int table_construct ( table **const pp_table, size_t columns, size_t rows )
{

    // argument check
    if ( pp_table == (void *) 0 ) goto no_table;
    if ( columns  ==          0 ) goto no_columns;
    if ( rows     ==          0 ) goto no_rows;

    // initialized data
    table *p_table = 0;

    // Allocate a table
    if ( table_create(&p_table) == 0 ) goto failed_to_create_table;

    // Store the quantity of rows 
    p_table->rows = rows;

    // Store the quantity of columns
    p_table->columns = columns;
    
    // Allocate a list of tuples
    p_table->pp_tuples = realloc(0, sizeof(tuple *) * rows);

    // error check
    if ( p_table->pp_tuples == 0 ) goto no_mem;

    // Allocate a list of locks
    p_table->p_locks = realloc(0, sizeof(mutex) * rows);

    // error check
    if ( p_table->p_locks == 0 ) goto no_mem;

    // Allocate a list of void pointers
    p_table->swap = realloc(0, sizeof(void *) * rows);

    // error check
    if ( p_table->swap == 0 ) goto no_mem;

    // Iterate over each row
    for (size_t i = 0; i < rows; i++)
    {

        // Construct a tuple
        if ( tuple_construct(&p_table->pp_tuples[i], columns) == 0 ) goto failed_to_construct_tuple;

        // Construct a mutex
        if ( mutex_create(&p_table->p_locks[i]) == 0 ) goto failed_to_create_mutex;

    }    

    // return a pointer to the caller
    *pp_table = p_table;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_table:
                #ifndef NDEBUG
                    printf("[table] Null pointer provided for parameter \"pp_table\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_columns:
                #ifndef NDEBUG
                    printf("[table] Zero provided for parameter \"columns\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            no_rows:
                #ifndef NDEBUG
                    printf("[table] Zero provided for parameter \"rows\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

        }

        // Table errors
        {
            failed_to_create_table:
                #ifndef NDEBUG
                    printf("[table] Failed to create table in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            failed_to_create_mutex:
                #ifndef NDEBUG
                    printf("[table] Failed to create mutex in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Tuple errors
        {

            failed_to_construct_tuple:
                #ifndef NDEBUG
                    printf("[table] Call to \"construct_tuple\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    printf("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int table_set_cell ( table *const p_table, size_t x, size_t y, void *p_element )
{

    // argument check
    if ( p_table == (void *) 0 ) goto no_table;
    
    // initialized data
    tuple *p_row = p_table->pp_tuples[y];
    
    // lock the row
    mutex_lock(p_table->p_locks[y]);

    // Store contents of the tuple on the stack
    // TODO: Error check
    tuple_get(p_row, p_table->swap, 0);

    // Update the element
    p_table->swap[x] = p_element;

    // Destroy the old tuple
    // TODO: Error check
    tuple_destroy(&p_table->pp_tuples[y]);

    // Construct a new tuple from the contents of the old tuple
    // TODO: Error check
    tuple_from_elements(&p_table->pp_tuples[y], p_table->swap, p_table->columns);

    // unlock the row
    mutex_unlock(p_table->p_locks[y]);

    // success
    return 1;

    // error handling
    {

        // argument check
        {
            no_table:
                #ifndef NDEBUG
                    printf("[table] Null pointer provided for parameter \"p_table\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int table_get_cell ( const table *const p_table, size_t x, size_t y, void **pp_element )
{

    // argument check
    if ( p_table    == (void *) 0 ) goto no_table;
    if ( pp_element == (void *) 0 ) goto no_element;

    // initialized data
    void *p_element = 0;

    // lock the row
    mutex_lock(p_table->p_locks[y]);

    // return a pointer to the caller
    if ( tuple_index(p_table->pp_tuples[y], x, &p_element) == 0 ) goto failed_to_index_tuple;

    // unlock the row
    mutex_unlock(p_table->p_locks[y]);

    // Return the element to the caller
    *pp_element = p_element;

    // success
    return 1;

    // error handling
    {

        // argument check
        {
            no_table:
                #ifndef NDEBUG
                    printf("[table] Null pointer provided for parameter \"p_table\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            no_element:
                #ifndef NDEBUG
                    printf("[table] Null pointer provided for parameter \"pp_element\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Tuple errors
        {

            failed_to_index_tuple:
                #ifndef NDEBUG
                    printf("[table] Call to function \"tuple_index\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int table_get_row ( const table *const p_table, size_t y, const void **const pp_elements )
{

    // argument check
    if ( p_table     ==    (void *) 0 ) goto no_table;
    if ( y            > p_table->rows ) goto out_of_bounds;
    if ( pp_elements ==    (void *) 0 ) goto no_elements;

    // initialized data
    tuple *p_tmp = 0;

    // lock the mutexes
    mutex_lock(p_table->p_locks[y]);
    
    // Dump the row
    if ( tuple_get(p_table->pp_tuples[y], pp_elements, 0) == 0 ) goto failed_to_get_tuple_contents;

    // unlock the mutexes
    mutex_unlock(p_table->p_locks[y]);

    // success
    return 1;

    // error handling
    {

        // argument check
        {
            no_table:
                #ifndef NDEBUG
                    printf("[table] Null pointer provided for parameter \"p_table\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            out_of_bounds:
                #ifndef NDEBUG
                    printf("[table] Parameter \"y\" is out of bounds [0..%d] in call to function \"%s\"\n", p_table->rows, __FUNCTION__);
                #endif

                // error
                return 0;

            no_elements:
                #ifndef NDEBUG
                    printf("[table] Null pointer provided for parameter \"pp_elements\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Tuple errors
        {

            failed_to_get_tuple_contents:
                #ifndef NDEBUG
                    printf("[table] Call to function \"tuple_get\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int table_swap_rows ( table *const p_table, size_t row1, size_t row2 )
{

    // argument check
    if ( p_table == (void *)    0 ) goto no_table;
    if ( row1     > p_table->rows ) goto row1_out_of_bounds;
    if ( row2     > p_table->rows ) goto row2_out_of_bounds;

    // initialized data
    tuple *p_tmp = 0;

    // lock the mutexes
    mutex_lock(p_table->p_locks[row1]);
    mutex_lock(p_table->p_locks[row2]);
    
    // Swap the row
    p_tmp                    = p_table->pp_tuples[row1];    
    p_table->pp_tuples[row1] = p_table->pp_tuples[row2];
    p_table->pp_tuples[row2] = p_tmp;

    // unlock the mutexes
    mutex_unlock(p_table->p_locks[row1]);
    mutex_unlock(p_table->p_locks[row2]);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_table:
                #ifndef NDEBUG
                    printf("[table] Null pointer provided for parameter \"p_table\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            row1_out_of_bounds:
                #ifndef NDEBUG
                    printf("[table] Parameter \"row1\" is out of bounds [0..%d] in call to function \"%s\"\n", p_table->rows, __FUNCTION__);
                #endif

                // error
                return 0;
            
            row2_out_of_bounds:
                #ifndef NDEBUG
                    printf("[table] Parameter \"row2\" is out of bounds [0..%d] in call to function \"%s\"\n", p_table->rows, __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}