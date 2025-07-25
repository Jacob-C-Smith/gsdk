/** !
 * Header for node module
 * 
 * @file node/node.h
 * 
 * @author Jacob Smith
 */

// header guard
#pragma once

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// core
#include <core/sync.h>
#include <core/log.h>

// data
#include <array/array.h>
#include <dict/dict.h>

// reflection
#include <json/json.h>

// structure declarations
struct node_s;
struct node_graph_s;

// type definitions
typedef struct node_s node;
typedef struct node_graph_s node_graph;

typedef int (*fn_node_data_constructor) ( const json_value *const p_value, void **pp_result );

// structure definitions
struct node_s
{
    char _name[255 + 1];

    size_t in_quantity;
    size_t out_quantity;

    struct
    {
        char _name[31+1];
        void *value;
        size_t out_index;
        node *p_in;
    } in [64];

    struct
    {
        char _name[31+1];
        void *value;
        size_t in_index;
        node *p_out;
    } out [64];

    void *value;
};

struct node_graph_s
{
    dict *p_nodes;

    struct
    {
        int i;
    } functions;
    
    size_t node_quantity;
    node *_p_nodes[];
};

// function declarations
// initializer
/** !
 * This gets called at runtime before main. 
 * 
 * @param void
 * 
 * @return void
 */
void node_init ( void ) __attribute__((constructor));

// Constructor
/** !
 * Construct a node from a json object
 * 
 * @param pp_node              result
 * @param p_name               the name of the node
 * @param p_value              the json object
 * @param pfn_data_constructor pointer to function that constructs node data from "data" property
 * 
 * @return 1 on success, 0 on error
 */
int node_construct (
    node **pp_node,
    const char *const p_name,
    const json_value *const p_value,
    fn_node_data_constructor *pfn_node_data_constructor
);

/** !
 * Construct a node graph from a json object
 * 
 * @param pp_node result
 * @param p_value the json object
 * 
 * @return 1 on success, 0 on error
 */
int node_graph_construct (
    node_graph **pp_node_graph,
    const json_value *const p_value
);

// info
/** !
 * Print a node graph to standard out
 * 
 * @param p_node_graph the node graph
 * 
 * @return 1 on success, 0 on error
 */
int node_graph_print ( const node_graph *const p_node_graph );
