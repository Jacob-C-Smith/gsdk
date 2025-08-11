#include <core/aes.h>

// Use the standard AES-128 test vector key (row-major order)
const unsigned char small_key[16] =
{
    0x2b, 0x7e, 0x15, 0x16, 
    0x28, 0xae, 0xd2, 0xa6, 
    0xab, 0xf7, 0x15, 0x88, 
    0x09, 0xcf, 0x4f, 0x3c
};

const unsigned char medium_key[24] =
{
    'a', 'b', 'c', 'd', 
    'e', 'f', 'g', 'h', 
    'i', 'j', 'k', 'l', 
    'm', 'n', 'o', 'p',
    'q', 'r', 's', 't', 
    'u', 'v', 'w', 'x',
};

const unsigned char large_key[32] =
{
    'a', 'b', 'c', 'd', 
    'e', 'f', 'g', 'h', 
    'i', 'j', 'k', 'l', 
    'm', 'n', 'o', 'p',
    'q', 'r', 's', 't', 
    'u', 'v', 'w', 'x',
    'y', 'z', '1', '2', 
    '3', '4', '5', '6'
};

char message[16] = 
{
    0x32, 0x43, 0xf6, 0xa8,
    0x88, 0x5a, 0x30, 0x8d,
    0x31, 0x31, 0x98, 0xa2,
    0xe0, 0x37, 0x07, 0x34
};

// entry point
int main ( int argc, const char *argv[] )
{
    
    // unused
    (void) argc;
    (void) argv;

    // initialized data
    char _iv[16] = { 0 };

    // construct an IV
    aes_iv_construct(_iv);

    // encrypt a block
    {

        // encrypt a message
        aes_encrypt_block(message, message, small_key);

        // write the encrypted block to a file
        {

            // initialized data
            FILE *f = fopen("resources/core/aes.enc", "wb");

            // write the file
            fwrite(message, 1, 32, f),
            fclose(f);
        }
    }

    // Print the IV
    printf("IV: ");
    for (size_t i = 0; i < sizeof(_iv); i++) 
        printf("%02x", (unsigned char)_iv[i]);
    printf("\n");

    // Success
    return EXIT_SUCCESS;
}