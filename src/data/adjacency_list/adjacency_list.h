#pragma once

#include <data/graph.h>
#include <data/graph.h>

int adjacency_list_construct ( adjacency_list **pp_adjacency_list, size_t size );

int adjacency_list_vertex_add(adjacency_list **pp_list, void *p_value);

int adjacency_list_vertex_remove(adjacency_list **pp_list, void *p_value);

int adjacency_list_vertex_search(adjacency_list *p_list, void *p_value, void **pp_value);

int adjacency_list_vertex_count(adjacency_list *p_list, size_t *p_count);

int adjacency_list_vertex_info(adjacency_list *p_list, void *p_value, char _name[], size_t name_size, void **pp_value);

int adjacency_list_edge_add(adjacency_list **pp_list, void *p_a_value, void *p_b_value, double weight);

int adjacency_list_edge_remove(adjacency_list **pp_list, void *p_a_value, void *p_b_value);

int adjacency_list_edge_search(adjacency_list *p_list, void *p_a_value, void *p_b_value, double *p_weight);

int adjacency_list_edge_count(adjacency_list *p_list, size_t *p_count);

int adjacency_list_edge_info(adjacency_list *p_list, void *p_a_value, void *p_b_value, double *p_weight);

int adjacency_list_print(adjacency_list *p_list, void (*print_func)(void *));