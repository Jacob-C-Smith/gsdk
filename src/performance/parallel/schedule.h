/** !
 * Schedule interface
 * 
 * @file src/performance/parallel/schedule.h
 *
 * @author Jacob Smith
 */

// header guard
#pragma once

// standard library
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// core
#include <core/sync.h>

// data
#include <data/dict.h>
#include <data/array.h>

// reflection
#include <reflection/json.h>

// performance
#include <performance/parallel.h>
#include <performance/thread.h>

// forward declarations
struct schedule_s;

// type definitions
typedef struct schedule_s schedule;

// function declarations
/// constructors
/** !
 * Construct a schedule from a file 
 * 
 * @param pp_schedule return
 * @param path        path to the file
 * 
 * @return 1 on success, 0 on error 
*/
int schedule_load ( schedule **const pp_schedule, const char *const path );

/** !
 * Construct a schedule from a json value 
 * 
 * @param pp_schedule return
 * @param p_value     the json value
 * 
 * @return 1 on success, 0 on error 
*/
int schedule_load_as_json_value ( schedule **const pp_schedule, const json_value *const p_value );

/// executors
/** !
 * Start running a schedule
 *
 * @param p_schedule  the schedule
 * @param p_parameter this parameter is passed to each task
 *
 * @return 1 on success, 0 on error
 */
int schedule_start ( schedule *const p_schedule, void *const p_parameter );

/// blockers
/** !
 * Block until a schedule is done
 * 
 * @param p_schedule the schedule
 * 
 * @return 1 on success, 0 on error
 */
int schedule_wait_idle ( schedule *const p_schedule );

/// destructors
/** !
 * Destroy a schedule
 * 
 * @param pp_schedule the schedule
 * 
 * @return 1 on success, 0 on error
 */
int schedule_destroy ( schedule **const pp_schedule );

