/** !
 * Edge list interface
 * 
 * @file include/data/edge_list.h 
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
typedef struct edge_list_s edge_list;

// function declarations
/// constructors
int edge_list_construct
( 
    edge_list **pp_edge_list,
    enum graph_edge_type_e _type,

    size_t vertex_size,
    size_t edge_size,

    fn_key_accessor *pfn_key_accessor,
    fn_comparator   *pfn_comparator
);

/// accessors
int edge_list_vertex_search 
( 
    edge_list *p_edge_list, 
    const void *p_key, 
    void **pp_vertex
);

size_t edge_list_vertex_count ( edge_list *p_edge_list );

int edge_list_edge_search 
( 
    edge_list *p_edge_list, 
    const void *p_from, 
    const void *p_to, 
    void **pp_edge
);

size_t edge_list_edge_count ( edge_list *p_edge_list );

int edge_list_neighbors_get 
( 
    edge_list *p_edge_list, 
    const void *p_key, 
    size_t *p_count, 
    void ***ppp_neighbors
);

int edge_list_vertex_degree
( 
    edge_list *p_edge_list, 
    const void *p_key, 
    size_t *p_in_degree, 
    size_t *p_out_degree 
);

/// mutators
int edge_list_vertex_add 
( 
    edge_list *p_edge_list, 
    void             *p_vertex
);

int edge_list_vertex_remove
( 
    edge_list *p_edge_list, 
    const void *p_key, 
    void **pp_vertex, 
    fn_allocator *pfn_allocator_vertex,
    fn_allocator *pfn_allocator_edge
);

int edge_list_edge_add 
( 
    edge_list *p_edge_list, 
    const void       *p_from, 
    const void       *p_to, 
    void             *p_edge
);

int edge_list_edge_remove 
( 
    edge_list *p_edge_list, 
    const void *p_from, 
    const void *p_to, 
    void **pp_edge,
    fn_allocator *pfn_allocator_edge
);

/// iterators
int edge_list_vertex_foreach
(
    edge_list *p_edge_list,
    fn_foreach       *pfn_foreach
);

int edge_list_edge_foreach
(
    edge_list *p_edge_list,
    fn_foreach       *pfn_foreach
);

/// reflection
int edge_list_pack
(
    void *p_buffer, 
    edge_list *p_edge_list,

    fn_pack *pfn_vertex,
    fn_pack *pfn_edge
);

int edge_list_unpack
(
    edge_list **pp_edge_list,
    void *p_buffer, 

    fn_unpack *pfn_vertex,
    fn_unpack *pfn_edge,

    fn_key_accessor *pfn_key_accessor,
    fn_comparator   *pfn_comparator
);

hash64 edge_list_hash
(
    edge_list *p_edge_list,
    fn_hash64 *pfn_hash64
);

/// destructors
int edge_list_destroy
(
    edge_list **pp_edge_list,
    
    fn_allocator *pfn_allocator_vertex,
    fn_allocator *pfn_allocator_edge
);
