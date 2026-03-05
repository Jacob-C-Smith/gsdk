/** !
 * Binary search tree implementation
 * 
 * @file src/data/binary/binary.c
 * 
 * @author Jacob Smith
 */

// header file
#include <data/binary.h>

// static data
static const unsigned long long eight_bytes_of_f = 0xffffffffffffffff;

// forward declarations
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
int binary_tree_allocate_node ( binary_tree *p_binary_tree, binary_tree_node **pp_binary_tree_node );

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
binary_tree_node *binary_tree_construct_balanced_recursive 
( 
    binary_tree  *p_binary_tree, 
    void        **pp_values, 
    size_t        start, 
    size_t        end
);

/** !
 * Compute the hash of a binary tree node
 * 
 * @param p_node     pointer to binary tree node
 * @param pfn_hash64 pointer to hash function
 * 
 * @return the 64-bit hash on success, 0 on error
*/
hash64 binary_tree_node_hash ( binary_tree_node *p_node, fn_hash64 *pfn_hash64 );

/** !
 * Traverse a binary tree using the pre order technique
 * 
 * @param p_binary_tree_node pointer to binary tree node
 * @param pfn_traverse       called for each node in the binary tree
 * 
 * @return 1 on success, 0 on error
*/
int binary_tree_node_traverse_preorder ( binary_tree_node *p_binary_tree_node, fn_binary_tree_traverse *pfn_traverse );

/** !
 * Traverse a binary tree using the in order technique
 * 
 * @param p_binary_tree_node pointer to binary tree node
 * @param pfn_traverse       called for each node in the binary tree
 * 
 * @return 1 on success, 0 on error
*/
int binary_tree_node_traverse_inorder ( binary_tree_node *p_binary_tree_node, fn_binary_tree_traverse *pfn_traverse );

/** !
 * Traverse a binary tree using the post order technique
 * 
 * @param p_binary_tree_node pointer to binary tree node
 * @param pfn_traverse       called for each node in the binary tree
 * 
 * @return 1 on success, 0 on error
*/
int binary_tree_node_traverse_postorder ( binary_tree_node *p_binary_tree_node, fn_binary_tree_traverse *pfn_traverse );

/** ! 
 * Pack a binary tree node into a buffer
 * 
 * @param p_buffer     the buffer
 * @param p_node       the node
 * @param pfn_elemenet pointer to pack function IF not null ELSE default
 */
int binary_tree_node_pack ( void *p_buffer, binary_tree_node *p_node, fn_pack *pfn_element );

/** ! 
 * Unpack a binary tree node from a buffer
 * 
 * @param pp_node      result
 * @param p_buffer     the buffer
 * @param pfn_elemenet pointer to unpack function IF not null ELSE default
 */
int binary_tree_node_unpack ( binary_tree_node **pp_node, void *p_buffer, fn_unpack *pfn_element );

/** !
 * Destroy and deallocate a binary tree node
 * 
 * @param pp_binary_tree_node pointer to binary tree node pointer
 * @param pfn_allocator  pointer to allocator function for deallocating elements
 * 
 * @return 1 on success, 0 on error
 */
int binary_tree_node_destroy ( binary_tree_node **const pp_binary_tree_node, fn_allocator *pfn_allocator );

