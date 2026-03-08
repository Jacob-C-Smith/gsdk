/** !
 * Bitmap implementation
 * 
 * @file src/data/bitmap/bitmap.c 
 * 
 * @author Jacob Smith
 */

// header
#include <data/bitmap.h>

// structure definitions
struct bitmap_s
{
    size_t   max;      // quantity of bits in the bitmap 
    mutex    _lock;    // lock
    void    *p_bitmap; // bitmap contents
};

/// constructors
int bitmap_construct ( bitmap **pp_bitmap, size_t bits )
{

    // argument check
    if ( NULL == pp_bitmap ) goto no_bitmap;
    if ( 0    == bits      ) goto no_bits;

    // initialized data
    bitmap *p_bitmap = NULL;
    size_t  bytes_required = (bits % 8 == 0) ? (bits / 8) : (bits / 8) + 1;
    
    // allocate memory for a bitmap
    p_bitmap = default_allocator(0, sizeof(bitmap));
    if ( NULL == p_bitmap ) goto no_mem;

    // store the quantity of bits
    p_bitmap->max = bits;

    // construct a lock
    mutex_create(&p_bitmap->_lock);

    // allocate memory for the bitmap
    p_bitmap->p_bitmap = default_allocator(0, bytes_required);

    // error check
    if ( NULL == p_bitmap->p_bitmap ) goto no_mem;

    // initialize
    memset(p_bitmap->p_bitmap, 0, bytes_required);

    // return a pointer to the caller
    *pp_bitmap = p_bitmap;
    
    // success
    return 1;

    // error handling
    {
        
        // argument errors
        {
            no_bitmap:
                #ifndef NDEBUG
                    log_error("[bitmap] Null pointer provided for parameter \"pp_bitmap\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            no_bits:
                
                #ifndef NDEBUG
                    log_error("[bitmap] Parameter \"bits\" must be greater than 0 in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[bitmap] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

/// mutators
int bitmap_set ( bitmap *p_bitmap, size_t index )
{
    
    // argument check
    if ( NULL == p_bitmap      ) goto no_bitmap;
    if ( index > p_bitmap->max ) goto out_of_bounds;

    // initialized data
    unsigned char *p      = NULL;
    size_t         offset = index / 8,
                   select = index % 8;

    // lock
    mutex_lock(&p_bitmap->_lock);

    // store a pointer to the bitmap
    p = p_bitmap->p_bitmap;

    // set the bit
    p[offset] |= ( 1 << select );

    // unlock
    mutex_unlock(&p_bitmap->_lock);

    // success
    return 1;

    // error handling
    {
        no_bitmap:
            #ifndef NDEBUG
                log_error("[bitmap] Null pointer provided for parameter \"p_bitmap\" in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // error
            return 0;

        out_of_bounds:
            #ifndef NDEBUG
                log_error("[bitmap] Parameter \"index\" is out of range [0, %d) in call to function \"%s\"\n", p_bitmap->max, __FUNCTION__);
            #endif

            // error
            return 0;
    }
}

int bitmap_clear ( bitmap *p_bitmap, size_t index )
{
    
    // argument check
    if ( NULL == p_bitmap      ) goto no_bitmap;
    if ( index > p_bitmap->max ) goto out_of_bounds;

    // initialized data
    unsigned char *p      = NULL;
    size_t         offset = index / 8,
                   select = index % 8;

    // lock
    mutex_lock(&p_bitmap->_lock);

    // store a pointer to the bitmap
    p = p_bitmap->p_bitmap;

    // clear
    p[offset] &= ~( 1 << select );

    // unlock
    mutex_unlock(&p_bitmap->_lock);

    // success
    return 0;

    // error handling
    {
        no_bitmap:
            #ifndef NDEBUG
                log_error("[bitmap] Null pointer provided for parameter \"p_bitmap\" in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // error
            return 0;

        out_of_bounds:
            #ifndef NDEBUG
                log_error("[bitmap] Parameter \"index\" is out of range [0, %d) in call to function \"%s\"\n", p_bitmap->max, __FUNCTION__);
            #endif

            // error
            return 0;
    }
}

/// accessors
int bitmap_test ( bitmap *p_bitmap, size_t index )
{
    
    // argument check
    if ( NULL == p_bitmap      ) goto no_bitmap;
    if ( index > p_bitmap->max ) goto out_of_bounds;

    // initialized data
    int            result = 0;
    unsigned char *p      = NULL;
    size_t         offset = index / 8,
                   select = index % 8;

    // lock
    mutex_lock(&p_bitmap->_lock);

    // store a pointer to the bitmap
    p = p_bitmap->p_bitmap;

    // test
    result = (p[offset] & ( 1 << select )) ? 1 : 0;

    // unlock
    mutex_unlock(&p_bitmap->_lock);

    // test the bit
    return result;

    // error handling
    {
        no_bitmap:
            #ifndef NDEBUG
                log_error("[bitmap] Null pointer provided for parameter \"p_bitmap\" in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // error
            return 0;

        out_of_bounds:
            #ifndef NDEBUG
                log_error("[bitmap] Parameter \"index\" is out of range [0, %d) in call to function \"%s\"\n", p_bitmap->max, __FUNCTION__);
            #endif

            // error
            return 0;
    }
}

/// logs
int bitmap_print ( bitmap *p_bitmap )
{
  
    // argument check
    if ( NULL == p_bitmap ) goto no_bitmap;

    // initialized data
    unsigned char *p = p_bitmap->p_bitmap;
    size_t bits_left = 0,
           i         = 0;

    // lock
    mutex_lock(&p_bitmap->_lock);

    // store the quantity of bits
    bits_left = p_bitmap->max;

    // walk the bitmap
    while ( bits_left != i )
    {

        // initialized data
        size_t offset = i / 8,
               select = i % 8;

        // print the bit
        printf("%c", (p[offset] & ( 1 << select )) ? '@' : '.'),
        
        // iterate
        i++;
    }
        
    // unlock
    mutex_unlock(&p_bitmap->_lock);

    // formatting
    putchar('\n');

    // success
    return 1;

    // error handling
    {
        no_bitmap:
            #ifndef NDEBUG
                log_error("[bitmap] Null pointer provided for parameter \"p_bitmap\" in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // error
            return 0;
    }
}

/// iterators
int bitmap_fori ( bitmap *p_bitmap, fn_fori *pfn_fori ) 
{

    // argument check
    if ( NULL == p_bitmap  ) goto no_bitmap;
    if ( NULL ==  pfn_fori ) goto no_fn_fori;

    // initialized data
    unsigned char *p = p_bitmap->p_bitmap;
    size_t bits_left = 0,
           i         = 0;

    // lock
    mutex_lock(&p_bitmap->_lock);

    // store the quantity of bits
    bits_left = p_bitmap->max;

    // walk the bitmap
    while ( bits_left != i )
    {

        // initialized data
        size_t offset = i / 8,
               select = i % 8;

        // call the fori function
        pfn_fori((void*)(size_t)(p[offset] & ( 1 << select )), i),
        
        // iterate
        i++;
    }  

    // unlock
    mutex_unlock(&p_bitmap->_lock);

    // success
    return 1;

    // error handling
    {
        
        // argument errors
        {
            no_bitmap:
                #ifndef NDEBUG
                    log_error("[bitmap] Null pointer provided for parameter \"p_bitmap\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            no_fn_fori:
                #ifndef NDEBUG
                    log_error("[bitmap] Null pointer provided for parameter \"pfn_fori\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int bitmap_foreach ( bitmap *p_bitmap, fn_foreach *pfn_foreach ) 
{

    // argument check
    if ( NULL ==    p_bitmap ) goto no_bitmap;
    if ( NULL == pfn_foreach ) goto no_fn_foreach;

    // initialized data
    unsigned char *p         = p_bitmap->p_bitmap;
    size_t         bits_left = 0,
                   i         = 0;

    // lock
    mutex_lock(&p_bitmap->_lock);

    // store the quantity of bits
    bits_left = p_bitmap->max;

    // walk the bitmap
    while ( bits_left != i )
    {

        // initialized data
        size_t offset = i / 8,
               select = i % 8;

        // call the foreach function
        pfn_foreach((void*)(size_t)(p[offset] & ( 1 << select ))),
        
        // iterate
        i++;
    }  

    // unlock
    mutex_unlock(&p_bitmap->_lock);

    // success
    return 1;

    // error handling
    {
        
        // argument errors
        {
            no_bitmap:
                #ifndef NDEBUG
                    log_error("[bitmap] Null pointer provided for parameter \"p_bitmap\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            no_fn_foreach:
                #ifndef NDEBUG
                    log_error("[bitmap] Null pointer provided for parameter \"pfn_foreach\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

/// reflection
int bitmap_pack ( void *p_buffer, bitmap *p_bitmap )
{
    
    // argument check
    if ( NULL == p_bitmap ) goto no_bitmap;
    if ( NULL == p_buffer ) goto no_buffer;

    // initialized data 
    char   *p              = p_buffer,
           *p_bytes        = p_bitmap->p_bitmap;
    size_t  bytes_required = 0;

    // lock
    mutex_lock(&p_bitmap->_lock);

    // compute the quantity of bytes required
    bytes_required = (p_bitmap->max % 8 == 0) ? 
                     (p_bitmap->max / 8)      :
                     (p_bitmap->max / 8) + 1;

    // pack the quantity of properties
    p += pack_pack(p, "%i64", p_bitmap->max);

    // iterate through the bitmap
    for (size_t i = 0; i < bytes_required; i++)
        p += pack_pack(p, "%i8", p_bytes[i]);
    
    // unlock
    mutex_unlock(&p_bitmap->_lock);

    // success
    return p - (char *)p_buffer;

    // error handling
    {
        
        // argument errors
        {
            no_bitmap:
                #ifndef NDEBUG
                    log_error("[bitmap] Null pointer provided for parameter \"p_bitmap\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            no_buffer:
                #ifndef NDEBUG
                    log_error("[bitmap] Null pointer provided for parameter \"p_buffer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int bitmap_unpack ( bitmap **pp_bitmap, void *p_buffer )
{
    
    // argument check
    if ( NULL == pp_bitmap ) goto no_bitmap;
    if ( NULL == p_buffer  ) goto no_buffer;

    // initialized data
    bitmap *p_bitmap       = NULL;
    char   *p              = p_buffer;
    size_t  len            = 0,
            bytes_required = 0;
            
    // unpack the length
    p += pack_unpack(p, "%i64", &len);

    // construct an bitmap
    bitmap_construct(&p_bitmap, len);

    // compute the required quantity of bytes
    bytes_required = (p_bitmap->max % 8 == 0) ? 
                     (p_bitmap->max / 8)      :
                     (p_bitmap->max / 8) + 1  ;

    // iterate through the bitmap
    for (size_t i = 0; i < bytes_required; i++)

        // unpack 8 bits at a time
        p += pack_unpack(p, "%i8", &((char *)p_bitmap->p_bitmap)[i]);
    
    // return the bitmap to the caller
    *pp_bitmap = p_bitmap;

    // success
    return p - (char *)p_buffer;
    
    // error handling
    {
        
        // argument errors
        {
            no_bitmap:
                #ifndef NDEBUG
                    log_error("[bitmap] Null pointer provided for parameter \"pp_bitmap\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_buffer:
                #ifndef NDEBUG
                    log_error("[bitmap] Null pointer provided for parameter \"p_buffer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

/// hash
hash64 bitmap_hash ( bitmap *p_bitmap, fn_hash64 *pfn_hash64 )
{

    // argument check
    if ( NULL == p_bitmap ) goto no_bitmap;

    // initialized data
    hash64 result = 0;
    size_t bytes_required = 0;

    // lock
    mutex_lock(&p_bitmap->_lock);

    // compute the quantity of bytes to hash
    bytes_required = (p_bitmap->max % 8 == 0) ? 
                     (p_bitmap->max / 8) :
                     (p_bitmap->max / 8) + 1;

    // compute the hash of the bitmap
    result = (pfn_hash64) ? pfn_hash64(p_bitmap->p_bitmap, bytes_required) : default_hash(p_bitmap->p_bitmap, bytes_required);

    // unlock
    mutex_unlock(&p_bitmap->_lock);

    // success
    return result;

    // error handling
    {

        // argument errors
        {
            no_bitmap:
                #ifndef NDEBUG
                    log_error("[bitmap] Null pointer provided for parameter \"p_bitmap\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

/// destructor
int bitmap_destroy ( bitmap **pp_bitmap )
{

    // argument check
    if ( NULL == pp_bitmap ) goto no_bitmap;

    // initialized data
    bitmap *p_bitmap = *pp_bitmap;

    // lock
    mutex_lock(&p_bitmap->_lock);

    // no more pointer for caller
    *pp_bitmap = NULL;

    // unlock
    mutex_unlock(&p_bitmap->_lock);

    // release the bits
    p_bitmap->p_bitmap = default_allocator(p_bitmap->p_bitmap, 0),

    // release the bitmap
    p_bitmap = default_allocator(p_bitmap, 0);
    
    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_bitmap:
                #ifndef NDEBUG
                    log_error("[bitmap] Null pointer provided for parameter \"pp_bitmap\" in call to function \"%s\"\n");
                #endif

                // error
                return 0;
        }
    }
}
