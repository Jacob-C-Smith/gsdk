#pragma once

#include <data/graph.h>

int adjacency_matrix_construct ( adjacency_matrix **pp_adjacency_matrix, size_t size );

int adjacency_matrix_vertex_add(adjacency_matrix *p_matrix, void *p_value);

int adjacency_matrix_vertex_remove(adjacency_matrix *p_matrix, void *p_value);

int adjacency_matrix_vertex_search(adjacency_matrix *p_matrix, void *p_value, void **pp_value);

int adjacency_matrix_vertex_count(adjacency_matrix *p_matrix, size_t *p_count);

int adjacency_matrix_vertex_info(adjacency_matrix *p_matrix, void *p_value, char _name[], size_t name_size, void **pp_value);

int adjacency_matrix_edge_add(adjacency_matrix *p_matrix, void *p_a_value, void *p_b_value, double weight);

int adjacency_matrix_edge_remove(adjacency_matrix *p_matrix, void *p_a_value, void *p_b_value);

int adjacency_matrix_edge_search(adjacency_matrix *p_matrix, void *p_a_value, void *p_b_value, double *p_weight);

int adjacency_matrix_edge_count(adjacency_matrix *p_matrix, size_t *p_count);

int adjacency_matrix_edge_info(adjacency_matrix *p_matrix, void *p_a_value, void *p_b_value, double *p_weight);

int adjacency_matrix_print(adjacency_matrix *p_matrix, void (*print_func)(void *));
