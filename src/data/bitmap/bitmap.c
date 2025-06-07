/** !
 * bitmap library
 * 
 * @file bitmap.c 
 * 
 * @author Jacob Smith
 */

// Header
#include <data/bitmap.h>

// Structure definitions
struct bitmap_s
{
    size_t   max;      // Quantity of bits in the bitmap 
    mutex    _lock;    // Locked when writing values
    void    *p_bitmap; // Bitmap contents
};

// Data
static bool initialized = false;

void bitmap_init ( void ) 
{

    // State check
    if ( initialized == true ) return;

    // Initialize the sync library
    sync_init();
    
    // Initialize the log library
    log_init();

    // Set the initialized flag
    initialized = true;

    // Done
    return; 
}

int bitmap_construct ( bitmap **pp_bitmap, size_t bits )
{
    
    // Stub
    return 0;
}

void bitmap_set ( bitmap *p_bitmap, size_t index )
{
    
    // Stub
    return 0;
}

void bitmap_clear ( bitmap *p_bitmap, size_t index )
{
    
    // Stub
    return 0;
}

int bitmap_test ( bitmap *p_bitmap, size_t index )
{
    
    // Stub
    return 0;
}

void bitmap_destroy ( bitmap **pp_bitmap )
{
    
    // Stub
    return 0;
}


void bitmap_exit ( void ) 
{

    // State check
    if ( initialized == false ) return;

    // Clean up sync
    sync_exit();

    // Clean up log
    log_exit();

    // Clear the initialized flag
    initialized = false;

    // Done
    return;
}

/*
size_t bmbits ( BM b )
{
    
    // Initialized data
    size_t *bits = b;
    
    // Done
    return *--bits;
}

size_t bmbytes ( BM b )
{

    // Done
    return bits2bytes( bmbits(b) );
}

void ok ( BM b, size_t i )
{

    // Ensure that the index is not greater than the size of the 
    if ( i < bmbits(b) ) return;

    // Write a message to standard error
    fprintf(stderr, "bitmap index out of range\n");
    
    // Abort
    exit(EXIT_FAILURE);
}         

BM bmcreate ( size_t bits )
{
  
    // Initialized data
    size_t  bytes = bits2bytes(bits);
    size_t *p     = mmalloc( sizeof(size_t) + bytes );
    BM      b     = (void *) 0;

    // Error check
    if ( (long)p == -1 ) return 0;
    
    // Store the quantity of bits in the allocation
    *p = bits;

    // Increment the pointer by 8 bytes, such that the pointer 
    // that is returned to the caller points to the bitmap itself.
    // Thus, the metadata is stored behind the bitmap.
    b = ++p;
    
    // Clear the bitmap 
    memset(b,0,bytes);
    
    // Success
    return b;
}

void bmdelete ( BM b )
{
    
    // Initialized data
    size_t *p = b;
    
    // Decrement the pointer by 8 bytes, such that the pointer
    // points to the memory returned by malloc. 
    p--;
    
    // Free the bitmap itself
    mmfree(p, *p);

    // Done
    return;
}

void bmset ( BM b, size_t i )
{
    
    ok(b,i);

    bitset( b + i / bitsperbyte, i % bitsperbyte );
}

void bmclr ( BM b, size_t i )
{

    ok(b,i);
    
    bitclr( b + i / bitsperbyte, i % bitsperbyte );
}

int bmtst ( BM b, size_t i )
{
    ok(b,i);

    return bittst( b + i / bitsperbyte, i % bitsperbyte );
}

void bmprt ( BM b )
{

    for (int byte = bmbytes(b) - 1; byte >= 0; byte--)

        printf("%02x%s", ((char *)b)[byte], (byte ? " " : "\n"));
}

void bmprtbin ( BM b, size_t e )
{
  
    // Initialized data
    size_t bits_left = bmbits(b);
    size_t i = 0;

    while(bits_left != i)
    {
        printf("%c",bmtst(b, i) ? '@' : '.'); 
        for (size_t j = (1 << e) - 1; j > 0; j--) putchar(' ');
        i++;
    }

    putchar('\n');

    // Done
    return;
}
*/