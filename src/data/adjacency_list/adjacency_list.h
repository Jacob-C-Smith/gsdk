/** !
 * Adjacency list interface
 * 
 * @file src/data/adjacency_list/adjacency_list.h 
 * 
 * @author Jacob Smith
 */

// header guard
#pragma once

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

// gsdk
/// core
#include <core/interfaces.h>
#include <core/log.h>
#include <core/hash.h>
#include <core/pack.h>
#include <core/sync.h>

/// data
#include <data/graph.h>

// type definitions
typedef struct adjacency_list_s adjacency_list;

// function declarations
/// constructors
int adjacency_list_construct
( 
    adjacency_list **pp_adjacency_list,
    enum graph_edge_type_e _type,

    size_t vertex_size,
    size_t edge_size,

    fn_key_accessor *pfn_key_accessor,
    fn_comparator   *pfn_comparator
);

/// accessors
int adjacency_list_vertex_search 
( 
    adjacency_list *p_adjacency_list, 
    const void *p_key, 
    void **pp_vertex
);

size_t adjacency_list_vertex_count ( adjacency_list *p_adjacency_list );

int adjacency_list_edge_search 
( 
    adjacency_list *p_adjacency_list, 
    const void *p_from, 
    const void *p_to, 
    void **pp_edge
);

size_t adjacency_list_edge_count ( adjacency_list *p_adjacency_list );

int adjacency_list_neighbors_get 
( 
    adjacency_list *p_adjacency_list, 
    const void *p_key, 
    size_t *p_count, 
    void ***ppp_neighbors
);

int adjacency_list_vertex_degree
( 
    adjacency_list *p_adjacency_list, 
    const void *p_key, 
    size_t *p_in_degree, 
    size_t *p_out_degree 
);

/// mutators
int adjacency_list_vertex_add 
( 
    adjacency_list *p_adjacency_list, 
    void             *p_vertex
);

int adjacency_list_vertex_remove
( 
    adjacency_list *p_adjacency_list, 
    const void *p_key, 
    void **pp_vertex, 
    fn_allocator *pfn_allocator_vertex,
    fn_allocator *pfn_allocator_edge
);

int adjacency_list_edge_add 
( 
    adjacency_list *p_adjacency_list, 
    const void       *p_from, 
    const void       *p_to, 
    void             *p_edge
);

int adjacency_list_edge_remove 
( 
    adjacency_list *p_adjacency_list, 
    const void *p_from, 
    const void *p_to, 
    void **pp_edge,
    fn_allocator *pfn_allocator_edge
);

/// iterators
int adjacency_list_vertex_foreach
(
    adjacency_list *p_adjacency_list,
    fn_foreach       *pfn_foreach
);

int adjacency_list_edge_foreach
(
    adjacency_list *p_adjacency_list,
    fn_foreach       *pfn_foreach
);

/// reflection
int adjacency_list_pack
(
    void *p_buffer, 
    adjacency_list *p_adjacency_list,

    fn_pack *pfn_vertex,
    fn_pack *pfn_edge
);

int adjacency_list_unpack
(
    adjacency_list **pp_adjacency_list,
    void *p_buffer, 

    fn_unpack *pfn_vertex,
    fn_unpack *pfn_edge,

    fn_key_accessor *pfn_key_accessor,
    fn_comparator   *pfn_comparator
);

hash64 adjacency_list_hash
(
    adjacency_list *p_adjacency_list,
    fn_hash64 *pfn_hash64
);

/// destructors
int adjacency_list_destroy
(
    adjacency_list **pp_adjacency_list,
    
    fn_allocator *pfn_allocator_vertex,
    fn_allocator *pfn_allocator_edge
);
