/** !
 * Header file for renderer shapes 
 * 
 * @file renderer/shape.h
 * 
 * @author Jacob Smith
 */

// Include guard
#pragma once

// Enumeration definitions
enum shape_e 
{
    SHAPE_INVALID        = 0,
    SHAPE_SPHERE         = 1,
    SHAPE_CYLINDER       = 2,
    SHAPE_DISK           = 3,
    SHAPE_TRIANGLE       = 4,
    SHAPE_BILINEAR_PATCH = 5,
    SHAPE_CURVE          = 6
};


// Structure definitions
struct shape_s
{
    enum shape_e type;

    union 
    {
        struct
        {
            /* data */
        } sphere;
        
        struct
        {
            /* data */
        } cylinder;
        
        struct
        {
            /* data */
        } disk;
        
        struct
        {
            /* data */
        } disk;

        struct
        {
            /* data */
        } triangle;

        struct
        {
            /* data */
        } bilinear_patch;

        struct
        {
            /* data */
        } curve;
    };
};

// Function definitions