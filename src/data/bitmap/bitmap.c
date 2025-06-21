/** !
 * bitmap library
 * 
 * @file bitmap.c 
 * 
 * @author Jacob Smith
 */

// header
#include <data/bitmap.h>

// core
#include <core/log.h>
#include <core/sync.h>
#include <core/pack.h>

// structure definitions
struct bitmap_s
{
    size_t   max;      // Quantity of bits in the bitmap 
    mutex    _lock;    // locked when writing values
    void    *p_bitmap; // Bitmap contents
};

int bitmap_construct ( bitmap **pp_bitmap, size_t bits )
{

    // argument check
    if ( NULL == pp_bitmap ) goto no_bitmap;
    if ( 0    == bits      ) goto no_bits;

    // initialized data
    bitmap *p_bitmap = realloc(0, sizeof(bitmap));
    size_t  bytes_required = (bits % 8 == 0) ? (bits / 8) : (bits / 8) + 1;
    
    // error check
    if ( NULL == p_bitmap ) goto no_mem;

    // store the quantity of bits
    p_bitmap->max = bits;

    // allocate a lock
    mutex_create(&p_bitmap->_lock);

    // allocate memory for the bitmap
    p_bitmap->p_bitmap = realloc(0, bytes_required);

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

int bitmap_set ( bitmap *p_bitmap, size_t index )
{
    
    // argument check
    if ( NULL == p_bitmap      ) goto no_bitmap;
    if ( index > p_bitmap->max ) goto out_of_bounds;

    // initialized data
    unsigned char *p      = p_bitmap->p_bitmap;
    size_t         offset = index / 8,
                   select = index % 8;

    // lock
    mutex_lock(&p_bitmap->_lock);

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
    unsigned char *p      = p_bitmap->p_bitmap;
    size_t         offset = index / 8,
                   select = index % 8;

    // lock
    mutex_lock(&p_bitmap->_lock);

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

int bitmap_test ( bitmap *p_bitmap, size_t index )
{
    
    // argument check
    if ( NULL == p_bitmap      ) goto no_bitmap;
    if ( index > p_bitmap->max ) goto out_of_bounds;

    // initialized data
    int            result = 0;
    unsigned char *p      = p_bitmap->p_bitmap;
    size_t         offset = index / 8,
                   select = index % 8;

    // lock
    mutex_lock(&p_bitmap->_lock);

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

int bitmap_print ( bitmap *p_bitmap )
{
  
    // argument check
    if ( NULL == p_bitmap ) goto no_bitmap;

    // initialized data
    unsigned char *p = p_bitmap->p_bitmap;
    size_t bits_left = p_bitmap->max,
           i         = 0;

    // lock
    mutex_lock(&p_bitmap->_lock);

    // walk the bitmap
    while(bits_left != i)
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


int bitmap_pack ( void *p_buffer, bitmap *p_bitmap )
{
    
    // argument check
    if ( NULL == p_bitmap ) goto no_bitmap;
    if ( NULL == p_buffer ) goto no_buffer;

    // initialized data 
    char   *p              = p_buffer,
           *p_bytes        = p_bitmap->p_bitmap;
    size_t  bytes_required = (p_bitmap->max % 8 == 0) ? 
                             (p_bitmap->max / 8) :
                             (p_bitmap->max / 8) + 1;

    // lock
    mutex_lock(&p_bitmap->_lock);

    // Pack the quantity of properties
    p += pack_pack(p, "%i64", p_bitmap->max);

    // Iterate through the bitmap
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
                    log_error("[bitmap] Null pointer provided for \"p_bitmap\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            no_buffer:
                #ifndef NDEBUG
                    log_error("[bitmap] Null pointer provided for \"p_buffer\" in call to function \"%s\"\n", __FUNCTION__);
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
    
    // Return the bitmap to the caller
    *pp_bitmap = p_bitmap;

    // success
    return p - (char *)p_buffer;
    
    // error handling
    {
        
        // argument errors
        {
            no_bitmap:
                #ifndef NDEBUG
                    log_error("[bitmap] Null pointer provided for \"p_bitmap\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_buffer:
                #ifndef NDEBUG
                    log_error("[bitmap] Null pointer provided for \"p_buffer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int bitmap_destroy ( bitmap **pp_bitmap )
{

    // initialized data
    bitmap *p_bitmap = *pp_bitmap;

    // no more pointer for caller
    *pp_bitmap = NULL;

    // release the bits
    p_bitmap->p_bitmap = realloc(p_bitmap->p_bitmap, 0),

    // release the bitmap
    p_bitmap = realloc(p_bitmap, 0);
    
    // success
    return 1;
}
