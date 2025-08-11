#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int print_block ( void *p_block );
void key_expansion ( const unsigned char *input, unsigned char *expanded );
void sub_bytes ( unsigned char *state );
void shift_rows ( unsigned char *state );
void mix_cols ( unsigned char *state );
void add_round_key ( unsigned char *state, unsigned char *round_key );
int aes_encrypt_block ( unsigned char *p_output, unsigned char *p_input, unsigned char *key );
void aes_encrypt_cbc ( const void *input, size_t length, const unsigned char *key, unsigned char* output, const unsigned char* iv );
int aes_iv_construct ( char *p_iv );
