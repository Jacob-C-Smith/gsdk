/** ! 
 * RSA (en/de)cryption with C23 BitInt
 * 
 * @file RSA.c
 * 
 * @author Jacob Smith
 */

// standard library
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <time.h>

// core
#include <core/log.h>
#include <core/rsa.h>

// preprocessor definitions
#define RED "\033[91m"
#define BLUE "\033[94m"
#define RESET "\033[0m"

size_t file_load ( const char *path, void *buffer, bool binary_mode );

// Data
i2048 primes[] =
{
    2WB  , 3WB  , 5WB  , 7WB  , 11WB , 13WB , 17WB , 19WB , 23WB , 29WB , 31WB ,
    37WB , 41WB , 43WB , 47WB , 53WB , 59WB , 61WB , 67WB , 71WB , 73WB , 79WB ,
    83WB , 89WB , 97WB , 101WB, 103WB, 107WB, 109WB, 113WB, 127WB, 131WB, 137WB,
    139WB, 149WB, 151WB, 157WB, 163WB, 167WB, 173WB, 179WB, 181WB, 191WB, 193WB,
    197WB, 199WB, 211WB, 223WB, 227WB, 229WB, 233WB, 239WB, 241WB, 251WB, 257WB,
    263WB, 269WB, 271WB, 277WB, 281WB, 283WB, 293WB, 307WB, 311WB, 313WB, 317WB,
    331WB, 337WB, 347WB, 349WB, 353WB, 359WB, 367WB, 373WB, 379WB, 383WB, 389WB,
    397WB, 401WB, 409WB, 419WB, 421WB, 431WB, 433WB, 439WB, 443WB, 449WB, 457WB,
    461WB, 463WB, 467WB, 479WB, 487WB, 491WB, 499WB, 503WB, 509WB, 521WB, 523WB,
    541WB
};

size_t len_primes = sizeof(primes)/sizeof(*primes);

size_t req_bits ( i2048 a );

i2048 egcd ( i2048 a, i2048 b, i2048 *x, i2048 *y );

i2048 mod_exp(i2048 x, i2048 a, i2048 n );

i2048 random_n_bit_int ( size_t bits );

bool is_divisible_by_small_primes ( i2048 n )
{

    // Iterate through some small prime numbers
    for (size_t i = 0; i < len_primes; i++)

        // Test
        if ( n % primes[i] == 0 ) return true;

    // done
    return false;
}

bool miller_rabin_iteration ( i2048 n )
{

    // Fast exit
    if ( is_divisible_by_small_primes(n) )
        return (n <= primes[len_primes - 1]);

    // initialized data
    i2048 a = 2 + random_n_bit_int(1024) % (n - 4);
    i2048 e = n - 1;
    i2048 first_check = mod_exp(a, e, n);
    bool prev = true;

    //printf("Testing a = %llu, mod_exp(a, n-1, n) = %llu\n", (unsigned long long)a, (unsigned long long)first_check);fflush(stdout);

    // error check
    if ( 1 != first_check ) return false;

    // Miller-Rabin inner loop
    while ( e % 2 == 0 )
    {   

        // initialized data
        i2048 x = 0;

        // Compute x
        e >>= 1,
        x = mod_exp(a, e, n);
        
        //printf(" + mod_exp(a, e, n) = %llu\n", (unsigned long long)x); fflush(stdout);
        
        
        if ( prev )
        {
            if      ( x == n - 1 ) prev = false;
            else if ( x != 1 )     return false;
        }
    }
    return true;
}

bool miller_rabin ( i2048 n, i2048 k )
{
    for (size_t i = 0; i < k; i++)
        if ( !miller_rabin_iteration(n) )
            return false;

    return true;
}

i2048 generate_random ( int num_digits )
{
    i2048 min_n_digit_number = 1;
    i2048 max_n_digit_number = 1;

    for (int i = 1; i < num_digits; i++)
        min_n_digit_number *= 10,
        max_n_digit_number *= 10;

    max_n_digit_number *= 10;

    while ( true )
    {
        i2048 n = random_n_bit_int(1024);
        n = min_n_digit_number + n % (max_n_digit_number - min_n_digit_number);

        // Reject trivial bad cases early
        if (n % 2 == 0 || n % 5 == 0)
            continue;

        int r = n % 10;
        if ( r == 1 || r == 3 || r == 7 || r == 9 ) {
            return n;
        }
    }
}

