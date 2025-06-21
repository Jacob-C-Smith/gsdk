/** ! 
 * Header file for renderer integrator
 * 
 * @file renderer/integrator.h
 * 
 * @author Jacob Smith
 */

// header guard
#pragma once

// header files
#include <renderer/rtypedef.h>
#include <renderer/primitive.h>
#include <renderer/light.h>

// preprocessor definitions
#define RENDERER_INTEGRATOR_MAX_LIGHTS

// structure definitions
struct integrator_s
{
    primitive *p_aggregate;
    light     *p_lights[RENDERER_INTEGRATOR_MAX_LIGHTS];
};

// function declarations
/** !
 * Construct an integrator from a primitive aggregate and a list of lights
 * 
 * @param p_aggregate the scene geometry
 * @param pp_lights   a null pointer terminated list of lights
 * 
 * @return 1 on success, 0 on error
 */
int renderer_integrator_construct ( integrator *p_integrator, primitive *p_aggregate, light **pp_lights );
