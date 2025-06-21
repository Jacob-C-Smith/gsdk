/** !
 * Rectangle tree implementation
 * 
 * @file rectangle.c
 * 
 * @author Jacob Smith
 */

#include <data/rectangle.h>

// function declarations
/** !
 * Get the root node of a B tree
 * 
 * @param p_rectangle_tree     the B tree
 * @param pp_root_node return
 * 
 * @return 1 on success, 0 on error
 */
int rectangle_tree_root_get ( const rectangle_tree *const p_rectangle_tree, const void **const pp_result )
{

    // argument check
    if ( p_rectangle_tree == (void *) 0 ) goto no_rectangle_tree;
    if ( pp_result        == (void *) 0 ) goto no_return;
    
    // error check
    if ( p_rectangle_tree->p_root == (void *) 0 ) goto no_root_node;

    // return a pointer to the caller
    *pp_result = p_rectangle_tree->p_root;
    
    // success
    return 1;

    // TODO
    no_rectangle_tree:
    no_return:
    no_root_node:

        // error
        return 0;

    // error handling
    {

    }
}

/** !
 * Read a B tree's metadata 
 * 
 * @param rectangle_tree the B tree
 * 
 * @return 1 on success, 0 on error
 */
int rectangle_tree_write_meta_data ( const rectangle_tree *const p_rectangle_tree )
{

    // argument check
    if ( p_rectangle_tree == (void *) 0 ) goto no_rectangle_tree;

    // error check
    if ( p_rectangle_tree->p_random_access == NULL ) goto no_random_access;

    // Seek start 
    fseek(p_rectangle_tree->p_random_access, 0, SEEK_SET);

    // Write the quantity of keys
    // fwrite(p_rectangle_tree->metadata.key_quantity, sizeof(unsigned long long), 1, p_rectangle_tree->p_random_access);

    // Write the address of the root node
    // fwrite(p_rectangle_tree->metadata.root_address, sizeof(unsigned long long), 1, p_rectangle_tree->p_random_access);

    // Write the degree of the B tree
    // fwrite(p_rectangle_tree->metadata.degree, sizeof(int), 1, p_rectangle_tree->p_random_access);

    // Write the quantity of nodes in the B tree
    // fwrite(p_rectangle_tree->metadata.node_quantity, sizeof(int), 1, p_rectangle_tree->p_random_access);

    // Write the height of the B tree
    // fwrite(p_rectangle_tree->metadata.height, sizeof(int), 1, p_rectangle_tree->p_random_access);

    // success
    return 1;

    // TODO
    no_rectangle_tree:
    no_random_access:

        // error
        return 0;

    // error handling
}

/** !
 * Read a B tree's metadata 
 * 
 * @param rectangle_tree the B tree
 * 
 * @return 1 on success, 0 on error
 */
int rectangle_tree_read_meta_data ( const rectangle_tree *const p_rectangle_tree )
{

    // argument check
    if ( p_rectangle_tree == (void *) 0 ) goto no_rectangle_tree;

    // error check
    if ( p_rectangle_tree->p_random_access == NULL ) goto no_random_access;

    // Seek start 
    fseek(p_rectangle_tree->p_random_access, 0, SEEK_SET);

    // Read the quantity of keys
    fread((void *)&p_rectangle_tree->metadata.key_quantity, sizeof(unsigned long long), 1, p_rectangle_tree->p_random_access);

    // Read the address of the root node
    fread((void *)&p_rectangle_tree->metadata.root_address, sizeof(unsigned long long), 1, p_rectangle_tree->p_random_access);

    // Read the degree of the B tree
    fread((void *)&p_rectangle_tree->metadata.degree, sizeof(int), 1, p_rectangle_tree->p_random_access);

    // Read the quantity of nodes in the B tree
    fread((void *)&p_rectangle_tree->metadata.node_quantity, sizeof(int), 1, p_rectangle_tree->p_random_access);

    // Read the height of the B tree
    fread((void *)&p_rectangle_tree->metadata.height, sizeof(int), 1, p_rectangle_tree->p_random_access);

    // success
    return 1;

    // TODO
    no_rectangle_tree:
    no_random_access:

        // error
        return 0;

    // error handling
}

/** !
 * Read a chunk of data from the random access file
 * 
 * @param p_rectangle_tree       pointer to B tree
 * @param disk_address           pointer to data on disk
 * @param pp_rectangle_tree_node return
 * 
 * @return 1 on success, 0 on error
*/
int rectangle_tree_disk_read ( const rectangle_tree *p_rectangle_tree, unsigned long long disk_address, rectangle_tree_node **pp_rectangle_tree_node )
{

    // Unused
    (void) p_rectangle_tree;
    (void) disk_address;
    (void) pp_rectangle_tree_node;

    // success
    return 1;
}

// function definitions
int rectangle_tree_create ( rectangle_tree **const pp_rectangle_tree )
{

    // Unused
    (void) pp_rectangle_tree;

    // success
    return 1;
}

int rectangle_tree_construct ( rectangle_tree **const pp_rectangle_tree, fn_tree_equal *pfn_is_equal, int degree, unsigned long long node_size )
{

    // Unused
    (void) pp_rectangle_tree;
    (void) pfn_is_equal;
    (void) degree;
    (void) node_size;

    // success
    return 1;
}

int rectangle_tree_search ( const rectangle_tree *const p_rectangle_tree, const void *const p_key, const void **const pp_value )
{

    // Unused
    (void) p_rectangle_tree;
    (void) p_key;
    (void) pp_value;

    // success
    return 1;
}

int rectangle_tree_insert ( rectangle_tree *const p_rectangle_tree, const void *const p_key, const void *const p_value )
{
    
    // Unused
    (void) p_rectangle_tree;
    (void) p_key;
    (void) p_value;

    // success
    return 1;
}

int rectangle_tree_remove ( rectangle_tree *const p_rectangle_tree, const void *const p_key, const void **const p_value )
{
    
    // Unused
    (void) p_rectangle_tree;
    (void) p_key;
    (void) p_value;

    // success
    return 1;
}

int rectangle_tree_parse ( rectangle_tree **const pp_rectangle_tree, FILE *p_file, fn_tree_equal *pfn_is_equal, fn_rectangle_tree_parse *pfn_parse_node )
{

    // Unused
    (void) pp_rectangle_tree;
    (void) p_file;
    (void) pfn_is_equal;
    (void) pfn_parse_node;

    // success
    return 1;
}

int rectangle_tree_serialize ( rectangle_tree *const p_rectangle_tree, const char *p_path, fn_rectangle_tree_serialize *pfn_serialize_node )
{

    // Unused
    (void) p_rectangle_tree;
    (void) p_path;
    (void) pfn_serialize_node;

    // success
    return 1;
}

int rectangle_tree_destroy ( rectangle_tree **const pp_rectangle_tree )
{

    // Unused
    (void) pp_rectangle_tree;

    // success
    return 1;
}