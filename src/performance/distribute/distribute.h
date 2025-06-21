/** !
 * High level abstractions for distributed computing
 *
 * @file distribute/distribute.h
 * 
 * @author Jacob Smith
 */

// header guard
#pragma once

// header files
#include <distribute/connection.h>
#include <json/json.h>
#include <sync/sync.h>
#include <parallel/parallel.h>

// function declarations
/** !
 * Distributed coin flip
 * 
 * @param p_connection
 * 
 * @return 1 on success, 0 on error
 */
// int distribute_coin_flip ( const connection *const p_connection );
