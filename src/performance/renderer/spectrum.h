/** ! 
 * Header file for renderer spectrum
 * 
 * @file renderer/spectrum.h
 * 
 * @author Jacob Smith
 */

// Include guard
#pragma once

// Header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <renderer/rtypedef.h>

// Preprocessor definitions
#define RENDERER_SPECTRUM_LAMBDA_MIN 360
#define RENDERER_SPECTRUM_LAMBDA_MAX 830

// Enumeration definitions
enum spectrum_e 
{
    SPECTRUM_INVALID          = 0,
    SPECTRUM_CONSTANT         = 1,
    SPECTRUM_DENSELY_SAMPLED  = 2,
    SPECTRUM_PIECEWISE_LINEAR = 3,
    SPECTRUM_RGB_ALBEDO       = 4,
    SPECTRUM_RGB_UNBOUNDED    = 5,
    SPECTRUM_RGB_ILLUMINANT   = 6,
    SPECTRUM_BLACKBODY        = 7
};

// Type definitions
typedef int (*fn_spectrum_lambda)(const spectrum *const p_spectrum, float *p_lambda );
typedef int (*fn_spectrum_max_value)(const spectrum *const p_spectrum, float *p_lambda );

// Structure definitions
struct spectrum_s
{
    enum spectrum_e type;

    union 
    {
        struct
        {
            float constant;
        } constant;
        
        struct
        {
            int lambda_min;
            int lambda_max;
            int lambda_range;
            float values[RENDERER_SPECTRUM_LAMBDA_MAX - RENDERER_SPECTRUM_LAMBDA_MIN];
        } densely_sampled;
        
        struct
        {
            /* data */
        } piecewise_linear;
        
        struct
        {
            /* data */
        } rgb_albedo;
        
        struct
        {
            /* data */
        } rgb_unbounded;
        
        struct
        {
            /* data */
        } rgb_illuminant;
        
        struct
        {
            /* data */
        } blackbody;
        
        
    };
    
    fn_spectrum_lambda    pfn_lambda;
    fn_spectrum_max_value pfn_max;
};

// Function declarations

/** !
 * Construct a constant spectrum from a wavelength
 * 
 * @param p_spectrum return
 * @param constant   the constant wavelength
 * 
 * @return 1 on success, 0 on error
 */
int spectrum_constant_construct ( spectrum *const p_spectrum, const float constant );

/** !
 * Get the wavelength of a constant spectrum
 * 
 * @param p_spectrum the spectrum
 * @param p_lambda   return
 * 
 * @return 1 on success, 0 on error
 */
int spectrum_constant_lambda ( const spectrum *const p_spectrum, float *const p_lambda );

/** !
 * Construct a Densely sampled spectrum 
 * 
 * @param p_spectrum        return
 * @param p_source_spectrum the spectrum to evaluate at each wavelength
 * @param lambda_min        the minimum wavelength
 * @param lambda_max        the maximum wavelength
 * 
 * @return 1 on success, 0 on error
 */
int spectrum_densely_sampled_construct ( spectrum *const p_spectrum, const spectrum *const p_source_spectrum, int lambda_min, int lambda_max );

/** !
 * Get the wavelength of a densely sampled spectrum
 * 
 * @param p_spectrum the spectrum
 * @param lambda     the wavelength
 * @param p_lambda   return
 * 
 * @return 1 on success, 0 on error
 */
int spectrum_densely_sampled_lambda ( spectrum *const p_spectrum, float lambda, float *const p_lambda );