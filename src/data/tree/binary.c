/** !
 * Implementation of binary search tree
 * 
 * @file binary.c
 * 
 * @author Jacob Smith
 */

// header file
#include <data/binary.h>

// static data
static const unsigned long long eight_bytes_of_f = 0xffffffffffffffff;

// forward declarations
/** !
 * Allocate memory for a binary tree
 * 
 * @param pp_binary_tree return
 * 
 * @return 1 on success, 0 on error
 */
int binary_tree_create ( binary_tree **const pp_binary_tree );

/** !
 * Allocate memory for a binary tree node
 * 
 * @param pp_binary_tree_node return
 * 
 * @return 1 on success, 0 on error
 */
int binary_tree_node_create ( binary_tree_node **pp_binary_tree_node );

/** !
 * Allocate a node for a specific binary tree, and set the node pointer. 
 * 
 * The node pointer is an integer ordinal that is atomically incremented 
 * each time the allocator is called. 
 * 
 * @param p_binary_tree       the binary tree to allocate a node to
 * @param pp_binary_tree_node return
 * 
 * @return 1 on success, 0 on error
 */
int binary_tree_node_allocate ( binary_tree *p_binary_tree, binary_tree_node **pp_binary_tree_node );

/** !
 * Recursively construct a balanced binary search tree from a sorted list of keys and values
 * 
 * @param p_binary_tree the binary tree
 * @param pp_values     the list of values
 * @param start         the starting index 
 * @param end           the ending index
 * 
 * @return the root node 
 */
binary_tree_node *binary_tree_construct_balanced_recursive ( binary_tree *p_binary_tree, void **pp_values, size_t start, size_t end );

/** !
 * Recursively serialize binary tree nodes to a file
 * 
 * @param p_file                    the file
 * @param p_binary_tree             the binary tree
 * @param p_binary_tree_node        the binary tree node
 * @param pfn_binary_tree_serialize the node serializer function
 * 
 * @return 1 on success, 0 on error 
 */
int binary_tree_serialize_node ( FILE *p_file, binary_tree *p_binary_tree, binary_tree_node *p_binary_tree_node, fn_binary_tree_serialize *pfn_binary_tree_serialize );

/** !
 * Recursively parse binary tree nodes from a file
 * 
 * @param p_file                    the file
 * @param p_binary_tree             the binary tree
 * @param pp_binary_tree_node       result
 * @param pfn_binary_tree_serialize the node parser function
 * 
 * @return 1 on success, 0 on error 
 */
int binary_tree_parse_node ( FILE *p_file, binary_tree *p_binary_tree, binary_tree_node **pp_binary_tree_node, fn_binary_tree_parse *pfn_binary_tree_parse );

/** !
 * Traverse a binary tree using the pre order technique
 * 
 * @param p_binary_tree_node pointer to binary tree node
 * @param pfn_traverse       called for each node in the binary tree
 * 
 * @return 1 on success, 0 on error
*/
int binary_tree_traverse_preorder_node ( binary_tree_node *p_binary_tree_node, fn_binary_tree_traverse *pfn_traverse );

/** !
 * Traverse a binary tree using the in order technique
 * 
 * @param p_binary_tree_node pointer to binary tree node
 * @param pfn_traverse       called for each node in the binary tree
 * 
 * @return 1 on success, 0 on error
*/
int binary_tree_traverse_inorder_node ( binary_tree_node *p_binary_tree_node, fn_binary_tree_traverse *pfn_traverse );

/** !
 * Traverse a binary tree using the post order technique
 * 
 * @param p_binary_tree_node pointer to binary tree node
 * @param pfn_traverse       called for each node in the binary tree
 * 
 * @return 1 on success, 0 on error
*/
int binary_tree_traverse_postorder_node ( binary_tree_node *p_binary_tree_node, fn_binary_tree_traverse *pfn_traverse );

/** !
 * Deallocate a binary tree node
 * 
 * @param pp_binary_tree_node pointer to binary tree node pointer
 * 
 * @return 1 on success, 0 on error
 */
int binary_tree_node_destroy ( binary_tree_node **const pp_binary_tree_node );

