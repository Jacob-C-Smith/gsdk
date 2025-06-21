/** !
 * Include header for table library
 *
 * @file table/table.h 
 * 
 * @author Jacob Smith
 */

// header guard
#pragma once

#undef NDEBUG

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// sync submodule
#include <core/sync.h>

// tuple submodule
#include <tuple/tuple.h>

// forward declarations
struct table_s;

// type definitions
/** !
 *  @brief The type definition of a table struct
 */
typedef struct table_s table;

// Allocaters
/** !
 *  Allocate memory for a table
 *
 * @param pp_table return
 *
 * @sa table_destroy
 *
 * @return 1 on success, 0 on error
 */
int table_create ( table **const pp_table );

// constructors
/** !
 *  Construct a table with a specific quantity of rows and columns
 *
 * @param pp_table return
 * @param columns  quantity of columns
 * @param rows     quantity of rows
 *
 * @sa table_create
 * @sa table_destroy
 *
 * @return 1 on success, 0 on error
 */
int table_construct ( table **const pp_table, size_t columns, size_t rows );

// accessors
/** !
 *  Get the contents of a table cell
 *
 * @param p_table   the table
 * @param x         the column cell
 * @param y         the row of the cell
 * @param p_element return
 *
 * @sa table_set_cell
 *
 * @return 1 on success, 0 on error
 */
int table_get_cell ( const table *const p_table, size_t x, size_t y, void **pp_element );

/** !
 *  Get the contents of a table row
 *
 * @param p_table     the table
 * @param y           the row of the cell
 * @param pp_elements return
 *
 * @sa table_get_cell
 *
 * @return 1 on success, 0 on error
 */
int table_get_row ( const table *const p_table, size_t y, const void **const pp_elements );

// mutators
/** !
 *  Set the contents of a table cell
 *
 * @param p_table   the table
 * @param x         the column cell
 * @param y         the row of the cell
 * @param p_element the content of the new cell
 *
 * @sa table_get_cell
 *
 * @return 1 on success, 0 on error
 */
int table_set_cell ( table *const p_table, size_t x, size_t y, void *p_element );

/** !
 *  Swap two rows in the table 
 *
 * @param p_table the table
 * @param row1    the index of the first row
 * @param row2    the index of the second row
 *
 * @sa table_set_cell
 *
 * @return 1 on success, 0 on error
 */
int table_swap_rows ( table *const p_table, size_t row1, size_t row2 );

// destructors
/** !
 *  Destroy and deallocate a table
 *
 * @param pp_table the table
 *
 * @sa table_create
 *
 * @return 1 on success, 0 on error
 */
// TODO: Implement
//int table_destroy ( table **const pp_table );