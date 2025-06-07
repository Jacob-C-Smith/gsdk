/** !
 * Include header for base64 library
 * 
 * @file reflection/base64.h 
 * 
 * @author Jacob Smith 
 */

// header guard
#pragma once

// standard library
#include <stdio.h>
#include <stdbool.h>

// core
#include <core/log.h>
#include <core/sync.h>

// Encode
/** !
 * Encode len bytes of data in base 64, and store in p_output
 * 
 * @param p_data   the data to be encoded
 * @param len      the quantity of bytes to be encoded
 * @param p_output the output, encoded in base 64
 * 
 * @sa base64_decode
 * 
 * @return 1 on success, 0 on error
 */
int base64_encode ( const void *const p_data, size_t len, char *const p_output );

// Decode
/** !
 * Decode len bytes of base 64 data, and store in p_output
 * 
 * @param p_data   the data to be decoded
 * @param len      the quantity of bytes to be decoded
 * @param p_output the output, decoded from base 64
 * 
 * @sa base64_encode
 * 
 * @return 1 on success, 0 on error
 */
int base64_decode ( const char *const p_data, size_t len, void *const p_output );
