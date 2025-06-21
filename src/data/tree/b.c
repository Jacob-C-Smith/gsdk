/** !
 * B tree implementation
 * 
 * @file b.c
 * 
 * @author Jacob Smith
 */

// header
#include <data/b.h>

// function declarations
/** !
 * Allocate a node for a specific b tree, and set the node pointer. 
 * 
 * The node pointer is an integer ordinal that is atomically incremented 
 * each time the allocator is called. 
 * 
 * @param p_b_tree       the b tree to allocate a node to
 * @param pp_b_tree_node return
 * 
 * @return 1 on success, 0 on error
 */
int b_tree_node_allocate ( b_tree *p_b_tree, b_tree_node **pp_b_tree_node );

/** !
 * Construct a b tree node
 * 
 * @param pp_b_tree_node result
 * @param p_b_tree       the b tree
 * @param on_disk        true if node is on disk else false
 * 
 * @return 1 on success, 0 on error
 */
int b_tree_node_construct ( b_tree_node **const pp_b_tree_node, b_tree *const p_b_tree, bool on_disk );

/** !
 * Get the root node of a B tree
 * 
 * @param p_b_tree     the B tree
 * @param pp_root_node return
 * 
 * @return 1 on success, 0 on error
 */
int b_tree_root ( const b_tree *const p_b_tree, b_tree_node **pp_root_node );

/** !
 * Split the root node of a B tree
 * 
 * @param p_b_tree the B tree
 * 
 * @return 1 on success, 0 on error
 */
int b_tree_split_root ( b_tree *const p_b_tree );

/** !
 * Split a child node in a b tree
 * 
 * @param p_b_tree the b tree
 * @param p_b_tree the node
 * @param i        which child
 * 
 * @return 1 on success, 0 on error
 */
int b_tree_split_child ( b_tree *const p_b_tree, b_tree_node *p_b_tree_node, size_t i );

/** !
 * Insert a property into a b tree in the correct location
 * 
 * @param p_b_tree      the b tree
 * @param p_b_tree_node the b tree node
 * @param p_property    the property
 * 
 * @return 1 on success, 0 on error
 */
int b_tree_insert_not_full ( b_tree *p_b_tree, b_tree_node *const p_b_tree_node, const void *const p_property );

/** !
 * Read a B tree's metadata 
 * 
 * @param b_tree the B tree
 * 
 * @return 1 on success, 0 on error
 */
int b_tree_write_meta_data ( const b_tree *const p_b_tree );

/** !
 * Read a B tree's metadata 
 * 
 * @param b_tree the B tree
 * 
 * @return 1 on success, 0 on error
 */
int b_tree_read_meta_data ( b_tree *const p_b_tree );

/** !
 * Read a chunk of data from the random access file
 * 
 * @param p_b_tree       pointer to B tree
 * @param disk_address   pointer to data on disk
 * @param pp_b_tree_node return
 * 
 * @return 1 on success, 0 on error
*/
int b_tree_disk_read ( b_tree *p_b_tree, unsigned long long disk_address, b_tree_node **pp_b_tree_node );

/** !
 * Traverse a b tree using the pre order technique
 * 
 * @param p_b_tree_node pointer to b tree node
 * @param pfn_traverse  called for each node in the binary tree
 * 
 * @return 1 on success, 0 on error
*/
int b_tree_traverse_preorder_node ( b_tree_node *p_b_tree_node, fn_b_tree_traverse *pfn_traverse );

/** !
 * Traverse a b tree using the in order technique
 * 
 * @param p_b_tree_node pointer to b tree node
 * @param pfn_traverse  called for each node in the binary tree
 * 
 * @return 1 on success, 0 on error
*/
int b_tree_traverse_inorder_node ( b_tree_node *p_b_tree_node, fn_b_tree_traverse *pfn_traverse );

/** !
 * Traverse a b tree using the post order technique
 * 
 * @param p_b_tree_node pointer to b tree node
 * @param pfn_traverse  called for each node in the binary tree
 * 
 * @return 1 on success, 0 on error
*/
int b_tree_traverse_postorder_node ( b_tree_node *p_b_tree_node, fn_b_tree_traverse *pfn_traverse );

