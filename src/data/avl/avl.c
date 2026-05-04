/** !
 * Adelson-Velsky and Landis tree implementation
 * 
 * @file src/data/avl/avl.c
 * 
 * @author Jacob Smith
 */

// header file
#include <data/avl.h>

// static data
static const unsigned long long eight_bytes_of_f = 0xffffffffffffffff;

// forward declarations
/** !
 * Allocate memory for an avl tree node
 * 
 * @param pp_avl_tree_node return
 * 
 * @return 1 on success, 0 on error
 */
int avl_tree_node_create ( avl_tree_node **pp_avl_tree_node );

/** !
 * Allocate a node for a specific avl tree, and set the node pointer. 
 * 
 * @param p_avl_tree       the avl tree to allocate a node to
 * @param pp_avl_tree_node result
 * 
 * @return 1 on success, 0 on error
 */
int avl_tree_allocate_node ( avl_tree *p_avl_tree, avl_tree_node **pp_avl_tree_node );

/** !
 * Compute the hash of an avl tree node
 * 
 * @param p_node     pointer to avl tree node
 * @param pfn_hash64 pointer to hash function
 * 
 * @return the 64-bit hash on success, 0 on error
*/
hash64 avl_tree_node_hash ( avl_tree_node *p_node, fn_hash64 *pfn_hash64 );

/** !
 * Traverse an avl tree using the pre order technique
 * 
 * @param p_avl_tree_node pointer to avl tree node
 * @param pfn_foreach     called for each node in the avl tree
 * 
 * @return 1 on success, 0 on error
*/
int avl_tree_node_traverse_preorder ( avl_tree_node *p_avl_tree_node, fn_foreach *pfn_foreach );

/** !
 * Traverse an avl tree using the in order technique
 * 
 * @param p_avl_tree_node pointer to avl tree node
 * @param pfn_foreach     called for each node in the avl tree
 * 
 * @return 1 on success, 0 on error
*/
int avl_tree_node_traverse_inorder ( avl_tree_node *p_avl_tree_node, fn_foreach *pfn_foreach );

/** !
 * Traverse an avl tree using the post order technique
 * 
 * @param p_avl_tree_node pointer to avl tree node
 * @param pfn_foreach     called for each node in the avl tree
 * 
 * @return 1 on success, 0 on error
*/
int avl_tree_node_traverse_postorder ( avl_tree_node *p_avl_tree_node, fn_foreach *pfn_foreach );

/** !
 * Call function on a node in an avl tree
 *
 * @param p_avl_tree_node the node
 * @param pfn_forcontext   pointer to forcontext function
 * @param p_context        the context
 * 
 * @return 1 on success, 0 on error
 */
int avl_tree_node_forcontext ( avl_tree_node *p_avl_tree_node, fn_forcontext *pfn_forcontext, void *p_context );

/** ! 
 * Pack an avl tree node into a buffer
 * 
 * @param p_buffer     the buffer
 * @param p_node       the node
 * @param pfn_elemenet pointer to pack function IF not null ELSE default
 * 
 * @return bytes written on success, 0 on error
 */
int avl_tree_node_pack ( void *p_buffer, avl_tree_node *p_node, fn_pack *pfn_element );

/** ! 
 * Unpack an avl tree node from a buffer
 * 
 * @param pp_node      result
 * @param p_buffer     the buffer
 * @param pfn_elemenet pointer to unpack function IF not null ELSE default
 * 
 * @return bytes read on success, 0 on error
 */
int avl_tree_node_unpack ( avl_tree_node **pp_node, void *p_buffer, fn_unpack *pfn_element );

/** !
 * Destroy and deallocate an avl tree node
 * 
 * @param pp_avl_tree_node pointer to avl tree node pointer
 * @param pfn_allocator    pointer to allocator function for deallocating elements
 * 
 * @return 1 on success, 0 on error
 */
int avl_tree_node_destroy ( avl_tree_node **const pp_avl_tree_node, fn_allocator *pfn_allocator );

/** !
 * Get the height of an avl tree node
 * 
 * @param p_node the node
 * 
 * @return the height
 */
int avl_tree_node_height ( avl_tree_node *p_node );

/** !
 * Update the height of an avl tree node
 * 
 * @param p_node the node
 * 
 * @return 1 on success, 0 on error
 */
int avl_tree_node_update_height ( avl_tree_node *p_node );

/** !
 * Get the balance factor of an avl tree node
 * 
 * @param p_node the node
 * 
 * @return the balance factor
 */
int avl_tree_node_balance_factor ( avl_tree_node *p_node );

/** !
 * Perform a right rotation on an avl tree node
 * 
 * @param pp_node result
 * 
 * @return 1 on success, 0 on error
 */
