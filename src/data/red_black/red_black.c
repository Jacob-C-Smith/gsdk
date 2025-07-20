/** !
 * Implementation of red_black search tree
 * 
 * @file red_black.c
 * 
 * @author Jacob Smith
 */

// header file
#include <data/red_black.h>
#include <string.h>

// static data
static const unsigned long long eight_bytes_of_f = 0xffffffffffffffff;

// forward declarations
/** !
 * Allocate memory for a red black tree
 * 
 * @param pp_red_black_tree return
 * 
 * @return 1 on success, 0 on error
 */
int red_black_tree_create ( red_black_tree **const pp_red_black_tree );

/** !
 * Allocate memory for a red_black tree node
 * 
 * @param pp_red_black_tree_node return
 * 
 * @return 1 on success, 0 on error
 */
int red_black_tree_node_create ( red_black_tree_node **pp_red_black_tree_node );

/** !
 * Allocate a node for a specific red_black tree, and set the node pointer. 
 * 
 * The node pointer is an integer ordinal that is atomically incremented 
 * each time the allocator is called. 
 * 
 * @param p_red_black_tree       the red_black tree to allocate a node to
 * @param pp_red_black_tree_node return
 * 
 * @return 1 on success, 0 on error
 */
int red_black_tree_node_allocate ( red_black_tree *p_red_black_tree, red_black_tree_node **pp_red_black_tree_node );

/** !
 * Recursively construct a balanced red_black search tree from a sorted list of keys and values
 * 
 * @param p_red_black_tree the red_black tree
 * @param pp_values        the list of values
 * @param start            the starting index 
 * @param end              the ending index
 * 
 * @return the root node 
 */
red_black_tree_node *red_black_tree_construct_balanced_recursive ( red_black_tree *p_red_black_tree, void **pp_values, size_t start, size_t end );

/** !
 * Recursively serialize red_black tree nodes to a file
 * 
 * @param p_file                       the file
 * @param p_red_black_tree             the red_black tree
 * @param p_red_black_tree_node        the red_black tree node
 * @param pfn_red_black_tree_serialize the node serializer function
 * 
 * @return 1 on success, 0 on error 
 */
int red_black_tree_serialize_node ( FILE *p_file, red_black_tree *p_red_black_tree, red_black_tree_node *p_red_black_tree_node, fn_red_black_tree_serialize *pfn_red_black_tree_serialize );

/** !
 * Recursively parse red_black tree nodes from a file
 * 
 * @param p_file                       the file
 * @param p_red_black_tree             the red_black tree
 * @param pp_red_black_tree_node       result
 * @param pfn_red_black_tree_serialize the node parser function
 * 
 * @return 1 on success, 0 on error 
 */
int red_black_tree_parse_node ( FILE *p_file, red_black_tree *p_red_black_tree, red_black_tree_node **pp_red_black_tree_node, fn_red_black_tree_parse *pfn_red_black_tree_parse );

/** !
 * Traverse a red_black tree using the pre order technique
 * 
 * @param p_red_black_tree_node pointer to red_black tree node
 * @param pfn_traverse          called for each node in the red_black tree
 * 
 * @return 1 on success, 0 on error
*/
int red_black_tree_traverse_preorder_node ( red_black_tree_node *p_red_black_tree_node, fn_red_black_tree_traverse *pfn_traverse );

/** !
 * Traverse a red_black tree using the in order technique
 * 
 * @param p_red_black_tree_node pointer to red_black tree node
 * @param pfn_traverse          called for each node in the red_black tree
 * 
 * @return 1 on success, 0 on error
*/
int red_black_tree_traverse_inorder_node ( red_black_tree_node *p_red_black_tree_node, fn_red_black_tree_traverse *pfn_traverse );

/** !
 * Traverse a red_black tree using the post order technique
 * 
 * @param p_red_black_tree_node pointer to red_black tree node
 * @param pfn_traverse          called for each node in the red_black tree
 * 
 * @return 1 on success, 0 on error
*/
int red_black_tree_traverse_postorder_node ( red_black_tree_node *p_red_black_tree_node, fn_red_black_tree_traverse *pfn_traverse );

/** !
 * Perform a left rotation on a red-black tree node
 * 
 * @param p_red_black_tree the red-black tree
 * @param p_node           the node to rotate
 * 
 * @return 1 on success, 0 on error
 */
int red_black_tree_rotate_left ( red_black_tree *p_red_black_tree, red_black_tree_node *p_node );

/** !
 * Perform a right rotation on a red-black tree node
 * 
 * @param p_red_black_tree the red-black tree
 * @param p_node           the node to rotate
 * 
 * @return 1 on success, 0 on error
 */
int red_black_tree_rotate_right ( red_black_tree *p_red_black_tree, red_black_tree_node *p_node );

/** !
 * Fix red-black tree violations after insertion
 * 
 * @param p_red_black_tree the red-black tree
 * @param p_node           the node that was inserted
 * 
 * @return 1 on success, 0 on error
 */
int red_black_tree_insert_fixup ( red_black_tree *p_red_black_tree, red_black_tree_node *p_node );

/** !
 * Fix red-black tree violations after deletion
 * 
 * @param p_red_black_tree the red-black tree
 * @param p_node           the node to fix from
 * 
 * @return 1 on success, 0 on error
 */
int red_black_tree_delete_fixup ( red_black_tree *p_red_black_tree, red_black_tree_node *p_node );

/** !
 * Get the minimum node in a subtree
 * 
 * @param p_node the root of the subtree
 * 
 * @return the minimum node
 */
red_black_tree_node *red_black_tree_minimum ( red_black_tree_node *p_node );

/** !
 * Get the successor of a node
 * 
 * @param p_node the node
 * 
 * @return the successor node
 */
red_black_tree_node *red_black_tree_successor ( red_black_tree_node *p_node );

/** !
 * Deallocate a red_black tree node
 * 
 * @param pp_red_black_tree_node pointer to red_black tree node pointer
 * 
 * @return 1 on success, 0 on error
 */
int red_black_tree_node_destroy ( red_black_tree_node **const pp_red_black_tree_node );

