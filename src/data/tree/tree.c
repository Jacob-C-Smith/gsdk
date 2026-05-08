/** !
 * tree implementation
 * 
 * @file src/data/tree/tree.c
 * 
 * @author Jacob Smith
 */

// header file
#include <data/tree.h>

#include <data/avl.h>
#include <data/binary.h>
#include <data/red_black.h>

// data
/// prototypes
tree _prototypes[TREE_QUANTITY] = 
{
    [TREE_BINARY] = 
    {
        .p_tree               = NULL,
        ._type                = TREE_BINARY,

        .pfn_insert           = (fn_tree_insert *)binary_tree_insert, 
        .pfn_remove           = (fn_tree_remove *)binary_tree_remove, 
        .pfn_search           = (fn_tree_search *)binary_tree_search, 
        .pfn_is_empty         = (fn_tree_is_empty *)binary_tree_is_empty,
        .pfn_size             = (fn_tree_size *)binary_tree_size,
        .pfn_traverse_inorder = (fn_tree_traverse_inorder *)binary_tree_traverse_inorder, 
        .pfn_pack             = (fn_tree_pack *)binary_tree_pack, 
        .pfn_unpack           = (fn_tree_unpack *)binary_tree_unpack, 
        .pfn_destroy          = (fn_tree_destroy *)binary_tree_destroy, 
        .pfn_hash             = (fn_tree_hash *)binary_tree_hash, 
    },
    [TREE_AVL] = 
    {
        .p_tree               = NULL,
        ._type                = TREE_AVL,

        .pfn_insert           = (fn_tree_insert *)avl_tree_insert, 
        .pfn_remove           = (fn_tree_remove *)avl_tree_remove, 
        .pfn_search           = (fn_tree_search *)avl_tree_search, 
        .pfn_is_empty         = (fn_tree_is_empty *)avl_tree_is_empty,
        .pfn_size             = (fn_tree_size *)avl_tree_size,
        .pfn_traverse_inorder = (fn_tree_traverse_inorder *)avl_tree_traverse_inorder, 
        .pfn_pack             = (fn_tree_pack *)avl_tree_pack, 
        .pfn_unpack           = (fn_tree_unpack *)avl_tree_unpack, 
        .pfn_destroy          = (fn_tree_destroy *)avl_tree_destroy, 
        .pfn_hash             = (fn_tree_hash *)avl_tree_hash, 
    },
    [TREE_RED_BLACK] = 
    {
        .p_tree               = NULL,
        ._type                = TREE_RED_BLACK,

        .pfn_insert           = (fn_tree_insert *)red_black_tree_insert, 
        .pfn_remove           = (fn_tree_remove *)red_black_tree_remove, 
        .pfn_search           = (fn_tree_search *)red_black_tree_search, 
        .pfn_is_empty         = (fn_tree_is_empty *)red_black_tree_is_empty,
        .pfn_size             = (fn_tree_size *)red_black_tree_size,
        .pfn_traverse_inorder = (fn_tree_traverse_inorder *)red_black_tree_traverse_inorder, 
        .pfn_pack             = (fn_tree_pack *)red_black_tree_pack, 
        .pfn_unpack           = (fn_tree_unpack *)red_black_tree_unpack, 
        .pfn_destroy          = (fn_tree_destroy *)red_black_tree_destroy, 
        .pfn_hash             = (fn_tree_hash *)red_black_tree_hash, 
    },
};