int avl_tree_node_right_rotate ( avl_tree_node **pp_node );

/** !
 * Perform a left rotation on an avl tree node
 * 
 * @param pp_node result
 * 
 * @return 1 on success, 0 on error
 */
int avl_tree_node_left_rotate ( avl_tree_node **pp_node );

/** !
 * Rebalance an avl tree node
 * 
 * @param pp_node result
 * 
 * @return 1 on success, 0 on error
 */
int avl_tree_node_rebalance ( avl_tree_node **pp_node );

/** !
 * Recursively insert a value into an avl tree
 * 
 * @param p_avl_tree the avl tree
 * @param pp_node    the current node
 * @param p_value    the value
 * 
 * @return 1 on success, 0 on error
 */
int avl_tree_insert_recursive ( avl_tree *p_avl_tree, avl_tree_node **pp_node, const void *const p_value );

/** !
 * Recursively remove a key from an avl tree
 * 
 * @param p_avl_tree the avl tree
 * @param pp_node    the current node
 * @param p_key      the key
 * @param pp_value   return
 * 
 * @return 1 on success, 0 on error
 */
int avl_tree_remove_recursive ( avl_tree *p_avl_tree, avl_tree_node **pp_node, const void *const p_key, const void **const pp_value );

// function definitions
int avl_tree_construct ( avl_tree **const pp_avl_tree, unsigned long long node_size, fn_comparator *pfn_comparator, fn_key_accessor *pfn_key_accessor )
{

    // argument check
    if ( NULL == pp_avl_tree ) goto no_avl_tree;

    // initialized data
    avl_tree *p_avl_tree = NULL;

    // allocate memory for an avl tree
    p_avl_tree = default_allocator(0, sizeof(avl_tree));
    if ( NULL == p_avl_tree ) goto failed_to_allocate_avl_tree;

    // populate the avl tree structure
    *p_avl_tree = (avl_tree)
    {
        .p_root   = NULL,
        .metadata =
        {
            .quantity = 0,
            .size     = node_size,
        },
        .pfn_comparator   = (pfn_comparator)   ? pfn_comparator   : default_comparator,
        .pfn_key_accessor = (pfn_key_accessor) ? pfn_key_accessor : default_key_accessor
    };

    // construct a lock
    mutex_create(&p_avl_tree->_lock);

    // return a pointer to the caller
    *pp_avl_tree = p_avl_tree;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_avl_tree:
                #ifndef NDEBUG
                    printf("[avl] Null pointer provided for parameter \"pp_avl_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // avl tree errors
        {
            failed_to_allocate_avl_tree:
                #ifndef NDEBUG
                    printf("[avl] Failed to allocate avl tree in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int avl_tree_allocate_node ( avl_tree *p_avl_tree, avl_tree_node **pp_avl_tree_node )
{

    // argument check
    if ( NULL ==       p_avl_tree ) goto no_avl_tree;
    if ( NULL == pp_avl_tree_node ) goto no_avl_tree_node;
    
    // initialized data
    avl_tree_node *p_avl_tree_node = NULL;

    // allocate a node
    if ( 0 == avl_tree_node_create(&p_avl_tree_node) ) goto failed_to_allocate_node;

    // store the node pointer
    p_avl_tree_node->node_pointer = p_avl_tree->metadata.quantity;

    // increment the node quantity
    p_avl_tree->metadata.quantity++;

    // return a pointer to the caller
    *pp_avl_tree_node = p_avl_tree_node;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_avl_tree:
                #ifndef NDEBUG
                    printf("[avl] Null pointer provided for parameter \"p_avl_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
                
            no_avl_tree_node:
                #ifndef NDEBUG
                    printf("[avl] Null pointer provided for parameter \"pp_avl_tree_node\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // avl errors
        {
            failed_to_allocate_node:
                #ifndef NDEBUG
                    printf("[tree] Call to function \"avl_tree_node_create\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int avl_tree_search ( avl_tree *p_avl_tree, const void *const p_key, void **pp_value )
{

    // argument check
    if ( NULL == p_avl_tree ) goto no_avl_tree;

    // state check
    if ( NULL == p_avl_tree->p_root ) return 0;

    // lock
    mutex_lock(&p_avl_tree->_lock);

    // initialized data
    avl_tree_node *p_node = p_avl_tree->p_root;
    int comparator_return = 0;

    try_again:

    // which side? 
    comparator_return = p_avl_tree->pfn_comparator
    (
        p_avl_tree->pfn_key_accessor( p_node->p_value ),
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
        mutex_unlock(&p_avl_tree->_lock);
        
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
        mutex_unlock(&p_avl_tree->_lock);
    
        // error
        return 0;
    }

    // return a pointer to the caller
    *pp_value = p_node->p_value;

    // unlock
    mutex_unlock(&p_avl_tree->_lock);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_avl_tree:
                #ifndef NDEBUG
                    printf("[avl] Null pointer provided for parameter \"p_avl_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int avl_tree_insert_recursive ( avl_tree *p_avl_tree, avl_tree_node **pp_node, const void *const p_value )
{

    // base case
    if ( NULL == *pp_node )
    {

        // initialized data
        avl_tree_node *p_node = NULL;

        // allocate a node
        if ( 0 == avl_tree_allocate_node(p_avl_tree, &p_node) ) return 0;

        // store the value
        p_node->p_value = (void *) p_value;

        // return a pointer to the caller
        *pp_node = p_node;

        // success
        return 1;
    }

    // initialized data
    avl_tree_node *p_node = *pp_node;
    int comparator_return = p_avl_tree->pfn_comparator(
        p_avl_tree->pfn_key_accessor(p_node->p_value),
        p_avl_tree->pfn_key_accessor(p_value)
    );

    // left?
    if ( comparator_return < 0 )
    {
        if ( 0 == avl_tree_insert_recursive(p_avl_tree, &p_node->p_left, p_value) ) return 0;
    }
    
    // right?
    else if ( comparator_return > 0 )
    {
        if ( 0 == avl_tree_insert_recursive(p_avl_tree, &p_node->p_right, p_value) ) return 0;
    }

    // exists
    else return 1;

    // rebalance
    avl_tree_node_rebalance(pp_node);

    // success
    return 1;
}

int avl_tree_insert ( avl_tree *p_avl_tree, const void *const p_value )
{

    // argument check
    if ( NULL == p_avl_tree ) goto no_avl_tree;

    // lock
    mutex_lock(&p_avl_tree->_lock);

    // recursively insert
    if ( 0 == avl_tree_insert_recursive(p_avl_tree, &p_avl_tree->p_root, p_value) ) goto failed_to_insert;

    // unlock
    mutex_unlock(&p_avl_tree->_lock);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_avl_tree:
                #ifndef NDEBUG
                    printf("[avl] Null pointer provided for parameter \"p_avl_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // avl tree errors
        {
            failed_to_insert:
                #ifndef NDEBUG
                    printf("[avl] Failed to insert value into avl tree in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // unlock
                mutex_unlock(&p_avl_tree->_lock);
                
                // error
                return 0;
        }
    }
}

int avl_tree_remove_recursive ( avl_tree *p_avl_tree, avl_tree_node **pp_node, const void *const p_key, const void **const pp_value )
{

    // base case: node not found
    if ( NULL == *pp_node ) return 0;

    // initialized data
    avl_tree_node *p_node = *pp_node;
    int comparator_return = p_avl_tree->pfn_comparator(
        p_avl_tree->pfn_key_accessor(p_node->p_value),
        p_key
    );

    // left?
    if ( comparator_return < 0 )
    {
        if ( 0 == avl_tree_remove_recursive(p_avl_tree, &p_node->p_left, p_key, pp_value) ) 
            return 0;
    }

    // right?
    else if ( comparator_return > 0 )
    {
        if ( 0 == avl_tree_remove_recursive(p_avl_tree, &p_node->p_right, p_key, pp_value) ) 
            return 0;
    }

    // found the node!
    else
    {

        // return the value
        if ( pp_value ) *pp_value = p_node->p_value;

        // leaf?
        if ( NULL == p_node->p_left && NULL == p_node->p_right )
        {

            // release the node
            avl_tree_node_destroy(&p_node, NULL);

            // no more pointer for caller
            *pp_node = NULL;

            // done
            return 1;
        }

        // one child
        else if 
        ( 
            NULL == p_node->p_left  ||
            NULL == p_node->p_right 
        )
        {

            // initialized data
            avl_tree_node *p_child = (p_node->p_left) ? p_node->p_left : p_node->p_right;

            // no more child pointers
            p_node->p_left  = NULL,
            p_node->p_right = NULL;

            // release the node
            avl_tree_node_destroy(&p_node, NULL);

            // update the state
            *pp_node = p_child;

            // done
            return 1;
        }

        // two children
        else
        {
            // find the successor
            avl_tree_node *p_successor       = p_node->p_right;
            void          *p_successor_value = NULL;

            // find the successor
            while ( p_successor->p_left ) p_successor = p_successor->p_left;

            // copy the successor value
            p_successor_value = p_successor->p_value;

            // remove the successor
            avl_tree_remove_recursive(p_avl_tree, &p_node->p_right, p_avl_tree->pfn_key_accessor(p_successor_value), NULL);

            // update the current node value
            p_node->p_value = p_successor_value;
        }
    }

    // rebalance
    avl_tree_node_rebalance(pp_node);

    // success
    return 1;
}

int avl_tree_remove ( avl_tree *const p_avl_tree, const void *const p_key, const void **const pp_value )
{

    // argument check
    if ( NULL == p_avl_tree ) goto no_avl_tree;

    // lock
    mutex_lock(&p_avl_tree->_lock);

    // recursively remove
    if ( 0 == avl_tree_remove_recursive(p_avl_tree, &p_avl_tree->p_root, p_key, pp_value) ) goto failed_to_remove;

    // unlock
    mutex_unlock(&p_avl_tree->_lock);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_avl_tree:
                #ifndef NDEBUG
                    printf("[avl] Null pointer provided for parameter \"p_avl_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // avl tree errors
        {
            failed_to_remove:
                #ifndef NDEBUG
                    printf("[avl] Failed to remove value from avl tree in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // unlock
                mutex_unlock(&p_avl_tree->_lock);
                
                // error
                return 0;
        }
    }
}

int avl_tree_traverse_preorder ( avl_tree *const p_avl_tree, fn_foreach *pfn_foreach )
{

    // argument check
    if ( NULL ==  p_avl_tree ) goto no_avl_tree;
    if ( NULL == pfn_foreach ) goto no_foreach_function;

    // state check
    if ( 0 == p_avl_tree->metadata.quantity ) return 1;

    // lock
    mutex_lock(&p_avl_tree->_lock);

    // traverse the tree
    if ( 0 == avl_tree_node_traverse_preorder(p_avl_tree->p_root, pfn_foreach) ) goto failed_to_traverse_avl_tree;    

    // unlock
    mutex_unlock(&p_avl_tree->_lock);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_avl_tree:
                #ifndef NDEBUG
                    log_error("[avl] Null pointer provided for parameter \"p_avl_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_foreach_function:
                #ifndef NDEBUG
                    log_error("[avl] Null pointer provided for parameter \"pfn_foreach\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // avl tree errors
        {
            failed_to_traverse_avl_tree:
                #ifndef NDEBUG
                    log_error("[avl] Failed to traverse avl tree in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // unlock
                mutex_unlock(&p_avl_tree->_lock);
                
                // error
                return 0;
        }
    }
}

int avl_tree_traverse_inorder ( avl_tree *const p_avl_tree, fn_foreach *pfn_foreach )
{

    // argument check
    if ( NULL ==  p_avl_tree ) goto no_avl_tree;
    if ( NULL == pfn_foreach ) goto no_foreach_function;

    // state check
    if ( 0 == p_avl_tree->metadata.quantity ) return 1;

    // lock
    mutex_lock(&p_avl_tree->_lock);

    // traverse the tree
    if ( 0 == avl_tree_node_traverse_inorder(p_avl_tree->p_root, pfn_foreach) ) goto failed_to_traverse_avl_tree;    

    // unlock
    mutex_unlock(&p_avl_tree->_lock);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_avl_tree:
                #ifndef NDEBUG
                    log_error("[avl] Null pointer provided for parameter \"p_avl_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_foreach_function:
                #ifndef NDEBUG
                    log_error("[avl] Null pointer provided for parameter \"pfn_foreach\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // avl tree errors
        {
            failed_to_traverse_avl_tree:
                #ifndef NDEBUG
                    log_error("[avl] Failed to traverse avl tree in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // unlock
                mutex_unlock(&p_avl_tree->_lock);
                
                // error
                return 0;
        }
    }
}

int avl_tree_traverse_postorder ( avl_tree *const p_avl_tree, fn_foreach *pfn_foreach )
{

    // argument check
    if ( NULL ==  p_avl_tree ) goto no_avl_tree;
    if ( NULL == pfn_foreach ) goto no_foreach_function;

    // state check
    if ( 0 == p_avl_tree->metadata.quantity ) return 1;
    
    // lock
    mutex_lock(&p_avl_tree->_lock);

    // traverse the tree
    if ( 0 == avl_tree_node_traverse_postorder(p_avl_tree->p_root, pfn_foreach) ) goto failed_to_traverse_avl_tree;    

    // unlock
    mutex_unlock(&p_avl_tree->_lock);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_avl_tree:
                #ifndef NDEBUG
                    log_error("[avl] Null pointer provided for parameter \"p_avl_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_foreach_function:
                #ifndef NDEBUG
                    log_error("[avl] Null pointer provided for parameter \"pfn_foreach\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // avl tree errors
        {
            failed_to_traverse_avl_tree:
                #ifndef NDEBUG
                    log_error("[avl] Failed to traverse avl tree in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // unlock
                mutex_unlock(&p_avl_tree->_lock);
                
                // error
                return 0;
        }
    }
}

int avl_tree_forcontext ( avl_tree *const p_avl_tree, fn_forcontext *pfn_forcontext, void *p_context )
{

    // argument check
    if ( NULL ==  p_avl_tree ) goto no_avl_tree;
    if ( NULL == pfn_forcontext ) goto no_forcontext_function;

    // edge case
    if ( 0 == p_avl_tree->metadata.quantity ) return 1;

    // lock
    mutex_lock(&p_avl_tree->_lock);

    // traverse the tree
    if ( 0 == avl_tree_node_forcontext(p_avl_tree->p_root, pfn_forcontext, p_context) ) goto failed_to_traverse_avl_tree;    

    // unlock
    mutex_unlock(&p_avl_tree->_lock);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_avl_tree:
                #ifndef NDEBUG
                    log_error("[avl] Null pointer provided for parameter \"p_avl_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_forcontext_function:
                #ifndef NDEBUG
                    log_error("[avl] Null pointer provided for parameter \"pfn_forcontext\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // avl tree errors
        {
            failed_to_traverse_avl_tree:
                #ifndef NDEBUG
                    log_error("[avl] Failed to traverse avl tree in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // unlock
                mutex_unlock(&p_avl_tree->_lock);
                
                // error
                return 0;
        }
    }
}

int avl_tree_pack ( void *p_buffer, avl_tree *p_avl_tree, fn_pack *pfn_element )
{
    
    // argument check
    if ( NULL ==    p_buffer ) goto no_buffer;
    if ( NULL ==  p_avl_tree ) goto no_avl_tree;
    if ( NULL == pfn_element ) goto no_pack;

    // initialized data 
    char *p = p_buffer;

    // lock
    mutex_lock(&p_avl_tree->_lock);

    // pack the metadata
    p += pack_pack(p, "%2i64", 
        p_avl_tree->metadata.quantity,
        p_avl_tree->metadata.size
    );

    // pack the tree
    p += avl_tree_node_pack(p, p_avl_tree->p_root, pfn_element);

    // unlock
    mutex_unlock(&p_avl_tree->_lock);

    // success
    return p - (char *)p_buffer;

    // error handling
    {
        
        // argument errors
        {

            no_buffer:
                #ifndef NDEBUG
                    log_error("[avl] Null pointer provided for \"p_buffer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_avl_tree:
                #ifndef NDEBUG
                    log_error("[avl] Null pointer provided for \"p_avl_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            no_pack:
                #ifndef NDEBUG
                    log_error("[avl] Null pointer provided for \"pfn_element\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int avl_tree_unpack
( 
    avl_tree **pp_avl_tree, 
    void      *p_buffer, 
    
    fn_unpack       *pfn_element, 
    fn_comparator   *pfn_comparator, 
    fn_key_accessor *pfn_key_accessor
)
{
    
    // argument check
    if ( NULL == pp_avl_tree ) goto no_avl_tree;
    if ( NULL == pfn_element ) goto no_unpack;

    // initialized data 
    avl_tree *p_avl_tree    = NULL;
    char     *p             = p_buffer;
    size_t    quantity      = 0, 
              size          = 0;

    // unpack the metadata
    p += pack_unpack(p, "%2i64", 
        &quantity,
        &size
    );

    // construct an avl tree
    avl_tree_construct(&p_avl_tree, size, pfn_comparator, pfn_key_accessor);

    // recursively unpack from the root
    p += avl_tree_node_unpack(&p_avl_tree->p_root, p, pfn_element);

    // store the quantity of nodes
    p_avl_tree->metadata.quantity = quantity;

    // return a pointer to the caller
    *pp_avl_tree = p_avl_tree;
    
    // success
    return p - (char *)p_buffer;

    // error handling
    {
        
        // argument errors
        {
            no_avl_tree:
                #ifndef NDEBUG
                    log_error("[avl] Null pointer provided for parameter \"pp_avl_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            no_unpack:
                #ifndef NDEBUG
                    log_error("[avl] Null pointer provided for parameter \"pfn_element\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

hash64 avl_tree_hash ( avl_tree *const p_avl_tree, fn_hash64 *pfn_hash64 )
{

    // argument check
    if ( NULL == p_avl_tree ) goto no_avl_tree;

    // initialize data
    hash64     result   = 0;
    fn_hash64 *pfn_hash = (pfn_hash64) ? pfn_hash64 : default_hash;

    // lock
    mutex_lock(&p_avl_tree->_lock);

    // compute the hash
    result = ( p_avl_tree->p_root )                           ? 
             avl_tree_node_hash(p_avl_tree->p_root, pfn_hash) : 
             pfn_hash(&eight_bytes_of_f, sizeof(eight_bytes_of_f));

    // unlock
    mutex_unlock(&p_avl_tree->_lock);

    // success
    return result;

    // error handling
    {

        // argument errors
        {
            no_avl_tree:
                #ifndef NDEBUG
                    printf("[avl] Null pointer provided for parameter \"p_avl_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int avl_tree_destroy ( avl_tree **const pp_avl_tree, fn_allocator *pfn_allocator )
{

    // argument check
    if ( NULL == pp_avl_tree ) goto no_avl_tree;

    // initialized data
    avl_tree *p_avl_tree = *pp_avl_tree;

    // fast exit
    if ( NULL == p_avl_tree ) return 1;

    // lock
    mutex_lock(&p_avl_tree->_lock);

    // no more pointer for caller
    *pp_avl_tree = NULL;

    // unlock
    mutex_unlock(&p_avl_tree->_lock);

    // recursively free nodes
    if ( 0 == avl_tree_node_destroy(&p_avl_tree->p_root, pfn_allocator) ) goto failed_to_free_nodes;

    // destroy the lock
    mutex_destroy(&p_avl_tree->_lock);

    // release the tree
    p_avl_tree = default_allocator(p_avl_tree, 0);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_avl_tree:
                #ifndef NDEBUG
                    printf("[avl] Null pointer provided for parameter \"pp_avl_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Tree error
        {
            failed_to_free_nodes:
                #ifndef NDEBUG
                    printf("[avl] Failed to free avl tree nodes in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int avl_tree_node_create ( avl_tree_node **pp_avl_tree_node )
{

    // argument check
    if ( NULL == pp_avl_tree_node ) goto no_avl_tree_node;

    // initialized data
    avl_tree_node *p_avl_tree_node = NULL;
    
    // allocate memory for an avl tree node
    p_avl_tree_node = default_allocator(0, sizeof(avl_tree_node));
    if ( NULL == p_avl_tree_node ) goto no_mem;

    // zero set the memory
    memset(p_avl_tree_node, 0, sizeof(avl_tree_node));

    // set the height
    p_avl_tree_node->height = 1;

    // return a pointer to the caller
    *pp_avl_tree_node = p_avl_tree_node;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_avl_tree_node:
                #ifndef NDEBUG
                    printf("[avl] Null pointer provided for parameter \"pp_avl_tree_node\" in call to function \"%s\"\n", __FUNCTION__);
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

int avl_tree_node_pack ( void *p_buffer, avl_tree_node *p_node, fn_pack *pfn_element )
{

    // initialized data 
    char *p = p_buffer;

    // pack the parent value
    p += pfn_element(p, p_node->p_value),

    // pack the node pointer
    p += pack_pack(p, "%i32%2i64", 
        p_node->height,
        p_node->p_left  ? p_node->p_left->node_pointer  : eight_bytes_of_f,
        p_node->p_right ? p_node->p_right->node_pointer : eight_bytes_of_f
    );

    // left
    if ( p_node->p_left ) p += avl_tree_node_pack(p, p_node->p_left, pfn_element);

    // right
    if ( p_node->p_right ) p += avl_tree_node_pack(p, p_node->p_right, pfn_element);

    // success
    return p - (char *)p_buffer;
}

int avl_tree_node_unpack ( avl_tree_node **pp_node, void *p_buffer, fn_unpack *pfn_element )
{

    // initialized data 
    avl_tree_node      *p_node   = NULL;
    char               *p        = p_buffer;
    unsigned long long  l        = 0, 
                        r        = 0;
    int                 h        = 0;

    // construct an avl tree node
    avl_tree_node_create(&p_node);
    
    // unpack the parent value
    p += pfn_element(&p_node->p_value, p);

    // unpack the node pointer
    p += pack_unpack(p, "%i32%2i64", &h, &l, &r);

    // set the height
    p_node->height = h;

    // left
    if ( l != eight_bytes_of_f ) 
        p += avl_tree_node_unpack(&p_node->p_left, p, pfn_element),
        p_node->p_left->node_pointer = l;

    // right
    if ( r != eight_bytes_of_f ) 
        p += avl_tree_node_unpack(&p_node->p_right, p, pfn_element),
        p_node->p_right->node_pointer = r;

    // return a pointer to the caller
    *pp_node = p_node;
    
    // success
    return p - (char *)p_buffer;
}

hash64 avl_tree_node_hash ( avl_tree_node *p_node, fn_hash64 *pfn_hash64 )
{

    // initialized data
    hash64 result = 0;

    // left
    if ( p_node->p_left ) result ^= avl_tree_node_hash(p_node->p_left, pfn_hash64);

    // parent
    result ^= pfn_hash64(p_node->p_value, sizeof(void *));

    // right
    if ( p_node->p_right ) result ^= avl_tree_node_hash(p_node->p_right, pfn_hash64);

    // success
    return result;
}

int avl_tree_node_traverse_preorder ( avl_tree_node *p_avl_tree_node, fn_foreach *pfn_foreach )
{

    // argument check
    if ( NULL == p_avl_tree_node ) goto no_avl_tree_node;
    if ( NULL ==     pfn_foreach ) goto no_foreach_function;

    // root
    pfn_foreach(p_avl_tree_node->p_value);

    // left
    if ( p_avl_tree_node->p_left ) avl_tree_node_traverse_preorder(p_avl_tree_node->p_left, pfn_foreach);

    // right
    if ( p_avl_tree_node->p_right ) avl_tree_node_traverse_preorder(p_avl_tree_node->p_right, pfn_foreach);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_avl_tree_node:
                #ifndef NDEBUG
                    log_error("[avl] Null pointer provided for parameter \"p_avl_tree_node\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_foreach_function:
                #ifndef NDEBUG
                    log_error("[avl] Null pointer provided for parameter \"pfn_foreach\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int avl_tree_node_traverse_inorder ( avl_tree_node *p_avl_tree_node, fn_foreach *pfn_foreach )
{

    // argument check
    if ( NULL == p_avl_tree_node ) goto no_avl_tree_node;
    if ( NULL ==     pfn_foreach ) goto no_foreach_function;

    // left
    if ( p_avl_tree_node->p_left ) avl_tree_node_traverse_inorder(p_avl_tree_node->p_left, pfn_foreach);

    // root
    pfn_foreach(p_avl_tree_node->p_value);

    // right
    if ( p_avl_tree_node->p_right ) avl_tree_node_traverse_inorder(p_avl_tree_node->p_right, pfn_foreach);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_avl_tree_node:
                #ifndef NDEBUG
                    log_error("[avl] Null pointer provided for parameter \"p_avl_tree_node\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_foreach_function:
                #ifndef NDEBUG
                    log_error("[avl] Null pointer provided for parameter \"pfn_foreach\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int avl_tree_node_traverse_postorder ( avl_tree_node *p_avl_tree_node, fn_foreach *pfn_foreach )
{

    // argument check
    if ( NULL == p_avl_tree_node ) goto no_avl_tree_node;
    if ( NULL ==     pfn_foreach ) goto no_foreach_function;

    // left
    if ( p_avl_tree_node->p_left ) avl_tree_node_traverse_postorder(p_avl_tree_node->p_left, pfn_foreach);

    // right
    if ( p_avl_tree_node->p_right ) avl_tree_node_traverse_postorder(p_avl_tree_node->p_right, pfn_foreach);

    // root
    pfn_foreach(p_avl_tree_node->p_value);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_avl_tree_node:
                #ifndef NDEBUG
                    log_error("[avl] Null pointer provided for parameter \"p_avl_tree_node\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_foreach_function:
                #ifndef NDEBUG
                    log_error("[avl] Null pointer provided for parameter \"pfn_foreach\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int avl_tree_node_forcontext ( avl_tree_node *p_avl_tree_node, fn_forcontext *pfn_forcontext, void *p_context )
{

    // argument check
    if ( NULL == p_avl_tree_node ) goto no_avl_tree_node;
    if ( NULL ==  pfn_forcontext ) goto no_forcontext_function;

    // left
    if ( p_avl_tree_node->p_left ) avl_tree_node_forcontext(p_avl_tree_node->p_left, pfn_forcontext, p_context);
    
    // root
    pfn_forcontext(p_avl_tree_node->p_value, p_context);

    // right
    if ( p_avl_tree_node->p_right ) avl_tree_node_forcontext(p_avl_tree_node->p_right, pfn_forcontext, p_context);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_avl_tree_node:
                #ifndef NDEBUG
                    log_error("[avl] Null pointer provided for parameter \"p_avl_tree_node\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_forcontext_function:
                #ifndef NDEBUG
                    log_error("[avl] Null pointer provided for parameter \"pfn_forcontext\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int avl_tree_node_destroy ( avl_tree_node **const pp_avl_tree_node, fn_allocator *pfn_allocator )
{

    // argument check
    if ( NULL == pp_avl_tree_node ) goto no_avl_tree_node;

    // initialized data
    avl_tree_node *p_avl_tree_node = *pp_avl_tree_node;

    // fast exit
    if ( NULL == p_avl_tree_node ) return 1;

    // free the value
    if ( pfn_allocator ) 
        p_avl_tree_node->p_value = pfn_allocator(p_avl_tree_node->p_value, 0);

    // recursively free the left node
    if ( 0 == avl_tree_node_destroy(&p_avl_tree_node->p_left, pfn_allocator) ) goto failed_to_free;

    // recursively free the right node
    if ( 0 == avl_tree_node_destroy(&p_avl_tree_node->p_right, pfn_allocator) ) goto failed_to_free;

    // free the node
    *pp_avl_tree_node = default_allocator(p_avl_tree_node, 0);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_avl_tree_node:
                #ifndef NDEBUG
                    printf("[avl] Null pointer provided for parameter \"pp_avl_tree_node\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // avl tree errors
        {
            failed_to_free:
                #ifndef NDEBUG
                    printf("[avl] Failed to free avl tree in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0; 
        }
    }
}

int avl_tree_node_height ( avl_tree_node *p_node )
{
    // done
    return ( p_node ) ? p_node->height : 0;
}

int avl_tree_node_update_height ( avl_tree_node *p_node )
{

    // argument check
    if ( NULL == p_node ) goto no_avl_tree_node;

    // initialized data
    int l_height = avl_tree_node_height(p_node->p_left);
    int r_height = avl_tree_node_height(p_node->p_right);

    // update the height
    p_node->height = ( (l_height > r_height) ? l_height : r_height ) + 1;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_avl_tree_node:
                #ifndef NDEBUG
                    printf("[avl] Null pointer provided for parameter \"p_node\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int avl_tree_node_balance_factor ( avl_tree_node *p_node )
{

    // argument check
    if ( NULL == p_node ) goto no_avl_tree_node;

    // done
    return avl_tree_node_height(p_node->p_left) - avl_tree_node_height(p_node->p_right);

    // error handling
    {

        // argument errors
        {
            no_avl_tree_node:
                #ifndef NDEBUG
                    printf("[avl] Null pointer provided for parameter \"p_node\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int avl_tree_node_right_rotate ( avl_tree_node **pp_node )
{

    // argument check
    if ( NULL == pp_node ) goto no_avl_tree_node;

    // initialized data
    avl_tree_node *y  = *pp_node;
    avl_tree_node *x  = y->p_left;
    avl_tree_node *t2 = x->p_right;

    // rotation
    x->p_right = y,
    y->p_left  = t2;

    // update heights
    avl_tree_node_update_height(y),
    avl_tree_node_update_height(x);

    // return the new root
    *pp_node = x;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_avl_tree_node:
                #ifndef NDEBUG
                    printf("[avl] Null pointer provided for parameter \"pp_node\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int avl_tree_node_left_rotate ( avl_tree_node **pp_node )
{
    
    // argument check
    if ( NULL == pp_node ) goto no_avl_tree_node;

    // initialized data
    avl_tree_node *x  = *pp_node;
    avl_tree_node *y  = x->p_right;
    avl_tree_node *t2 = y->p_left;

    // rotation
    y->p_left  = x,
    x->p_right = t2;

    // update heights
    avl_tree_node_update_height(x),
    avl_tree_node_update_height(y);

    // return the new root
    *pp_node = y;

    // success
    return 1;
    
    // error handling
    {

        // argument errors
        {
            no_avl_tree_node:
                #ifndef NDEBUG
                    printf("[avl] Null pointer provided for parameter \"pp_node\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int avl_tree_node_rebalance ( avl_tree_node **pp_node )
{

    // argument check
    if ( NULL == pp_node ) goto no_avl_tree_node;

    // initialized data
    avl_tree_node *p_node  = *pp_node;
    int            balance = 0;

    // update the height
    avl_tree_node_update_height(p_node);

    // get the balance factor
    balance = avl_tree_node_balance_factor(p_node);

    // left heavy?
    if ( balance > 1 )
    {

        // left right case?
        if ( avl_tree_node_balance_factor(p_node->p_left) < 0 )
            avl_tree_node_left_rotate(&p_node->p_left);

        // right rotate
        avl_tree_node_right_rotate(&p_node);
    }

    // right heavy?
    else if ( balance < -1 )
    {

        // right left case?
        if ( avl_tree_node_balance_factor(p_node->p_right) > 0 )
            avl_tree_node_right_rotate(&p_node->p_right);

        // left rotate
        avl_tree_node_left_rotate(&p_node);
    }

    // return a pointer to the caller
    *pp_node = p_node;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_avl_tree_node:
                #ifndef NDEBUG
                    printf("[avl] Null pointer provided for parameter \"pp_node\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}