/** !
 * Return the size of a file IF buffer == 0 ELSE read a file into buffer
 * 
 * @param path path to the file
 * @param buffer buffer
 * @param binary_mode "wb" IF true ELSE "w"
 * 
 * @return 1 on success, 0 on error
 */
size_t load_file ( const char *path, void *buffer, bool binary_mode );

// function definitions
int b_tree_node_create ( b_tree_node **const pp_b_tree_node )
{
    
    // argument check
    if ( pp_b_tree_node == (void *) 0 ) goto no_b_tree_node;

    // initialized data
    b_tree_node *p_b_tree_node = realloc(0, sizeof(b_tree_node));

    // error check
    if ( p_b_tree_node == (void *) 0 ) goto no_mem;

    // Zero set the struct
    memset(p_b_tree_node, 0, sizeof(b_tree));

    // return a pointer to the caller
    *pp_b_tree_node = p_b_tree_node;

    // success
    return 1;

    // error handling
    {
        
        // argument errors
        {
            no_b_tree_node:
                #ifndef NDEBUG
                    log_error("[tree] [b] Null pointer provided for parameter \"pp_b_tree_node\" in call to function \"%s\"\n", __FUNCTION__);
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

int b_tree_create ( b_tree **const pp_b_tree )
{
    
    // argument check
    if ( pp_b_tree == (void *) 0 ) goto no_b_tree;

    // initialized data
    b_tree *p_b_tree = realloc(0, sizeof(b_tree));

    // error check
    if ( p_b_tree == (void *) 0 ) goto no_mem;

    // Zero set the struct
    memset(p_b_tree, 0, sizeof(b_tree));

    // return a pointer to the caller
    *pp_b_tree = p_b_tree;

    // success
    return 1;

    // error handling
    {
        
        // argument errors
        {
            no_b_tree:
                #ifndef NDEBUG
                    log_error("[tree] [b] Null pointer provided for parameter \"pp_b_tree\" in call to function \"%s\"\n", __FUNCTION__);
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

int b_tree_construct ( b_tree **const pp_b_tree, const char *const path, fn_tree_equal *pfn_is_equal, int degree, unsigned long long node_size )
{

    // argument check
    if ( pp_b_tree == (void *) 0 ) goto no_b_tree;
    if ( degree    <           2 ) goto no_degree;
    if ( pp_b_tree == (void *) 0 ) goto no_node_size;

    // initialized data
    b_tree *p_b_tree = (void *) 0;
    bool  file_exists = load_file(path, 0, true);
    FILE *p_random_access_file = (void *) 0;

    // File does not exist
    if ( file_exists == false )
        
        // Create the file
        p_random_access_file = fopen(path, "w+");

    // File exists
    else 

        // Load the file
        p_random_access_file = fopen(path, "rw");

    // error check
    if ( p_random_access_file == (void *) 0 ) goto failed_to_get_random_access_file;

    // Allocate a b tree
    if ( b_tree_create(&p_b_tree) == 0 ) goto failed_to_allocate_b_tree;
    
    // Populate the struct
    *p_b_tree = (b_tree)
    {
        .p_random_access = p_random_access_file,
        .p_root = 0,
        .functions = 
        {
            .pfn_is_equal       = 0,
            .pfn_serialize_node = 0,
            .pfn_parse_node     = 0
        },
        ._metadata = (b_tree_metadata) 
        {
            .node_quantity     = 1,
            .node_size         = node_size,
            .key_quantity      = 0,
            .degree            = degree,
            .height            = 0,
            .next_disk_address = sizeof(b_tree_metadata)
        }
    };
    
    // Read the metadata from the file
    if ( file_exists )
    {

        // Read the metadata
        b_tree_read_meta_data(p_b_tree);

        // Load the root of the B tree
        b_tree_disk_read(p_b_tree, p_b_tree->_metadata.root_address, &p_b_tree->p_root);
    }

    // Populate B tree metadata. Write root
    else
    {

        // Write the b tree metadata
        b_tree_write_meta_data(p_b_tree);

        // Allocate the root node
        if ( b_tree_node_construct(&p_b_tree->p_root, p_b_tree, false) == 0 ) goto failed_to_construct_b_tree_node;
    }

    // Store the comparator
    p_b_tree->functions.pfn_is_equal = ( pfn_is_equal ) ? pfn_is_equal : tree_compare_function;

    // return a pointer to the caller
    *pp_b_tree = p_b_tree;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_b_tree:
                #ifndef NDEBUG
                    log_error("[tree] [b] Null pointer provided for parameter \"pp_b_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_degree:
                #ifndef NDEBUG
                    log_error("[tree] [b] Parameter \"degree\" must be greater than or equal to 2 in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_node_size:
                #ifndef NDEBUG
                    log_error("[tree] [b] Parameter \"node_size\" must be greater than zero in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Tree errors
        {
            failed_to_allocate_b_tree:
                #ifndef NDEBUG
                    log_error("[tree] [b] Failed to allocate b tree in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            failed_to_get_random_access_file:
                #ifndef NDEBUG
                    log_error("[tree] [b] Failed to open random access file \"%s\" in call to function \"%s\"\n", path, __FUNCTION__);
                #endif

                // error
                return 0;

            failed_to_construct_b_tree_node:
                #ifndef NDEBUG
                    log_error("[tree] [b] Failed to construct b tree node in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int b_tree_node_allocate ( b_tree *p_b_tree, b_tree_node **pp_b_tree_node )
{

    // argument check
    if ( p_b_tree       == (void *) 0 ) goto no_b_tree;
    if ( pp_b_tree_node == (void *) 0 ) goto no_b_tree_node;

    // initialized data
    b_tree_node *p_b_tree_node = (void *) 0;

    // Allocate a node
    if ( b_tree_node_create(&p_b_tree_node) == 0 ) goto failed_to_allocate_node;

    // Store the node pointer
    p_b_tree_node->node_pointer = p_b_tree->_metadata.node_quantity;

    // Increment the node quantity
    p_b_tree->_metadata.node_quantity++;

    // return a pointer to the caller
    *pp_b_tree_node = p_b_tree_node;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_b_tree:
                #ifndef NDEBUG
                    printf("[tree] [b] Null pointer provided for parameter \"p_b_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
                
            no_b_tree_node:
                #ifndef NDEBUG
                    printf("[tree] [b] Null pointer provided for parameter \"pp_b_tree_node\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Tree
        {
            failed_to_allocate_node:
                #ifndef NDEBUG
                    printf("[tree] [b] Call to function \"b_tree_node_create\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int b_tree_node_construct ( b_tree_node **const pp_b_tree_node, b_tree *const p_b_tree, bool on_disk )
{

    // argument check
    if ( pp_b_tree_node == (void *) 0 ) goto no_b_tree_node;
    if ( p_b_tree       == (void *) 0 ) goto no_b_tree;

    // initialized data
    b_tree_node *p_b_tree_node = realloc(0, sizeof(b_tree_node) + ((p_b_tree->_metadata.degree * 2) * sizeof(unsigned long long)));

    // error check
    if ( p_b_tree_node == (void *) 0 ) goto no_mem;

    // Is a leaf
    p_b_tree_node->leaf = true;

    // Allocate memory for properties
    p_b_tree_node->properties = realloc(0, sizeof(void *) * ( (p_b_tree->_metadata.degree * 2) - 1 ));

    // error check
    if ( p_b_tree_node->properties == (void *) 0 ) goto no_mem;

    // Set the location
    if ( on_disk )
    {
        
        // Store the next node address
        p_b_tree_node->node_pointer = p_b_tree->_metadata.next_disk_address;

        // Update the next node address
        p_b_tree->_metadata.next_disk_address = p_b_tree->_metadata.node_size;
    }

    // return a pointer to the caller
    *pp_b_tree_node = p_b_tree_node;

    // success
    return 1;

    // error handling
    {
        
        // argument errors
        {
            no_b_tree_node:
                #ifndef NDEBUG
                    log_error("[tree] [b] Null pointer provided for parameter \"pp_b_tree_node\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_b_tree:
                #ifndef NDEBUG
                    log_error("[tree] [b] Null pointer provided for parameter \"p_b_tree\" in call to function \"%s\"\n", __FUNCTION__);
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

int b_tree_root ( const b_tree *const p_b_tree, b_tree_node **pp_root_node )
{

    // argument check
    if ( p_b_tree     == (void *) 0 ) goto no_b_tree;
    if ( pp_root_node == (void *) 0 ) goto no_return;
    
    // error check
    if ( p_b_tree->p_root == (void *) 0 ) goto no_root_node;

    // return a pointer to the caller
    *pp_root_node = p_b_tree->p_root;
    
    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_b_tree:
                #ifndef NDEBUG
                    log_error("[tree] [b] Null pointer provided for parameter \"p_b_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error 
                return 0;

            no_return:
                #ifndef NDEBUG
                    log_error("[tree] [b] Null pointer provided for parameter \"pp_root_node\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error 
                return 0;
        }

        // Tree errors
        {
            no_root_node:
                #ifndef NDEBUG
                    log_error("[tree] [b] Parameter \"p_b_tree\" contains no root node in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error 
                return 0;
        }
    }
}

int b_tree_split_root ( b_tree *const p_b_tree )
{

    // argument check
    if ( p_b_tree == (void *) 0 ) goto no_b_tree;

    // initialized data
    b_tree_node *p_new_root_node = (void *) 0;

    // Allocate a node
    if ( b_tree_node_allocate(p_b_tree, &p_new_root_node) == 0 ) goto failed_to_allocate_node;

    // Populate the new root
    *p_new_root_node = (b_tree_node)
    {
        .leaf = false,
        .key_quantity = 0,        
    };

    // Store the pointer to the old root
    p_new_root_node->_child_pointers[0] = p_b_tree->p_root->node_pointer;

    // Update the root node
    p_b_tree->p_root = p_new_root_node;

    // Split the new root node
    //b_tree_split_child(p_b_tree->p_root, 0);

    // Update the height
    p_b_tree->_metadata.height++;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_b_tree:
                #ifndef NDEBUG
                    log_error("[tree] [b] Null pointer provided for parameter \"p_b_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Tree errors
        {
            failed_to_allocate_node:
                #ifndef NDEBUG
                    log_error("[tree] [b] Failed to allocate b tree node in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int b_tree_split_child ( b_tree *const p_b_tree, b_tree_node *p_b_tree_node, size_t i )
{
    
    // argument check
    if ( p_b_tree      == (void *) 0 ) goto no_b_tree;
    if ( p_b_tree_node == (void *) 0 ) goto no_b_tree_node;

    // initialized data
    b_tree_node *p_left_node  = (void *) 0,
                *p_right_node = (void *) 0;

    // Read the left node
    if ( b_tree_disk_read(p_b_tree, p_b_tree_node->_child_pointers[i], &p_left_node) == 0 ) goto failed_to_read_node; 

    // Construct the right node
    if ( b_tree_node_allocate(p_b_tree, &p_right_node) == 0 ) goto failed_to_allocate_node;

    // Set the leaf flag
    p_right_node->leaf = p_left_node->leaf;

    // Update the quantity of keys
    p_right_node->key_quantity = p_b_tree->_metadata.degree - 1;

    // Construct the right node
    for (int j = 0; j < p_b_tree->_metadata.degree - 1; j++)

        // Transfer elements from left node to right node
        p_right_node->properties[j] = p_left_node->properties[j + p_b_tree->_metadata.degree];
    
    // Update pointers
    if ( p_left_node->leaf == false )

        // Shift pointers
        for (int j = 0; j < p_b_tree->_metadata.degree; j++)

            // Move pointers from the left node to the right node
            p_right_node->_child_pointers[j] = p_left_node->_child_pointers[j + p_b_tree->_metadata.degree];
    
    // Update the quantity of keys
    p_left_node->key_quantity = p_b_tree->_metadata.degree - 1;
        
    // Make room for the right node in the parent node
    for (size_t j = p_left_node->key_quantity; j > i; j--)

        // Shift pointers
        p_b_tree_node->_child_pointers[j + 1] = p_b_tree_node->_child_pointers[j];

    // Insert the right node into the parent node
    p_b_tree_node->_child_pointers[i + 1] = p_right_node->node_pointer;

    // Shift keys in the right node
    for (size_t j = p_b_tree_node->key_quantity; j >= i; j--)

        // Shift keys
        p_b_tree_node->properties[j + 1] = p_b_tree_node->properties[j];

    // Insert the median of the left node
    p_b_tree_node->properties[i] = p_left_node->properties[p_b_tree->_metadata.degree - 1];

    // Increment the quantity of keys in the parent node
    p_b_tree_node->key_quantity++;

    // Increment the quantity of nodes in the b tree
    p_b_tree->_metadata.node_quantity++;

    // Disk write left
    //

    // Disk write right
    //

    // Disk write parent
    //

    // success
    return 1;
    
    // error handling
    {
        
        // argument errors
        {
            no_b_tree:
                #ifndef NDEBUG
                    log_error("[tree] [b] Null pointer provided for parameter \"p_b_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_b_tree_node:
                #ifndef NDEBUG
                    log_error("[tree] [b] Null pointer provided for parameter \"p_b_tree_node\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Tree errors
        {
            failed_to_read_node:
                #ifndef NDEBUG
                    log_error("[tree] [b] Failed to read b tree node in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
                
            failed_to_allocate_node:
                #ifndef NDEBUG
                    log_error("[tree] [b] Failed to allocate b tree node in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int b_tree_insert_not_full ( b_tree *p_b_tree, b_tree_node *const p_b_tree_node, const void *const p_property )
{

    // argument check
    if ( p_b_tree      == (void *) 0 ) goto no_b_tree;
    if ( p_b_tree_node == (void *) 0 ) goto no_b_tree_node;
    if ( p_property    == (void *) 0 ) goto no_property;

    // initialized data
    signed i = p_b_tree_node->key_quantity - 1;

    // Insert into a leaf node
    if ( p_b_tree_node->leaf )
    {

        // Check for duplicates
        //

        // Search for existing key
        while (i >= 0 && p_b_tree->functions.pfn_is_equal(p_property, p_b_tree_node->properties[i]) > 0 )
        {
            
            // Shift properties
            p_b_tree_node->properties[i + 1] = p_b_tree_node->properties[i];
            
            // Decrement i
            i--;
        }

        // Store the property
        p_b_tree_node->properties[i + 1] = (void *) p_property;
        
        // Increment the quantity of keys
        p_b_tree_node->key_quantity++;

        // Increment the quantity of properties
        p_b_tree->_metadata.node_quantity++;

        //b_tree_disk_write(p_b_tree, p_b_tree_node);
    }

    // Find the child pointer
    else
    {
        
        // initialized data
        b_tree_node *p_child_node = (void *) 0;

        // Check for duplicates
        //

        // Shift keys up
        while (i >= 0 && p_b_tree->functions.pfn_is_equal(p_property, p_b_tree_node->properties[i]) ) i--;

        b_tree_disk_read(p_b_tree, p_b_tree_node->_child_pointers[i], &p_child_node);

        // Check for duplicates
        //

        // Split?
        if ( p_child_node->key_quantity == ( (2 * p_b_tree->_metadata.degree) - 1) )
        {
            
            // Split the child node
            //

            // Store the child node
            //

            // Read the new child
            // TODO: '>' or '<' ?
            if ( p_b_tree->functions.pfn_is_equal(p_property, p_b_tree_node->properties[i]) < 0 )
            {

                // Increment i
                i++;

                // Store the new child node
                //b_tree_disk_read(p_b_tree, p_b_tree_node->_child_pointers[i], &p_child_node);
            }
        }

        // Insert the property
        b_tree_insert_not_full(p_b_tree, p_child_node, p_property);
    }

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_b_tree:
                #ifndef NDEBUG
                    log_error("[tree] [b] Null pointer provided for parameter \"p_b_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_b_tree_node:
                #ifndef NDEBUG
                    log_error("[tree] [b] Null pointer provided for parameter \"p_b_tree_node\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_property:
                #ifndef NDEBUG
                    log_error("[tree] [b] Null pointer provided for parameter \"p_property\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int b_tree_write_meta_data ( const b_tree *const p_b_tree )
{

    // argument check
    if ( p_b_tree == (void *) 0 ) goto no_b_tree;

    // error check
    if ( p_b_tree->p_random_access == NULL ) goto no_random_access;

    // Seek start 
    fseek(p_b_tree->p_random_access, 0, SEEK_SET);

    // Write the quantity of keys
    fwrite(&p_b_tree->_metadata.key_quantity, sizeof(unsigned long long), 1, p_b_tree->p_random_access);

    // Write the address of the root node
    fwrite(&p_b_tree->_metadata.root_address, sizeof(unsigned long long), 1, p_b_tree->p_random_access);

    // Write the degree of the B tree
    fwrite(&p_b_tree->_metadata.degree, sizeof(int), 1, p_b_tree->p_random_access);

    // Write the quantity of nodes in the B tree
    fwrite(&p_b_tree->_metadata.node_quantity, sizeof(int), 1, p_b_tree->p_random_access);

    // Write the height of the B tree
    fwrite(&p_b_tree->_metadata.height, sizeof(int), 1, p_b_tree->p_random_access);

    // Flush the stream
    fflush(p_b_tree->p_random_access);

    // success
    return 1;

    // error handling
    {
        
        // argument errors
        {
            no_b_tree:
                #ifndef NDEBUG
                    log_error("[tree] [b] Null pointer provided for parameter \"p_b_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error 
                return 0;
        }

        // Tree errors
        {

            no_random_access:
                #ifndef NDEBUG
                    log_error("[tree] [b] Parameter \"p_b_tree\" does not have a random access file in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error 
                return 0;
        }
    }
}

int b_tree_read_meta_data ( b_tree *const p_b_tree )
{

    // argument check
    if ( p_b_tree == (void *) 0 ) goto no_b_tree;

    // error check
    if ( p_b_tree->p_random_access == NULL ) goto no_random_access;

    // Seek start 
    fseek(p_b_tree->p_random_access, 0, SEEK_SET);

    // Read the quantity of keys
    fread(&p_b_tree->_metadata.key_quantity, sizeof(unsigned long long), 1, p_b_tree->p_random_access);

    // Read the address of the root node
    fread(&p_b_tree->_metadata.root_address, sizeof(unsigned long long), 1, p_b_tree->p_random_access);

    // Read the degree of the B tree
    fread(&p_b_tree->_metadata.degree, sizeof(int), 1, p_b_tree->p_random_access);

    // Read the quantity of nodes in the B tree
    fread(&p_b_tree->_metadata.node_quantity, sizeof(int), 1, p_b_tree->p_random_access);

    // Read the height of the B tree
    fread(&p_b_tree->_metadata.height, sizeof(int), 1, p_b_tree->p_random_access);

    // success
    return 1;

    // error handling
    {
        
        // argument errors
        {
            no_b_tree:
                #ifndef NDEBUG
                    log_error("[tree] [b] Null pointer provided for parameter \"p_b_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error 
                return 0;
        }

        // Tree errors
        {

            no_random_access:
                #ifndef NDEBUG
                    log_error("[tree] [b] Parameter \"p_b_tree\" does not have a random access file in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error 
                return 0;
        }
    }
}

int b_tree_disk_read ( b_tree *p_b_tree, unsigned long long disk_address, b_tree_node **pp_b_tree_node )
{

    // Unused
    (void) p_b_tree;
    (void) disk_address;
    (void) pp_b_tree_node;
    
    // success
    return 0;
}

size_t load_file ( const char *path, void *buffer, bool binary_mode )
{

    // argument checking 
    if ( path == 0 ) goto no_path;

    // initialized data
    size_t  ret = 0;
    FILE   *f   = fopen(path, (binary_mode) ? "rb" : "r");
    
    // Check if file is valid
    if ( f == NULL ) goto invalid_file;

    // Find file size and prep for read
    fseek(f, 0, SEEK_END);
    ret = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    // Read to data
    if ( buffer ) ret = fread(buffer, 1, ret, f);

    // The file is no longer needed
    fclose(f);
    
    // success
    return ret;

    // error handling
    {

        // argument errors
        {
            no_path:
                #ifndef NDEBUG
                    log_error("Null pointer provided for parameter \"path\" in call to function \"%s\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // File errors
        {
            invalid_file:

                // error
                return 0;
        }
    }
}

int b_tree_flush ( b_tree *const p_b_tree )
{

    // Unused
    (void) p_b_tree;

    // argument check
    // 

    // initialized data
    //

    // Disk write the root node
    //

    // Update metadata root address to root node location
    //

    // Write the metadata
    //
    
    // success
    return 1;
}

int b_tree_search ( const b_tree *const p_b_tree, const void *const p_key, const void **const pp_value )
{
    
    // Unused
    (void) p_b_tree;
    (void) p_key;
    (void) pp_value;
    
    // success
    return 0;
}

int b_tree_insert ( b_tree *const p_b_tree, const void *const p_property )
{

    // argument check
    if ( p_b_tree   == (void *) 0 ) goto no_b_tree;
    if ( p_property == (void *) 0 ) goto no_property;
    
    // Is the root full?
    if ( p_b_tree->p_root->key_quantity == ( ( 2 * p_b_tree->_metadata.degree ) - 1 ) )
    {

        // Edge case
        if ( p_b_tree->p_root->leaf )
            
            // Increment the quantity of nodes
            p_b_tree->_metadata.node_quantity++;
        
        // Split the root before inserting 
        b_tree_split_root(p_b_tree);
    }

    // Insert the key
    b_tree_insert_not_full(p_b_tree, p_b_tree->p_root, p_property);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_b_tree:
                #ifndef NDEBUG
                    log_error("[tree] [b] Null pointer provided for parameter \"p_b_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_property:
                #ifndef NDEBUG
                    log_error("[tree] [b] Null pointer provided for parameter \"p_property\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int b_tree_remove ( b_tree *const p_b_tree, const void *const p_key, const void **const p_value )
{
    
    // Unused
    (void) p_b_tree;
    (void) p_key;
    (void) p_value;

    // success
    return 0;
}

int b_tree_traverse_inorder_node ( b_tree_node *p_b_tree_node, fn_b_tree_traverse *pfn_traverse )
{

    // argument check
    if ( p_b_tree_node == (void *) 0 ) goto no_b_tree_node;
    if ( pfn_traverse  == (void *) 0 ) goto no_traverse_function;

    for (int i = 0; i < p_b_tree_node->key_quantity; i++)
        printf("%p\n", p_b_tree_node->properties[i]);
    
    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_b_tree_node:
                #ifndef NDEBUG
                    log_error("[tree] [b] Null pointer provided for parameter \"p_b_tree_node\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_traverse_function:
                #ifndef NDEBUG
                    log_error("[tree] [b] Null pointer provided for parameter \"pfn_traverse\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int b_tree_traverse_inorder ( b_tree *p_b_tree, fn_b_tree_traverse *pfn_traverse )
{

    // argument check
    if ( p_b_tree     == (void *) 0 ) goto no_b_tree;
    if ( pfn_traverse == (void *) 0 ) goto no_traverse_function;

    // Traverse the tree
    if ( b_tree_traverse_inorder_node(p_b_tree->p_root, pfn_traverse) == 0 ) goto failed_to_traverse_b_tree;    

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_b_tree:
                #ifndef NDEBUG
                    log_error("[tree] [b] Null pointer provided for parameter \"p_b_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_traverse_function:
                #ifndef NDEBUG
                    log_error("[tree] [b] Null pointer provided for parameter \"pfn_traverse\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Tree errors
        {
            failed_to_traverse_b_tree:
                #ifndef NDEBUG
                    log_error("[tree] [b] Failed to traverse b tree in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // error
                return 0;
        }
    }
}

int b_tree_parse ( b_tree **const pp_b_tree, FILE *p_file, fn_tree_equal *pfn_is_equal, fn_b_tree_parse *pfn_parse_node )
{
    
    // Unused
    (void) pp_b_tree;
    (void) p_file;
    (void) pfn_is_equal;
    (void) pfn_parse_node;

    // success
    return 1;
}

int b_tree_serialize ( b_tree *const p_b_tree, const char *p_path, fn_b_tree_serialize *pfn_serialize_node )
{
    
    // Unused
    (void) p_b_tree;
    (void) p_path;
    (void) pfn_serialize_node;

    // success
    return 1;
}

int b_tree_destroy ( b_tree **const pp_b_tree )
{

    // Unused
    (void) pp_b_tree;

    // argument check
    if ( pp_b_tree == (void *) 0 ) goto no_b_tree;

    // initialized data
    // b_tree *p_b_tree = *pp_b_tree;

    // lock
    //

    // No more pointer for caller
    *pp_b_tree = (void *) 0;

    // unlock
    //
    
    // TODO: Deallocate

    // success
    return 1;

    // error handling
    {
        
        // argument errors
        {
            no_b_tree:
                #ifndef NDEBUG
                    log_error("[tree] [b] Null pointer provided for parameter \"pp_b_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}
