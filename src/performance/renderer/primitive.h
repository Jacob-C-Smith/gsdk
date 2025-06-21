/** !
 * Header file for renderer geometric primitives
 * 
 * @file renderer/primitive.h
 * 
 * @author Jacob Smith
 */

// Include guard
#pragma once

// Enumeration definitions
enum primitive_e 
{
    PRIMITIVE_INVALID           = 0,
    PRIMITIVE_SIMPLE            = 1,
    PRIMITIVE_GEOMETRIC         = 2,
    PRIMITIVE_TRANSFORMED       = 3,
    PRIMITIVE_ANIMATED          = 4,
    PRIMITIVE_BVH_AGGREGATE     = 5,
    PRIMITIVE_KD_TREE_AGGREGATE = 6
};

// Structure definitions
struct primitive_s
{
    enum primitive_e type;

    union 
    {
        struct
        {
            /* data */
        } simple;
        
        struct
        {
            /* data */
        } geometric;
        
        struct
        {
            /* data */
        } transformed;
        
        struct
        {
            /* data */
        } animated;

        struct
        {
            /* data */
        } bvh_aggregate;

        struct
        {
            /* data */
        } kd_tree_aggregate;
    };
};
