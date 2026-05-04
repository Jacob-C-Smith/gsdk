/** !
 * Red Black tree implementation
 * 
 * @file src/data/red_black/red_black.c
 * 
 * @author Jacob Smith
 */

// header file
#include <data/red_black.h>

// static data
static const unsigned long long eight_bytes_of_f = 0xffffffffffffffff;

// forward declarations
/** !
 * Allocate memory for a red black tree node
 * 
 * @param pp_red_black_tree_node result
 * 
 * @return 1 on success, 0 on error
 */
int red_black_tree_node_create ( red_black_tree_node **pp_red_black_tree_node );

/** !
 * Allocate a node for a specific red black tree, and set the node pointer. 
 * 
 * @param p_red_black_tree       the red black tree to allocate a node to
 * @param pp_red_black_tree_node result
 * 
 * @return 1 on success, 0 on error
 */
int red_black_tree_allocate_node ( red_black_tree *p_red_black_tree, red_black_tree_node **pp_red_black_tree_node );

/** !
 * Compute the hash of a red black tree node
 * 
 * @param p_node     pointer to red black tree node
 * @param pfn_hash64 pointer to hash function
 * 
 * @return the 64-bit hash on success, 0 on error
*/
hash64 red_black_tree_node_hash ( red_black_tree_node *p_node, fn_hash64 *pfn_hash64 );

/** !
 * Traverse a red black tree using the pre order technique
 * 
 * @param p_red_black_tree_node pointer to red black tree node
 * @param pfn_foreach           called for each node in the red black tree
 * 
 * @return 1 on success, 0 on error
*/
int red_black_tree_node_traverse_preorder ( red_black_tree_node *p_red_black_tree_node, fn_foreach *pfn_foreach );

/** !
 * Traverse a red black tree using the in order technique
 * 
 * @param p_red_black_tree_node pointer to red black tree node
 * @param pfn_foreach           called for each node in the red black tree
 * 
 * @return 1 on success, 0 on error
*/
int red_black_tree_node_traverse_inorder ( red_black_tree_node *p_red_black_tree_node, fn_foreach *pfn_foreach );

/** !
 * Traverse a red black tree using the post order technique
 * 
 * @param p_red_black_tree_node pointer to red black tree node
 * @param pfn_foreach           called for each node in the red black tree
 * 
 * @return 1 on success, 0 on error
*/
int red_black_tree_node_traverse_postorder ( red_black_tree_node *p_red_black_tree_node, fn_foreach *pfn_foreach );

/** !
 * Call function on a node in a red black tree
 *
 * @param p_red_black_tree_node the node
 * @param pfn_forcontext        pointer to forcontext function
 * @param p_context             the context
 * 
 * @return 1 on success, 0 on error
 */
int red_black_tree_node_forcontext ( red_black_tree_node *p_red_black_tree_node, fn_forcontext *pfn_forcontext, void *p_context );

/** ! 
 * Pack a red black tree node into a buffer
 * 
 * @param p_buffer     the buffer
 * @param p_node       the node
 * @param pfn_elemenet pointer to pack function IF not null ELSE default
 * 
 * @return bytes written on success, 0 on error
 */
int red_black_tree_node_pack ( void *p_buffer, red_black_tree_node *p_node, fn_pack *pfn_element );

/** ! 
 * Unpack a red black tree node from a buffer
 * 
 * @param pp_node      result
 * @param p_buffer     the buffer
 * @param pfn_elemenet pointer to unpack function IF not null ELSE default
 * 
 * @return bytes read on success, 0 on error
 */
int red_black_tree_node_unpack ( red_black_tree_node **pp_node, void *p_buffer, fn_unpack *pfn_element );

/** !
 * Destroy and deallocate a red black tree node
 * 
 * @param pp_red_black_tree_node pointer to red black tree node pointer
 * @param pfn_allocator  pointer to allocator function for deallocating elements
 * 
 * @return 1 on success, 0 on error
 */
int red_black_tree_node_destroy ( red_black_tree_node **const pp_red_black_tree_node, fn_allocator *pfn_allocator );

int red_black_tree_left_rotate ( red_black_tree *p_red_black_tree, red_black_tree_node *x );
int red_black_tree_right_rotate ( red_black_tree *p_red_black_tree, red_black_tree_node *y );
int red_black_tree_insert_fixup ( red_black_tree *p_red_black_tree, red_black_tree_node *z );
int red_black_tree_remove_fixup ( red_black_tree *p_red_black_tree, red_black_tree_node *x, red_black_tree_node *p, bool is_left );
int red_black_tree_transplant ( red_black_tree *p_red_black_tree, red_black_tree_node *u, red_black_tree_node *v );
red_black_tree_node *red_black_tree_minimum ( red_black_tree_node *x );