// function definitions
int binary_tree_construct ( binary_tree **const pp_binary_tree, unsigned long long node_size, fn_comparator *pfn_comparator, fn_key_accessor *pfn_key_accessor )
{

    // argument check
    if ( NULL == pp_binary_tree ) goto no_binary_tree;

    // initialized data
    binary_tree *p_binary_tree = NULL;

    // allocate memory for a binary tree
    p_binary_tree = default_allocator(0, sizeof(binary_tree));
    if ( NULL == p_binary_tree ) goto failed_to_allocate_binary_tree;

    // populate the binary tree structure
    *p_binary_tree = (binary_tree)
    {
        .p_root   = (void *) 0,
        .metadata =
        {
            .quantity = 0,
            .size     = node_size,
        },
        .pfn_comparator   = (pfn_comparator)   ? pfn_comparator   : default_comparator,
        .pfn_key_accessor = (pfn_key_accessor) ? pfn_key_accessor : default_key_accessor
    };

    // construct a lock
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
                    printf("[binary] Null pointer provided for parameter \"pp_binary_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // binary tree errors
        {
            failed_to_allocate_binary_tree:
                #ifndef NDEBUG
                    printf("[binary] Failed to allocate binary tree in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

// FIXME
binary_tree_node *binary_tree_construct_balanced_recursive ( binary_tree *p_binary_tree, void **pp_values, size_t start, size_t end )
{

    // NOTE: This function has undefined behavior if p_binary_tree, pp_keys, 
    //       and/or pp_values is null. Check your parameters before you call.

    // initialized data
    binary_tree_node *p_binary_tree_node = (void *) 0;

    // Base case
    if ( start == end )
    {

        // allocate a binary tree node
        if (binary_tree_allocate_node(p_binary_tree, &p_binary_tree_node) == 0) goto failed_to_allocate_node;

        // store the value
        p_binary_tree_node->p_value = pp_values[start];
        
        // done
        return p_binary_tree_node;
    }

    // One property
    else if ( end - start == 1 )
    {

        // allocate a binary tree node
        if (binary_tree_allocate_node(p_binary_tree, &p_binary_tree_node) == 0) goto failed_to_allocate_node;

        // store the value
        p_binary_tree_node->p_value = pp_values[end];

        // allocate the left node
        if (binary_tree_allocate_node(p_binary_tree, &p_binary_tree_node->p_left) == 0) goto failed_to_allocate_node;

        // store the left value
        p_binary_tree_node->p_left->p_value = pp_values[start];
        
        // done
        return p_binary_tree_node;
    }

    // Two properties
    else
    {

        // initialized data
        size_t median = (start + end) / 2;

        // allocate a binary tree node
        if (binary_tree_allocate_node(p_binary_tree, &p_binary_tree_node) == 0) goto failed_to_allocate_node;

        // store the value
        p_binary_tree_node->p_value = pp_values[median];

        // Construct the left
        p_binary_tree_node->p_left = binary_tree_construct_balanced_recursive(p_binary_tree, pp_values, start, median - 1);

        // Construct the right
        p_binary_tree_node->p_right = binary_tree_construct_balanced_recursive(p_binary_tree, pp_values, median + 1, end);

        // done
        return p_binary_tree_node;
    }

    // error
    return (void *) 0;

    // error handling
    {

        // binary tree errors
        {
            failed_to_allocate_node:
                #ifndef NDEBUG
                    printf("[binary] Failed to allocate binary tree node in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

// FIXME : parameter order
int binary_tree_construct_balanced
( 
    binary_tree **const pp_binary_tree, 
    void **pp_values, 
    size_t property_quantity, 
    fn_comparator *pfn_comparator,  
    fn_key_accessor *pfn_key_accessor, 
    unsigned long long node_size
)
{

    // argument check
    if ( NULL == pp_binary_tree ) goto no_binary_tree;

    // initialized data
    binary_tree *p_binary_tree = NULL;
    
    // allocate memory for a binary tree
    p_binary_tree = default_allocator(0, sizeof(binary_tree));
    if ( NULL == p_binary_tree ) goto failed_to_allocate_binary_tree;

    // populate the binary tree fields
    *p_binary_tree = (binary_tree)
    {
        .p_root   = (void *) 0,
        .metadata =
        {
            .quantity = 0,
            .size     = node_size + ( 2 * sizeof(unsigned long long) )
        },

        .pfn_comparator   = (pfn_comparator)   ? pfn_comparator   : default_comparator,
        .pfn_key_accessor = (pfn_key_accessor) ? pfn_key_accessor : default_key_accessor
    };

    // recursively construct a binary search tree, and store the root
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
                    printf("[binary] Null pointer provided for parameter \"pp_binary_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // binary tree errors
        {
            failed_to_allocate_binary_tree:
                #ifndef NDEBUG
                    printf("[binary] Failed to allocate binary tree in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int binary_tree_allocate_node ( binary_tree *p_binary_tree, binary_tree_node **pp_binary_tree_node )
{

    // argument check
    if ( NULL ==       p_binary_tree ) goto no_binary_tree;
    if ( NULL == pp_binary_tree_node ) goto no_binary_tree_node;
    
    // initialized data
    binary_tree_node *p_binary_tree_node = NULL;

    // allocate a node
    if ( 0 == binary_tree_node_create(&p_binary_tree_node) ) goto failed_to_allocate_node;

    // store the node pointer
    p_binary_tree_node->node_pointer = p_binary_tree->metadata.quantity;

    // increment the node quantity
    p_binary_tree->metadata.quantity++;

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
                    printf("[binary] Null pointer provided for parameter \"p_binary_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
                
            no_binary_tree_node:
                #ifndef NDEBUG
                    printf("[binary] Null pointer provided for parameter \"pp_binary_tree_node\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // binary errors
        {
            failed_to_allocate_node:
                #ifndef NDEBUG
                    printf("[tree] Call to function \"binary_tree_node_create\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int binary_tree_search ( binary_tree *p_binary_tree, const void *const p_key, void **pp_value )
{

    // argument check
    if ( NULL == p_binary_tree ) goto no_binary_tree;

    // state check
    if ( NULL == p_binary_tree->p_root ) return 0;

    // lock
    mutex_lock(&p_binary_tree->_lock);

    // initialized data
    binary_tree_node *p_node = p_binary_tree->p_root;
    int comparator_return = 0;

    try_again:

    // which side? 
    comparator_return = p_binary_tree->pfn_comparator
    (
        p_binary_tree->pfn_key_accessor( p_node->p_value ),
        p_key
    );

    // search the left node
    if ( comparator_return < 0 )
    {

        // if the left node exists ...
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

    // search the right node
    else if ( comparator_return > 0 )
    {

        // if the right node exists ...
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
                    printf("[binary] Null pointer provided for parameter \"p_binary_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int binary_tree_insert ( binary_tree *p_binary_tree, const void *const p_value )
{

    // argument check
    if ( NULL == p_binary_tree ) goto no_binary_tree;

    // lock
    mutex_lock(&p_binary_tree->_lock);

    // initialized data
    binary_tree_node *p_node = p_binary_tree->p_root;
    int comparator_return = 0;

    // state check
    if ( p_binary_tree->p_root == (void *) 0 ) goto no_root;

    try_again:

    // which side? 
    comparator_return = p_binary_tree->pfn_comparator
    (
        p_binary_tree->pfn_key_accessor(p_node->p_value),
        p_binary_tree->pfn_key_accessor(p_value)
    );

    // store the node on the left 
    if ( comparator_return < 0 )
    {

        // if the left node is occupied ...
        if ( p_node->p_left )
        {

            // ... update the state ...
            p_node = p_node->p_left;

            // ... and try again
            goto try_again;
        }

        // construct a binary tree node
        if ( binary_tree_allocate_node(p_binary_tree, &p_node->p_left) == 0 ) goto failed_to_allocate_binary_tree_node;

        // store the value
        p_node->p_left->p_value = (void *) p_value;
    }

    // store the node on the right
    else if ( comparator_return > 0 )
    {

        // if the left node is occupied ...
        if ( p_node->p_right )
        {

            // ... update the state ...
            p_node = p_node->p_right;

            // ... and try again
            goto try_again;
        }

        // construct a binary tree node
        if ( binary_tree_allocate_node(p_binary_tree, &p_node->p_right) == 0 ) goto failed_to_allocate_binary_tree_node;

        // store the value
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

        // construct a binary tree node
        if ( binary_tree_allocate_node(p_binary_tree, &p_node) == 0 ) goto failed_to_allocate_binary_tree_node;

        // store the value
        p_node->p_value = (void *) p_value;

        // store the node as the root of the tree
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
                    printf("[binary] Null pointer provided for parameter \"p_binary_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // binary tree errors
        {
            failed_to_allocate_binary_tree_node:
                #ifndef NDEBUG
                    printf("[binary] Failed to allocate binary tree node in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // unlock
                mutex_unlock(&p_binary_tree->_lock);
                
                // error
                return 0;
        }
    }
}

// FIXME 
int binary_tree_remove ( binary_tree *const p_binary_tree, const void *const p_key, const void **const pp_value )
{

    // argument check
    if ( NULL == p_binary_tree ) goto no_binary_tree;

    // lock
    mutex_lock(&p_binary_tree->_lock);

    // FIXME : should be a label
    // state check
    if ( NULL == p_binary_tree->p_root ) 
    {
        mutex_unlock(&p_binary_tree->_lock);
        return 0;
    }

    // initialized data
    binary_tree_node *p_node  = p_binary_tree->p_root, *p_parent = NULL;
    int comparator_return = 0;

    // find the node to be removed
    while ( p_node != NULL ) 
    {
        comparator_return = p_binary_tree->pfn_comparator(
            p_binary_tree->pfn_key_accessor(p_node->p_value),
            p_key
        );

        if (comparator_return == 0) break;

        p_parent = p_node;
        p_node = (comparator_return < 0) ? p_node->p_left :  p_node->p_right;
    }

    // node not found
    if (p_node == NULL) 
    {
        mutex_unlock(&p_binary_tree->_lock);
        return 0;
    }
    
    if ( pp_value )
        *pp_value = p_node->p_value;

    // Case 1: Node has no children (it's a leaf)
    if (p_node->p_left == NULL && p_node->p_right == NULL) 
    {
        if (p_parent == NULL) 
        {
            p_binary_tree->p_root = NULL;
        } 
        else if (p_parent->p_left == p_node) 
        {
            p_parent->p_left = NULL;
        } 
        else 
        {
            p_parent->p_right = NULL;
        }
        binary_tree_node_destroy(&p_node, NULL);
    }

    // Case 2: Node has one child
    else if (p_node->p_left == NULL || p_node->p_right == NULL) 
    {
        binary_tree_node *p_child = (p_node->p_left) ? p_node->p_left : p_node->p_right;

        if (p_parent == NULL) 
        {
            p_binary_tree->p_root = p_child;
        } 
        else if (p_parent->p_left == p_node) 
        {
            p_parent->p_left = p_child;
        } 
        else 
        {
            p_parent->p_right = p_child;
        }
        
        // Nullify child pointers to prevent recursive destruction of moved child
        p_node->p_left = NULL;
        p_node->p_right = NULL;
        binary_tree_node_destroy(&p_node, NULL);
    }
    // Case 3: Node has two children
    else 
    {
        binary_tree_node *p_successor_parent = p_node;
        binary_tree_node *p_successor = p_node->p_right;
        while (p_successor->p_left != NULL) 
        {
            p_successor_parent = p_successor;
            p_successor = p_successor->p_left;
        }

        p_node->p_value = p_successor->p_value;

        if (p_successor_parent->p_left == p_successor) 
        {
            p_successor_parent->p_left = p_successor->p_right;
        } 
        else 
        {
            p_successor_parent->p_right = p_successor->p_right;
        }
        
        // Nullify child pointers to prevent recursive destruction of moved child
        p_successor->p_left = NULL;
        p_successor->p_right = NULL;
        binary_tree_node_destroy(&p_successor, NULL);
    }

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
                    printf("[binary] Null pointer provided for parameter \"p_binary_tree\" in call to function \"%s\"\n", __FUNCTION__);
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

    // edge case
    if ( 0 == p_binary_tree->metadata.quantity ) return 1;

    // lock
    mutex_lock(&p_binary_tree->_lock);

    // traverse the tree
    if ( binary_tree_node_traverse_preorder(p_binary_tree->p_root, pfn_traverse) == 0 ) goto failed_to_traverse_binary_tree;    

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
                    log_error("[binary] Null pointer provided for parameter \"p_binary_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_traverse_function:
                #ifndef NDEBUG
                    log_error("[binary] Null pointer provided for parameter \"pfn_traverse\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // binary tree errors
        {
            failed_to_traverse_binary_tree:
                #ifndef NDEBUG
                    log_error("[binary] Failed to traverse binary tree in call to function \"%s\"\n", __FUNCTION__);
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
    if ( NULL == p_binary_tree ) goto no_binary_tree;
    if ( NULL ==  pfn_traverse ) goto no_traverse_function;

    // edge case
    if ( 0 == p_binary_tree->metadata.quantity ) return 1;

    // lock
    mutex_lock(&p_binary_tree->_lock);

    // traverse the tree
    if ( binary_tree_node_traverse_inorder(p_binary_tree->p_root, pfn_traverse) == 0 ) goto failed_to_traverse_binary_tree;    

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
                    log_error("[binary] Null pointer provided for parameter \"p_binary_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_traverse_function:
                #ifndef NDEBUG
                    log_error("[binary] Null pointer provided for parameter \"pfn_traverse\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // binary tree errors
        {
            failed_to_traverse_binary_tree:
                #ifndef NDEBUG
                    log_error("[binary] Failed to traverse binary tree in call to function \"%s\"\n", __FUNCTION__);
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
    if ( NULL == p_binary_tree ) goto no_binary_tree;
    if ( NULL ==  pfn_traverse ) goto no_traverse_function;

    // edge case
    if ( 0 == p_binary_tree->metadata.quantity ) return 1;
    
    // lock
    mutex_lock(&p_binary_tree->_lock);

    // traverse the tree
    if ( binary_tree_node_traverse_postorder(p_binary_tree->p_root, pfn_traverse) == 0 ) goto failed_to_traverse_binary_tree;    

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
                    log_error("[binary] Null pointer provided for parameter \"p_binary_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_traverse_function:
                #ifndef NDEBUG
                    log_error("[binary] Null pointer provided for parameter \"pfn_traverse\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // binary tree errors
        {
            failed_to_traverse_binary_tree:
                #ifndef NDEBUG
                    log_error("[binary] Failed to traverse binary tree in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // unlock
                mutex_unlock(&p_binary_tree->_lock);
                
                // error
                return 0;
        }
    }
}

int binary_tree_pack ( void *p_buffer, binary_tree *p_binary_tree, fn_pack *pfn_element )
{
    
    // argument check
    if ( NULL == p_binary_tree ) goto no_binary_tree;
    if ( NULL ==   pfn_element ) return 0;

    // initialized data 
    char *p = p_buffer;

    // lock
    mutex_lock(&p_binary_tree->_lock);

    // pack the metadata
    p += pack_pack(p, "%2i64", 
        p_binary_tree->metadata.quantity,
        p_binary_tree->metadata.size
    );

    // pack the tree
    p += binary_tree_node_pack(p, p_binary_tree->p_root, pfn_element);

    // unlock
    mutex_unlock(&p_binary_tree->_lock);

    // success
    return p - (char *)p_buffer;

    // error handling
    {
        
        // argument errors
        {
            no_binary_tree:
                #ifndef NDEBUG
                    log_error("[binary] Null pointer provided for \"p_binary_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int binary_tree_unpack
( 
    binary_tree **pp_binary_tree, 
    void *p_buffer, 
    
    fn_unpack       *pfn_element, 
    fn_comparator   *pfn_comparator, 
    fn_key_accessor *pfn_key_accessor
)
{
    
    // argument check
    if ( NULL == pp_binary_tree ) goto no_binary_tree;
    if ( NULL ==    pfn_element ) return 0;

    // initialized data 
    binary_tree *p_binary_tree = NULL;
    char        *p             = p_buffer;
    size_t       quantity      = 0, 
                 size          = 0;

    // unpack the metadata
    p += pack_unpack(p, "%2i64", 
        &quantity,
        &size
    );

    // construct a binary tree
    binary_tree_construct(&p_binary_tree, size, pfn_comparator, pfn_key_accessor);

    // recursively unpack from the root
    p += binary_tree_node_unpack(&p_binary_tree->p_root, p, pfn_element);

    // store the quantity of nodes
    p_binary_tree->metadata.quantity = quantity;

    // return a pointer to the caller
    *pp_binary_tree = p_binary_tree;
    
    // success
    return p - (char *)p_buffer;

    // error handling
    {
        
        // argument errors
        {
            no_binary_tree:
                #ifndef NDEBUG
                    log_error("[binary] Null pointer provided for parameter \"pp_binary_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

hash64 binary_tree_hash ( binary_tree *const p_binary_tree, fn_hash64 *pfn_hash64 )
{

    // argument check
    if ( NULL == p_binary_tree ) goto no_binary_tree;

    // initialize data
    hash64 result = 0;
    fn_hash64 *pfn_hash = (pfn_hash64) ? pfn_hash64 : default_hash;

    // lock
    mutex_lock(&p_binary_tree->_lock);

    // compute the hash
    result = ( p_binary_tree->p_root )                              ? 
             binary_tree_node_hash(p_binary_tree->p_root, pfn_hash) : 
             default_hash(&eight_bytes_of_f, sizeof(eight_bytes_of_f));

    // unlock
    mutex_unlock(&p_binary_tree->_lock);

    // success
    return result;

    // error handling
    {

        // argument errors
        {
            no_binary_tree:
                #ifndef NDEBUG
                    printf("[binary] Null pointer provided for parameter \"p_binary_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int binary_tree_destroy ( binary_tree **const pp_binary_tree, fn_allocator *pfn_allocator )
{

    // argument check
    if ( pp_binary_tree == (void *) 0 ) goto no_binary_tree;

    // initialized data
    binary_tree *p_binary_tree = *pp_binary_tree;

    // fast exit
    if ( p_binary_tree == (void *) 0 ) return 1;

    // lock
    mutex_lock(&p_binary_tree->_lock);

    // no more pointer for caller
    *pp_binary_tree = (void *) 0;

    // unlock
    mutex_unlock(&p_binary_tree->_lock);

    // recursively free nodes
    if ( binary_tree_node_destroy(&p_binary_tree->p_root, pfn_allocator) == 0 ) goto failed_to_free_nodes;

    // destroy the lock
    mutex_destroy(&p_binary_tree->_lock);

    // release the tree
    p_binary_tree = default_allocator(p_binary_tree, 0);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_binary_tree:
                #ifndef NDEBUG
                    printf("[binary] Null pointer provided for parameter \"pp_binary_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Tree error
        {
            failed_to_free_nodes:
                #ifndef NDEBUG
                    printf("[binary] Failed to free binary tree nodes in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int binary_tree_node_create ( binary_tree_node **pp_binary_tree_node )
{

    // argument check
    if ( NULL == pp_binary_tree_node ) goto no_binary_tree_node;

    // initialized data
    binary_tree_node *p_binary_tree_node = NULL;
    
    // allocate memory for a binary tree node
    p_binary_tree_node = default_allocator(0, sizeof(binary_tree_node));
    if ( NULL == p_binary_tree_node ) goto no_mem;

    // zero set the memory
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
                    printf("[binary] Null pointer provided for parameter \"pp_binary_tree_node\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library
        {
            no_mem:
                #ifndef NDEBUG
                    printf("[standard library] Call to function \"default_allocator\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int binary_tree_node_pack ( void *p_buffer, binary_tree_node *p_node, fn_pack *pfn_element )
{

    // initialized data 
    char *p = p_buffer;

    // pack the parent value
    p += pfn_element(p, p_node->p_value),

    // pack the node pointer
    p += pack_pack(p, "%2i64", 
        p_node->p_left  ? p_node->p_left->node_pointer  : eight_bytes_of_f,
        p_node->p_right ? p_node->p_right->node_pointer : eight_bytes_of_f
    );

    // left
    if ( p_node->p_left ) p += binary_tree_node_pack(p, p_node->p_left, pfn_element);

    // right
    if ( p_node->p_right ) p += binary_tree_node_pack(p, p_node->p_right, pfn_element);

    // success
    return p - (char *)p_buffer;
}

int binary_tree_node_unpack ( binary_tree_node **pp_node, void *p_buffer, fn_unpack *pfn_element )
{

    // initialized data 
    binary_tree_node *p_node   = NULL;
    char             *p        = p_buffer;
    size_t            quantity = 0, 
                      size     = 0;
    unsigned long long l = 0, r = 0;

    // construct a binary tree node
    binary_tree_node_create(&p_node);
    
    // unpack the parent value
    p += pfn_element(&p_node->p_value, p);

    // unpack the node pointer
    p += pack_unpack(p, "%2i64", &l, &r);

    // left
    if ( l != eight_bytes_of_f ) 
        p += binary_tree_node_unpack(&p_node->p_left, p, pfn_element),
        p_node->p_left->node_pointer = l;

    // right
    if ( r != eight_bytes_of_f ) 
        p += binary_tree_node_unpack(&p_node->p_right, p, pfn_element),
        p_node->p_right->node_pointer = r;

    // return a pointer to the caller
    *pp_node = p_node;
    
    // success
    return p - (char *)p_buffer;
}

hash64 binary_tree_node_hash ( binary_tree_node *p_node, fn_hash64 *pfn_hash64 )
{

    // initialized data
    hash64 result = 0;

    // left
    if ( p_node->p_left ) result ^= binary_tree_node_hash(p_node->p_left, pfn_hash64);

    // parent
    result ^= pfn_hash64(p_node->p_value, sizeof(void *));

    // right
    if ( p_node->p_right ) result ^= binary_tree_node_hash(p_node->p_right, pfn_hash64);

    // success
    return result;
}

int binary_tree_node_traverse_preorder ( binary_tree_node *p_binary_tree_node, fn_binary_tree_traverse *pfn_traverse )
{

    // argument check
    if ( NULL == p_binary_tree_node ) goto no_binary_tree_node;
    if ( NULL ==       pfn_traverse ) goto no_traverse_function;

    // root
    pfn_traverse(p_binary_tree_node->p_value);

    // left
    if ( p_binary_tree_node->p_left ) binary_tree_node_traverse_preorder(p_binary_tree_node->p_left, pfn_traverse);

    // right
    if ( p_binary_tree_node->p_right ) binary_tree_node_traverse_preorder(p_binary_tree_node->p_right, pfn_traverse);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_binary_tree_node:
                #ifndef NDEBUG
                    log_error("[binary] Null pointer provided for parameter \"p_binary_tree_node\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_traverse_function:
                #ifndef NDEBUG
                    log_error("[binary] Null pointer provided for parameter \"pfn_traverse\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int binary_tree_node_traverse_inorder ( binary_tree_node *p_binary_tree_node, fn_binary_tree_traverse *pfn_traverse )
{

    // argument check
    if ( NULL == p_binary_tree_node ) goto no_binary_tree_node;
    if ( NULL ==       pfn_traverse ) goto no_traverse_function;

    // left
    if ( p_binary_tree_node->p_left ) binary_tree_node_traverse_inorder(p_binary_tree_node->p_left, pfn_traverse);

    // root
    pfn_traverse(p_binary_tree_node->p_value);

    // right
    if ( p_binary_tree_node->p_right ) binary_tree_node_traverse_inorder(p_binary_tree_node->p_right, pfn_traverse);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_binary_tree_node:
                #ifndef NDEBUG
                    log_error("[binary] Null pointer provided for parameter \"p_binary_tree_node\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_traverse_function:
                #ifndef NDEBUG
                    log_error("[binary] Null pointer provided for parameter \"pfn_traverse\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int binary_tree_node_traverse_postorder ( binary_tree_node *p_binary_tree_node, fn_binary_tree_traverse *pfn_traverse )
{

    // argument check
    if ( NULL == p_binary_tree_node ) goto no_binary_tree_node;
    if ( NULL ==       pfn_traverse ) goto no_traverse_function;

    // left
    if ( p_binary_tree_node->p_left ) binary_tree_node_traverse_postorder(p_binary_tree_node->p_left, pfn_traverse);

    // right
    if ( p_binary_tree_node->p_right ) binary_tree_node_traverse_postorder(p_binary_tree_node->p_right, pfn_traverse);

    // root
    pfn_traverse(p_binary_tree_node->p_value);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_binary_tree_node:
                #ifndef NDEBUG
                    log_error("[binary] Null pointer provided for parameter \"p_binary_tree_node\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_traverse_function:
                #ifndef NDEBUG
                    log_error("[binary] Null pointer provided for parameter \"pfn_traverse\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int binary_tree_node_destroy ( binary_tree_node **const pp_binary_tree_node, fn_allocator *pfn_allocator )
{

    // argument check
    if ( pp_binary_tree_node == (void *) 0 ) goto no_binary_tree_node;

    // initialized data
    binary_tree_node *p_binary_tree_node = *pp_binary_tree_node;

    // fast exit
    if ( p_binary_tree_node == (void *) 0 ) return 1;

    // free the value
    if ( pfn_allocator ) 
        p_binary_tree_node->p_value = pfn_allocator(p_binary_tree_node->p_value, 0);

    // recursively free the left node
    if ( binary_tree_node_destroy(&p_binary_tree_node->p_left, pfn_allocator) == 0 ) goto failed_to_free;

    // recursively free the right node
    if ( binary_tree_node_destroy(&p_binary_tree_node->p_right, pfn_allocator) == 0 ) goto failed_to_free;

    // free the node
    *pp_binary_tree_node = default_allocator(p_binary_tree_node, 0);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_binary_tree_node:
                #ifndef NDEBUG
                    printf("[binary] Null pointer provided for parameter \"pp_binary_tree_node\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // binary tree errors
        {
            failed_to_free:
                #ifndef NDEBUG
                    printf("[binary] Failed to free binary tree in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0; 
        }
    }
}