// function definitions
int tree_construct 
(
    tree **const         pp_tree,
    enum tree_type_e     _type,

    unsigned long long   node_size,
    fn_comparator       *pfn_comparator, 
    fn_key_accessor     *pfn_key_accessor 
)
{

    // argument check
    if ( NULL == pp_tree ) goto no_tree;

    // initialized data
    tree *p_tree = NULL;
    void *p_concrete_tree = NULL;
    int   result = 0;

    // allocate memory for a tree
    p_tree = default_allocator(0, sizeof(tree));
    if ( NULL == p_tree ) goto failed_to_allocate_tree;

    // strategy
    switch (_type)
    {
        case TREE_BINARY:
            result = binary_tree_construct((binary_tree **)&p_concrete_tree, node_size, pfn_comparator, pfn_key_accessor);
            break;
        case TREE_AVL:
            result = avl_tree_construct((avl_tree **)&p_concrete_tree, node_size, pfn_comparator, pfn_key_accessor);
            break;
        case TREE_RED_BLACK:
            result = red_black_tree_construct((red_black_tree **)&p_concrete_tree, node_size, pfn_comparator, pfn_key_accessor);
            break;
        default: goto invalid_tree_type;
    }

    // error check
    if ( 0 == result ) goto failed_to_construct_concrete_tree;

    // clone the prototype
    memcpy(p_tree, &_prototypes[_type], sizeof(tree));

    // store the concrete tree
    p_tree->p_tree = p_concrete_tree;

    // return a pointer to the caller
    *pp_tree = p_tree;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_tree:
                #ifndef NDEBUG
                    printf("[tree] Null pointer provided for parameter \"pp_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // tree errors
        {
            failed_to_allocate_tree:
                #ifndef NDEBUG
                    printf("[tree] Failed to allocate tree in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            invalid_tree_type:
                #ifndef NDEBUG
                    printf("[tree] Invalid tree type in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // release the tree
                p_tree = default_allocator(p_tree, 0);

                // error
                return 0;

            failed_to_construct_concrete_tree:
                #ifndef NDEBUG
                    printf("[tree] Failed to construct concrete tree in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // release the tree
                p_tree = default_allocator(p_tree, 0);

                // error
                return 0;
        }
    }
}

int tree_search ( tree *p_tree, const void *const p_key, void **pp_value )
{

    // argument check
    if ( NULL == p_tree ) goto no_tree;
    
    // done
    return p_tree->pfn_search(p_tree->p_tree, p_key, pp_value);

    // error handling
    {

        // argument errors
        {
            no_tree:
                #ifndef NDEBUG
                    printf("[tree] Null pointer provided for parameter \"p_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

bool tree_is_empty ( tree *p_tree )
{

    // argument check
    if ( NULL == p_tree ) goto no_tree;
    
    // done
    return p_tree->pfn_is_empty(p_tree->p_tree);

    // error handling
    {

        // argument errors
        {
            no_tree:
                #ifndef NDEBUG
                    printf("[tree] Null pointer provided for parameter \"p_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

size_t tree_size ( tree *p_tree )
{

    // argument check
    if ( NULL == p_tree ) goto no_tree;
    
    // done
    return p_tree->pfn_size(p_tree->p_tree);

    // error handling
    {

        // argument errors
        {
            no_tree:
                #ifndef NDEBUG
                    printf("[tree] Null pointer provided for parameter \"p_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int tree_insert ( tree *p_tree, const void *const p_value )
{

    // argument check
    if ( NULL == p_tree ) goto no_tree;

    // done
    return p_tree->pfn_insert(p_tree->p_tree, p_value);

    // error handling
    {

        // argument errors
        {
            no_tree:
                #ifndef NDEBUG
                    printf("[tree] Null pointer provided for parameter \"p_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int tree_remove ( tree *const p_tree, const void *const p_key, const void **const pp_value )
{

    // argument check
    if ( NULL == p_tree ) goto no_tree;

    // done
    return p_tree->pfn_remove(p_tree->p_tree, p_key, pp_value);

    // error handling
    {

        // argument errors
        {
            no_tree:
                #ifndef NDEBUG
                    printf("[tree] Null pointer provided for parameter \"p_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int tree_traverse_inorder ( tree *const p_tree, fn_foreach *pfn_foreach )
{

    // argument check
    if ( NULL ==      p_tree ) goto no_tree;
    if ( NULL == pfn_foreach ) goto no_foreach_function;

    // done
    return p_tree->pfn_traverse_inorder(p_tree->p_tree, pfn_foreach);

    // error handling
    {

        // argument errors
        {
            no_tree:
                #ifndef NDEBUG
                    log_error("[tree] Null pointer provided for parameter \"p_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_foreach_function:
                #ifndef NDEBUG
                    log_error("[tree] Null pointer provided for parameter \"pfn_foreach\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int tree_pack ( void *p_buffer, tree *p_tree, fn_pack *pfn_element )
{
    
    // argument check
    if ( NULL ==    p_buffer ) goto no_buffer;
    if ( NULL ==      p_tree ) goto no_tree;
    if ( NULL == pfn_element ) goto no_pack;

    // initialized data 
    char *p = p_buffer;

    // pack the type
    p += pack_pack(p, "%i32", (int)p_tree->_type);

    // pack the tree
    p += p_tree->pfn_pack(p, p_tree->p_tree, pfn_element);

    // success
    return p - (char *)p_buffer;

    // error handling
    {
        
        // argument errors
        {

            no_buffer:
                #ifndef NDEBUG
                    log_error("[tree] Null pointer provided for \"p_buffer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_tree:
                #ifndef NDEBUG
                    log_error("[tree] Null pointer provided for \"p_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            no_pack:
                #ifndef NDEBUG
                    log_error("[tree] Null pointer provided for \"pfn_element\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int tree_unpack
( 
    tree **pp_tree, 
    void  *p_buffer, 
    
    fn_unpack       *pfn_element, 
    fn_comparator   *pfn_comparator, 
    fn_key_accessor *pfn_key_accessor
)
{
    
    // argument check
    if ( NULL ==     pp_tree ) goto no_tree;
    if ( NULL == pfn_element ) goto no_unpack;

    // initialized data 
    tree *p_tree           = NULL;
    char *p                = p_buffer;
    enum tree_type_e _type = -1;

    // allocate memory for a tree
    p_tree = default_allocator(0, sizeof(tree));
    if ( NULL == p_tree ) goto failed_to_allocate_tree;

    // unpack the type
    p += pack_unpack(p, "%i32", &_type);

    // validate the type
    if ( _type < 0 || _type >= TREE_QUANTITY ) goto invalid_tree_type;

    // clone the prototype
    memcpy(p_tree, &_prototypes[_type], sizeof(tree));

    // construct a tree
    p += _prototypes[_type].pfn_unpack(&p_tree->p_tree, p, pfn_element, pfn_comparator, pfn_key_accessor);

    // return a pointer to the caller
    *pp_tree = p_tree;

    // success
    return p - (char *)p_buffer;

    // error handling
    {
        
        // argument errors
        {
            no_tree:
                #ifndef NDEBUG
                    log_error("[tree] Null pointer provided for parameter \"pp_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            no_unpack:
                #ifndef NDEBUG
                    log_error("[tree] Null pointer provided for parameter \"pfn_element\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }


        // tree errors
        {
            failed_to_allocate_tree:
                #ifndef NDEBUG
                    printf("[tree] Failed to allocate tree in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            invalid_tree_type:
                #ifndef NDEBUG
                    printf("[tree] Invalid tree type in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // release the tree
                p_tree = default_allocator(p_tree, 0);

                // error
                return 0;
        }
    }
}

hash64 tree_hash ( tree *const p_tree, fn_hash64 *pfn_hash64 )
{

    // argument check
    if ( NULL == p_tree ) goto no_tree;

    // done
    return p_tree->pfn_hash(p_tree->p_tree, pfn_hash64);

    // error handling
    {

        // argument errors
        {
            no_tree:
                #ifndef NDEBUG
                    printf("[tree] Null pointer provided for parameter \"p_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int tree_destroy ( tree **const pp_tree, fn_allocator *pfn_allocator )
{

    // argument check
    if ( NULL == pp_tree ) goto no_tree;

    // initialized data
    tree *p_tree = *pp_tree;

    // fast exit
    if ( NULL == p_tree ) return 1;

    // no more pointer for caller
    *pp_tree = NULL;

    // strategy
    p_tree->pfn_destroy(&p_tree->p_tree, pfn_allocator);
    
    // release the tree
    p_tree = default_allocator(p_tree, 0);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_tree:
                #ifndef NDEBUG
                    printf("[tree] Null pointer provided for parameter \"pp_tree\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}