// function definitions
int red_black_tree_create ( red_black_tree **pp_red_black_tree )
{

    // argument check
    if ( pp_red_black_tree == (void *) 0 ) goto no_red_black_tree;

    // initialized data
    red_black_tree *p_red_black_tree = realloc(0, sizeof(red_black_tree));

    // error checking
    if ( p_red_black_tree == (void *) 0 ) goto no_mem;

    // Zero set the memory
    memset(p_red_black_tree, 0, sizeof(red_black_tree));

    // return a pointer to the caller
    *pp_red_black_tree = p_red_black_tree;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_red_black_tree:
                #ifndef NDEBUG
                    printf("[tree] [red_black] Null pointer provided for parameter \"pp_red_black_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library
        {
            no_mem:
                #ifndef NDEBUG
                    printf("[Standard Library] Call to function \"realloc\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int red_black_tree_node_create ( red_black_tree_node **pp_red_black_tree_node )
{

    // argument check
    if ( pp_red_black_tree_node == (void *) 0 ) goto no_red_black_tree_node;

    // initialized data
    red_black_tree_node *p_red_black_tree_node = realloc(0, sizeof(red_black_tree_node));

    // error checking
    if ( p_red_black_tree_node == (void *) 0 ) goto no_mem;

    // Zero set the memory
    memset(p_red_black_tree_node, 0, sizeof(red_black_tree_node));

    // Initialize red-black tree node properties
    p_red_black_tree_node->color = RED;  // New nodes are always red initially
    p_red_black_tree_node->p_parent = NULL;

    // return a pointer to the caller
    *pp_red_black_tree_node = p_red_black_tree_node;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_red_black_tree_node:
                #ifndef NDEBUG
                    printf("[tree] [red_black] Null pointer provided for parameter \"pp_red_black_tree_node\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library
        {
            no_mem:
                #ifndef NDEBUG
                    printf("[Standard Library] Call to function \"realloc\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int red_black_tree_node_allocate ( red_black_tree *p_red_black_tree, red_black_tree_node **pp_red_black_tree_node )
{

    // argument check
    if ( p_red_black_tree       == (void *) 0 ) goto no_red_black_tree;
    if ( pp_red_black_tree_node == (void *) 0 ) goto no_red_black_tree_node;
    
    // initialized data
    red_black_tree_node *p_red_black_tree_node = (void *) 0;

    // Allocate a node
    if ( red_black_tree_node_create(&p_red_black_tree_node) == 0 ) goto failed_to_allocate_node;

    // Store the node pointer
    p_red_black_tree_node->node_pointer = p_red_black_tree->metadata.node_quantity;

    // Increment the node quantity
    p_red_black_tree->metadata.node_quantity++;

    // return a pointer to the caller
    *pp_red_black_tree_node = p_red_black_tree_node;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_red_black_tree:
                #ifndef NDEBUG
                    printf("[tree] [red_black] Null pointer provided for parameter \"p_red_black_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
                
            no_red_black_tree_node:
                #ifndef NDEBUG
                    printf("[tree] [red_black] Null pointer provided for parameter \"pp_red_black_tree_node\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Tree
        {
            failed_to_allocate_node:
                #ifndef NDEBUG
                    printf("[tree] Call to function \"red_black_tree_node_create\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int red_black_tree_construct ( red_black_tree **const pp_red_black_tree, fn_comparator *pfn_comparator, fn_key_accessor *pfn_key_accessor, unsigned long long node_size )
{

    // argument check
    if ( pp_red_black_tree == (void *) 0 ) goto no_red_black_tree;

    // initialized data
    red_black_tree *p_red_black_tree = realloc(0, sizeof(red_black_tree));

    // error checking
    if ( p_red_black_tree == (void *) 0 ) goto failed_to_allocate_red_black_tree;

    // Populate the red_black tree structure
    *p_red_black_tree = (red_black_tree)
    {
        .p_root    = (void *) 0,
        .functions =
        {
            .pfn_comparator     = (pfn_comparator)     ? pfn_comparator     : default_comparator,
            .pfn_key_accessor = (pfn_key_accessor) ? pfn_key_accessor : default_key_accessor
        },
        .metadata =
        {
            .node_quantity = 0,
            .node_size     = node_size + ( 2 * sizeof(unsigned long long) )
        }
    };

    // Construct a lock
    mutex_create(&p_red_black_tree->_lock);

    // return a pointer to the caller
    *pp_red_black_tree = p_red_black_tree;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_red_black_tree:
                #ifndef NDEBUG
                    printf("[tree] [red_black] Null pointer provided for parameter \"pp_red_black_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Tree errors
        {
            failed_to_allocate_red_black_tree:
                #ifndef NDEBUG
                    printf("[tree] [red_black] Failed to allocate red_black tree in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

red_black_tree_node *red_black_tree_construct_balanced_recursive ( red_black_tree *p_red_black_tree, void **pp_values, size_t start, size_t end )
{

    // NOTE: This function has undefined behavior if p_red_black_tree, pp_keys, 
    //       and/or pp_values is null. Check your parameters before you call.

    // initialized data
    red_black_tree_node *p_red_black_tree_node = (void *) 0;

    // Base case
    if ( start == end )
    {

        // Allocate a red_black tree node
        if (red_black_tree_node_allocate(p_red_black_tree, &p_red_black_tree_node) == 0) goto failed_to_allocate_node;

        // Store the value
        p_red_black_tree_node->p_value = pp_values[start];
        
        // Set color to black for balanced tree construction
        p_red_black_tree_node->color = BLACK;
        
        // Done
        return p_red_black_tree_node;
    }

    // One property
    else if ( end - start == 1 )
    {

        // Allocate a red_black tree node
        if (red_black_tree_node_allocate(p_red_black_tree, &p_red_black_tree_node) == 0) goto failed_to_allocate_node;

        // Store the value
        p_red_black_tree_node->p_value = pp_values[end];

        // Allocate the left node
        if (red_black_tree_node_allocate(p_red_black_tree, &p_red_black_tree_node->p_left) == 0) goto failed_to_allocate_node;

        // Store the left value
        p_red_black_tree_node->p_left->p_value = pp_values[start];
        
        // Set parent pointer
        p_red_black_tree_node->p_left->p_parent = p_red_black_tree_node;
        
        // Set colors for balanced tree construction
        p_red_black_tree_node->color = BLACK;
        p_red_black_tree_node->p_left->color = BLACK;
        
        // Done
        return p_red_black_tree_node;
    }

    // Two properties
    else
    {

        // initialized data
        size_t median = (start + end) / 2;

        // Allocate a red_black tree node
        if (red_black_tree_node_allocate(p_red_black_tree, &p_red_black_tree_node) == 0) goto failed_to_allocate_node;

        // Store the value
        p_red_black_tree_node->p_value = pp_values[median];

        // Construct the left
        p_red_black_tree_node->p_left = red_black_tree_construct_balanced_recursive(p_red_black_tree, pp_values, start, median - 1);

        // Construct the right
        p_red_black_tree_node->p_right = red_black_tree_construct_balanced_recursive(p_red_black_tree, pp_values, median + 1, end);

        // Set parent pointers
        if ( p_red_black_tree_node->p_left != (void *) 0 )
            p_red_black_tree_node->p_left->p_parent = p_red_black_tree_node;
        
        if ( p_red_black_tree_node->p_right != (void *) 0 )
            p_red_black_tree_node->p_right->p_parent = p_red_black_tree_node;

        // Set color to black for balanced tree construction
        p_red_black_tree_node->color = BLACK;

        // Done
        return p_red_black_tree_node;
    }

    // error
    return (void *) 0;

    // error handling
    {

        // Tree errors
        {
            failed_to_allocate_node:
                #ifndef NDEBUG
                    printf("[tree] [red_black] Failed to allocate red_black tree node in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int red_black_tree_construct_balanced ( red_black_tree **const pp_red_black_tree, void **pp_values, size_t property_quantity, fn_comparator *pfn_comparator,  fn_key_accessor *pfn_key_accessor, unsigned long long node_size )
{

    // argument check
    if ( pp_red_black_tree == (void *) 0 ) goto no_red_black_tree;

    // initialized data
    red_black_tree *p_red_black_tree = realloc(0, sizeof(red_black_tree));

    // error checking
    if ( p_red_black_tree == (void *) 0 ) goto failed_to_allocate_red_black_tree;

    // Populate the red_black tree structure
    *p_red_black_tree = (red_black_tree)
    {
        .p_root    = (void *) 0,
        .functions =
        {
            .pfn_comparator = (pfn_comparator) ? pfn_comparator : default_comparator,
            .pfn_key_accessor = (pfn_key_accessor) ? pfn_key_accessor : default_key_accessor
        },
        .metadata =
        {
            .node_quantity = 0,
            .node_size     = node_size + ( 2 * sizeof(unsigned long long) )
        }
    };

    // Recursively construct a red_black search tree, and store the root
    p_red_black_tree->p_root = red_black_tree_construct_balanced_recursive(p_red_black_tree, pp_values, 0, property_quantity);

    // Ensure root is black (red-black tree property)
    if ( p_red_black_tree->p_root != (void *) 0 )
        p_red_black_tree->p_root->color = BLACK;

    // Construct a lock
    mutex_create(&p_red_black_tree->_lock);

    // return a pointer to the caller
    *pp_red_black_tree = p_red_black_tree;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_red_black_tree:
                #ifndef NDEBUG
                    printf("[tree] [red_black] Null pointer provided for parameter \"pp_red_black_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Tree errors
        {
            failed_to_allocate_red_black_tree:
                #ifndef NDEBUG
                    printf("[tree] [red_black] Failed to allocate red_black tree in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int red_black_tree_search ( red_black_tree *p_red_black_tree, const void *const p_key, void **pp_value )
{

    // argument check
    if ( p_red_black_tree == (void *) 0 ) goto no_red_black_tree;

    // State check
    if ( p_red_black_tree->p_root == (void *) 0 ) return 0;

    // lock
    mutex_lock(&p_red_black_tree->_lock);

    // initialized data
    red_black_tree_node *p_node = p_red_black_tree->p_root;
    int comparator_return = 0;

    try_again:

    // Which side? 
    comparator_return = p_red_black_tree->functions.pfn_comparator
    (
        p_red_black_tree->functions.pfn_key_accessor( p_node->p_value ),
        p_key
    );

    // Search the left node
    if ( comparator_return < 0 )
    {

        // If the left node exists ...
        if ( p_node->p_left )
        {

            // ... update the state ...
            p_node = p_node->p_left;

            // ... and try again
            goto try_again;
        }

        // unlock
        mutex_unlock(&p_red_black_tree->_lock);
        
        // error
        return 0;
    }

    // Search the right node
    else if ( comparator_return > 0 )
    {

        // If the right node exists ...
        if ( p_node->p_right )
        {

            // ... update the state ...
            p_node = p_node->p_right;

            // ... and try again
            goto try_again;
        }

        // unlock
        mutex_unlock(&p_red_black_tree->_lock);
    
        // error
        return 0;
    }

    // return a pointer to the caller
    *pp_value = p_node->p_value;

    // unlock
    mutex_unlock(&p_red_black_tree->_lock);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_red_black_tree:
                #ifndef NDEBUG
                    printf("[tree] [red_black] Null pointer provided for parameter \"p_red_black_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int red_black_tree_insert ( red_black_tree *p_red_black_tree, const void *const p_value )
{
    // argument check
    if ( p_red_black_tree == (void *) 0 ) goto no_red_black_tree;

    // lock
    mutex_lock(&p_red_black_tree->_lock);

    // initialized data
    red_black_tree_node *p_new_node = (void *) 0;
    red_black_tree_node *p_current = p_red_black_tree->p_root;
    red_black_tree_node *p_parent = (void *) 0;
    int comparator_return = 0;

    // Find the correct position for the new node
    while ( p_current != (void *) 0 )
    {
        p_parent = p_current;
        
        comparator_return = p_red_black_tree->functions.pfn_comparator
        (
            p_red_black_tree->functions.pfn_key_accessor(p_value),
            p_red_black_tree->functions.pfn_key_accessor(p_current->p_value)
        );

        if ( comparator_return < 0 )
            p_current = p_current->p_left;
        else if ( comparator_return > 0 )
            p_current = p_current->p_right;
        else
        {
            // Key already exists - update value and return
            // Note: This behavior depends on your requirements
            mutex_unlock(&p_red_black_tree->_lock);
            return 1;
        }
    }

    // Allocate a new node
    if ( red_black_tree_node_allocate(p_red_black_tree, &p_new_node) == 0 ) goto failed_to_allocate_red_black_tree_node;

    // Initialize the new node
    p_new_node->p_value = (void *) p_value;
    p_new_node->p_parent = p_parent;
    p_new_node->color = RED;  // New nodes are always red initially
    p_new_node->p_left = (void *) 0;
    p_new_node->p_right = (void *) 0;

    // Insert the node into the tree
    if ( p_parent == (void *) 0 )
    {
        // New node is the root
        p_red_black_tree->p_root = p_new_node;
        p_new_node->color = BLACK;  // Root is always black
    }
    else if ( comparator_return < 0 )
        p_parent->p_left = p_new_node;
    else
        p_parent->p_right = p_new_node;

    // Fix red-black tree properties
    if ( red_black_tree_insert_fixup(p_red_black_tree, p_new_node) == 0 ) goto failed_to_fixup;

    // unlock
    mutex_unlock(&p_red_black_tree->_lock);

    // success
    return 1;

    // error handling
    {
        // argument errors
        {
            no_red_black_tree:
                #ifndef NDEBUG
                    printf("[tree] [red_black] Null pointer provided for parameter \"p_red_black_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Tree errors
        {
            failed_to_allocate_red_black_tree_node:
                #ifndef NDEBUG
                    printf("[tree] [red_black] Failed to allocate red_black tree node in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // unlock
                mutex_unlock(&p_red_black_tree->_lock);
                
                // error
                return 0;

            failed_to_fixup:
                #ifndef NDEBUG
                    printf("[tree] [red_black] Failed to fix red-black tree properties in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // unlock
                mutex_unlock(&p_red_black_tree->_lock);
                
                // error
                return 0;
        }
    }
}

int red_black_tree_remove ( red_black_tree *const p_red_black_tree, const void *const p_key, const void **const pp_value )
{

    // argument check
    if ( p_red_black_tree == (void *) 0 ) goto no_red_black_tree;

    // lock
    mutex_lock(&p_red_black_tree->_lock);

    // State check
    if ( p_red_black_tree->p_root == (void *) 0 ) 
    {
        mutex_unlock(&p_red_black_tree->_lock);
        return 0;
    }

    // initialized data
    red_black_tree_node *p_node  = p_red_black_tree->p_root, *p_parent = NULL;
    int comparator_return = 0;

    // Find the node to be removed
    while (p_node != NULL) 
    {
        comparator_return = p_red_black_tree->functions.pfn_comparator(
            p_red_black_tree->functions.pfn_key_accessor(p_node->p_value),
            p_key
        );

        if (comparator_return == 0)
            break;

        p_parent = p_node;
        if (comparator_return > 0)
            p_node = p_node->p_left;
        else
            p_node = p_node->p_right;
    }

    // Node not found
    if (p_node == NULL) 
    {
        mutex_unlock(&p_red_black_tree->_lock);
        return 0;
    }

    // Return the value
    if ( pp_value != (void *) 0 )
        *pp_value = p_node->p_value;

    // initialized data for red-black tree deletion
    red_black_tree_node *p_to_delete = p_node;
    red_black_tree_node *p_replacement = (void *) 0;
    red_black_color original_color = p_to_delete->color;

    // Case 1: Node has no left child
    if ( p_to_delete->p_left == (void *) 0 )
    {
        p_replacement = p_to_delete->p_right;
        
        // Replace p_to_delete with its right child
        if ( p_to_delete->p_parent == (void *) 0 )
            p_red_black_tree->p_root = p_replacement;
        else if ( p_to_delete == p_to_delete->p_parent->p_left )
            p_to_delete->p_parent->p_left = p_replacement;
        else
            p_to_delete->p_parent->p_right = p_replacement;
        
        if ( p_replacement != (void *) 0 )
            p_replacement->p_parent = p_to_delete->p_parent;
    }
    // Case 2: Node has no right child
    else if ( p_to_delete->p_right == (void *) 0 )
    {
        p_replacement = p_to_delete->p_left;
        
        // Replace p_to_delete with its left child
        if ( p_to_delete->p_parent == (void *) 0 )
            p_red_black_tree->p_root = p_replacement;
        else if ( p_to_delete == p_to_delete->p_parent->p_left )
            p_to_delete->p_parent->p_left = p_replacement;
        else
            p_to_delete->p_parent->p_right = p_replacement;
        
        if ( p_replacement != (void *) 0 )
            p_replacement->p_parent = p_to_delete->p_parent;
    }
    // Case 3: Node has two children
    else
    {
        // Find the successor (minimum in right subtree)
        red_black_tree_node *p_successor = red_black_tree_minimum(p_to_delete->p_right);
        
        // Save original color of successor
        original_color = p_successor->color;
        p_replacement = p_successor->p_right;
        
        if ( p_successor->p_parent == p_to_delete )
        {
            // Successor is direct child of node to delete
            if ( p_replacement != (void *) 0 )
                p_replacement->p_parent = p_successor;
        }
        else
        {
            // Replace successor with its right child
            if ( p_replacement != (void *) 0 )
                p_replacement->p_parent = p_successor->p_parent;
            p_successor->p_parent->p_left = p_replacement;
            
            // Move successor to replace node to delete
            p_successor->p_right = p_to_delete->p_right;
            p_successor->p_right->p_parent = p_successor;
        }
        
        // Replace node to delete with successor
        if ( p_to_delete->p_parent == (void *) 0 )
            p_red_black_tree->p_root = p_successor;
        else if ( p_to_delete == p_to_delete->p_parent->p_left )
            p_to_delete->p_parent->p_left = p_successor;
        else
            p_to_delete->p_parent->p_right = p_successor;
            
        p_successor->p_parent = p_to_delete->p_parent;
        p_successor->p_left = p_to_delete->p_left;
        p_successor->p_left->p_parent = p_successor;
        p_successor->color = p_to_delete->color;
    }

    // Decrement node count
    p_red_black_tree->metadata.node_quantity--;

    // Free the deleted node
    p_to_delete->p_left = (void *) 0;
    p_to_delete->p_right = (void *) 0;
    p_to_delete->p_parent = (void *) 0;
    red_black_tree_node_destroy(&p_to_delete);

    // Fix red-black tree properties if a black node was deleted
    if ( original_color == BLACK && p_red_black_tree->p_root != (void *) 0 )
    {
        red_black_tree_delete_fixup(p_red_black_tree, p_replacement);
    }

    // unlock
    mutex_unlock(&p_red_black_tree->_lock);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_red_black_tree:
                #ifndef NDEBUG
                    printf("[tree] [red_black] Null pointer provided for parameter \"p_red_black_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int red_black_tree_traverse_preorder_node ( red_black_tree_node *p_red_black_tree_node, fn_red_black_tree_traverse *pfn_traverse )
{

    // argument check
    if ( p_red_black_tree_node == (void *) 0 ) goto no_red_black_tree_node;
    if ( pfn_traverse       == (void *) 0 ) goto no_traverse_function;

    // Root
    pfn_traverse(p_red_black_tree_node->p_value);

    // Left
    if ( p_red_black_tree_node->p_left ) red_black_tree_traverse_preorder_node(p_red_black_tree_node->p_left, pfn_traverse);

    // Right
    if ( p_red_black_tree_node->p_right ) red_black_tree_traverse_preorder_node(p_red_black_tree_node->p_right, pfn_traverse);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_red_black_tree_node:
                #ifndef NDEBUG
                    log_error("[tree] [red_black] Null pointer provided for parameter \"p_red_black_tree_node\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_traverse_function:
                #ifndef NDEBUG
                    log_error("[tree] [red_black] Null pointer provided for parameter \"pfn_traverse\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int red_black_tree_traverse_inorder_node ( red_black_tree_node *p_red_black_tree_node, fn_red_black_tree_traverse *pfn_traverse )
{

    // argument check
    if ( p_red_black_tree_node == (void *) 0 ) goto no_red_black_tree_node;
    if ( pfn_traverse       == (void *) 0 ) goto no_traverse_function;

    // Left
    if ( p_red_black_tree_node->p_left ) red_black_tree_traverse_inorder_node(p_red_black_tree_node->p_left, pfn_traverse);

    // Root
    pfn_traverse(p_red_black_tree_node->p_value);

    // Right
    if ( p_red_black_tree_node->p_right ) red_black_tree_traverse_inorder_node(p_red_black_tree_node->p_right, pfn_traverse);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_red_black_tree_node:
                #ifndef NDEBUG
                    log_error("[tree] [red_black] Null pointer provided for parameter \"p_red_black_tree_node\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_traverse_function:
                #ifndef NDEBUG
                    log_error("[tree] [red_black] Null pointer provided for parameter \"pfn_traverse\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int red_black_tree_traverse_postorder_node ( red_black_tree_node *p_red_black_tree_node, fn_red_black_tree_traverse *pfn_traverse )
{

    // argument check
    if ( p_red_black_tree_node == (void *) 0 ) goto no_red_black_tree_node;
    if ( pfn_traverse       == (void *) 0 ) goto no_traverse_function;

    // Left
    if ( p_red_black_tree_node->p_left ) red_black_tree_traverse_postorder_node(p_red_black_tree_node->p_left, pfn_traverse);

    // Right
    if ( p_red_black_tree_node->p_right ) red_black_tree_traverse_postorder_node(p_red_black_tree_node->p_right, pfn_traverse);

    // Root
    pfn_traverse(p_red_black_tree_node->p_value);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_red_black_tree_node:
                #ifndef NDEBUG
                    log_error("[tree] [red_black] Null pointer provided for parameter \"p_red_black_tree_node\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_traverse_function:
                #ifndef NDEBUG
                    log_error("[tree] [red_black] Null pointer provided for parameter \"pfn_traverse\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int red_black_tree_traverse_preorder ( red_black_tree *const p_red_black_tree, fn_red_black_tree_traverse *pfn_traverse )
{

    // argument check
    if ( p_red_black_tree == (void *) 0 ) goto no_red_black_tree;
    if ( pfn_traverse  == (void *) 0 ) goto no_traverse_function;

    // edge case
    if ( 0 == p_red_black_tree->metadata.node_quantity ) return 1;

    // lock
    mutex_lock(&p_red_black_tree->_lock);

    // Traverse the tree
    if ( red_black_tree_traverse_preorder_node(p_red_black_tree->p_root, pfn_traverse) == 0 ) goto failed_to_traverse_red_black_tree;    

    // unlock
    mutex_unlock(&p_red_black_tree->_lock);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_red_black_tree:
                #ifndef NDEBUG
                    log_error("[tree] [red_black] Null pointer provided for parameter \"p_red_black_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_traverse_function:
                #ifndef NDEBUG
                    log_error("[tree] [red_black] Null pointer provided for parameter \"pfn_traverse\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Tree errors
        {
            failed_to_traverse_red_black_tree:
                #ifndef NDEBUG
                    log_error("[tree] [red_black] Failed to traverse red_black tree in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // unlock
                mutex_unlock(&p_red_black_tree->_lock);
                
                // error
                return 0;
        }
    }
}

int red_black_tree_traverse_inorder ( red_black_tree *const p_red_black_tree, fn_red_black_tree_traverse *pfn_traverse )
{

    // argument check
    if ( p_red_black_tree == (void *) 0 ) goto no_red_black_tree;
    if ( pfn_traverse  == (void *) 0 ) goto no_traverse_function;

    // edge case
    if ( 0 == p_red_black_tree->metadata.node_quantity ) return 1;

    // lock
    mutex_lock(&p_red_black_tree->_lock);

    // Traverse the tree
    if ( red_black_tree_traverse_inorder_node(p_red_black_tree->p_root, pfn_traverse) == 0 ) goto failed_to_traverse_red_black_tree;    

    // unlock
    mutex_unlock(&p_red_black_tree->_lock);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_red_black_tree:
                #ifndef NDEBUG
                    log_error("[tree] [red_black] Null pointer provided for parameter \"p_red_black_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_traverse_function:
                #ifndef NDEBUG
                    log_error("[tree] [red_black] Null pointer provided for parameter \"pfn_traverse\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Tree errors
        {
            failed_to_traverse_red_black_tree:
                #ifndef NDEBUG
                    log_error("[tree] [red_black] Failed to traverse red_black tree in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // unlock
                mutex_unlock(&p_red_black_tree->_lock);
                
                // error
                return 0;
        }
    }
}

int red_black_tree_traverse_postorder ( red_black_tree *const p_red_black_tree, fn_red_black_tree_traverse *pfn_traverse )
{


    // argument check
    if ( p_red_black_tree == (void *) 0 ) goto no_red_black_tree;
    if ( pfn_traverse  == (void *) 0 ) goto no_traverse_function;

    // edge case
    if ( 0 == p_red_black_tree->metadata.node_quantity ) return 1;
    
    // lock
    mutex_lock(&p_red_black_tree->_lock);

    // Traverse the tree
    if ( red_black_tree_traverse_postorder_node(p_red_black_tree->p_root, pfn_traverse) == 0 ) goto failed_to_traverse_red_black_tree;    

    // unlock
    mutex_unlock(&p_red_black_tree->_lock);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_red_black_tree:
                #ifndef NDEBUG
                    log_error("[tree] [red_black] Null pointer provided for parameter \"p_red_black_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_traverse_function:
                #ifndef NDEBUG
                    log_error("[tree] [red_black] Null pointer provided for parameter \"pfn_traverse\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Tree errors
        {
            failed_to_traverse_red_black_tree:
                #ifndef NDEBUG
                    log_error("[tree] [red_black] Failed to traverse red_black tree in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // unlock
                mutex_unlock(&p_red_black_tree->_lock);
                
                // error
                return 0;
        }
    }
}

int red_black_tree_parse ( red_black_tree **const pp_red_black_tree, const char *p_file, fn_comparator *pfn_comparator, fn_key_accessor *pfn_key_accessor, fn_red_black_tree_parse *pfn_parse_node )
{
    
    // argument check
    if ( pp_red_black_tree == (void *) 0 ) goto no_red_black_tree;
    if ( p_file         == (void *) 0 ) goto no_file;
    if ( pfn_parse_node == (void *) 0 ) goto no_parser;

    // initialized data
    red_black_tree *p_red_black_tree = 0;
    unsigned long long node_quantity = 0,
                       node_size     = 0;
    FILE *p_f = fopen(p_file, "rw");

    // Read the metadata
    {

        // Set the cursor to the start of the file
        fseek(p_f, 0, SEEK_SET);

        // Read the quantity of nodes
        fread(&node_quantity, sizeof(unsigned long long), 1, p_f);

        // Read the size of a node
        fread(&node_size, sizeof(unsigned long long), 1, p_f);
    }

    // Allocate a red_black tree
    if ( red_black_tree_construct(&p_red_black_tree, pfn_comparator, pfn_key_accessor, node_size) == 0 ) goto failed_to_construct_red_black_tree;

    // Read the root node
    if ( red_black_tree_parse_node(p_f, p_red_black_tree, &p_red_black_tree->p_root, pfn_parse_node) == 0 ) goto failed_to_construct_red_black_tree;

    // error check
    if ( p_red_black_tree->metadata.node_quantity != node_quantity ) goto failed_to_parse_red_black_tree; 

    // return a pointer to the caller
    *pp_red_black_tree = p_red_black_tree;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {            
            no_red_black_tree:
                #ifndef NDEBUG
                    printf("[tree] [red_black] Null pointer provided for parameter \"pp_red_black_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_file:
                #ifndef NDEBUG
                    printf("[tree] [red_black] Null pointer provided for parameter \"p_file\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_parser:
                #ifndef NDEBUG
                    printf("[tree] [red_black] Null pointer provided for parameter \"pfn_parse_node\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Tree errors
        {
            failed_to_construct_red_black_tree:
                #ifndef NDEBUG
                    printf("[tree] [red_black] Failed to construct red_black tree in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            failed_to_parse_red_black_tree:
                #ifndef NDEBUG
                    printf("[tree] [red_black] Failed to parse red_black tree in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int red_black_tree_parse_node ( FILE *p_file, red_black_tree *p_red_black_tree, red_black_tree_node **pp_red_black_tree_node, fn_red_black_tree_parse *pfn_red_black_tree_parse )
{

    // argument check
    if ( p_file                == (void *) 0 ) goto no_file;
    if ( p_red_black_tree         == (void *) 0 ) goto no_red_black_tree;
    if ( pp_red_black_tree_node   == (void *) 0 ) goto no_red_black_tree_node;
    if ( pfn_red_black_tree_parse == (void *) 0 ) goto no_red_black_tree_parser; 

    // initialized data
    red_black_tree_node *p_red_black_tree_node = realloc(0, sizeof(red_black_tree_node));
    unsigned long long left_pointer, right_pointer;
    
    memset(p_red_black_tree_node, 0, sizeof(red_black_tree_node));

    p_red_black_tree_node->node_pointer = ( ftell(p_file) ) / (p_red_black_tree->metadata.node_size);

    // Set the pointer correctly
    fseek(p_file, (long) ( sizeof(p_red_black_tree->metadata) + (p_red_black_tree_node->node_pointer * ( p_red_black_tree->metadata.node_size ))), SEEK_SET);

    // User provided parsing function
    pfn_red_black_tree_parse(p_file, p_red_black_tree_node);
    
    // Store the left pointer
    fread(&left_pointer, 8, 1, p_file);
    // printf("[%04d] -> left : < %llu, %lld >\n", p_red_black_tree_node->node_pointer, ftell(p_file), left_pointer);

    // Store the right pointer
    fread(&right_pointer, 8, 1, p_file);
    // printf("       -> right: < %llu, %lld >\n", ftell(p_file), right_pointer);
    
    // State check
    if ( left_pointer == eight_bytes_of_f ) goto parse_right;

    // Set the pointer correctly
    fseek(p_file, (long) ( sizeof(p_red_black_tree->metadata) + (left_pointer * ( p_red_black_tree->metadata.node_size ))), SEEK_SET);

    // Parse the left node
    if ( red_black_tree_parse_node(p_file, p_red_black_tree, &p_red_black_tree_node->p_left, pfn_red_black_tree_parse) == 0 ) goto failed_to_parse_node;

    parse_right:

    // State check
    if ( right_pointer == eight_bytes_of_f ) goto done;

    // Set the pointer correctly
    fseek(p_file, (long) ( sizeof(p_red_black_tree->metadata) + (right_pointer * ( p_red_black_tree->metadata.node_size))), SEEK_SET);

    // Parse the right node
    if ( red_black_tree_parse_node(p_file, p_red_black_tree, &p_red_black_tree_node->p_right, pfn_red_black_tree_parse) == 0 ) goto failed_to_parse_node;

    done:

    // return a pointer to the caller
    *pp_red_black_tree_node = p_red_black_tree_node;

    // Increment the node quantity
    p_red_black_tree->metadata.node_quantity++;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_file:
                #ifndef NDEBUG
                    printf("[tree] [red_black] Null pointer provided for parameter \"p_file\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            no_red_black_tree:
                #ifndef NDEBUG
                    printf("[tree] [red_black] Null pointer provided for parameter \"p_red_black_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_red_black_tree_node:
                #ifndef NDEBUG
                    printf("[tree] [red_black] Null pointer provided for parameter \"p_red_black_tree_node\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            no_red_black_tree_parser:
                #ifndef NDEBUG
                    printf("[tree] [red_black] Null pointer provided for parameter \"pfn_red_black_tree_parse\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Tree errors
        {
            failed_to_parse_node:
                #ifndef NDEBUG
                    printf("[tree] [red_black] Failed to parse node in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int red_black_tree_serialize_node ( FILE *p_file, red_black_tree *p_red_black_tree, red_black_tree_node *p_red_black_tree_node, fn_red_black_tree_serialize *pfn_red_black_tree_serialize )
{

    // argument check
    if ( p_file                    == (void *) 0 ) goto no_file;
    if ( p_red_black_tree             == (void *) 0 ) goto no_red_black_tree;
    if ( p_red_black_tree_node        == (void *) 0 ) goto no_red_black_tree_node;
    if ( pfn_red_black_tree_serialize == (void *) 0 ) goto no_red_black_tree_serializer; 

    // initialized data
    long offset = (long) ( sizeof(p_red_black_tree->metadata) + (p_red_black_tree_node->node_pointer * p_red_black_tree->metadata.node_size) );

    // Set the pointer correctly
    fseek(p_file, offset, SEEK_SET);

    // Serialize the node
    pfn_red_black_tree_serialize(p_file, p_red_black_tree_node);
    
    // Write the left pointer to the output
    fwrite((p_red_black_tree_node->p_left) ? &p_red_black_tree_node->p_left->node_pointer : &eight_bytes_of_f, sizeof(void *), 1, p_file);

    // Write the right pointer to the output
    fwrite((p_red_black_tree_node->p_right) ? &p_red_black_tree_node->p_right->node_pointer : &eight_bytes_of_f, sizeof(void *), 1, p_file);

    // Write the left node to the output
    if ( p_red_black_tree_node->p_left ) red_black_tree_serialize_node(p_file, p_red_black_tree, p_red_black_tree_node->p_left, pfn_red_black_tree_serialize);

    // Write the right node to the output
    if ( p_red_black_tree_node->p_right ) red_black_tree_serialize_node(p_file, p_red_black_tree, p_red_black_tree_node->p_right, pfn_red_black_tree_serialize);

    // success
    return 1;
    
    // error handling
    {

        // argument errors
        {
            no_file:
                #ifndef NDEBUG
                    printf("[tree] [red_black] Null pointer provided for parameter \"p_file\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            no_red_black_tree:
                #ifndef NDEBUG
                    printf("[tree] [red_black] Null pointer provided for parameter \"p_red_black_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_red_black_tree_node:
                #ifndef NDEBUG
                    printf("[tree] [red_black] Null pointer provided for parameter \"p_red_black_tree_node\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            no_red_black_tree_serializer:
                #ifndef NDEBUG
                    printf("[tree] [red_black] Null pointer provided for parameter \"pfn_red_black_tree_serialize\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int red_black_tree_serialize ( red_black_tree *const p_red_black_tree, const char *p_path, fn_red_black_tree_serialize *pfn_serialize_node )
{

    // argument check
    if ( p_red_black_tree      == (void *) 0 ) goto no_red_black_tree;
    if ( p_path             == (void *) 0 ) goto no_file;
    if ( pfn_serialize_node == (void *) 0 ) goto no_serializer;

    // lock
    mutex_lock(&p_red_black_tree->_lock);

    // Open the file
    p_red_black_tree->p_random_access = fopen(p_path, "wb+");

    // error check
    if ( p_red_black_tree->p_random_access == NULL ) goto failed_to_open_file;

    // Write the metadata
    {

        // Set the cursor to the start of the file
        fseek(p_red_black_tree->p_random_access, 0, SEEK_SET);

        // Write the quantity of nodes
        fwrite(&p_red_black_tree->metadata.node_quantity, sizeof(unsigned long long), 1, p_red_black_tree->p_random_access);

        // Write the size of a node
        fwrite(&p_red_black_tree->metadata.node_size, sizeof(unsigned long long), 1, p_red_black_tree->p_random_access);
    }

    // Write the root node
    if ( red_black_tree_serialize_node(p_red_black_tree->p_random_access, p_red_black_tree, p_red_black_tree->p_root, pfn_serialize_node) == 0 ) goto failed_to_serialize_node;

    // Flush the file
    fflush(p_red_black_tree->p_random_access);

    // unlock
    mutex_unlock(&p_red_black_tree->_lock);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_red_black_tree:
                #ifndef NDEBUG
                    printf("[tree] [red_black] Null pointer provided for parameter \"p_red_black_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            no_file:
                #ifndef NDEBUG
                    printf("[tree] [red_black] Null pointer provided for parameter \"p_file\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_serializer:
                #ifndef NDEBUG
                    printf("[tree] [red_black] Null pointer provided for parameter \"pfn_serialize_node\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    
        // tree errors
        {
            failed_to_serialize_node:
                #ifndef NDEBUG
                    printf("[tree] [red_black] Failed to serialize node in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library errrors
        {
            failed_to_open_file:
                #ifndef NDEBUG
                    printf("[Standard Library] Failed to open file\n");
                #endif

                // error
                return 0;
        }
    }
}

int red_black_tree_node_destroy ( red_black_tree_node **const pp_red_black_tree_node )
{

    // argument check
    if ( pp_red_black_tree_node == (void *) 0 ) goto no_red_black_tree_node;

    // initialized data
    red_black_tree_node *p_red_black_tree_node = *pp_red_black_tree_node;

    // Fast exit
    if ( p_red_black_tree_node == (void *) 0 ) return 1;

    // Recursively free the left node
    if ( red_black_tree_node_destroy(&p_red_black_tree_node->p_left) == 0 ) goto failed_to_free;

    // Recursively free the right node
    if ( red_black_tree_node_destroy(&p_red_black_tree_node->p_right) == 0 ) goto failed_to_free;

    // Free the node
    *pp_red_black_tree_node = realloc(p_red_black_tree_node, 0);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_red_black_tree_node:
                #ifndef NDEBUG
                    printf("[tree] [red_black] Null pointer provided for parameter \"pp_red_black_tree_node\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Tree errors
        {
            failed_to_free:
                #ifndef NDEBUG
                    printf("[tree] [red_black] Failed to free red_black tree in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0; 
        }
    }
}

// Red-Black Tree Helper Functions

int red_black_tree_rotate_left ( red_black_tree *p_red_black_tree, red_black_tree_node *p_node )
{
    // argument check
    if ( p_red_black_tree == (void *) 0 ) return 0;
    if ( p_node == (void *) 0 ) return 0;
    if ( p_node->p_right == (void *) 0 ) return 0;

    // initialized data
    red_black_tree_node *p_right_child = p_node->p_right;

    // Turn right child's left subtree into node's right subtree
    p_node->p_right = p_right_child->p_left;
    if ( p_right_child->p_left != (void *) 0 )
        p_right_child->p_left->p_parent = p_node;

    // Link node's parent to right child
    p_right_child->p_parent = p_node->p_parent;
    if ( p_node->p_parent == (void *) 0 )
        p_red_black_tree->p_root = p_right_child;
    else if ( p_node == p_node->p_parent->p_left )
        p_node->p_parent->p_left = p_right_child;
    else
        p_node->p_parent->p_right = p_right_child;

    // Put node on right child's left
    p_right_child->p_left = p_node;
    p_node->p_parent = p_right_child;

    return 1;
}

int red_black_tree_rotate_right ( red_black_tree *p_red_black_tree, red_black_tree_node *p_node )
{
    // argument check
    if ( p_red_black_tree == (void *) 0 ) return 0;
    if ( p_node == (void *) 0 ) return 0;
    if ( p_node->p_left == (void *) 0 ) return 0;

    // initialized data
    red_black_tree_node *p_left_child = p_node->p_left;

    // Turn left child's right subtree into node's left subtree
    p_node->p_left = p_left_child->p_right;
    if ( p_left_child->p_right != (void *) 0 )
        p_left_child->p_right->p_parent = p_node;

    // Link node's parent to left child
    p_left_child->p_parent = p_node->p_parent;
    if ( p_node->p_parent == (void *) 0 )
        p_red_black_tree->p_root = p_left_child;
    else if ( p_node == p_node->p_parent->p_right )
        p_node->p_parent->p_right = p_left_child;
    else
        p_node->p_parent->p_left = p_left_child;

    // Put node on left child's right
    p_left_child->p_right = p_node;
    p_node->p_parent = p_left_child;

    return 1;
}

red_black_tree_node *red_black_tree_minimum ( red_black_tree_node *p_node )
{
    if ( p_node == (void *) 0 ) return (void *) 0;
    
    while ( p_node->p_left != (void *) 0 )
        p_node = p_node->p_left;
    
    return p_node;
}

red_black_tree_node *red_black_tree_successor ( red_black_tree_node *p_node )
{
    if ( p_node == (void *) 0 ) return (void *) 0;

    if ( p_node->p_right != (void *) 0 )
        return red_black_tree_minimum(p_node->p_right);

    red_black_tree_node *p_parent = p_node->p_parent;
    while ( p_parent != (void *) 0 && p_node == p_parent->p_right )
    {
        p_node = p_parent;
        p_parent = p_parent->p_parent;
    }

    return p_parent;
}

int red_black_tree_insert_fixup ( red_black_tree *p_red_black_tree, red_black_tree_node *p_node )
{
    // argument check
    if ( p_red_black_tree == (void *) 0 ) return 0;
    if ( p_node == (void *) 0 ) return 0;

    while ( p_node->p_parent != (void *) 0 && p_node->p_parent->color == RED )
    {
        if ( p_node->p_parent == p_node->p_parent->p_parent->p_left )
        {
            red_black_tree_node *p_uncle = p_node->p_parent->p_parent->p_right;
            
            // Case 1: Uncle is red
            if ( p_uncle != (void *) 0 && p_uncle->color == RED )
            {
                p_node->p_parent->color = BLACK;
                p_uncle->color = BLACK;
                p_node->p_parent->p_parent->color = RED;
                p_node = p_node->p_parent->p_parent;
            }
            else
            {
                // Case 2: Uncle is black and node is a right child
                if ( p_node == p_node->p_parent->p_right )
                {
                    p_node = p_node->p_parent;
                    red_black_tree_rotate_left(p_red_black_tree, p_node);
                }
                
                // Case 3: Uncle is black and node is a left child
                p_node->p_parent->color = BLACK;
                p_node->p_parent->p_parent->color = RED;
                red_black_tree_rotate_right(p_red_black_tree, p_node->p_parent->p_parent);
            }
        }
        else
        {
            red_black_tree_node *p_uncle = p_node->p_parent->p_parent->p_left;
            
            // Case 1: Uncle is red
            if ( p_uncle != (void *) 0 && p_uncle->color == RED )
            {
                p_node->p_parent->color = BLACK;
                p_uncle->color = BLACK;
                p_node->p_parent->p_parent->color = RED;
                p_node = p_node->p_parent->p_parent;
            }
            else
            {
                // Case 2: Uncle is black and node is a left child
                if ( p_node == p_node->p_parent->p_left )
                {
                    p_node = p_node->p_parent;
                    red_black_tree_rotate_right(p_red_black_tree, p_node);
                }
                
                // Case 3: Uncle is black and node is a right child
                p_node->p_parent->color = BLACK;
                p_node->p_parent->p_parent->color = RED;
                red_black_tree_rotate_left(p_red_black_tree, p_node->p_parent->p_parent);
            }
        }
    }

    // Root is always black
    p_red_black_tree->p_root->color = BLACK;

    return 1;
}

int red_black_tree_delete_fixup ( red_black_tree *p_red_black_tree, red_black_tree_node *p_node )
{
    // argument check
    if ( p_red_black_tree == (void *) 0 ) return 0;

    while ( p_node != p_red_black_tree->p_root && (p_node == (void *) 0 || p_node->color == BLACK) )
    {
        if ( p_node == (p_node ? p_node->p_parent->p_left : p_red_black_tree->p_root) )
        {
            red_black_tree_node *p_sibling = (p_node && p_node->p_parent) ? p_node->p_parent->p_right : (void *) 0;
            
            if ( p_sibling && p_sibling->color == RED )
            {
                p_sibling->color = BLACK;
                if ( p_node && p_node->p_parent ) p_node->p_parent->color = RED;
                if ( p_node && p_node->p_parent ) red_black_tree_rotate_left(p_red_black_tree, p_node->p_parent);
                p_sibling = (p_node && p_node->p_parent) ? p_node->p_parent->p_right : (void *) 0;
            }
            
            if ( (!p_sibling || !p_sibling->p_left || p_sibling->p_left->color == BLACK) &&
                 (!p_sibling || !p_sibling->p_right || p_sibling->p_right->color == BLACK) )
            {
                if ( p_sibling ) p_sibling->color = RED;
                p_node = (p_node && p_node->p_parent) ? p_node->p_parent : p_red_black_tree->p_root;
            }
            else
            {
                if ( !p_sibling || !p_sibling->p_right || p_sibling->p_right->color == BLACK )
                {
                    if ( p_sibling && p_sibling->p_left ) p_sibling->p_left->color = BLACK;
                    if ( p_sibling ) p_sibling->color = RED;
                    if ( p_sibling ) red_black_tree_rotate_right(p_red_black_tree, p_sibling);
                    p_sibling = (p_node && p_node->p_parent) ? p_node->p_parent->p_right : (void *) 0;
                }
                
                if ( p_sibling && p_node && p_node->p_parent ) p_sibling->color = p_node->p_parent->color;
                if ( p_node && p_node->p_parent ) p_node->p_parent->color = BLACK;
                if ( p_sibling && p_sibling->p_right ) p_sibling->p_right->color = BLACK;
                if ( p_node && p_node->p_parent ) red_black_tree_rotate_left(p_red_black_tree, p_node->p_parent);
                p_node = p_red_black_tree->p_root;
            }
        }
        else
        {
            red_black_tree_node *p_sibling = (p_node && p_node->p_parent) ? p_node->p_parent->p_left : (void *) 0;
            
            if ( p_sibling && p_sibling->color == RED )
            {
                p_sibling->color = BLACK;
                if ( p_node && p_node->p_parent ) p_node->p_parent->color = RED;
                if ( p_node && p_node->p_parent ) red_black_tree_rotate_right(p_red_black_tree, p_node->p_parent);
                p_sibling = (p_node && p_node->p_parent) ? p_node->p_parent->p_left : (void *) 0;
            }
            
            if ( (!p_sibling || !p_sibling->p_right || p_sibling->p_right->color == BLACK) &&
                 (!p_sibling || !p_sibling->p_left || p_sibling->p_left->color == BLACK) )
            {
                if ( p_sibling ) p_sibling->color = RED;
                p_node = (p_node && p_node->p_parent) ? p_node->p_parent : p_red_black_tree->p_root;
            }
            else
            {
                if ( !p_sibling || !p_sibling->p_left || p_sibling->p_left->color == BLACK )
                {
                    if ( p_sibling && p_sibling->p_right ) p_sibling->p_right->color = BLACK;
                    if ( p_sibling ) p_sibling->color = RED;
                    if ( p_sibling ) red_black_tree_rotate_left(p_red_black_tree, p_sibling);
                    p_sibling = (p_node && p_node->p_parent) ? p_node->p_parent->p_left : (void *) 0;
                }
                
                if ( p_sibling && p_node && p_node->p_parent ) p_sibling->color = p_node->p_parent->color;
                if ( p_node && p_node->p_parent ) p_node->p_parent->color = BLACK;
                if ( p_sibling && p_sibling->p_left ) p_sibling->p_left->color = BLACK;
                if ( p_node && p_node->p_parent ) red_black_tree_rotate_right(p_red_black_tree, p_node->p_parent);
                p_node = p_red_black_tree->p_root;
            }
        }
    }

    if ( p_node ) p_node->color = BLACK;

    return 1;
}

int red_black_tree_destroy ( red_black_tree **const pp_red_black_tree )
{

    // argument check
    if ( pp_red_black_tree == (void *) 0 ) goto no_red_black_tree;

    // initialized data
    red_black_tree *p_red_black_tree = *pp_red_black_tree;

    // Fast exit
    if ( p_red_black_tree == (void *) 0 ) return 1;

    // lock
    mutex_lock(&p_red_black_tree->_lock);

    // No more pointer for caller
    *pp_red_black_tree = (void *) 0;

    // unlock
    mutex_unlock(&p_red_black_tree->_lock);

    // Recursively free nodes
    if ( red_black_tree_node_destroy(&p_red_black_tree->p_root) == 0 ) goto failed_to_free_nodes;

    // Close the file
    if ( p_red_black_tree->p_random_access ) fclose(p_red_black_tree->p_random_access);

    // Destroy the lock
    mutex_destroy(&p_red_black_tree->_lock);

    // Release the tree
    p_red_black_tree = realloc(p_red_black_tree, 0);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_red_black_tree:
                #ifndef NDEBUG
                    printf("[tree] [red_black] Null pointer provided for parameter \"pp_red_black_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Tree error
        {
            failed_to_free_nodes:
                #ifndef NDEBUG
                    printf("[tree] [red_black] Failed to free red_black tree nodes in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}
