/** !
 * base64 module interface
 * 
 * @file include/reflection/base64.h 
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

// function declarations
/// encode
/** !
 * Encode len bytes of p_data, and store in p_output
 * 
 * @param p_output result
 * @param p_data   pointer to the data to be encoded
 * @param len      the quantity of bytes to be encoded
 * 
 * @return 1 on success, 0 on error
 */
int base64_encode ( char *const p_output, const void *const p_data, size_t len );

/// decode
/** !
 * Decode len bytes of p_data, and store in p_output
 * 
 * @param p_output result
 * @param p_data   pointer to the data to be decoded
 * @param len      the quantity of bytes to be decoded
 * 
 * @return 1 on success, 0 on error
 */
int base64_decode ( void *const p_output, const char *const p_data, size_t len );