// function definitions
int binary_tree_create ( binary_tree **pp_binary_tree )
{

    // argument check
    if ( pp_binary_tree == (void *) 0 ) goto no_binary_tree;

    // initialized data
    binary_tree *p_binary_tree = realloc(0, sizeof(binary_tree));

    // error checking
    if ( p_binary_tree == (void *) 0 ) goto no_mem;

    // Zero set the memory
    memset(p_binary_tree, 0, sizeof(binary_tree));

    // return a pointer to the caller
    *pp_binary_tree = p_binary_tree;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_binary_tree:
                #ifndef NDEBUG
                    printf("[tree] [binary] Null pointer provided for parameter \"pp_binary_tree\" in call to function \"%s\"\n", __FUNCTION__);
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

int binary_tree_node_create ( binary_tree_node **pp_binary_tree_node )
{

    // argument check
    if ( pp_binary_tree_node == (void *) 0 ) goto no_binary_tree_node;

    // initialized data
    binary_tree_node *p_binary_tree_node = realloc(0, sizeof(binary_tree_node));

    // error checking
    if ( p_binary_tree_node == (void *) 0 ) goto no_mem;

    // Zero set the memory
    memset(p_binary_tree_node, 0, sizeof(binary_tree_node));

    // return a pointer to the caller
    *pp_binary_tree_node = p_binary_tree_node;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_binary_tree_node:
                #ifndef NDEBUG
                    printf("[tree] [binary] Null pointer provided for parameter \"pp_binary_tree_node\" in call to function \"%s\"\n", __FUNCTION__);
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

int binary_tree_node_allocate ( binary_tree *p_binary_tree, binary_tree_node **pp_binary_tree_node )
{

    // argument check
    if ( p_binary_tree       == (void *) 0 ) goto no_binary_tree;
    if ( pp_binary_tree_node == (void *) 0 ) goto no_binary_tree_node;
    
    // initialized data
    binary_tree_node *p_binary_tree_node = (void *) 0;

    // Allocate a node
    if ( binary_tree_node_create(&p_binary_tree_node) == 0 ) goto failed_to_allocate_node;

    // Store the node pointer
    p_binary_tree_node->node_pointer = p_binary_tree->metadata.node_quantity;

    // Increment the node quantity
    p_binary_tree->metadata.node_quantity++;

    // return a pointer to the caller
    *pp_binary_tree_node = p_binary_tree_node;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_binary_tree:
                #ifndef NDEBUG
                    printf("[tree] [binary] Null pointer provided for parameter \"p_binary_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
                
            no_binary_tree_node:
                #ifndef NDEBUG
                    printf("[tree] [binary] Null pointer provided for parameter \"pp_binary_tree_node\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Tree
        {
            failed_to_allocate_node:
                #ifndef NDEBUG
                    printf("[tree] Call to function \"binary_tree_node_create\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // unlock
                mutex_unlock(&p_binary_tree->_lock);
                
                // error
                return 0;
        }
    }
}

int binary_tree_construct ( binary_tree **const pp_binary_tree, fn_tree_equal *pfn_is_equal, fn_tree_key_accessor *pfn_key_accessor, unsigned long long node_size )
{

    // argument check
    if ( pp_binary_tree == (void *) 0 ) goto no_binary_tree;

    // initialized data
    binary_tree *p_binary_tree = realloc(0, sizeof(binary_tree));

    // error checking
    if ( p_binary_tree == (void *) 0 ) goto failed_to_allocate_binary_tree;

    // Populate the binary tree structure
    *p_binary_tree = (binary_tree)
    {
        .p_root    = (void *) 0,
        .functions =
        {
            .pfn_is_equal     = (pfn_is_equal)     ? pfn_is_equal     : tree_compare_function,
            .pfn_key_accessor = (pfn_key_accessor) ? pfn_key_accessor : tree_key_is_value
        },
        .metadata =
        {
            .node_quantity = 0,
            .node_size     = node_size + ( 2 * sizeof(unsigned long long) )
        }
    };

    // Construct a lock
    mutex_create(&p_binary_tree->_lock);

    // return a pointer to the caller
    *pp_binary_tree = p_binary_tree;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_binary_tree:
                #ifndef NDEBUG
                    printf("[tree] [binary] Null pointer provided for parameter \"pp_binary_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Tree errors
        {
            failed_to_allocate_binary_tree:
                #ifndef NDEBUG
                    printf("[tree] [binary] Failed to allocate binary tree in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

binary_tree_node *binary_tree_construct_balanced_recursive ( binary_tree *p_binary_tree, void **pp_values, size_t start, size_t end )
{

    // NOTE: This function has undefined behavior if p_binary_tree, pp_keys, 
    //       and/or pp_values is null. Check your parameters before you call.

    // initialized data
    binary_tree_node *p_binary_tree_node = (void *) 0;

    // Base case
    if ( start == end )
    {

        // Allocate a binary tree node
        if (binary_tree_node_allocate(p_binary_tree, &p_binary_tree_node) == 0) goto failed_to_allocate_node;

        // Store the value
        p_binary_tree_node->p_value = pp_values[start];
        
        // Done
        return p_binary_tree_node;
    }

    // One property
    else if ( end - start == 1 )
    {

        // Allocate a binary tree node
        if (binary_tree_node_allocate(p_binary_tree, &p_binary_tree_node) == 0) goto failed_to_allocate_node;

        // Store the value
        p_binary_tree_node->p_value = pp_values[end];

        // Allocate the left node
        if (binary_tree_node_allocate(p_binary_tree, &p_binary_tree_node->p_left) == 0) goto failed_to_allocate_node;

        // Store the left value
        p_binary_tree_node->p_left->p_value = pp_values[start];
        
        // Done
        return p_binary_tree_node;
    }

    // Two properties
    else
    {

        // initialized data
        size_t median = (start + end) / 2;

        // Allocate a binary tree node
        if (binary_tree_node_allocate(p_binary_tree, &p_binary_tree_node) == 0) goto failed_to_allocate_node;

        // Store the value
        p_binary_tree_node->p_value = pp_values[median];

        // Construct the left
        p_binary_tree_node->p_left = binary_tree_construct_balanced_recursive(p_binary_tree, pp_values, start, median - 1);

        // Construct the right
        p_binary_tree_node->p_right = binary_tree_construct_balanced_recursive(p_binary_tree, pp_values, median + 1, end);

        // Done
        return p_binary_tree_node;
    }

    // error
    return (void *) 0;

    // error handling
    {

        // Tree errors
        {
            failed_to_allocate_node:
                #ifndef NDEBUG
                    printf("[tree] [binary] Failed to allocate binary tree node in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int binary_tree_construct_balanced ( binary_tree **const pp_binary_tree, void **pp_values, size_t property_quantity, fn_tree_equal *pfn_is_equal,  fn_tree_key_accessor *pfn_key_accessor, unsigned long long node_size )
{

    // argument check
    if ( pp_binary_tree == (void *) 0 ) goto no_binary_tree;

    // initialized data
    binary_tree *p_binary_tree = realloc(0, sizeof(binary_tree));

    // error checking
    if ( p_binary_tree == (void *) 0 ) goto failed_to_allocate_binary_tree;

    // Populate the binary tree structure
    *p_binary_tree = (binary_tree)
    {
        .p_root    = (void *) 0,
        .functions =
        {
            .pfn_is_equal = (pfn_is_equal) ? pfn_is_equal : tree_compare_function,
            .pfn_key_accessor = (pfn_key_accessor) ? pfn_key_accessor : tree_key_is_value
        },
        .metadata =
        {
            .node_quantity = 0,
            .node_size     = node_size + ( 2 * sizeof(unsigned long long) )
        }
    };

    // Recursively construct a binary search tree, and store the root
    p_binary_tree->p_root = binary_tree_construct_balanced_recursive(p_binary_tree, pp_values, 0, property_quantity);

    // return a pointer to the caller
    *pp_binary_tree = p_binary_tree;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_binary_tree:
                #ifndef NDEBUG
                    printf("[tree] [binary] Null pointer provided for parameter \"pp_binary_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Tree errors
        {
            failed_to_allocate_binary_tree:
                #ifndef NDEBUG
                    printf("[tree] [binary] Failed to allocate binary tree in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int binary_tree_search ( binary_tree *p_binary_tree, const void *const p_key, void **pp_value )
{

    // argument check
    if ( p_binary_tree == (void *) 0 ) goto no_binary_tree;

    // State check
    if ( p_binary_tree->p_root == (void *) 0 ) return 0;

    // lock
    mutex_lock(&p_binary_tree->_lock);

    // initialized data
    binary_tree_node *p_node = p_binary_tree->p_root;
    int comparator_return = 0;

    try_again:

    // Which side? 
    comparator_return = p_binary_tree->functions.pfn_is_equal
    (
        p_binary_tree->functions.pfn_key_accessor( p_node->p_value ),
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
        mutex_unlock(&p_binary_tree->_lock);
        
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
        mutex_unlock(&p_binary_tree->_lock);
    
        // error
        return 0;
    }

    // return a pointer to the caller
    *pp_value = p_node->p_value;

    // unlock
    mutex_unlock(&p_binary_tree->_lock);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_binary_tree:
                #ifndef NDEBUG
                    printf("[tree] [binary] Null pointer provided for parameter \"p_binary_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int binary_tree_insert ( binary_tree *p_binary_tree, const void *const p_value )
{

    // argument check
    if ( p_binary_tree == (void *) 0 ) goto no_binary_tree;

    // lock
    mutex_lock(&p_binary_tree->_lock);

    // initialized data
    binary_tree_node *p_node = p_binary_tree->p_root;
    int comparator_return = 0;

    // State check
    if ( p_binary_tree->p_root == (void *) 0 ) goto no_root;

    try_again:

    // Which side? 
    comparator_return = p_binary_tree->functions.pfn_is_equal
    (
        p_binary_tree->functions.pfn_key_accessor(p_node->p_value),
        p_binary_tree->functions.pfn_key_accessor(p_value)
    );

    // Store the node on the left 
    if ( comparator_return < 0 )
    {

        // If the left node is occupied ...
        if ( p_node->p_left )
        {

            // ... update the state ...
            p_node = p_node->p_left;

            // ... and try again
            goto try_again;
        }

        // Construct a binary tree node
        if ( binary_tree_node_allocate(p_binary_tree, &p_node->p_left) == 0 ) goto failed_to_allocate_binary_tree_node;

        // Store the value
        p_node->p_left->p_value = (void *) p_value;
    }

    // Store the node on the right
    else if ( comparator_return > 0 )
    {

        // If the left node is occupied ...
        if ( p_node->p_right )
        {

            // ... update the state ...
            p_node = p_node->p_right;

            // ... and try again
            goto try_again;
        }

        // Construct a binary tree node
        if ( binary_tree_node_allocate(p_binary_tree, &p_node->p_right) == 0 ) goto failed_to_allocate_binary_tree_node;

        // Store the value
        p_node->p_right->p_value = (void *) p_value;
        
    }

    // unlock
    mutex_unlock(&p_binary_tree->_lock);

    // success
    return 1;

    // This branch runs if there is no root node
    no_root:
    {

        // initialized data
        p_node = (void *) 0;

        // Construct a binary tree node
        if ( binary_tree_node_allocate(p_binary_tree, &p_node) == 0 ) goto failed_to_allocate_binary_tree_node;

        // Store the value
        p_node->p_value = (void *) p_value;

        // Store the node as the root of the tree
        p_binary_tree->p_root = p_node;

        // unlock
        mutex_unlock(&p_binary_tree->_lock);
        
        // success
        return 1;
    }

    // error handling
    {

        // argument errors
        {
            no_binary_tree:
                #ifndef NDEBUG
                    printf("[tree] [binary] Null pointer provided for parameter \"p_binary_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Tree errors
        {
            failed_to_allocate_binary_tree_node:
                #ifndef NDEBUG
                    printf("[tree] [binary] Failed to allocate binary tree node in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // unlock
                mutex_unlock(&p_binary_tree->_lock);
                
                // error
                return 0;
        }
    }
}

int binary_tree_remove ( binary_tree *const p_binary_tree, const void *const p_key, const void **const pp_value )
{

    // argument check
    if ( p_binary_tree == (void *) 0 ) goto no_binary_tree;

    // lock
    mutex_lock(&p_binary_tree->_lock);

    // State check
    if ( p_binary_tree->p_root == (void *) 0 ) return 0;

    // initialized data
    binary_tree_node *p_node  = p_binary_tree->p_root;
    void *p_value = 0;
    int comparator_return = 0;

    try_again:

    // Which side? 
    comparator_return = p_binary_tree->functions.pfn_is_equal(
        p_binary_tree->functions.pfn_key_accessor( p_node->p_value ),
        p_key
    );

    // Check the node on the left 
    if ( comparator_return < 0 )
    {

        // If the left node is occupied ...
        if ( p_node->p_left )
        {
            
            // If the left node is the target, remove it ...
            if ( p_binary_tree->functions.pfn_is_equal(p_binary_tree->functions.pfn_key_accessor(p_node->p_left->p_value), p_key) == 0 ) goto remove_left;

            // ... otherwise, update the state ...
            p_node = p_node->p_left;

            // ... and try again
            goto try_again;
        }

        // error
        return 0;
    }

    // Check the node on the right
    else if ( comparator_return > 0 )
    {

        // If the left node is occupied ...
        if ( p_node->p_right )
        {

            // If the left node is the target, remove it ...
            if ( p_binary_tree->functions.pfn_is_equal(p_binary_tree->functions.pfn_key_accessor(p_node->p_right->p_value), p_key) == 0 ) goto remove_right;

            // ... otherwise, update the state ...
            p_node = p_node->p_right;

            // ... and try again
            goto try_again;
        }

        // error
        return 0;
    }

    done:

    // return a pointer to the caller
    *pp_value = p_value;

    // unlock
    mutex_unlock(&p_binary_tree->_lock);

    // success
    return 1;

    // Remove left
    remove_left:
    {

        // initialized data
        binary_tree_node *p_left = p_node->p_left;

        // Store the return value
        p_value = p_left->p_value;

        // Leaf
        if ( ( p_left->p_left || p_left->p_right ) == 0 )
        {

            // Free the node
            binary_tree_node_destroy(&p_node->p_left);
        }

        // Left
        else if ( p_left->p_left && ( p_left->p_right == 0 ) )
        {
            
            // Update the new left node
            p_left = p_left->p_left;

            // Free the node
            binary_tree_node_destroy(&p_node->p_left);

            // Repair the tree
            p_node->p_left = p_left;
        }

        // Right
        else if ( ( p_left->p_left == 0 ) && p_left->p_right )
        {
            
            // Update the new left node
            p_left = p_left->p_right;

            // Free the node
            binary_tree_node_destroy(&p_node->p_left);

            // Repair the tree
            p_node->p_left = p_left;
        }

        // TODO: Left AND right
        else
        {
            // Find the successor
            // Swap the successor
            // Done
        }
        
        // Done
        goto done;
    }

    // Remove right
    remove_right:
    {

        // initialized data
        binary_tree_node *p_right = p_node->p_right;
        
        // Store the return value
        p_value = p_right->p_value;

        // Leaf
        if ( ( p_right->p_left || p_right->p_right ) == 0 )
        {

            // Free the node
            binary_tree_node_destroy(&p_node->p_right);
        }


        // Left
        else if ( p_right->p_left && ( p_right->p_right == 0 ) )
        {
            
            // Update the new left node
            p_right = p_right->p_left;

            // Free the node
            binary_tree_node_destroy(&p_node->p_right);

            // Repair the tree
            p_node->p_right = p_right;
        }

        // Right
        else if ( ( p_right->p_left == 0 ) && p_right->p_right )
        {
            
            // Update the new left node
            p_right = p_right->p_right;

            // Free the node
            binary_tree_node_destroy(&p_node->p_right);

            // Repair the tree
            p_node->p_right = p_right;
        }

        // TODO: Left AND right
        else
        {
            // Find the successor
            // Swap the successor
            // Done
        }

        // Done
        goto done;
    }

    // error handling
    {

        // argument errors
        {
            no_binary_tree:
                #ifndef NDEBUG
                    printf("[tree] [binary] Null pointer provided for parameter \"p_binary_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int binary_tree_traverse_preorder_node ( binary_tree_node *p_binary_tree_node, fn_binary_tree_traverse *pfn_traverse )
{

    // argument check
    if ( p_binary_tree_node == (void *) 0 ) goto no_binary_tree_node;
    if ( pfn_traverse       == (void *) 0 ) goto no_traverse_function;

    // Root
    pfn_traverse(p_binary_tree_node->p_value);

    // Left
    if ( p_binary_tree_node->p_left ) binary_tree_traverse_preorder_node(p_binary_tree_node->p_left, pfn_traverse);

    // Right
    if ( p_binary_tree_node->p_right ) binary_tree_traverse_preorder_node(p_binary_tree_node->p_right, pfn_traverse);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_binary_tree_node:
                #ifndef NDEBUG
                    log_error("[tree] [binary] Null pointer provided for parameter \"p_binary_tree_node\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_traverse_function:
                #ifndef NDEBUG
                    log_error("[tree] [binary] Null pointer provided for parameter \"pfn_traverse\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int binary_tree_traverse_inorder_node ( binary_tree_node *p_binary_tree_node, fn_binary_tree_traverse *pfn_traverse )
{

    // argument check
    if ( p_binary_tree_node == (void *) 0 ) goto no_binary_tree_node;
    if ( pfn_traverse       == (void *) 0 ) goto no_traverse_function;

    // Left
    if ( p_binary_tree_node->p_left ) binary_tree_traverse_inorder_node(p_binary_tree_node->p_left, pfn_traverse);

    // Root
    pfn_traverse(p_binary_tree_node->p_value);

    // Right
    if ( p_binary_tree_node->p_right ) binary_tree_traverse_inorder_node(p_binary_tree_node->p_right, pfn_traverse);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_binary_tree_node:
                #ifndef NDEBUG
                    log_error("[tree] [binary] Null pointer provided for parameter \"p_binary_tree_node\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_traverse_function:
                #ifndef NDEBUG
                    log_error("[tree] [binary] Null pointer provided for parameter \"pfn_traverse\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int binary_tree_traverse_postorder_node ( binary_tree_node *p_binary_tree_node, fn_binary_tree_traverse *pfn_traverse )
{

    // argument check
    if ( p_binary_tree_node == (void *) 0 ) goto no_binary_tree_node;
    if ( pfn_traverse       == (void *) 0 ) goto no_traverse_function;

    // Left
    if ( p_binary_tree_node->p_left ) binary_tree_traverse_postorder_node(p_binary_tree_node->p_left, pfn_traverse);

    // Right
    if ( p_binary_tree_node->p_right ) binary_tree_traverse_postorder_node(p_binary_tree_node->p_right, pfn_traverse);

    // Root
    pfn_traverse(p_binary_tree_node->p_value);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_binary_tree_node:
                #ifndef NDEBUG
                    log_error("[tree] [binary] Null pointer provided for parameter \"p_binary_tree_node\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_traverse_function:
                #ifndef NDEBUG
                    log_error("[tree] [binary] Null pointer provided for parameter \"pfn_traverse\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int binary_tree_traverse_preorder ( binary_tree *const p_binary_tree, fn_binary_tree_traverse *pfn_traverse )
{

    // argument check
    if ( p_binary_tree == (void *) 0 ) goto no_binary_tree;
    if ( pfn_traverse  == (void *) 0 ) goto no_traverse_function;

    // lock
    mutex_lock(&p_binary_tree->_lock);

    // Traverse the tree
    if ( binary_tree_traverse_preorder_node(p_binary_tree->p_root, pfn_traverse) == 0 ) goto failed_to_traverse_binary_tree;    

    // unlock
    mutex_unlock(&p_binary_tree->_lock);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_binary_tree:
                #ifndef NDEBUG
                    log_error("[tree] [binary] Null pointer provided for parameter \"p_binary_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_traverse_function:
                #ifndef NDEBUG
                    log_error("[tree] [binary] Null pointer provided for parameter \"pfn_traverse\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Tree errors
        {
            failed_to_traverse_binary_tree:
                #ifndef NDEBUG
                    log_error("[tree] [binary] Failed to traverse binary tree in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // unlock
                mutex_unlock(&p_binary_tree->_lock);
                
                // error
                return 0;
        }
    }
}

int binary_tree_traverse_inorder ( binary_tree *const p_binary_tree, fn_binary_tree_traverse *pfn_traverse )
{

    // argument check
    if ( p_binary_tree == (void *) 0 ) goto no_binary_tree;
    if ( pfn_traverse  == (void *) 0 ) goto no_traverse_function;

    // lock
    mutex_lock(&p_binary_tree->_lock);

    // Traverse the tree
    if ( binary_tree_traverse_inorder_node(p_binary_tree->p_root, pfn_traverse) == 0 ) goto failed_to_traverse_binary_tree;    

    // unlock
    mutex_unlock(&p_binary_tree->_lock);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_binary_tree:
                #ifndef NDEBUG
                    log_error("[tree] [binary] Null pointer provided for parameter \"p_binary_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_traverse_function:
                #ifndef NDEBUG
                    log_error("[tree] [binary] Null pointer provided for parameter \"pfn_traverse\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Tree errors
        {
            failed_to_traverse_binary_tree:
                #ifndef NDEBUG
                    log_error("[tree] [binary] Failed to traverse binary tree in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // unlock
                mutex_unlock(&p_binary_tree->_lock);
                
                // error
                return 0;
        }
    }
}

int binary_tree_traverse_postorder ( binary_tree *const p_binary_tree, fn_binary_tree_traverse *pfn_traverse )
{


    // argument check
    if ( p_binary_tree == (void *) 0 ) goto no_binary_tree;
    if ( pfn_traverse  == (void *) 0 ) goto no_traverse_function;

    // lock
    mutex_lock(&p_binary_tree->_lock);

    // Traverse the tree
    if ( binary_tree_traverse_postorder_node(p_binary_tree->p_root, pfn_traverse) == 0 ) goto failed_to_traverse_binary_tree;    

    // unlock
    mutex_unlock(&p_binary_tree->_lock);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_binary_tree:
                #ifndef NDEBUG
                    log_error("[tree] [binary] Null pointer provided for parameter \"p_binary_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_traverse_function:
                #ifndef NDEBUG
                    log_error("[tree] [binary] Null pointer provided for parameter \"pfn_traverse\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Tree errors
        {
            failed_to_traverse_binary_tree:
                #ifndef NDEBUG
                    log_error("[tree] [binary] Failed to traverse binary tree in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // unlock
                mutex_unlock(&p_binary_tree->_lock);
                
                // error
                return 0;
        }
    }
}

int binary_tree_parse ( binary_tree **const pp_binary_tree, const char *p_file, fn_tree_equal *pfn_is_equal, fn_tree_key_accessor *pfn_tree_key_accessor, fn_binary_tree_parse *pfn_parse_node )
{
    
    // argument check
    if ( pp_binary_tree == (void *) 0 ) goto no_binary_tree;
    if ( p_file         == (void *) 0 ) goto no_file;
    if ( pfn_parse_node == (void *) 0 ) goto no_parser;

    // initialized data
    binary_tree *p_binary_tree = 0;
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

    // Allocate a binary tree
    if ( binary_tree_construct(&p_binary_tree, pfn_is_equal, pfn_tree_key_accessor, node_size-sizeof(p_binary_tree->metadata)) == 0 ) goto failed_to_construct_binary_tree;

    // Read the root node
    if ( binary_tree_parse_node(p_f, p_binary_tree, &p_binary_tree->p_root, pfn_parse_node) == 0 ) goto failed_to_construct_binary_tree;

    // error check
    if ( p_binary_tree->metadata.node_quantity != node_quantity ) goto failed_to_parse_binary_tree; 

    // return a pointer to the caller
    *pp_binary_tree = p_binary_tree;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {            
            no_binary_tree:
                #ifndef NDEBUG
                    printf("[tree] [binary] Null pointer provided for parameter \"pp_binary_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_file:
                #ifndef NDEBUG
                    printf("[tree] [binary] Null pointer provided for parameter \"p_file\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_parser:
                #ifndef NDEBUG
                    printf("[tree] [binary] Null pointer provided for parameter \"pfn_parse_node\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Tree errors
        {
            failed_to_construct_binary_tree:
                #ifndef NDEBUG
                    printf("[tree] [binary] Failed to construct binary tree in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            failed_to_parse_binary_tree:
                #ifndef NDEBUG
                    printf("[tree] [binary] Failed to parse binary tree in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int binary_tree_parse_node ( FILE *p_file, binary_tree *p_binary_tree, binary_tree_node **pp_binary_tree_node, fn_binary_tree_parse *pfn_binary_tree_parse )
{

    // argument check
    if ( p_file                == (void *) 0 ) goto no_file;
    if ( p_binary_tree         == (void *) 0 ) goto no_binary_tree;
    if ( pp_binary_tree_node   == (void *) 0 ) goto no_binary_tree_node;
    if ( pfn_binary_tree_parse == (void *) 0 ) goto no_binary_tree_parser; 

    // initialized data
    binary_tree_node *p_binary_tree_node = realloc(0, sizeof(binary_tree_node));
    unsigned long long left_pointer, right_pointer;
    
    memset(p_binary_tree_node, 0, sizeof(binary_tree_node));

    p_binary_tree_node->node_pointer = ( ftell(p_file) ) / (p_binary_tree->metadata.node_size);

    // Set the pointer correctly
    fseek(p_file, (long) ( sizeof(p_binary_tree->metadata) + (p_binary_tree_node->node_pointer * ( p_binary_tree->metadata.node_size ))), SEEK_SET);

    // User provided parsing function
    pfn_binary_tree_parse(p_file, p_binary_tree_node);
    
    // Store the left pointer
    fread(&left_pointer, 8, 1, p_file);
    // printf("[%04d] -> left : < %llu, %lld >\n", p_binary_tree_node->node_pointer, ftell(p_file), left_pointer);

    // Store the right pointer
    fread(&right_pointer, 8, 1, p_file);
    // printf("       -> right: < %llu, %lld >\n", ftell(p_file), right_pointer);
    
    // State check
    if ( left_pointer == eight_bytes_of_f ) goto parse_right;

    // Set the pointer correctly
    fseek(p_file, (long) ( sizeof(p_binary_tree->metadata) + (left_pointer * ( p_binary_tree->metadata.node_size ))), SEEK_SET);

    // Parse the left node
    if ( binary_tree_parse_node(p_file, p_binary_tree, &p_binary_tree_node->p_left, pfn_binary_tree_parse) == 0 ) goto failed_to_parse_node;

    parse_right:

    // State check
    if ( right_pointer == eight_bytes_of_f ) goto done;

    // Set the pointer correctly
    fseek(p_file, (long) ( sizeof(p_binary_tree->metadata) + (right_pointer * ( p_binary_tree->metadata.node_size))), SEEK_SET);

    // Parse the right node
    if ( binary_tree_parse_node(p_file, p_binary_tree, &p_binary_tree_node->p_right, pfn_binary_tree_parse) == 0 ) goto failed_to_parse_node;

    done:

    // return a pointer to the caller
    *pp_binary_tree_node = p_binary_tree_node;

    // Increment the node quantity
    p_binary_tree->metadata.node_quantity++;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_file:
                #ifndef NDEBUG
                    printf("[tree] [binary] Null pointer provided for parameter \"p_file\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            no_binary_tree:
                #ifndef NDEBUG
                    printf("[tree] [binary] Null pointer provided for parameter \"p_binary_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_binary_tree_node:
                #ifndef NDEBUG
                    printf("[tree] [binary] Null pointer provided for parameter \"p_binary_tree_node\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            no_binary_tree_parser:
                #ifndef NDEBUG
                    printf("[tree] [binary] Null pointer provided for parameter \"pfn_binary_tree_parse\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Tree errors
        {
            failed_to_parse_node:
                #ifndef NDEBUG
                    printf("[tree] [binary] Failed to parse node in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int binary_tree_serialize_node ( FILE *p_file, binary_tree *p_binary_tree, binary_tree_node *p_binary_tree_node, fn_binary_tree_serialize *pfn_binary_tree_serialize )
{

    // argument check
    if ( p_file                    == (void *) 0 ) goto no_file;
    if ( p_binary_tree             == (void *) 0 ) goto no_binary_tree;
    if ( p_binary_tree_node        == (void *) 0 ) goto no_binary_tree_node;
    if ( pfn_binary_tree_serialize == (void *) 0 ) goto no_binary_tree_serializer; 

    // initialized data
    long offset = (long) ( sizeof(p_binary_tree->metadata) + (p_binary_tree_node->node_pointer * p_binary_tree->metadata.node_size) );

    // Set the pointer correctly
    fseek(p_file, offset, SEEK_SET);

    // Serialize the node
    pfn_binary_tree_serialize(p_file, p_binary_tree_node);
    
    // Write the left pointer to the output
    fwrite((p_binary_tree_node->p_left) ? &p_binary_tree_node->p_left->node_pointer : &eight_bytes_of_f, sizeof(void *), 1, p_file);

    // Write the right pointer to the output
    fwrite((p_binary_tree_node->p_right) ? &p_binary_tree_node->p_right->node_pointer : &eight_bytes_of_f, sizeof(void *), 1, p_file);

    // Write the left node to the output
    if ( p_binary_tree_node->p_left ) binary_tree_serialize_node(p_file, p_binary_tree, p_binary_tree_node->p_left, pfn_binary_tree_serialize);

    // Write the right node to the output
    if ( p_binary_tree_node->p_right ) binary_tree_serialize_node(p_file, p_binary_tree, p_binary_tree_node->p_right, pfn_binary_tree_serialize);

    // success
    return 1;
    
    // error handling
    {

        // argument errors
        {
            no_file:
                #ifndef NDEBUG
                    printf("[tree] [binary] Null pointer provided for parameter \"p_file\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            no_binary_tree:
                #ifndef NDEBUG
                    printf("[tree] [binary] Null pointer provided for parameter \"p_binary_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_binary_tree_node:
                #ifndef NDEBUG
                    printf("[tree] [binary] Null pointer provided for parameter \"p_binary_tree_node\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            no_binary_tree_serializer:
                #ifndef NDEBUG
                    printf("[tree] [binary] Null pointer provided for parameter \"pfn_binary_tree_serialize\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int binary_tree_serialize ( binary_tree *const p_binary_tree, const char *p_path, fn_binary_tree_serialize *pfn_serialize_node )
{

    // argument check
    if ( p_binary_tree      == (void *) 0 ) goto no_binary_tree;
    if ( p_path             == (void *) 0 ) goto no_file;
    if ( pfn_serialize_node == (void *) 0 ) goto no_serializer;

    // lock
    mutex_lock(&p_binary_tree->_lock);

    // Open the file
    p_binary_tree->p_random_access = fopen(p_path, "wb+");

    // error check
    if ( p_binary_tree->p_random_access == NULL ) goto failed_to_open_file;

    // Write the metadata
    {

        // Set the cursor to the start of the file
        fseek(p_binary_tree->p_random_access, 0, SEEK_SET);

        // Write the quantity of nodes
        fwrite(&p_binary_tree->metadata.node_quantity, sizeof(unsigned long long), 1, p_binary_tree->p_random_access);

        // Write the size of a node
        fwrite(&p_binary_tree->metadata.node_size, sizeof(unsigned long long), 1, p_binary_tree->p_random_access);
    }

    // Write the root node
    if ( binary_tree_serialize_node(p_binary_tree->p_random_access, p_binary_tree, p_binary_tree->p_root, pfn_serialize_node) == 0 ) goto failed_to_serialize_node;

    // Flush the file
    fflush(p_binary_tree->p_random_access);

    // unlock
    mutex_unlock(&p_binary_tree->_lock);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_binary_tree:
                #ifndef NDEBUG
                    printf("[tree] [binary] Null pointer provided for parameter \"p_binary_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            no_file:
                #ifndef NDEBUG
                    printf("[tree] [binary] Null pointer provided for parameter \"p_file\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_serializer:
                #ifndef NDEBUG
                    printf("[tree] [binary] Null pointer provided for parameter \"pfn_serialize_node\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    
        // tree errors
        {
            failed_to_serialize_node:
                #ifndef NDEBUG
                    printf("[tree] [binary] Failed to serialize node in call to function \"%s\"\n", __FUNCTION__);
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

int binary_tree_node_destroy ( binary_tree_node **const pp_binary_tree_node )
{

    // argument check
    if ( pp_binary_tree_node == (void *) 0 ) goto no_binary_tree_node;

    // initialized data
    binary_tree_node *p_binary_tree_node = *pp_binary_tree_node;

    // Fast exit
    if ( p_binary_tree_node == (void *) 0 ) return 1;

    // Recursively free the left node
    if ( binary_tree_node_destroy(&p_binary_tree_node->p_left) == 0 ) goto failed_to_free;

    // Recursively free the right node
    if ( binary_tree_node_destroy(&p_binary_tree_node->p_right) == 0 ) goto failed_to_free;

    // Free the node
    p_binary_tree_node = realloc(p_binary_tree_node, 0);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_binary_tree_node:
                #ifndef NDEBUG
                    printf("[tree] [binary] Null pointer provided for parameter \"pp_binary_tree_node\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Tree errors
        {
            failed_to_free:
                #ifndef NDEBUG
                    printf("[tree] [binary] Failed to free binary tree in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0; 
        }
    }
}

int binary_tree_destroy ( binary_tree **const pp_binary_tree )
{

    // argument check
    if ( pp_binary_tree == (void *) 0 ) goto no_binary_tree;

    // initialized data
    binary_tree *p_binary_tree = *pp_binary_tree;

    // Fast exit
    if ( p_binary_tree == (void *) 0 ) return 1;

    // lock
    mutex_lock(&p_binary_tree->_lock);

    // No more pointer for caller
    *pp_binary_tree = (void *) 0;

    // unlock
    mutex_unlock(&p_binary_tree->_lock);

    // Recursively free nodes
    if ( binary_tree_node_destroy(&p_binary_tree->p_root) == 0 ) goto failed_to_free_nodes;

    // Close the file
    if ( p_binary_tree->p_random_access ) fclose(p_binary_tree->p_random_access);

    // Destroy the lock
    mutex_destroy(&p_binary_tree->_lock);

    // Release the tree
    p_binary_tree = realloc(p_binary_tree, 0);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_binary_tree:
                #ifndef NDEBUG
                    printf("[tree] [binary] Null pointer provided for parameter \"pp_binary_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Tree error
        {
            failed_to_free_nodes:
                #ifndef NDEBUG
                    printf("[tree] [binary] Failed to free binary tree nodes in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}
