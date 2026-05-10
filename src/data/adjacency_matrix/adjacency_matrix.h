/** !
 * adjacency matrix interface
 * 
 * @file src/data/adjacency_matrix/adjacency_matrix.h 
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
typedef struct adjacency_matrix_s adjacency_matrix;

// function declarations
/// constructors
int adjacency_matrix_construct
( 
    adjacency_matrix **pp_adjacency_matrix,
    enum graph_edge_type_e _type,

    size_t vertex_size,
    size_t edge_size,

    fn_key_accessor *pfn_key_accessor,
    fn_comparator   *pfn_comparator
);

/// accessors
int adjacency_matrix_vertex_get 
(
    adjacency_matrix  *p_adjacency_matrix,
    void             **p_p_vertices
);

int adjacency_matrix_vertex_search 
( 
    adjacency_matrix  *p_adjacency_matrix, 
    const void        *p_key, 
    void             **pp_vertex
);

size_t adjacency_matrix_vertex_count ( adjacency_matrix *p_adjacency_matrix );

int adjacency_matrix_edge_search 
( 
    adjacency_matrix  *p_adjacency_matrix, 
    const void        *p_from, 
    const void        *p_to, 
    void             **pp_edge
);

size_t adjacency_matrix_edge_count ( adjacency_matrix *p_adjacency_matrix );

int adjacency_matrix_neighbors_get 
( 
    adjacency_matrix   *p_adjacency_matrix, 
    const void         *p_key, 
    size_t             *p_count, 
    void             ***ppp_neighbors
);

int adjacency_matrix_vertex_degree
( 
    adjacency_matrix *p_adjacency_matrix, 
    const void       *p_key, 
    size_t           *p_in_degree, 
    size_t           *p_out_degree 
);

/// mutators
int adjacency_matrix_vertex_add 
( 
    adjacency_matrix *p_adjacency_matrix, 
    void             *p_vertex
);

int adjacency_matrix_vertex_remove
( 
    adjacency_matrix  *p_adjacency_matrix, 
    const void        *p_key, 
    void             **pp_vertex, 

    fn_allocator *pfn_allocator_vertex,
    fn_allocator *pfn_allocator_edge
);

int adjacency_matrix_edge_add 
( 
    adjacency_matrix *p_adjacency_matrix, 
    const void       *p_from, 
    const void       *p_to, 
    void             *p_edge
);

int adjacency_matrix_edge_remove 
( 
    adjacency_matrix  *p_adjacency_matrix, 
    const void        *p_from, 
    const void        *p_to, 
    void             **pp_edge,
    fn_allocator      *pfn_allocator_edge
);

/// iterators
int adjacency_matrix_vertex_foreach
(
    adjacency_matrix *p_adjacency_matrix,
    fn_foreach       *pfn_foreach
);

int adjacency_matrix_edge_foreach
(
    adjacency_matrix *p_adjacency_matrix,
    fn_foreach       *pfn_foreach
);

/// reflection
int adjacency_matrix_pack
(
    void             *p_buffer, 
    adjacency_matrix *p_adjacency_matrix,

    fn_pack *pfn_vertex,
    fn_pack *pfn_edge
);

int adjacency_matrix_unpack
(
    adjacency_matrix **pp_adjacency_matrix,
    void              *p_buffer, 

    fn_unpack *pfn_vertex,
    fn_unpack *pfn_edge,

    fn_key_accessor *pfn_key_accessor,
    fn_comparator   *pfn_comparator
);

hash64 adjacency_matrix_hash
(
    adjacency_matrix *p_adjacency_matrix,
    fn_hash64        *pfn_hash64
);

/// destructors
int adjacency_matrix_destroy
(
    adjacency_matrix **pp_adjacency_matrix,
    
    fn_allocator *pfn_allocator_vertex,
    fn_allocator *pfn_allocator_edge
);
