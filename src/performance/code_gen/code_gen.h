/** !
 * Code generator
 * 
 * @file code_gen/code_gen.h 
 * 
 * @author Jacob Smith 
 */

// header guard
#pragma once

// standard library
#include <stdio.h>
#include <time.h>

// log module
#include <log/log.h>

// sync module
#include <sync/sync.h>

// pack module
#include <pack/pack.h>

// Platform dependent includes
#include <sys/mman.h>

// Platform dependent macros
//

// Platform dependent typedefs
//

// forward declarations
//

// type definitions
//

// function declarations

// initializer
/** !
 * This gets called at runtime before main. 
 * 
 * @param void
 * 
 * @return void
 */
void code_gen_init ( void ) __attribute__((constructor));

// cleanup
/** !
 * This gets called at runtime after main
 * 
 * @param void
 * 
 * @return void
 */
void code_gen_exit ( void ) __attribute__((destructor));