i2048 generate_prime ( int num_digits, int k )
{

    // initialized data
    int i = 0;

    // Attempt
    while (i < 10000)
    {

        // initialized data
        i2048 n = generate_random(num_digits);

        // Increment attempt
        i++;

        // Verbose
        // printf("iteration %d (expected %2.2lf total)\n", i, num_digits * 2.3);fflush(stdout);
        
        // Test
        if ( miller_rabin(n, k) )

            // success
            return n;
    }

    // error
    return 0;
}

int key_pair_construct ( public_key **pp_public_key, private_key **pp_private_key )
{

    // argument check
    if ( NULL == pp_public_key )  goto no_public_key;
    if ( NULL == pp_private_key ) goto no_private_key;

    // initialized data
    public_key  *p_public_key  = default_allocator(0, sizeof(public_key));
    private_key *p_private_key = default_allocator(0, sizeof(private_key));
    i2048 n = 0, a = 0,
          p = 0, q = 0, b = 0;

    // Seed the RNG
    srand(time(0));

    // (Not) random primes
    p = generate_prime(133, 5); 
    q = generate_prime(133, 5); 

    // Store the product of the prime factors in the public key
    n = p * q;

    // Public
    {

        // initialized data
        i2048 euler_totient = (p - 1) * (q - 1),
              gcd           = 0,
              x             = 0,
              y             = 0;
              
        size_t bits = req_bits(euler_totient);
        
        // 1000 attempts to generate a
        for (size_t i = 0; i < 1000; i++)
        {

            // Generate a random number A
            a = random_n_bit_int(bits);

            // Fast exit
            if ( a <= 1 || a >= euler_totient) continue;
            
            // Clear
            x = y = 0;

            // Compute the GCD 
            gcd = egcd(a, euler_totient, &x, &y);

            // This value of A works :D 
            if ( gcd == 1 ) break;
        }
    }

    // Private
    {
        
        // initialized data
        i2048 phi = (p - 1) * (q - 1);
        i2048 gcd = 0;
        i2048 y   = 0;
        
        // Compute the GCD, storing a factor as B
        gcd = egcd(a, phi, &b, &y);

        // Not acceptable
        if ( abs((int)gcd) != 1 ) return EXIT_FAILURE;

        // Posative mod
        if ( b < 0 ) b = b + phi;
    }

    // Populate the public and private keys
    *p_public_key = (public_key)
    {
        .a = a,
        .n = n
    },
    *p_private_key = (private_key)
    {
        .p = p,
        .q = q,
        .b = b
    },

    // Return pointers to the caller
    *pp_private_key = p_private_key,
    *pp_public_key  = p_public_key;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_public_key:
                #ifndef NDEBUG
                    log_error("[rsa] Null pointer provided for parameter \"pp_public_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_private_key:
                #ifndef NDEBUG
                    log_error("[rsa] Null pointer provided for parameter \"pp_private_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int key_pair_from_files
(
    public_key  **pp_public_key,
    private_key **pp_private_key,
    const char   *p_public_key_path,
    const char   *p_private_key_path 
)
{
    
    // argument check
    if ( NULL == pp_public_key      ) goto no_public_key;
    if ( NULL == pp_private_key     ) goto no_private_key;
    if ( NULL == p_public_key_path  ) goto no_public_key_path;
    if ( NULL == p_private_key_path ) goto no_private_key_path;

    // initialized data
    public_key   *p_public_key         = default_allocator(0, sizeof(public_key));
    private_key  *p_private_key        = default_allocator(0, sizeof(private_key));
    size_t        public_key_size      = file_load(p_public_key_path, 0, true),
                  private_key_size     = file_load(p_private_key_path, 0, true);
    void         *p_public_key_buffer  = default_allocator(0, public_key_size),
                 *p_private_key_buffer = default_allocator(0, private_key_size);

    // error check
    if ( NULL == p_public_key         ) goto no_mem;
    if ( NULL == p_private_key        ) goto no_mem;
    if ( NULL == p_public_key_buffer  ) goto no_mem;
    if ( NULL == p_private_key_buffer ) goto no_mem;
    
    // Load the public and private keys
    if ( 0 == file_load(p_public_key_path , p_public_key_buffer , true) ) goto failed_to_load_public_key;
    if ( 0 == file_load(p_private_key_path, p_private_key_buffer, true) ) goto failed_to_load_private_key;

    // Construct the public key
    if ( 0 == public_key_unpack(p_public_key, p_public_key_buffer)    ) goto failed_to_parse_public_key;
    if ( 0 == private_key_unpack(p_private_key, p_private_key_buffer) ) goto failed_to_parse_private_key;

    // Release buffers
    p_public_key_buffer  = default_allocator(p_public_key_buffer, 0),
    p_private_key_buffer = default_allocator(p_private_key_buffer, 0);

    // Return pointers to the caller
    *pp_public_key  = p_public_key,
    *pp_private_key = p_private_key;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_public_key:
                #ifndef NDEBUG
                    log_error("[rsa] Null pointer provided for parameter \"pp_public_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_private_key:
                #ifndef NDEBUG
                    log_error("[rsa] Null pointer provided for parameter \"pp_private_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_public_key_path:
                #ifndef NDEBUG
                    log_error("[rsa] Null pointer provided for parameter \"p_public_key_path\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_private_key_path:
                #ifndef NDEBUG
                    printf("[rsa] Null pointer provided for parameter \"p_private_key_path\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
        
        // pack errors
        {
            failed_to_parse_public_key:
                #ifndef NDEBUG
                    log_error("[rsa] Failed to parse public key in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            failed_to_parse_private_key:
                #ifndef NDEBUG
                    log_error("[rsa] Failed to parse private key in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            failed_to_load_public_key:
                #ifndef NDEBUG
                    log_error("[rsa] Failed to load public key file \"%s\" in call to function \"%s\"\n", p_public_key_path, __FUNCTION__);
                #endif

                // error
                return 0;

            failed_to_load_private_key:
                #ifndef NDEBUG
                    log_error("[rsa] Failed to load private key file \"%s\" in call to function \"%s\"\n", p_private_key_path, __FUNCTION__);
                #endif

                // error
                return 0;

            no_mem:
                #ifndef NDEBUG
                    log_error("[hash cache] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

size_t req_bits ( i2048 n )
{

    // Special case
    if ( n == 0 ) return 1;

    // initialized data
    size_t ret = 0;

    // Right shift until n == 0 
    while ( n > 0 )
        n >>= 1, ret++;

    // success
    return ret;
}

i2048 egcd ( i2048 a, i2048 b, i2048 *x, i2048 *y )
{

    // Base case
    if ( b == 0 )
    {

        // Store x and y
        *x = 1, *y = 0;

        // done
        return a;
    }

    // initialized data
    i2048 x1, y1;
    i2048 gcd = egcd(b, a % b, &x1, &y1);

    // Store x and y
    *x = y1,
    *y = x1 - ( a / b ) * y1;

    // done
    return gcd;
}

i2048 mod_exp ( i2048 base, i2048 exp, i2048 mod )
{

    // initialized data
    i2048 result = 1;

    // Modulate
    base %= mod;

    // Exponent
    while (exp > 0)
    {
        if (exp & 1) result = (result * base) % mod;
        exp >>= 1,
        base = (base * base) % mod;
    }

    // success
    return result;
}

i2048 enc_block ( i2048 x, public_key *p_public_key )
{
    
    // done
    return mod_exp(x, p_public_key->a, p_public_key->n);
}

i2048 dec_block ( i2048 y, public_key *p_public_key, private_key *p_private_key )
{

    // done
    return mod_exp(y, p_private_key->b, p_public_key->n);
}

int enc ( void *p_x, void *p_y, public_key *p_public_key )
{

    // initialized data
    i2048 x = *(i2048 *)p_x,
          y = mod_exp(x, p_public_key->a, p_public_key->n);

    // Store the result
    *(i2048 *)p_y = y;
        
    // done
    return 1;
}

int dec ( void *p_y, void *p_z, public_key *p_public_key, private_key *p_private_key )
{

    // initialized data
    i2048 y = *(i2048 *)p_y,
          z = mod_exp(y, p_private_key->b, p_public_key->n);

    // Store the result
    *(i2048 *)p_z = z;
    
    // done
    return 1;
}

int print_n_bit_int ( i2048 a )
{ 

    // initialized data
    size_t bits = 0;
    void *p_int = &a;

    // Compute how many bits are required to represent a
    bits = req_bits(a) + 32;
    
    // Formatting
    putchar('0'), putchar('x');
  
    for (signed i = (bits >> 5) - 1; i >= 0; i--)
        printf("%08x", ((unsigned int *) p_int)[i]);

    // done
    return 1;
}

i2048 random_n_bit_int ( size_t bits )
{

    // initialized data
    i2048 result = 0;
    size_t bytes = (bits + 7) / 8;
    FILE *urandom = fopen("/dev/urandom", "rb");

    // error check
    if ( urandom == NULL ) goto no_random;

    // Read random
    fread(&result, bytes, 1, urandom),
    fclose(urandom);

    // Ensure result is within the desired bit range
    if (bits < 1024)
        result &= ((i2048) 1 << bits) - 1;

    // success
    return result;

    no_random:
        return 0;
}

int print_public_key ( public_key *p_public_key )
{

    // Print the public key
    printf("\n" BLUE "PUBLIC KEY" RESET ":\n"),
    
    printf(" n = " BLUE),
    print_n_bit_int(p_public_key->n),

    printf("\n" RESET " a = " BLUE),
    print_n_bit_int(p_public_key->a),
    printf(RESET "\n");

    // success
    return 1;
}

int print_private_key ( private_key *p_private_key )
{
    
    // Print the private key
    printf(RED "PRIVATE KEY" RESET ":\n"),

    printf(" p = " RED),
    print_n_bit_int(p_private_key->p),

    printf("\n" RESET " q = " RED),
    print_n_bit_int(p_private_key->q),

    printf("\n" RESET " b = " RED),
    print_n_bit_int(p_private_key->b);
    
    printf(RESET "\n");
    
    // success
    return 1;
}

int public_key_pack ( void *p_buffer, public_key *p_public_key )
{
    
    // initialized data
    char               *p   = p_buffer;
    unsigned long long *p_a = (unsigned long long *) &p_public_key->a;
    unsigned long long *p_n = (unsigned long long *) &p_public_key->n;

    // pack n
    p += pack_pack(
        p, "%s%32i64",
        "n",
        p_n[0] , p_n[1] , p_n[2] , p_n[3],
        p_n[4] , p_n[5] , p_n[6] , p_n[7],
        p_n[8] , p_n[9] , p_n[10], p_n[11],
        p_n[12], p_n[13], p_n[14], p_n[15],
        p_n[16], p_n[17], p_n[18], p_n[19],
        p_n[20], p_n[21], p_n[22], p_n[23],
        p_n[24], p_n[25], p_n[26], p_n[27],
        p_n[28], p_n[29], p_n[30], p_n[31]
    );

    // pack a
    p += pack_pack(
        p, "%s%32i64",
        "a",
        p_a[0] , p_a[1] , p_a[2] , p_a[3],
        p_a[4] , p_a[5] , p_a[6] , p_a[7],
        p_a[8] , p_a[9] , p_a[10], p_a[11],
        p_a[12], p_a[13], p_a[14], p_a[15],
        p_a[16], p_a[17], p_a[18], p_a[19],
        p_a[20], p_a[21], p_a[22], p_a[23],
        p_a[24], p_a[25], p_a[26], p_a[27],
        p_a[28], p_a[29], p_a[30], p_a[31]
    );

    // success
    return p - (char*) p_buffer;
}

int private_key_pack ( void *p_buffer, private_key *p_private_key )
{
    
    // initialized data
    char               *p   = p_buffer;
    unsigned long long *p_p = (unsigned long long *) &p_private_key->p;
    unsigned long long *p_q = (unsigned long long *) &p_private_key->q;
    unsigned long long *p_b = (unsigned long long *) &p_private_key->b;

    // pack p
    p += pack_pack(
        p, "%s%32i64",
        "p",
        p_p[0] , p_p[1] , p_p[2] , p_p[3],
        p_p[4] , p_p[5] , p_p[6] , p_p[7],
        p_p[8] , p_p[9] , p_p[10], p_p[11],
        p_p[12], p_p[13], p_p[14], p_p[15],
        p_p[16], p_p[17], p_p[18], p_p[19],
        p_p[20], p_p[21], p_p[22], p_p[23],
        p_p[24], p_p[25], p_p[26], p_p[27],
        p_p[28], p_p[29], p_p[30], p_p[31]
    );

    // pack q
    p += pack_pack(
        p, "%s%32i64",
        "q",
        p_q[0] , p_q[1] , p_q[2] , p_q[3],
        p_q[4] , p_q[5] , p_q[6] , p_q[7],
        p_q[8] , p_q[9] , p_q[10], p_q[11],
        p_q[12], p_q[13], p_q[14], p_q[15],
        p_q[16], p_q[17], p_q[18], p_q[19],
        p_q[20], p_q[21], p_q[22], p_q[23],
        p_q[24], p_q[25], p_q[26], p_q[27],
        p_q[28], p_q[29], p_q[30], p_q[31]
    );

    // pack b
    p += pack_pack(
        p, "%s%32i64",
        "b",
        p_b[0] , p_b[1] , p_b[2] , p_b[3],
        p_b[4] , p_b[5] , p_b[6] , p_b[7],
        p_b[8] , p_b[9] , p_b[10], p_b[11],
        p_b[12], p_b[13], p_b[14], p_b[15],
        p_b[16], p_b[17], p_b[18], p_b[19],
        p_b[20], p_b[21], p_b[22], p_b[23],
        p_b[24], p_b[25], p_b[26], p_b[27],
        p_b[28], p_b[29], p_b[30], p_b[31]
    );

    // success
    return p - (char *)p_buffer;
}

int public_key_unpack ( public_key *p_public_key, void *p_buffer )
{
 
    // initialized data
    char               *p   = p_buffer;
    unsigned long long *p_a = (unsigned long long *) &p_public_key->a;
    unsigned long long *p_n = (unsigned long long *) &p_public_key->n;
    char buf[64] = { 0 };

    // unpack n
    p += pack_unpack(
        p, "%s%32i64",
        buf,
        &p_n[0] , &p_n[1] , &p_n[2] , &p_n[3],
        &p_n[4] , &p_n[5] , &p_n[6] , &p_n[7],
        &p_n[8] , &p_n[9] , &p_n[10], &p_n[11],
        &p_n[12], &p_n[13], &p_n[14], &p_n[15],
        &p_n[16], &p_n[17], &p_n[18], &p_n[19],
        &p_n[20], &p_n[21], &p_n[22], &p_n[23],
        &p_n[24], &p_n[25], &p_n[26], &p_n[27],
        &p_n[28], &p_n[29], &p_n[30], &p_n[31]
    );

    // error check
    if ( 0 != strncmp("n", buf, 2) ) goto no_public_n;

    // unpack a
    p += pack_unpack(
        p, "%s%32i64",
        buf,
        &p_a[0] , &p_a[1] , &p_a[2] , &p_a[3],
        &p_a[4] , &p_a[5] , &p_a[6] , &p_a[7],
        &p_a[8] , &p_a[9] , &p_a[10], &p_a[11],
        &p_a[12], &p_a[13], &p_a[14], &p_a[15],
        &p_a[16], &p_a[17], &p_a[18], &p_a[19],
        &p_a[20], &p_a[21], &p_a[22], &p_a[23],
        &p_a[24], &p_a[25], &p_a[26], &p_a[27],
        &p_a[28], &p_a[29], &p_a[30], &p_a[31]
    );

    // error check
    if ( 0 != strncmp("a", buf, 2) ) goto no_public_a;

    // success
    return 1;

    // error handling
    {

        // format errors
        {
            no_public_n:
                #ifndef NDEBUG
                    log_error("[rsa] Failed to unpack \"n\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_public_a:
                #ifndef NDEBUG
                    log_error("[rsa] Failed to unpack \"a\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int private_key_unpack ( private_key *p_private_key, void *p_buffer )
{
 
    // initialized data
    char          *p                 = p_buffer;
    unsigned long long *p_p          = (unsigned long long *) &p_private_key->p;
    unsigned long long *p_q          = (unsigned long long *) &p_private_key->q;
    unsigned long long *p_b          = (unsigned long long *) &p_private_key->b;
    char buf[64] = { 0 };

    // unpack p
    p += pack_unpack(
        p, "%s%32i64",
        buf,
        &p_p[0] , &p_p[1] , &p_p[2] , &p_p[3],
        &p_p[4] , &p_p[5] , &p_p[6] , &p_p[7],
        &p_p[8] , &p_p[9] , &p_p[10], &p_p[11],
        &p_p[12], &p_p[13], &p_p[14], &p_p[15],
        &p_p[16], &p_p[17], &p_p[18], &p_p[19],
        &p_p[20], &p_p[21], &p_p[22], &p_p[23],
        &p_p[24], &p_p[25], &p_p[26], &p_p[27],
        &p_p[28], &p_p[29], &p_p[30], &p_p[31]
    );

    // error check
    if ( 0 != strncmp("p", buf, 2) ) goto no_private_p;

    // unpack q
    p += pack_unpack(
        p, "%s%32i64",
        buf,
        &p_q[0] , &p_q[1] , &p_q[2] , &p_q[3],
        &p_q[4] , &p_q[5] , &p_q[6] , &p_q[7],
        &p_q[8] , &p_q[9] , &p_q[10], &p_q[11],
        &p_q[12], &p_q[13], &p_q[14], &p_q[15],
        &p_q[16], &p_q[17], &p_q[18], &p_q[19],
        &p_q[20], &p_q[21], &p_q[22], &p_q[23],
        &p_q[24], &p_q[25], &p_q[26], &p_q[27],
        &p_q[28], &p_q[29], &p_q[30], &p_q[31]
    );

    // error check
    if ( 0 != strncmp("q", buf, 2) ) goto no_private_q;

    // unpack b
    p += pack_unpack(
        p, "%s%32i64",
        buf,
        &p_b[0] , &p_b[1] , &p_b[2] , &p_b[3],
        &p_b[4] , &p_b[5] , &p_b[6] , &p_b[7],
        &p_b[8] , &p_b[9] , &p_b[10], &p_b[11],
        &p_b[12], &p_b[13], &p_b[14], &p_b[15],
        &p_b[16], &p_b[17], &p_b[18], &p_b[19],
        &p_b[20], &p_b[21], &p_b[22], &p_b[23],
        &p_b[24], &p_b[25], &p_b[26], &p_b[27],
        &p_b[28], &p_b[29], &p_b[30], &p_b[31]
    );

    // error check
    if ( 0 != strncmp("b", buf, 2) ) goto no_private_b;

    // success
    return 1;

    // error handling
    {

        // format errors
        {
            no_private_p:
                #ifndef NDEBUG
                    log_error("[rsa] Failed to unpack \"p\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_private_q:
                #ifndef NDEBUG
                    log_error("[rsa] Failed to unpack \"q\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_private_b:
                #ifndef NDEBUG
                    log_error("[rsa] Failed to unpack \"b\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

size_t file_load ( const char *path, void *buffer, bool binary_mode )
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
    ret = (size_t) ftell(f);
    fseek(f, 0, SEEK_SET);
    
    // Read to data
    if ( buffer ) 
        ret = fread(buffer, 1, ret, f);

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
                    log_error("[rsa] Null path provided to function \"%s\n", __FUNCTION__);
                #endif

            // error
            return 0;
        }

        // File errors
        {
            invalid_file:
                #ifndef NDEBUG
                    log_error("[Standard library] Failed to load file \"%s\". %s\n", path, strerror(errno));
                #endif

            // error
            return 0;
        }
    }
}