// function definitions
int red_black_tree_construct ( red_black_tree **const pp_red_black_tree, unsigned long long node_size, fn_comparator *pfn_comparator, fn_key_accessor *pfn_key_accessor )
{

    // argument check
    if ( NULL == pp_red_black_tree ) goto no_red_black_tree;

    // initialized data
    red_black_tree *p_red_black_tree = NULL;

    // allocate memory for a red black tree
    p_red_black_tree = default_allocator(0, sizeof(red_black_tree));
    if ( NULL == p_red_black_tree ) goto failed_to_allocate_red_black_tree;

    // populate the red black tree structure
    *p_red_black_tree = (red_black_tree)
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
                    log_error("[red black] Null pointer provided for parameter \"pp_red_black_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // red black tree errors
        {
            failed_to_allocate_red_black_tree:
                #ifndef NDEBUG
                    log_error("[red black] Failed to allocate red black tree in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int red_black_tree_allocate_node ( red_black_tree *p_red_black_tree, red_black_tree_node **pp_red_black_tree_node )
{

    // argument check
    if ( NULL ==       p_red_black_tree ) goto no_red_black_tree;
    if ( NULL == pp_red_black_tree_node ) goto no_red_black_tree_node;
    
    // initialized data
    red_black_tree_node *p_red_black_tree_node = NULL;

    // allocate a node
    if ( 0 == red_black_tree_node_create(&p_red_black_tree_node) ) goto failed_to_allocate_node;

    // store the node pointer
    p_red_black_tree_node->node_pointer = p_red_black_tree->metadata.quantity;

    // increment the node quantity
    p_red_black_tree->metadata.quantity++;

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
                    log_error("[red black] Null pointer provided for parameter \"p_red_black_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
                
            no_red_black_tree_node:
                #ifndef NDEBUG
                    log_error("[red black] Null pointer provided for parameter \"pp_red_black_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // red black errors
        {
            failed_to_allocate_node:
                #ifndef NDEBUG
                    log_error("[red black] Call to function \"red_black_tree_node_create\" returned an erroneous value in call to function \"%s\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int red_black_tree_search ( red_black_tree *p_red_black_tree, const void *const p_key, void **pp_value )
{

    // argument check
    if ( NULL == p_red_black_tree ) goto no_red_black_tree;

    // state check
    if ( NULL == p_red_black_tree->p_root ) return 0;

    // lock
    mutex_lock(&p_red_black_tree->_lock);

    // initialized data
    red_black_tree_node *p_node = p_red_black_tree->p_root;
    int comparator_return = 0;

    try_again:

    // which side? 
    comparator_return = p_red_black_tree->pfn_comparator
    (
        p_red_black_tree->pfn_key_accessor( p_node->p_value ),
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
        mutex_unlock(&p_red_black_tree->_lock);
        
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
                    log_error("[red black] Null pointer provided for parameter \"p_red_black_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int red_black_tree_insert ( red_black_tree *p_red_black_tree, const void *const p_value )
{

    // argument check
    if ( NULL == p_red_black_tree ) goto no_red_black_tree;

    // initialized data
    red_black_tree_node *x = NULL,
                        *y = NULL,
                        *z = NULL;

    // lock
    mutex_lock(&p_red_black_tree->_lock);

    // allocate a node
    if ( 0 == red_black_tree_allocate_node(p_red_black_tree, &z) ) goto failed_to_allocate_red_black_tree_node;

    // populate the struct
    *z = (red_black_tree_node)
    {
        .p_value = (void *)p_value,
        .p_left   = NULL,
        .p_right  = NULL,
        .p_parent = NULL,
        .color    = RED_BLACK_TREE_NODE_COLOR_RED
    };

    // store the root
    x = p_red_black_tree->p_root;

    // insert the node
    while ( x )
    {

        // store the current node
        y = x;

        // left?
        if 
        (
            p_red_black_tree->pfn_comparator
            (
                p_red_black_tree->pfn_key_accessor(x->p_value),
                p_red_black_tree->pfn_key_accessor(z->p_value)
            ) < 0
        )
            x = x->p_left;
        
        // right?
        else
            x = x->p_right;
    }

    // store the parent pointer
    z->p_parent = y;

    // update the root
    if ( NULL == y )
        p_red_black_tree->p_root = z;
    else
    {

        // left?
        if
        (
            p_red_black_tree->pfn_comparator(
                p_red_black_tree->pfn_key_accessor(y->p_value),
                p_red_black_tree->pfn_key_accessor(z->p_value)
            ) < 0
        )
            y->p_left = z;

        // right?
        else
            y->p_right = z;
    }

    // repair the red black tree
    red_black_tree_insert_fixup(p_red_black_tree, z);

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
                    log_error("[red black] Null pointer provided for parameter \"p_red_black_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // red black errors
        {
            failed_to_allocate_red_black_tree_node:
                #ifndef NDEBUG
                    log_error("[red black] Failed to allocate red black tree node in call to function \"%s\"\n", __FUNCTION__);
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
    if ( NULL ==         p_red_black_tree ) goto no_red_black_tree;
    if ( NULL == p_red_black_tree->p_root ) goto no_root;

    // initialized data
    red_black_tree_node *x = NULL,
                        *y = NULL,
                        *z = NULL,
                        *p = NULL;
    bool is_left = false;
    enum red_black_tree_node_color_e  y_original_color = -1;
    
    // lock
    mutex_lock(&p_red_black_tree->_lock);

    // store the root
    z = p_red_black_tree->p_root;

    // find the node
    while ( z )
    {

        // initialized data
        int result = p_red_black_tree->pfn_comparator
        (
            p_red_black_tree->pfn_key_accessor(z->p_value),
            p_key
        );

        // this?
        if ( 0 == result ) break;

        // left?
        if ( result < 0 )
            z = z->p_left;
        
        // right?
        else
            z = z->p_right;
    }

    // not found?
    if ( NULL == z ) goto not_found;

    // store the result
    if ( pp_value ) *pp_value = z->p_value;

    // update y
    y = z;

    // store the original color
    y_original_color = y->color;

    // replace z by its right child
    if ( NULL == z->p_left )
    {

        // store z's right child
        x = z->p_right, p = z->p_parent;

        // update left flag
        is_left = ( p && p->p_left == z );

        // replace z by its right child
        red_black_tree_transplant(p_red_black_tree, z, z->p_right);
    }

    // replace z by its left child
    else if ( NULL == z->p_right )
    {

        // store z's left child
        x = z->p_left, p = z->p_parent;

        // update left flag
        is_left = ( p && p->p_left == z );

        // replace z by its left child
        red_black_tree_transplant(p_red_black_tree, z, z->p_left);
    }

    // y is z's successor
    else
    {

        // store z's minimum
        y = red_black_tree_minimum(z->p_right);

        // store the original color
        y_original_color = y->color;

        // store y's right child
        x = y->p_right;

        // is y farther down the tree?
        if ( y->p_parent == z )
        {

            // replace y by its right child
            p = y;

            // clear the left flag
            is_left = false;

            // update x's parent
            if ( x ) x->p_parent = y;
        }
        else
        {

            // store y's parent
            p = y->p_parent;

            // set the left flag
            is_left = true;

            // transplant
            red_black_tree_transplant(p_red_black_tree, y, y->p_right);

            // update y's right child
            y->p_right = z->p_right;
            y->p_right->p_parent = y;
        }

        // replace z by its successor y
        red_black_tree_transplant(p_red_black_tree, z, y);

        // give z's left child to y, which has no left child
        y->p_left = z->p_left,
        y->p_left->p_parent = y,
        y->color = z->color;
    }

    // enforce red black constraints
    if ( RED_BLACK_TREE_NODE_COLOR_BLACK == y_original_color )
        red_black_tree_remove_fixup(p_red_black_tree, x, p, is_left);

    // no more children for node
    z->p_left = NULL, z->p_right = NULL;

    // release the node
    red_black_tree_node_destroy(&z, NULL);

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
                    log_error("[red black] Null pointer provided for parameter \"p_red_black_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
                
            no_root:
                #ifndef NDEBUG
                    log_error("[red black] Empty tree in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // red black errors
        {
            not_found:
                #ifndef NDEBUG
                    log_error("[red black] Failed to find key in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // unlock
                mutex_unlock(&p_red_black_tree->_lock);
                
                // error
                return 0;
        }
    }
}

int red_black_tree_traverse_preorder ( red_black_tree *const p_red_black_tree, fn_foreach *pfn_foreach )
{

    // argument check
    if ( NULL == p_red_black_tree ) goto no_red_black_tree;
    if ( NULL ==      pfn_foreach ) goto no_foreach_function;

    // edge case
    if ( 0 == p_red_black_tree->metadata.quantity ) return 1;

    // lock
    mutex_lock(&p_red_black_tree->_lock);

    // traverse the tree
    if ( 0 == red_black_tree_node_traverse_preorder(p_red_black_tree->p_root, pfn_foreach) ) goto failed_to_traverse_red_black_tree;    

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
                    log_error("[red black] Null pointer provided for parameter \"p_red_black_tree\" in call to function \"%s\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_foreach_function:
                #ifndef NDEBUG
                    log_error("[red black] Null pointer provided for parameter \"pfn_foreach\" in call to function \"%s\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // red black tree errors
        {
            failed_to_traverse_red_black_tree:
                #ifndef NDEBUG
                    log_error("[red black] Failed to traverse red black tree in call to function \"%s\n", __FUNCTION__);
                #endif
                
                // unlock
                mutex_unlock(&p_red_black_tree->_lock);
                
                // error
                return 0;
        }
    }
}

int red_black_tree_traverse_inorder ( red_black_tree *const p_red_black_tree, fn_foreach *pfn_foreach )
{

    // argument check
    if ( NULL == p_red_black_tree ) goto no_red_black_tree;
    if ( NULL ==      pfn_foreach ) goto no_foreach_function;

    // edge case
    if ( 0 == p_red_black_tree->metadata.quantity ) return 1;

    // lock
    mutex_lock(&p_red_black_tree->_lock);

    // traverse the tree
    if ( 0 == red_black_tree_node_traverse_inorder(p_red_black_tree->p_root, pfn_foreach) ) goto failed_to_traverse_red_black_tree;    

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
                    log_error("[red black] Null pointer provided for parameter \"p_red_black_tree\" in call to function \"%s\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_foreach_function:
                #ifndef NDEBUG
                    log_error("[red black] Null pointer provided for parameter \"pfn_foreach\" in call to function \"%s\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // red black tree errors
        {
            failed_to_traverse_red_black_tree:
                #ifndef NDEBUG
                    log_error("[red black] Failed to traverse red black tree in call to function \"%s\n", __FUNCTION__);
                #endif
                
                // unlock
                mutex_unlock(&p_red_black_tree->_lock);
                
                // error
                return 0;
        }
    }
}

int red_black_tree_traverse_postorder ( red_black_tree *const p_red_black_tree, fn_foreach *pfn_foreach )
{

    // argument check
    if ( NULL == p_red_black_tree ) goto no_red_black_tree;
    if ( NULL ==      pfn_foreach ) goto no_foreach_function;

    // edge case
    if ( 0 == p_red_black_tree->metadata.quantity ) return 1;
    
    // lock
    mutex_lock(&p_red_black_tree->_lock);

    // traverse the tree
    if ( 0 == red_black_tree_node_traverse_postorder(p_red_black_tree->p_root, pfn_foreach) ) goto failed_to_traverse_red_black_tree;    

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
                    log_error("[red black] Null pointer provided for parameter \"p_red_black_tree\" in call to function \"%s\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_foreach_function:
                #ifndef NDEBUG
                    log_error("[red black] Null pointer provided for parameter \"pfn_foreach\" in call to function \"%s\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // red black tree errors
        {
            failed_to_traverse_red_black_tree:
                #ifndef NDEBUG
                    log_error("[red black] Failed to traverse red black tree in call to function \"%s\n", __FUNCTION__);
                #endif
                
                // unlock
                mutex_unlock(&p_red_black_tree->_lock);
                
                // error
                return 0;
        }
    }
}

int red_black_tree_forcontext ( red_black_tree *const p_red_black_tree, fn_forcontext *pfn_forcontext, void *p_context )
{

    // argument check
    if ( NULL == p_red_black_tree ) goto no_red_black_tree;
    if ( NULL ==   pfn_forcontext ) goto no_forcontext_function;

    // edge case
    if ( 0 == p_red_black_tree->metadata.quantity ) return 1;

    // lock
    mutex_lock(&p_red_black_tree->_lock);

    // traverse the tree
    if ( 0 == red_black_tree_node_forcontext(p_red_black_tree->p_root, pfn_forcontext, p_context) ) goto failed_to_traverse_red_black_tree;    

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
                    log_error("[red black] Null pointer provided for parameter \"p_red_black_tree\" in call to function \"%s\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_forcontext_function:
                #ifndef NDEBUG
                    log_error("[red black] Null pointer provided for parameter \"pfn_forcontext\" in call to function \"%s\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // red black tree errors
        {
            failed_to_traverse_red_black_tree:
                #ifndef NDEBUG
                    log_error("[red black] Failed to traverse red black tree in call to function \"%s\n", __FUNCTION__);
                #endif
                
                // unlock
                mutex_unlock(&p_red_black_tree->_lock);
                
                // error
                return 0;
        }
    }
}

int red_black_tree_pack ( void *p_buffer, red_black_tree *p_red_black_tree, fn_pack *pfn_element )
{
    
    // argument check
    if ( NULL == p_red_black_tree ) goto no_red_black_tree;
    if ( NULL ==      pfn_element ) goto no_pack;

    // initialized data 
    char *p = p_buffer;

    // lock
    mutex_lock(&p_red_black_tree->_lock);

    // pack the metadata
    p += pack_pack(p, "%2i64", 
        p_red_black_tree->metadata.quantity,
        p_red_black_tree->metadata.size
    );

    // pack the tree
    p += red_black_tree_node_pack(p, p_red_black_tree->p_root, pfn_element);

    // unlock
    mutex_unlock(&p_red_black_tree->_lock);

    // success
    return p - (char *)p_buffer;

    // error handling
    {
        
        // argument errors
        {
            no_red_black_tree:
                #ifndef NDEBUG
                    log_error("[red black] Null pointer provided for \"p_red_black_tree\" in call to function \"%s\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_pack:
                
                #ifndef NDEBUG
                    log_error("[red black] Null pointer provided for \"pfn_pack\" in call to function \"%s\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int red_black_tree_unpack
( 
    red_black_tree **pp_red_black_tree, 
    void *p_buffer, 
    
    fn_unpack       *pfn_element, 
    fn_comparator   *pfn_comparator, 
    fn_key_accessor *pfn_key_accessor
)
{
    
    // argument check
    if ( NULL == pp_red_black_tree ) goto no_red_black_tree;
    if ( NULL ==       pfn_element ) goto no_unpack;

    // initialized data 
    red_black_tree *p_red_black_tree = NULL;
    char        *p             = p_buffer;
    size_t       quantity      = 0, 
                 size          = 0;

    // unpack the metadata
    p += pack_unpack(p, "%2i64", 
        &quantity,
        &size
    );

    // construct a red black tree
    red_black_tree_construct(&p_red_black_tree, size, pfn_comparator, pfn_key_accessor);

    // recursively unpack from the root
    p += red_black_tree_node_unpack(&p_red_black_tree->p_root, p, pfn_element);

    // store the quantity of nodes
    p_red_black_tree->metadata.quantity = quantity;

    // return a pointer to the caller
    *pp_red_black_tree = p_red_black_tree;
    
    // success
    return p - (char *)p_buffer;

    // error handling
    {
        
        // argument errors
        {
            no_red_black_tree:
                #ifndef NDEBUG
                    log_error("[red black] Null pointer provided for parameter \"pp_red_black_tree\" in call to function \"%s\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_unpack:
                #ifndef NDEBUG
                    log_error("[red black] Null pointer provided for parameter \"pfn_element\" in call to function \"%s\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

hash64 red_black_tree_hash ( red_black_tree *const p_red_black_tree, fn_hash64 *pfn_hash64 )
{

    // argument check
    if ( NULL == p_red_black_tree ) goto no_red_black_tree;

    // initialize data
    hash64 result = 0;
    fn_hash64 *pfn_hash = (pfn_hash64) ? pfn_hash64 : default_hash;

    // lock
    mutex_lock(&p_red_black_tree->_lock);

    // compute the hash
    result = ( p_red_black_tree->p_root )                                 ? 
             red_black_tree_node_hash(p_red_black_tree->p_root, pfn_hash) : 
             default_hash(&eight_bytes_of_f, sizeof(eight_bytes_of_f));

    // unlock
    mutex_unlock(&p_red_black_tree->_lock);

    // success
    return result;

    // error handling
    {

        // argument errors
        {
            no_red_black_tree:
                #ifndef NDEBUG
                    log_error("[red black] Null pointer provided for parameter \"p_red_black_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int red_black_tree_destroy ( red_black_tree **const pp_red_black_tree, fn_allocator *pfn_allocator )
{

    // argument check
    if ( NULL == pp_red_black_tree ) goto no_red_black_tree;

    // initialized data
    red_black_tree *p_red_black_tree = *pp_red_black_tree;

    // fast exit
    if ( NULL == p_red_black_tree ) return 1;

    // lock
    mutex_lock(&p_red_black_tree->_lock);

    // no more pointer for caller
    *pp_red_black_tree = NULL;

    // unlock
    mutex_unlock(&p_red_black_tree->_lock);

    // recursively free nodes
    if ( 0 == red_black_tree_node_destroy(&p_red_black_tree->p_root, pfn_allocator) ) goto failed_to_free_nodes;

    // destroy the lock
    mutex_destroy(&p_red_black_tree->_lock);

    // release the tree
    p_red_black_tree = default_allocator(p_red_black_tree, 0);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_red_black_tree:
                #ifndef NDEBUG
                    log_error("[red black] Null pointer provided for parameter \"pp_red_black_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Tree error
        {
            failed_to_free_nodes:
                #ifndef NDEBUG
                    log_error("[red black] Failed to allocate red black tree node in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int red_black_tree_node_create ( red_black_tree_node **pp_red_black_tree_node )
{

    // argument check
    if ( NULL == pp_red_black_tree_node ) goto no_red_black_tree_node;

    // initialized data
    red_black_tree_node *p_red_black_tree_node = NULL;
    
    // allocate memory for a red black tree node
    p_red_black_tree_node = default_allocator(0, sizeof(red_black_tree_node));
    if ( NULL == p_red_black_tree_node ) goto no_mem;

    // zero set the memory
    memset(p_red_black_tree_node, 0, sizeof(red_black_tree_node));

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
                    log_error("[red black] Null pointer provided for parameter \"pp_red_black_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[standard library] Call to function \"default_allocator\" returned an erroneous value in call to function \"%s\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int red_black_tree_node_pack ( void *p_buffer, red_black_tree_node *p_node, fn_pack *pfn_element )
{

    // initialized data
    char *p = p_buffer;
    int color = (int)p_node->color;

    // pack the value
    p += pfn_element(p, p_node->p_value);

    // pack the color, left, and right children
    p += pack_pack(p, "%i32%2i64", 
        color,
        p_node->p_left  ? p_node->p_left->node_pointer  : eight_bytes_of_f,
        p_node->p_right ? p_node->p_right->node_pointer : eight_bytes_of_f
    );

    // pack the left node
    if ( p_node->p_left ) p += red_black_tree_node_pack(p, p_node->p_left, pfn_element);

    // pack the right node
    if ( p_node->p_right ) p += red_black_tree_node_pack(p, p_node->p_right, pfn_element);

    // success
    return p - (char *)p_buffer;
}

int red_black_tree_node_unpack ( red_black_tree_node **pp_node, void *p_buffer, fn_unpack *pfn_element )
{
    
    // initialized data
    red_black_tree_node *p_node = NULL;
    char                *p      = p_buffer;
    unsigned long long l = 0, r = 0;
    int color = 0;

    // construct a node
    red_black_tree_node_create(&p_node);
    
    // unpack the value
    p += pfn_element(&p_node->p_value, p);

    // unpack the color, left, and right children
    p += pack_unpack(p, "%i32%2i64", &color, &l, &r);
    
    // store the color
    p_node->color = (enum red_black_tree_node_color_e)color;

    // unpack the left node
    if ( l != eight_bytes_of_f ) 

        // unpack the left node
        p += red_black_tree_node_unpack(&p_node->p_left, p, pfn_element),

        // store the left node pointer
        p_node->p_left->node_pointer = l,

        // store the parent pointer
        p_node->p_left->p_parent = p_node;

    // unpack the right node
    if ( r != eight_bytes_of_f ) 

        // unpack the right node
        p += red_black_tree_node_unpack(&p_node->p_right, p, pfn_element),

        // store the right node pointer
        p_node->p_right->node_pointer = r,

        // store the parent pointer
        p_node->p_right->p_parent = p_node;

    // return a pointer to the caller
    *pp_node = p_node;

    // success
    return p - (char *)p_buffer;
}

hash64 red_black_tree_node_hash ( red_black_tree_node *p_node, fn_hash64 *pfn_hash64 )
{

    // argument check
    if ( NULL == p_node ) goto no_node;

    // initialized data
    fn_hash64 *pfn_hash = (pfn_hash64) ? pfn_hash64 : default_hash;
    hash64 result = 0;

    // left
    if ( p_node->p_left ) result ^= red_black_tree_node_hash(p_node->p_left, pfn_hash);

    // parent
    result ^= pfn_hash(p_node->p_value, sizeof(void *));

    // right
    if ( p_node->p_right ) result ^= red_black_tree_node_hash(p_node->p_right, pfn_hash);

    // success
    return result;

    // error handling
    {

        // argument errors
        {
            no_node:
                #ifndef NDEBUG
                    log_error("[red black] Null pointer provided for parameter \"p_node\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int red_black_tree_node_traverse_preorder ( red_black_tree_node *p_red_black_tree_node, fn_foreach *pfn_foreach )
{

    // argument check
    if ( NULL == p_red_black_tree_node ) goto no_red_black_tree_node;
    if ( NULL ==           pfn_foreach ) goto no_foreach_function;

    // root
    pfn_foreach(p_red_black_tree_node->p_value);

    // left
    if ( p_red_black_tree_node->p_left ) red_black_tree_node_traverse_preorder(p_red_black_tree_node->p_left, pfn_foreach);

    // right
    if ( p_red_black_tree_node->p_right ) red_black_tree_node_traverse_preorder(p_red_black_tree_node->p_right, pfn_foreach);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_red_black_tree_node:
                #ifndef NDEBUG
                    log_error("[red black] Null pointer provided for parameter \"p_red_black_tree_node\" in call to function \"%s\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_foreach_function:
                #ifndef NDEBUG
                    log_error("[red black] Null pointer provided for parameter \"pfn_foreach\" in call to function \"%s\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int red_black_tree_node_traverse_inorder ( red_black_tree_node *p_red_black_tree_node, fn_foreach *pfn_foreach )
{

    // argument check
    if ( NULL == p_red_black_tree_node ) goto no_red_black_tree_node;
    if ( NULL ==           pfn_foreach ) goto no_foreach_function;

    // left
    if ( p_red_black_tree_node->p_left ) red_black_tree_node_traverse_inorder(p_red_black_tree_node->p_left, pfn_foreach);

    // root
    pfn_foreach(p_red_black_tree_node->p_value);

    // right
    if ( p_red_black_tree_node->p_right ) red_black_tree_node_traverse_inorder(p_red_black_tree_node->p_right, pfn_foreach);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_red_black_tree_node:
                #ifndef NDEBUG
                    log_error("[red black] Null pointer provided for parameter \"p_red_black_tree_node\" in call to function \"%s\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_foreach_function:
                #ifndef NDEBUG
                    log_error("[red black] Null pointer provided for parameter \"pfn_foreach\" in call to function \"%s\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int red_black_tree_node_traverse_postorder ( red_black_tree_node *p_red_black_tree_node, fn_foreach *pfn_foreach )
{

    // argument check
    if ( NULL == p_red_black_tree_node ) goto no_red_black_tree_node;
    if ( NULL ==           pfn_foreach ) goto no_foreach_function;

    // left
    if ( p_red_black_tree_node->p_left ) red_black_tree_node_traverse_postorder(p_red_black_tree_node->p_left, pfn_foreach);

    // right
    if ( p_red_black_tree_node->p_right ) red_black_tree_node_traverse_postorder(p_red_black_tree_node->p_right, pfn_foreach);

    // root
    pfn_foreach(p_red_black_tree_node->p_value);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_red_black_tree_node:
                #ifndef NDEBUG
                    log_error("[red black] Null pointer provided for parameter \"p_red_black_tree_node\" in call to function \"%s\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_foreach_function:
                #ifndef NDEBUG
                    log_error("[red black] Null pointer provided for parameter \"pfn_foreach\" in call to function \"%s\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int red_black_tree_node_forcontext ( red_black_tree_node *p_red_black_tree_node, fn_forcontext *pfn_forcontext, void *p_context )
{

    // argument check
    if ( NULL == p_red_black_tree_node ) goto no_red_black_tree_node;
    if ( NULL ==        pfn_forcontext ) goto no_forcontext_function;

    // left
    if ( p_red_black_tree_node->p_left ) red_black_tree_node_forcontext(p_red_black_tree_node->p_left, pfn_forcontext, p_context);
    
    // root
    pfn_forcontext(p_red_black_tree_node->p_value, p_context);

    // right
    if ( p_red_black_tree_node->p_right ) red_black_tree_node_forcontext(p_red_black_tree_node->p_right, pfn_forcontext, p_context);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_red_black_tree_node:
                #ifndef NDEBUG
                    log_error("[red black] Null pointer provided for parameter \"p_red_black_tree_node\" in call to function \"%s\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_forcontext_function:
                #ifndef NDEBUG
                    log_error("[red black] Null pointer provided for parameter \"pfn_forcontext\" in call to function \"%s\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int red_black_tree_node_destroy ( red_black_tree_node **const pp_red_black_tree_node, fn_allocator *pfn_allocator )
{

    // argument check
    if ( NULL == pp_red_black_tree_node ) goto no_red_black_tree_node;

    // initialized data
    red_black_tree_node *p_red_black_tree_node = *pp_red_black_tree_node;

    // fast exit
    if ( NULL == p_red_black_tree_node ) return 1;

    // no more pointer for caller
    *pp_red_black_tree_node = NULL;

    // release the value
    if ( pfn_allocator ) 
        p_red_black_tree_node->p_value = pfn_allocator(p_red_black_tree_node->p_value, 0);

    // recursively free the left node
    if ( 0 == red_black_tree_node_destroy(&p_red_black_tree_node->p_left, pfn_allocator) ) goto failed_to_free;

    // recursively free the right node
    if ( 0 == red_black_tree_node_destroy(&p_red_black_tree_node->p_right, pfn_allocator) ) goto failed_to_free;

    // free the node
    p_red_black_tree_node = default_allocator(p_red_black_tree_node, 0);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_red_black_tree_node:
                #ifndef NDEBUG
                    log_error("[red black] Null pointer provided for parameter \"pp_red_black_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // red black tree errors
        {
            failed_to_free:
                #ifndef NDEBUG
                    log_error("[red black] Failed to allocate red black tree in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0; 
        }
    }
}

int red_black_tree_left_rotate ( red_black_tree *p_red_black_tree, red_black_tree_node *x )
{

    // initialized data
    red_black_tree_node *y = x->p_right;

    // x's right is y's left
    x->p_right = y->p_left;
    if ( y->p_left ) 
        y->p_left->p_parent = x;

    // y replaces x
    y->p_parent = x->p_parent;

    // x was root
    if ( NULL == x->p_parent )
        p_red_black_tree->p_root = y;

    // x was left 
    else if ( x == x->p_parent->p_left )
        x->p_parent->p_left = y;

    // x was right
    else
        x->p_parent->p_right = y;

    // x is y's left child
    y->p_left = x, x->p_parent = y;

    // success
    return 1;
}

int red_black_tree_right_rotate ( red_black_tree *p_red_black_tree, red_black_tree_node *y )
{

    // initialized data
    red_black_tree_node *x = y->p_left;

    // x's left is y's right 
    y->p_left = x->p_right;
    if ( x->p_right )
        x->p_right->p_parent = y;

    // y replaces x
    x->p_parent = y->p_parent;

    // x was root
    if ( NULL == y->p_parent )
        p_red_black_tree->p_root = x;

    // x was right
    else if (y == y->p_parent->p_right)
        y->p_parent->p_right = x;

    // x was left
    else
        y->p_parent->p_left = x;

    // x is y's right child
    x->p_right = y, y->p_parent = x;

    // success
    return 1;
}

int red_black_tree_insert_fixup ( red_black_tree *p_red_black_tree, red_black_tree_node *z )
{

    // fixup
    while 
    (
        RED_BLACK_TREE_NODE_COLOR_RED == z->p_parent->color && 
        z->p_parent
    )
    {

        // z's parent is left?
        if ( z->p_parent == z->p_parent->p_parent->p_left )
        {

            // initialized data
            red_black_tree_node *y = z->p_parent->p_parent->p_right;

            // y is red
            if 
            (
                y && 
                RED_BLACK_TREE_NODE_COLOR_RED == y->color 
            )
            {

                // recolor
                z->p_parent->color           = RED_BLACK_TREE_NODE_COLOR_BLACK,
                y->color                     = RED_BLACK_TREE_NODE_COLOR_BLACK,
                z->p_parent->p_parent->color = RED_BLACK_TREE_NODE_COLOR_RED;

                // update z
                z = z->p_parent->p_parent;
            }

            // y is black
            else
            {

                // y is black and z is the right child
                if ( z == z->p_parent->p_right )
                {

                    // update z
                    z = z->p_parent;

                    // left rotate
                    red_black_tree_left_rotate(p_red_black_tree, z);
                }

                // recolor
                z->p_parent->color           = RED_BLACK_TREE_NODE_COLOR_BLACK,
                z->p_parent->p_parent->color = RED_BLACK_TREE_NODE_COLOR_RED;

                // right rotate
                red_black_tree_right_rotate(p_red_black_tree, z->p_parent->p_parent);
            }
        }
        
        // z's parent is right?
        else
        {

            // initialized data
            red_black_tree_node *y = z->p_parent->p_parent->p_left;

            // y is red
            if 
            (
                y &&
                RED_BLACK_TREE_NODE_COLOR_RED == y->color  
            )
            {

                // recolor
                z->p_parent->color           = RED_BLACK_TREE_NODE_COLOR_BLACK,
                y->color                     = RED_BLACK_TREE_NODE_COLOR_BLACK,
                z->p_parent->p_parent->color = RED_BLACK_TREE_NODE_COLOR_RED;

                // update z
                z = z->p_parent->p_parent;
            }

            // y is black
            else
            {

                // y is black and z is the left child
                if ( z == z->p_parent->p_left )
                {

                    // update z
                    z = z->p_parent;

                    // right rotate
                    red_black_tree_right_rotate(p_red_black_tree, z);
                }

                // recolor
                z->p_parent->color           = RED_BLACK_TREE_NODE_COLOR_BLACK,
                z->p_parent->p_parent->color = RED_BLACK_TREE_NODE_COLOR_RED;

                // left rotate
                red_black_tree_left_rotate(p_red_black_tree, z->p_parent->p_parent);
            }
        }
    }
    
    // update the root's color
    p_red_black_tree->p_root->color = RED_BLACK_TREE_NODE_COLOR_BLACK;

    // success
    return 1;
}

int red_black_tree_remove_fixup ( red_black_tree *p_red_black_tree, red_black_tree_node *x, red_black_tree_node *p, bool is_left )
{

    // fixup
    while 
    ( 
        x != p_red_black_tree->p_root && 
        (
            NULL == x || 
            x->color == RED_BLACK_TREE_NODE_COLOR_BLACK
        )
    )
    {

        // x is left?
        if ( is_left )
        {

            // initialized data
            red_black_tree_node *w = p->p_right;

            // sibling is red?
            if ( RED_BLACK_TREE_NODE_COLOR_RED == w->color )
            {

                // recolor
                w->color = RED_BLACK_TREE_NODE_COLOR_BLACK,
                p->color = RED_BLACK_TREE_NODE_COLOR_RED;

                // left rotate
                red_black_tree_left_rotate(p_red_black_tree, p);

                // update sibling
                w = p->p_right;
            }

            // sibling is black with black children?
            if 
            (
                (
                    NULL                            == w->p_left || 
                    RED_BLACK_TREE_NODE_COLOR_BLACK == w->p_left->color 
                ) 
                &&
                (
                    NULL                            == w->p_right ||
                    RED_BLACK_TREE_NODE_COLOR_BLACK == w->p_right->color 
                )
            )
            {

                // recolor
                w->color = RED_BLACK_TREE_NODE_COLOR_RED;

                // update x
                x = p;

                // update parent
                p = x->p_parent;

                // update left flag
                if ( p ) is_left = (x == p->p_left);
            }
            
            // sibling is black with at least one red child?
            else
            {

                // right child is black?
                if 
                (
                    NULL                            == w->p_right ||
                    RED_BLACK_TREE_NODE_COLOR_BLACK == w->p_right->color 
                )
                {

                    // recolor
                    if ( w->p_left ) 
                        w->p_left->color = RED_BLACK_TREE_NODE_COLOR_BLACK;

                    w->color = RED_BLACK_TREE_NODE_COLOR_RED;
                    
                    // right rotate
                    red_black_tree_right_rotate(p_red_black_tree, w);
                    
                    // update sibling
                    w = p->p_right;
                }

                // recolor
                w->color = p->color;
                p->color = RED_BLACK_TREE_NODE_COLOR_BLACK;
                
                if ( w->p_right ) w->p_right->color = RED_BLACK_TREE_NODE_COLOR_BLACK;
                
                // left rotate
                red_black_tree_left_rotate(p_red_black_tree, p);
                
                // update x
                x = p_red_black_tree->p_root;
            }
        }

        // x is right
        else
        {

            // initialized data
            red_black_tree_node *w = p->p_left;

            // sibling is red?
            if ( RED_BLACK_TREE_NODE_COLOR_RED == w->color )
            {

                // recolor
                w->color = RED_BLACK_TREE_NODE_COLOR_BLACK,
                p->color = RED_BLACK_TREE_NODE_COLOR_RED;

                // right rotate
                red_black_tree_right_rotate(p_red_black_tree, p);

                // update sibling
                w = p->p_left;
            }

            // sibling is black with black children?
            if
            (
                (
                    NULL                            == w->p_right ||
                    RED_BLACK_TREE_NODE_COLOR_BLACK == w->p_right->color 
                ) 
                &&
                (
                    NULL                            == w->p_left ||
                    RED_BLACK_TREE_NODE_COLOR_BLACK == w->p_left->color 
                )
            )
            {

                // recolor
                w->color = RED_BLACK_TREE_NODE_COLOR_RED;

                // update x
                x = p;

                // update parent
                p = x->p_parent;

                // update left flag
                if ( p ) is_left = (x == p->p_left);
            }

            // sibling is black with at least one red child?
            else
            {


                // left child is black?
                if
                (
                    NULL                            == w->p_left ||
                    RED_BLACK_TREE_NODE_COLOR_BLACK == w->p_left->color 
                )
                {

                    // recolor
                    if ( w->p_right )
                        w->p_right->color = RED_BLACK_TREE_NODE_COLOR_BLACK;

                    w->color = RED_BLACK_TREE_NODE_COLOR_RED;

                    // left rotate
                    red_black_tree_left_rotate(p_red_black_tree, w);

                    // update sibling
                    w = p->p_left;
                }

                // recolor
                w->color = p->color,
                p->color = RED_BLACK_TREE_NODE_COLOR_BLACK;

                if ( w->p_left ) w->p_left->color = RED_BLACK_TREE_NODE_COLOR_BLACK;

                // right rotate
                red_black_tree_right_rotate(p_red_black_tree, p);

                // update x
                x = p_red_black_tree->p_root;
            }
        }
    }
    
    // update the color
    if ( x ) x->color = RED_BLACK_TREE_NODE_COLOR_BLACK;

    // success
    return 1;
}

int red_black_tree_transplant ( red_black_tree *p_red_black_tree, red_black_tree_node *u, red_black_tree_node *v )
{

    // if u is root ...
    if ( NULL == u->p_parent )

        // ... replace root with v
        p_red_black_tree->p_root = v;

    // if u is left child of parent ...
    else if ( u == u->p_parent->p_left )

        // ... replace it with v
        u->p_parent->p_left = v;

    // u is right child of parent ...
    else

        // replace it with v
        u->p_parent->p_right = v;

    // update v's parent
    if ( v )
        v->p_parent = u->p_parent;

    // success
    return 1;
}

red_black_tree_node *red_black_tree_minimum ( red_black_tree_node *x )
{
    
    // walk left
    while ( x->p_left ) x = x->p_left;
    
    // done
    return x;
}
