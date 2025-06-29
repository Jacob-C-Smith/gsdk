/** ! 
 * Header file for renderer camera
 * 
 * @file renderer/camera.h
 * 
 * @author Jacob Smith
 */

// header guard
#pragma once

// enumeration definitions
enum camera_e 
{
    CAMERA_INVALID      = 0,
    CAMERA_PERSPECTIVE  = 1,
    CAMERA_ORTHOGRAPHIC = 2,
    CAMERA_SPHERICAL    = 3,
    CAMERA_REALISTIC    = 4
};

// structure definitions
struct camera_s
{
    enum camera_e type;

    union 
    {
        struct
        {
            float constant;
        } perspective;
        
        struct
        {
            /* data */
        } orthographic;
        
        struct
        {
            /* data */
        } spherical;
        
        struct
        {
            /* data */
        } realistic;
    };
};
