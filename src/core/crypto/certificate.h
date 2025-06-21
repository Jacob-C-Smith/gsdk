/** ! 
 * A wrapper around a public rsa key
 * 
 * @file certificate.c
 * 
 * @author Jacob Smith
 */

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>

// structure declarations
struct certificate_s;

// type definitions
typedef struct certificate_s certificate;

// function declarations
/** !
 * Parse a certificate
 * 
 * 
 * 
 * @return 1 on success, 0 on error
 */
int key_pair_parse ( certificate **pp_certificate, const char *p_path );