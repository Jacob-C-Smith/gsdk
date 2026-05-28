/** !
 * Tester for graph module
 * 
 * @file src/test/graph_test.c
 * 
 * @author Jacob Smith
 */

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <float.h>

// gsdk
/// core
#include <core/log.h>
#include <core/sync.h>
#include <core/test.h>
#include <core/stream.h>

/// data
#include <data/graph.h>

// enumeration definitions
enum graph_test_scenario_type_e
{
    ADJACENCY_MATRIX_UNDIRECTED_UNWEIGHTED,
    ADJACENCY_MATRIX_UNDIRECTED_WEIGHTED,
    ADJACENCY_MATRIX_DIRECTED_UNWEIGHTED,
    ADJACENCY_MATRIX_DIRECTED_WEIGHTED,

    ADJACENCY_LIST_UNDIRECTED_UNWEIGHTED,
    ADJACENCY_LIST_UNDIRECTED_WEIGHTED,
    ADJACENCY_LIST_DIRECTED_UNWEIGHTED,
    ADJACENCY_LIST_DIRECTED_WEIGHTED,

    EDGE_LIST_UNDIRECTED_UNWEIGHTED,
    EDGE_LIST_UNDIRECTED_WEIGHTED,
    EDGE_LIST_DIRECTED_UNWEIGHTED,
    EDGE_LIST_DIRECTED_WEIGHTED,

    GRAPH_TEST_SCENARIO_QUANTITY,
};

// structure declarations
struct vertex_data_s;
struct edge_data_s;
struct graph_test_s;

// type definitions
typedef struct vertex_data_s        vertex_data;
typedef struct edge_data_s          edge_data;
typedef struct graph_test_s graph_test;
typedef void (fn_setup_graph)(graph *p_graph);

// structure definitions
struct vertex_data_s 
{
    char id[16];
};

struct edge_data_s 
{
    double weight;
};

struct graph_test_s 
{
    fn_setup_graph *pfn_setup_graph;
    size_t          expected_vertex_count;
    size_t          expected_edge_count;
    const char     *bfs_start;
    size_t          expected_bfs_count;
    const char     *sssp_start;
    const char     *sssp_target;
    double          expected_sssp_dist;
};

// function declarations
fn_key_accessor    vertex_key_accessor;
fn_comparator      vertex_comparator;
fn_weight_accessor edge_weight_accessor;
fn_allocator       destruct_graph;
fn_foreach         bfs_callback; 
fn_foreach         dfs_callback; 

fn_test_case test_vertex_add;
fn_test_case test_vertex_remove;
fn_test_case test_vertex_search;
fn_test_case test_vertex_count;
fn_test_case test_vertex_get;
fn_test_case test_vertex_degree;
fn_test_case test_neighbors_get;
fn_test_case test_edge_add;
fn_test_case test_edge_remove;
fn_test_case test_edge_search;
fn_test_case test_edge_count;
fn_test_case test_bfs;
fn_test_case test_dfs;
fn_test_case test_dijkstra;
fn_test_case test_bellman_ford;
fn_test_case test_floyd_warshall;
fn_test_case test_johnson;
fn_test_case test_kruskal;
fn_test_case test_prim;

fn_scenario_constructor construct_adjacency_matrix_undirected_unweighted;
fn_scenario_constructor construct_adjacency_list_undirected_unweighted;
fn_scenario_constructor construct_edge_list_undirected_unweighted;
fn_scenario_constructor adjacency_matrix_undirected_weighted;
fn_scenario_constructor construct_adjacency_list_undirected_weighted;
fn_scenario_constructor construct_edge_list_undirected_weighted;
fn_scenario_constructor adjacency_matrix_directed_unweighted;
fn_scenario_constructor construct_adjacency_list_directed_unweighted;
fn_scenario_constructor construct_edge_list_directed_unweighted;
fn_scenario_constructor adjacency_matrix_directed_weighted;
fn_scenario_constructor construct_adjacency_list_directed_weighted;
fn_scenario_constructor construct_edge_list_directed_weighted;

fn_setup_graph setup_graph_adjacency_matrix_undirected_unweighted;
fn_setup_graph setup_graph_adjacency_list_undirected_unweighted;
fn_setup_graph setup_graph_edge_list_undirected_unweighted;
fn_setup_graph setup_graph_adjacency_matrix_undirected_weighted;
fn_setup_graph setup_graph_adjacency_list_undirected_weighted;
fn_setup_graph setup_graph_edge_list_undirected_weighted;
fn_setup_graph setup_graph_adjacency_matrix_directed_unweighted;
fn_setup_graph setup_graph_adjacency_list_directed_unweighted;
fn_setup_graph setup_graph_edge_list_directed_unweighted;
fn_setup_graph setup_graph_adjacency_matrix_directed_weighted;
fn_setup_graph setup_graph_adjacency_list_directed_weighted;
fn_setup_graph setup_graph_edge_list_directed_weighted;

static size_t g_bfs_count = 0;
static size_t g_dfs_count = 0;

graph_test _tests[GRAPH_TEST_SCENARIO_QUANTITY] = 
{
    [ADJACENCY_MATRIX_UNDIRECTED_UNWEIGHTED] = 
    {
        .pfn_setup_graph       = setup_graph_adjacency_matrix_undirected_unweighted,
        .expected_vertex_count = 3,
        .expected_edge_count   = 2,
        .bfs_start             = "A",
        .expected_bfs_count    = 3,
        .sssp_start            = "A",
        .sssp_target           = "C",
    },
    [ADJACENCY_MATRIX_UNDIRECTED_WEIGHTED] = 
    {
        .pfn_setup_graph       = setup_graph_adjacency_matrix_undirected_weighted,
        .expected_vertex_count = 3,
        .expected_edge_count   = 3,
        .bfs_start             = "A",
        .expected_bfs_count    = 3,
        .sssp_start            = "A",
        .sssp_target           = "C",
        .expected_sssp_dist    = 3.0
    },
    [ADJACENCY_MATRIX_DIRECTED_UNWEIGHTED] = 
    {
        .pfn_setup_graph       = setup_graph_adjacency_matrix_directed_unweighted,
        .expected_vertex_count = 3,
        .expected_edge_count   = 2,
        .bfs_start             = "A",
        .expected_bfs_count    = 3,
        .sssp_start            = "A",
        .sssp_target           = "C",
    },
    [ADJACENCY_MATRIX_DIRECTED_WEIGHTED] = 
    {
        .pfn_setup_graph       = setup_graph_adjacency_matrix_directed_weighted,
        .expected_vertex_count = 3,
        .expected_edge_count   = 3,
        .bfs_start             = "A",
        .expected_bfs_count    = 3,
        .sssp_start            = "A",
        .sssp_target           = "C",
        .expected_sssp_dist    = 5.0
    },
    [ADJACENCY_LIST_UNDIRECTED_UNWEIGHTED] = 
    {
        .pfn_setup_graph       = setup_graph_adjacency_list_undirected_unweighted,
        .expected_vertex_count = 4,
        .expected_edge_count   = 3,
        .bfs_start             = "A",
        .expected_bfs_count    = 4,
        .sssp_start            = "A",
        .sssp_target           = "D",
    },
    [ADJACENCY_LIST_UNDIRECTED_WEIGHTED] = 
    {
        .pfn_setup_graph       = setup_graph_adjacency_list_undirected_weighted,
        .expected_vertex_count = 4,
        .expected_edge_count   = 4,
        .bfs_start             = "A",
        .expected_bfs_count    = 4,
        .sssp_start            = "A",
        .sssp_target           = "D",
        .expected_sssp_dist    = 3.0
    },
    [ADJACENCY_LIST_DIRECTED_UNWEIGHTED] = 
    {
        .pfn_setup_graph       = setup_graph_adjacency_list_directed_unweighted,
        .expected_vertex_count = 4,
        .expected_edge_count   = 4,
        .bfs_start             = "A",
        .expected_bfs_count    = 4,
        .sssp_start            = "A",
        .sssp_target           = "D",
    },
    [ADJACENCY_LIST_DIRECTED_WEIGHTED] = 
    {
        .pfn_setup_graph       = setup_graph_adjacency_list_directed_weighted,
        .expected_vertex_count = 4,
        .expected_edge_count   = 5,
        .bfs_start             = "A",
        .expected_bfs_count    = 4,
        .sssp_start            = "A",
        .sssp_target           = "D",
        .expected_sssp_dist    = 3.0
    },
    [EDGE_LIST_UNDIRECTED_UNWEIGHTED] = 
    {
        .pfn_setup_graph       = setup_graph_edge_list_undirected_unweighted,
        .expected_vertex_count = 5,
        .expected_edge_count   = 4,
        .bfs_start             = "A",
        .expected_bfs_count    = 5,
        .sssp_start            = "A",
        .sssp_target           = "E",
    },
    [EDGE_LIST_UNDIRECTED_WEIGHTED] = 
    {
        .pfn_setup_graph       = setup_graph_edge_list_undirected_weighted,
        .expected_vertex_count = 5,
        .expected_edge_count   = 5,
        .bfs_start             = "A",
        .expected_bfs_count    = 5,
        .sssp_start            = "A",
        .sssp_target           = "E",
        .expected_sssp_dist    = 6.0
    },
    [EDGE_LIST_DIRECTED_UNWEIGHTED] = 
    {
        .pfn_setup_graph       = setup_graph_edge_list_directed_unweighted,
        .expected_vertex_count = 5,
        .expected_edge_count   = 4,
        .bfs_start             = "A",
        .expected_bfs_count    = 5,
        .sssp_start            = "A",
        .sssp_target           = "E",
    },
    [EDGE_LIST_DIRECTED_WEIGHTED] = 
    {
        .pfn_setup_graph       = setup_graph_edge_list_directed_weighted,
        .expected_vertex_count = 5,
        .expected_edge_count   = 6,
        .bfs_start             = "A",
        .expected_bfs_count    = 5,
        .sssp_start            = "A",
        .sssp_target           = "E",
        .expected_sssp_dist    = 4.0
    },
};

test_case cases_undirected_unweighted[] = 
{
    TEST_CASE("vertex add"   , test_vertex_add   , NULL, TEST_RESULT_ONE),
    TEST_CASE("vertex remove", test_vertex_remove, NULL, TEST_RESULT_ONE),
    TEST_CASE("vertex search", test_vertex_search, NULL, TEST_RESULT_ONE),
    TEST_CASE("vertex count" , test_vertex_count , NULL, TEST_RESULT_ONE),
    TEST_CASE("vertex get"   , test_vertex_get   , NULL, TEST_RESULT_ONE),
    TEST_CASE("vertex degree", test_vertex_degree, NULL, TEST_RESULT_ONE),
    TEST_CASE("neighbors get", test_neighbors_get, NULL, TEST_RESULT_ONE),
    TEST_CASE("edge add"     , test_edge_add     , NULL, TEST_RESULT_ONE),
    TEST_CASE("edge remove"  , test_edge_remove  , NULL, TEST_RESULT_ONE),
    TEST_CASE("edge search"  , test_edge_search  , NULL, TEST_RESULT_ONE),
    TEST_CASE("edge count"   , test_edge_count   , NULL, TEST_RESULT_ONE),
    TEST_CASE("bfs"          , test_bfs          , NULL, TEST_RESULT_ONE),
    TEST_CASE("dfs"          , test_dfs          , NULL, TEST_RESULT_ONE),
};

test_case cases_undirected_weighted[] = 
{
    TEST_CASE("vertex add"    , test_vertex_add    , NULL, TEST_RESULT_ONE),
    TEST_CASE("vertex remove" , test_vertex_remove , NULL, TEST_RESULT_ONE),
    TEST_CASE("vertex search" , test_vertex_search , NULL, TEST_RESULT_ONE),
    TEST_CASE("vertex count"  , test_vertex_count  , NULL, TEST_RESULT_ONE),
    TEST_CASE("vertex get"    , test_vertex_get    , NULL, TEST_RESULT_ONE),
    TEST_CASE("vertex degree" , test_vertex_degree , NULL, TEST_RESULT_ONE),
    TEST_CASE("neighbors get" , test_neighbors_get , NULL, TEST_RESULT_ONE),
    TEST_CASE("edge add"      , test_edge_add      , NULL, TEST_RESULT_ONE),
    TEST_CASE("edge remove"   , test_edge_remove   , NULL, TEST_RESULT_ONE),
    TEST_CASE("edge search"   , test_edge_search   , NULL, TEST_RESULT_ONE),
    TEST_CASE("edge count"    , test_edge_count    , NULL, TEST_RESULT_ONE),
    TEST_CASE("bfs"           , test_bfs           , NULL, TEST_RESULT_ONE),
    TEST_CASE("dfs"           , test_dfs           , NULL, TEST_RESULT_ONE),
    TEST_CASE("dijkstra"      , test_dijkstra      , NULL, TEST_RESULT_ONE),
    TEST_CASE("bellman_ford"  , test_bellman_ford  , NULL, TEST_RESULT_ONE),
    TEST_CASE("floyd_warshall", test_floyd_warshall, NULL, TEST_RESULT_ONE),
    TEST_CASE("johnson"       , test_johnson       , NULL, TEST_RESULT_ONE),
    TEST_CASE("kruskal"       , test_kruskal       , NULL, TEST_RESULT_ONE),
    TEST_CASE("prim"          , test_prim          , NULL, TEST_RESULT_ONE),
};

test_case cases_directed_unweighted[] = 
{
    TEST_CASE("vertex add"   , test_vertex_add   , NULL, TEST_RESULT_ONE),
    TEST_CASE("vertex remove", test_vertex_remove, NULL, TEST_RESULT_ONE),
    TEST_CASE("vertex search", test_vertex_search, NULL, TEST_RESULT_ONE),
    TEST_CASE("vertex count" , test_vertex_count , NULL, TEST_RESULT_ONE),
    TEST_CASE("vertex get"   , test_vertex_get   , NULL, TEST_RESULT_ONE),
    TEST_CASE("vertex degree", test_vertex_degree, NULL, TEST_RESULT_ONE),
    TEST_CASE("neighbors get", test_neighbors_get, NULL, TEST_RESULT_ONE),
    TEST_CASE("edge add"     , test_edge_add     , NULL, TEST_RESULT_ONE),
    TEST_CASE("edge remove"  , test_edge_remove  , NULL, TEST_RESULT_ONE),
    TEST_CASE("edge search"  , test_edge_search  , NULL, TEST_RESULT_ONE),
    TEST_CASE("edge count"   , test_edge_count   , NULL, TEST_RESULT_ONE),
    TEST_CASE("bfs"          , test_bfs          , NULL, TEST_RESULT_ONE),
    TEST_CASE("dfs"          , test_dfs          , NULL, TEST_RESULT_ONE),
};

test_case cases_directed_weighted[] = 
{
    TEST_CASE("vertex add"    , test_vertex_add    , NULL, TEST_RESULT_ONE),
    TEST_CASE("vertex remove" , test_vertex_remove , NULL, TEST_RESULT_ONE),
    TEST_CASE("vertex search" , test_vertex_search , NULL, TEST_RESULT_ONE),
    TEST_CASE("vertex count"  , test_vertex_count  , NULL, TEST_RESULT_ONE),
    TEST_CASE("vertex get"    , test_vertex_get    , NULL, TEST_RESULT_ONE),
    TEST_CASE("vertex degree" , test_vertex_degree , NULL, TEST_RESULT_ONE),
    TEST_CASE("neighbors get" , test_neighbors_get , NULL, TEST_RESULT_ONE),
    TEST_CASE("edge add"      , test_edge_add      , NULL, TEST_RESULT_ONE),
    TEST_CASE("edge remove"   , test_edge_remove   , NULL, TEST_RESULT_ONE),
    TEST_CASE("edge search"   , test_edge_search   , NULL, TEST_RESULT_ONE),
    TEST_CASE("edge count"    , test_edge_count    , NULL, TEST_RESULT_ONE),
    TEST_CASE("bfs"           , test_bfs           , NULL, TEST_RESULT_ONE),
    TEST_CASE("dfs"           , test_dfs           , NULL, TEST_RESULT_ONE),
    TEST_CASE("dijkstra"      , test_dijkstra      , NULL, TEST_RESULT_ONE),
    TEST_CASE("bellman_ford"  , test_bellman_ford  , NULL, TEST_RESULT_ONE),
    TEST_CASE("floyd_warshall", test_floyd_warshall, NULL, TEST_RESULT_ONE),
    TEST_CASE("johnson"       , test_johnson       , NULL, TEST_RESULT_ONE),
};

test_scenario _scenarios[] = 
{
    TEST_SCENARIO("adjacency matrix undirected unweighted", &_tests[ADJACENCY_MATRIX_UNDIRECTED_UNWEIGHTED], cases_undirected_unweighted, construct_adjacency_matrix_undirected_unweighted, destruct_graph),
    TEST_SCENARIO("adjacency matrix undirected weighted"  , &_tests[ADJACENCY_MATRIX_UNDIRECTED_WEIGHTED]  , cases_undirected_weighted  , adjacency_matrix_undirected_weighted  , destruct_graph),
    TEST_SCENARIO("adjacency matrix directed unweighted"  , &_tests[ADJACENCY_MATRIX_DIRECTED_UNWEIGHTED]  , cases_directed_unweighted  , adjacency_matrix_directed_unweighted  , destruct_graph),
    TEST_SCENARIO("adjacency matrix directed weighted"    , &_tests[ADJACENCY_MATRIX_DIRECTED_WEIGHTED]    , cases_directed_weighted    , adjacency_matrix_directed_weighted    , destruct_graph),
    TEST_SCENARIO("adjacency list undirected unweighted"  , &_tests[ADJACENCY_LIST_UNDIRECTED_UNWEIGHTED]  , cases_undirected_unweighted, construct_adjacency_list_undirected_unweighted, destruct_graph),
    TEST_SCENARIO("adjacency list undirected weighted"    , &_tests[ADJACENCY_LIST_UNDIRECTED_WEIGHTED]    , cases_undirected_weighted  , construct_adjacency_list_undirected_weighted  , destruct_graph),
    TEST_SCENARIO("adjacency list directed unweighted"    , &_tests[ADJACENCY_LIST_DIRECTED_UNWEIGHTED]    , cases_directed_unweighted  , construct_adjacency_list_directed_unweighted  , destruct_graph),
    TEST_SCENARIO("adjacency list directed weighted"      , &_tests[ADJACENCY_LIST_DIRECTED_WEIGHTED]      , cases_directed_weighted    , construct_adjacency_list_directed_weighted    , destruct_graph),
    TEST_SCENARIO("edge list undirected unweighted"       , &_tests[EDGE_LIST_UNDIRECTED_UNWEIGHTED]       , cases_undirected_unweighted, construct_edge_list_undirected_unweighted, destruct_graph),
    TEST_SCENARIO("edge list undirected weighted"         , &_tests[EDGE_LIST_UNDIRECTED_WEIGHTED]         , cases_undirected_weighted  , construct_edge_list_undirected_weighted  , destruct_graph),
    TEST_SCENARIO("edge list directed unweighted"         , &_tests[EDGE_LIST_DIRECTED_UNWEIGHTED]         , cases_directed_unweighted  , construct_edge_list_directed_unweighted  , destruct_graph),
    TEST_SCENARIO("edge list directed weighted"           , &_tests[EDGE_LIST_DIRECTED_WEIGHTED]           , cases_directed_weighted    , construct_edge_list_directed_weighted    , destruct_graph),
};

test_suite _suite = TEST_SUITE("graph", _scenarios);

int main ( int argc, const char *argv[] ) 
{

    // unused
    (void) argc;
    (void) argv;
    
    // run tests
    test_suite_test(&_suite); 

    // done
    return (_suite.counters.total.fails == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

void *vertex_key_accessor ( const void *const p_value )
{

    // initialized data
    vertex_data *p_vertex_data = (vertex_data *)p_value;

    // success
    return (void *)p_vertex_data->id;
}

int vertex_comparator ( const void *const p_a, const void *const p_b )
{

    // initialized data
    const char *a = (const char *)p_a;
    const char *b = (const char *)p_b;

    // success
    return strcmp(a, b);
}

double edge_weight_accessor ( const void *p_edge )
{

    // initialized data
    edge_data *p_edge_data = (edge_data *)p_edge;

    // success
    return p_edge_data->weight;
}

void *destruct_graph ( void *p_pointer, unsigned long long size )
{

    // unused
    (void) size;

    // initialized data
    graph *p_graph = (graph *)p_pointer;

    // release the graph
    if ( p_graph ) 
        graph_destroy(&p_graph, NULL, NULL);

    // success
    return NULL;
}

void *test_vertex_add ( test_case *p_case, void *p_subject ) 
{

    // unused
    (void)p_case;

    // static data
    static vertex_data v1 = {"X"};

    // initialized data
    graph *p_graph = p_subject;
    
    // add?
    if (0 == graph_vertex_add(p_graph, &v1)) return NULL;

    // quantity?
    if (1 != graph_vertex_count(p_graph)) return NULL;

    // success
    return (void *)1;
}

void *test_vertex_remove ( test_case *p_case, void *p_subject ) 
{

    // unused
    (void)p_case;

    // static data
    static vertex_data v1 = {"X"};
    
    // initialized data
    graph *p_graph  = p_subject;
    void  *p_vertex = NULL;
    
    // add
    graph_vertex_add(p_graph, &v1);

    // remove? 
    if ( 0 == graph_vertex_remove(p_graph, "X", &p_vertex, NULL, NULL) || p_vertex != &v1 ) return NULL;

    // quantity?
    if ( 0 != graph_vertex_count(p_graph) ) return NULL;
    
    // success
    return (void *)1;
}

void *test_vertex_search ( test_case *p_case, void *p_subject ) 
{

    // unused
    (void)p_case;
    
    // static data
    static vertex_data v1 = {"X"};

    // initialized data
    graph *p_graph  = p_subject;
    void  *p_vertex = NULL;
    
    // add
    graph_vertex_add(p_graph, &v1);

    // search?
    if ( 0 == graph_vertex_search(p_graph, "X", &p_vertex) || p_vertex != &v1 ) return NULL;

    // search?
    if ( 1 == graph_vertex_search(p_graph, "Y", &p_vertex) ) return NULL;

    // success
    return (void *)1;
}

void *test_vertex_count ( test_case *p_case, void *p_subject ) 
{

    // unused
    (void)p_case;
    
    // static data
    static vertex_data v1 = {"X"}, v2 = {"Y"};

    // initialized data
    graph *p_graph = p_subject;
    
    // quantity?
    if ( 0 != graph_vertex_count(p_graph) ) return NULL;

    // add
    graph_vertex_add(p_graph, &v1),
    graph_vertex_add(p_graph, &v2);

    // quantity?
    if ( 2 != graph_vertex_count(p_graph) ) return NULL;

    // remove
    graph_vertex_remove(p_graph, "X", NULL, NULL, NULL);

    // quantity?
    if ( 1 != graph_vertex_count(p_graph) ) return NULL;

    // success
    return (void *)1;
}

void *test_vertex_get ( test_case *p_case, void *p_subject ) 
{

    // unused
    (void)p_case;

    // static data
    static vertex_data v1 = {"X"};
    
    // initialized data
    graph *p_graph       = p_subject;
    void  *_p_results[2] = { 0 };

    // add
    graph_vertex_add(p_graph, &v1);

    // get?
    if ( 0 == graph_vertex_get(p_graph, _p_results) || _p_results[0] != &v1) return NULL;
    
    // success
    return (void *)1;
}

void *test_vertex_degree ( test_case *p_case, void *p_subject ) 
{

    // unused
    (void)p_case;

    // static data
    static vertex_data v1 = {"X"}, v2 = {"Y"};
    static edge_data e = {1.0};

    // initialized data
    graph  *p_graph = p_subject;
    size_t  in      = 0, 
            out     = 0;
    
    // add
    graph_vertex_add(p_graph, &v1),
    graph_vertex_add(p_graph, &v2),
    graph_edge_add(p_graph, "X", "Y", &e);

    // degree?
    if (0 == graph_vertex_degree(p_graph, "X", &in, &out)) return NULL;

    // done
    if ( p_graph->_edge_type & GRAPH_DIRECTED ) 
        return (in != 0 || out != 1) ? NULL : (void *)1;
    else 
        return (in != 1 || out != 1) ? NULL : (void *)1;
}

void *test_neighbors_get ( test_case *p_case, void *p_subject ) 
{

    // unused
    (void)p_case;
    
    // static data
    static vertex_data v1 = {"X"}, v2 = {"Y"};
    static edge_data e = {1.0};

    // initialized data
    graph   *p_graph   = p_subject;
    size_t   count     = 0;
    void   **neighbors = NULL;

    // add
    graph_vertex_add(p_graph, &v1),
    graph_vertex_add(p_graph, &v2),
    graph_edge_add(p_graph, "X", "Y", &e);

    // neighbors?
    if
    (
        0            == graph_neighbors_get(p_graph, "X", &count, &neighbors) || 
        count        != 1  || 
        neighbors[0] != &v2
    ) 
    {

        // release neighbors
        if (neighbors) neighbors = default_allocator(neighbors, 0);

        // error
        return NULL;
    }

    // release neighbors
    neighbors = default_allocator(neighbors, 0);

    // success
    return (void *)1;
}

void *test_edge_add ( test_case *p_case, void *p_subject ) 
{

    // unused
    (void)p_case;
    
    // static data
    static vertex_data v1 = {"X"}, v2 = {"Y"};
    static edge_data   e  = {1.0};

    // initialized data
    graph *p_graph = p_subject;
    
    // add
    graph_vertex_add(p_graph, &v1),
    graph_vertex_add(p_graph, &v2);

    // add?
    if ( 0 == graph_edge_add(p_graph, "X", "Y", &e) ) return NULL;

    // success
    return (void *)1;
}

void *test_edge_remove ( test_case *p_case, void *p_subject ) 
{

    // unused
    (void)p_case;
    
    // static data
    static vertex_data v1 = {"X"}, v2 = {"Y"};
    static edge_data e = {1.0};

    // initialized data
    graph *p_graph = p_subject;
    void  *p_edge  = NULL;

    // add
    graph_vertex_add(p_graph, &v1),
    graph_vertex_add(p_graph, &v2),
    graph_edge_add(p_graph, "X", "Y", &e);

    // remove?
    if ( 0 == graph_edge_remove(p_graph, "X", "Y", &p_edge, NULL) || p_edge != &e ) return NULL;
    
    // success
    return (void *)1;
}

void *test_edge_search ( test_case *p_case, void *p_subject ) 
{

    // unused
    (void)p_case;
    
    // static data
    static vertex_data  v1     = {"X"}, v2 = {"Y"};
    static edge_data    e      = {1.0};
    void               *p_edge = NULL;

    // initialized data
    graph *p_graph = p_subject;

    // add
    graph_vertex_add(p_graph, &v1),
    graph_vertex_add(p_graph, &v2),
    graph_edge_add(p_graph, "X", "Y", &e);

    // search?
    if ( 0 == graph_edge_search(p_graph, "X", "Y", &p_edge) || p_edge != &e ) return NULL;

    // success
    return (void *)1;
}

void *test_edge_count ( test_case *p_case, void *p_subject ) 
{

    // unused
    (void)p_case;
    
    // static data
    static vertex_data v1 = {"X"}, v2 = {"Y"};
    static edge_data e = {1.0};
    
    // initialized data
    graph  *p_graph  = p_subject;
    size_t  expected = (p_graph->_edge_type & GRAPH_DIRECTED) ? 1 : 2;

    // add
    graph_vertex_add(p_graph, &v1),
    graph_vertex_add(p_graph, &v2),
    graph_edge_add(p_graph, "X", "Y", &e);

    // quantity?
    if ( expected != graph_edge_count(p_graph) ) return NULL;

    // success
    return (void *)1;
}

void *test_bfs ( test_case *p_case, void *p_subject ) 
{

    // initialized data
    graph      *p_graph = p_subject;
    graph_test *test    = p_case->p_data;

    // setup
    test->pfn_setup_graph(p_graph);
    
    // clear bfs counter
    g_bfs_count = 0;

    // bfs?
    if ( 0 == graph_algorithm_bfs(p_graph, test->bfs_start, bfs_callback) ) return NULL;

    // visited?
    if (g_bfs_count != test->expected_bfs_count) return NULL;

    // success
    return (void *)1;
}

void *test_dfs ( test_case *p_case, void *p_subject ) 
{

    // initialized data
    graph      *p_graph = p_subject;
    graph_test *test    = p_case->p_data;

    // setup
    test->pfn_setup_graph(p_graph);
    
    // clear the dfs counter
    g_dfs_count = 0;

    // dfs? 
    if ( 0 == graph_algorithm_dfs(p_graph, test->bfs_start, dfs_callback) ) return NULL;

    // visited?
    if ( g_dfs_count != test->expected_bfs_count ) return NULL;

    // success
    return (void *)1;
}

void *test_dijkstra ( test_case *p_case, void *p_subject ) 
{

    // initialized data
    graph             *p_graph = p_subject;
    graph_test        *test    = p_case->p_data;
    graph_sssp_result *results = NULL;
    bool               ok      = false;

    // setup
    test->pfn_setup_graph(p_graph);

    // dijkstra?
    if ( 0 == graph_algorithm_sssp_dijkstra(p_graph, test->sssp_start, edge_weight_accessor, &results) ) return NULL;
    
    // results?
    if ( NULL == results ) return NULL;
    
    // iterate through each vertex
    for (size_t i = 0; i < graph_vertex_count(p_graph); i++) 

        // target?
        if ( 0 == strcmp(((vertex_data*)results[i].p_vertex)->id, test->sssp_target) ) 
            
            // distance?
            if ( results[i].distance == test->expected_sssp_dist ) ok = true;
    
    // release results
    results = default_allocator(results, 0);
    
    // done
    return ok ? (void *)1 : NULL;
}

void *test_bellman_ford ( test_case *p_case, void *p_subject ) 
{

    // initialized data
    graph             *p_graph = p_subject;
    graph_test        *test    = p_case->p_data;
    graph_sssp_result *results = NULL;
    bool               ok      = false;

    // setup
    test->pfn_setup_graph(p_graph);

    // bellman-ford?
    if ( 0 == graph_algorithm_sssp_bellman_ford(p_graph, test->sssp_start, edge_weight_accessor, &results) ) return NULL;
    
    // results?
    if ( NULL == results ) return NULL;
    
    // iterate through each vertex
    for (size_t i = 0; i < graph_vertex_count(p_graph); i++) 

        // target?
        if ( 0 == strcmp(((vertex_data*)results[i].p_vertex)->id, test->sssp_target) ) 
            
            // distance?
            if ( results[i].distance == test->expected_sssp_dist ) ok = true;
    
    // release results
    results = default_allocator(results, 0);
    
    // done
    return ok ? (void *)1 : NULL;
}

void *test_floyd_warshall ( test_case *p_case, void *p_subject ) 
{

    // initialized data
    graph       *p_graph = p_subject;
    graph_test  *test    = p_case->p_data;
    double     **matrix  = NULL;
    size_t       count   = 0;

    // setup
    test->pfn_setup_graph(p_graph);

    // store the quantity of vertices
    count = graph_vertex_count(p_graph);

    // floyd-warshall?
    if ( 0 == graph_algorithm_apsp_floyd_warshall(p_graph, edge_weight_accessor, &matrix) ) return NULL;
    
    // results?
    if ( NULL == matrix ) return NULL;
    
    // release result rows
    for (size_t i = 0; i < count; i++) 
        matrix[i] = default_allocator(matrix[i], 0);
    
    // release result matrix
    default_allocator(matrix, 0);

    // success
    return (void *)1;
}

void *test_johnson ( test_case *p_case, void *p_subject ) 
{

    // initialized data
    graph       *p_graph = p_subject;
    graph_test  *test    = p_case->p_data;
    double     **matrix  = NULL;
    size_t       count   = 0;

    // setup
    test->pfn_setup_graph(p_graph);

    // store the quantity of vertices
    count = graph_vertex_count(p_graph);

    // johnson?
    if ( 0 == graph_algorithm_apsp_johnson(p_graph, edge_weight_accessor, &matrix) ) return NULL;
    
    // results?
    if ( NULL == matrix ) return NULL;
    
    // release result rows
    for (size_t i = 0; i < count; i++) 
        matrix[i] = default_allocator(matrix[i], 0);
    
    // release result matrix
    default_allocator(matrix, 0);

    // success
    return (void *)1;
}

void *test_kruskal ( test_case *p_case, void *p_subject ) 
{

    // initialized data
    graph      *p_graph = p_subject;
    graph_test *test    = p_case->p_data;
    graph      *mst     = NULL;
    bool        ok      = false;

    // setup
    test->pfn_setup_graph(p_graph);
    
    // kruskal?
    if ( 0 == graph_algorithm_mst_kruskal(p_graph, edge_weight_accessor, &mst) ) return NULL;
    
    // results?
    if ( NULL == mst ) return NULL;
    
    // mst has all vertices?
    if ( test->expected_vertex_count == graph_vertex_count(mst) ) ok = true;
    
    // release the mst
    graph_destroy(&mst, NULL, NULL);
    
    // done
    return ok ? (void *)1 : NULL;
}

void *test_prim ( test_case *p_case, void *p_subject ) 
{

    // initialized data
    graph      *p_graph = p_subject;
    graph_test *test    = p_case->p_data;
    graph      *mst     = NULL;
    bool        ok      = false;

    // setup
    test->pfn_setup_graph(p_graph);
    
    // prim?
    if ( 0 == graph_algorithm_mst_prim(p_graph, edge_weight_accessor, &mst) ) return NULL;
    
    // results?
    if ( NULL == mst ) return NULL;
    
    // mst has all vertices?
    if ( test->expected_vertex_count == graph_vertex_count(mst) ) ok = true;
    
    // release the mst
    graph_destroy(&mst, NULL, NULL);
    
    // done
    return ok ? (void *)1 : NULL;
}

void bfs_callback ( void *v ) 
{ 

    // unused
    (void)v;

    // increment the counter
    g_bfs_count++;

    // done
    return;
}

void dfs_callback ( void *v ) 
{ 

    // unused
    (void)v;

    // increment the counter
    g_dfs_count++;

    // done
    return;
}

int construct_adjacency_matrix_undirected_unweighted ( void **pp_result ) { return graph_construct((graph **)pp_result, GRAPH_ADJACENCY_MATRIX, GRAPH_UNDIRECTED | GRAPH_UNWEIGHTED, sizeof(vertex_data), sizeof(edge_data), vertex_key_accessor, vertex_comparator); }
int construct_adjacency_list_undirected_unweighted   ( void **pp_result ) { return graph_construct((graph **)pp_result, GRAPH_ADJACENCY_LIST  , GRAPH_UNDIRECTED | GRAPH_UNWEIGHTED, sizeof(vertex_data), sizeof(edge_data), vertex_key_accessor, vertex_comparator); }
int construct_edge_list_undirected_unweighted        ( void **pp_result ) { return graph_construct((graph **)pp_result, GRAPH_EDGE_LIST       , GRAPH_UNDIRECTED | GRAPH_UNWEIGHTED, sizeof(vertex_data), sizeof(edge_data), vertex_key_accessor, vertex_comparator); }
int adjacency_matrix_undirected_weighted             ( void **pp_result ) { return graph_construct((graph **)pp_result, GRAPH_ADJACENCY_MATRIX, GRAPH_UNDIRECTED | GRAPH_WEIGHTED  , sizeof(vertex_data), sizeof(edge_data), vertex_key_accessor, vertex_comparator); }
int construct_adjacency_list_undirected_weighted     ( void **pp_result ) { return graph_construct((graph **)pp_result, GRAPH_ADJACENCY_LIST  , GRAPH_UNDIRECTED | GRAPH_WEIGHTED  , sizeof(vertex_data), sizeof(edge_data), vertex_key_accessor, vertex_comparator); }
int construct_edge_list_undirected_weighted          ( void **pp_result ) { return graph_construct((graph **)pp_result, GRAPH_EDGE_LIST       , GRAPH_UNDIRECTED | GRAPH_WEIGHTED  , sizeof(vertex_data), sizeof(edge_data), vertex_key_accessor, vertex_comparator); }
int adjacency_matrix_directed_unweighted             ( void **pp_result ) { return graph_construct((graph **)pp_result, GRAPH_ADJACENCY_MATRIX, GRAPH_DIRECTED   | GRAPH_UNWEIGHTED, sizeof(vertex_data), sizeof(edge_data), vertex_key_accessor, vertex_comparator); }
int construct_adjacency_list_directed_unweighted     ( void **pp_result ) { return graph_construct((graph **)pp_result, GRAPH_ADJACENCY_LIST  , GRAPH_DIRECTED   | GRAPH_UNWEIGHTED, sizeof(vertex_data), sizeof(edge_data), vertex_key_accessor, vertex_comparator); }
int construct_edge_list_directed_unweighted          ( void **pp_result ) { return graph_construct((graph **)pp_result, GRAPH_EDGE_LIST       , GRAPH_DIRECTED   | GRAPH_UNWEIGHTED, sizeof(vertex_data), sizeof(edge_data), vertex_key_accessor, vertex_comparator); }
int adjacency_matrix_directed_weighted               ( void **pp_result ) { return graph_construct((graph **)pp_result, GRAPH_ADJACENCY_MATRIX, GRAPH_DIRECTED   | GRAPH_WEIGHTED  , sizeof(vertex_data), sizeof(edge_data), vertex_key_accessor, vertex_comparator); }
int construct_adjacency_list_directed_weighted       ( void **pp_result ) { return graph_construct((graph **)pp_result, GRAPH_ADJACENCY_LIST  , GRAPH_DIRECTED   | GRAPH_WEIGHTED  , sizeof(vertex_data), sizeof(edge_data), vertex_key_accessor, vertex_comparator); }
int construct_edge_list_directed_weighted            ( void **pp_result ) { return graph_construct((graph **)pp_result, GRAPH_EDGE_LIST       , GRAPH_DIRECTED   | GRAPH_WEIGHTED  , sizeof(vertex_data), sizeof(edge_data), vertex_key_accessor, vertex_comparator); }

void setup_graph_adjacency_matrix_undirected_unweighted ( graph *p_graph )
{

    // initialized data
    static vertex_data vertices[3] = {{"A"}, {"B"}, {"C"}};
    static edge_data   edges[2]    = {{0.0}, {0.0}};
    
    // add vertices
    for (int i = 0; i < 3; i++) 
        graph_vertex_add(p_graph, &vertices[i]);

    // add edges
    graph_edge_add(p_graph, "A", "B", &edges[0]);
    graph_edge_add(p_graph, "B", "C", &edges[1]);
}

void setup_graph_adjacency_list_undirected_unweighted ( graph *p_graph )
{

    // initialized data
    static vertex_data vertices[4] = {{"A"}, {"B"}, {"C"}, {"D"}};
    static edge_data   edges[3]    = {{0.0}, {0.0}, {0.0}};
    
    // add vertices
    for (int i = 0; i < 4; i++) 
        graph_vertex_add(p_graph, &vertices[i]);

    // add edges
    graph_edge_add(p_graph, "A", "B", &edges[0]);
    graph_edge_add(p_graph, "A", "C", &edges[1]);
    graph_edge_add(p_graph, "A", "D", &edges[2]);
}

void setup_graph_edge_list_undirected_unweighted ( graph *p_graph )
{

    // initialized data
    static vertex_data vertices[5] = {{"A"}, {"B"}, {"C"}, {"D"}, {"E"}};
    static edge_data   edges[4]    = {{0.0}, {0.0}, {0.0}, {0.0}};
    
    // add vertices
    for (int i = 0; i < 5; i++) 
        graph_vertex_add(p_graph, &vertices[i]);

    // add edges
    graph_edge_add(p_graph, "A", "B", &edges[0]);
    graph_edge_add(p_graph, "B", "C", &edges[1]);
    graph_edge_add(p_graph, "C", "D", &edges[2]);
    graph_edge_add(p_graph, "D", "E", &edges[3]);
}

void setup_graph_adjacency_matrix_undirected_weighted ( graph *p_graph )
{

    // initialized data
    static vertex_data vertices[3] = {{"A"}, {"B"}, {"C"}};
    static edge_data   edges[3]    = {{1.0}, {2.0}, {4.0}};
    
    // add vertices
    for (int i = 0; i < 3; i++) 
        graph_vertex_add(p_graph, &vertices[i]);

    // add edges
    graph_edge_add(p_graph, "A", "B", &edges[0]);
    graph_edge_add(p_graph, "B", "C", &edges[1]);
    graph_edge_add(p_graph, "A", "C", &edges[2]);
}

void setup_graph_adjacency_list_undirected_weighted ( graph *p_graph )
{

    // initialized data
    static vertex_data vertices[4] = {{"A"}, {"B"}, {"C"}, {"D"}};
    static edge_data   edges[4]    = {{1.0}, {1.0}, {1.0}, {4.0}};
    
    // add vertices
    for (int i = 0; i < 4; i++) 
        graph_vertex_add(p_graph, &vertices[i]);

    // add edges
    graph_edge_add(p_graph, "A", "B", &edges[0]);
    graph_edge_add(p_graph, "B", "C", &edges[1]);
    graph_edge_add(p_graph, "C", "D", &edges[2]);
    graph_edge_add(p_graph, "A", "D", &edges[3]);
}

void setup_graph_edge_list_undirected_weighted ( graph *p_graph )
{

    // initialized data
    static vertex_data vertices[5] = {{"A"}, {"B"}, {"C"}, {"D"}, {"E"}};
    static edge_data   edges[5]    = {{1.0}, {2.0}, {1.0}, {2.0}, {10.0}};
    
    // add vertices
    for (int i = 0; i < 5; i++) 
        graph_vertex_add(p_graph, &vertices[i]);

    // add edges
    graph_edge_add(p_graph, "A", "B", &edges[0]);
    graph_edge_add(p_graph, "B", "C", &edges[1]);
    graph_edge_add(p_graph, "C", "D", &edges[2]);
    graph_edge_add(p_graph, "D", "E", &edges[3]);
    graph_edge_add(p_graph, "A", "E", &edges[4]);
}

void setup_graph_adjacency_matrix_directed_unweighted ( graph *p_graph )
{

    // initialized data
    static vertex_data vertices[3] = {{"A"}, {"B"}, {"C"}};
    static edge_data   edges[2]    = {{0.0}, {0.0}};
    
    // add vertices
    for (int i = 0; i < 3; i++) 
        graph_vertex_add(p_graph, &vertices[i]);

    // add edges
    graph_edge_add(p_graph, "A", "B", &edges[0]);
    graph_edge_add(p_graph, "B", "C", &edges[1]);
}

void setup_graph_adjacency_list_directed_unweighted ( graph *p_graph )
{

    // initialized data
    static vertex_data vertices[4] = {{"A"}, {"B"}, {"C"}, {"D"}};
    static edge_data   edges[4]    = {{0.0}, {0.0}, {0.0}, {0.0}};
    
    // add vertices
    for (int i = 0; i < 4; i++) 
        graph_vertex_add(p_graph, &vertices[i]);

    // add edges
    graph_edge_add(p_graph, "A", "B", &edges[0]);
    graph_edge_add(p_graph, "B", "C", &edges[1]);
    graph_edge_add(p_graph, "C", "D", &edges[2]);
    graph_edge_add(p_graph, "D", "A", &edges[3]);
}

void setup_graph_edge_list_directed_unweighted ( graph *p_graph )
{

    // initialized data
    static vertex_data vertices[5] = {{"A"}, {"B"}, {"C"}, {"D"}, {"E"}};
    static edge_data   edges[4]    = {{0.0}, {0.0}, {0.0}, {0.0}};
    
    // add vertices
    for (int i = 0; i < 5; i++) 
        graph_vertex_add(p_graph, &vertices[i]);

    // add edges
    graph_edge_add(p_graph, "A", "B", &edges[0]);
    graph_edge_add(p_graph, "A", "C", &edges[1]);
    graph_edge_add(p_graph, "B", "D", &edges[2]);
    graph_edge_add(p_graph, "C", "E", &edges[3]);
}

void setup_graph_adjacency_matrix_directed_weighted ( graph *p_graph )
{

    // initialized data
    static vertex_data vertices[3] = {{"A"}, {"B"}, {"C"}};
    static edge_data   edges[3]    = {{2.0}, {3.0}, {6.0}};
    
    // add vertices
    for (int i = 0; i < 3; i++) 
        graph_vertex_add(p_graph, &vertices[i]);

    // add edges
    graph_edge_add(p_graph, "A", "B", &edges[0]);
    graph_edge_add(p_graph, "B", "C", &edges[1]);
    graph_edge_add(p_graph, "A", "C", &edges[2]);
}

void setup_graph_adjacency_list_directed_weighted ( graph *p_graph )
{

    // initialized data
    static vertex_data vertices[4] = {{"A"}, {"B"}, {"C"}, {"D"}};
    static edge_data   edges[5]    = {{1.0}, {2.0}, {3.0}, {1.0}, {5.0}};
    
    // add vertices
    for (int i = 0; i < 4; i++) 
        graph_vertex_add(p_graph, &vertices[i]);

    // add edges
    graph_edge_add(p_graph, "A", "B", &edges[0]);
    graph_edge_add(p_graph, "A", "C", &edges[1]);
    graph_edge_add(p_graph, "B", "D", &edges[2]);
    graph_edge_add(p_graph, "C", "D", &edges[3]);
    graph_edge_add(p_graph, "A", "D", &edges[4]);
}

void setup_graph_edge_list_directed_weighted ( graph *p_graph )
{

    // initialized data
    static vertex_data vertices[5] = {{"A"}, {"B"}, {"C"}, {"D"}, {"E"}};
    static edge_data   edges[6]    = {{1.0}, {1.0}, {1.0}, {1.0}, {3.0}, {10.0}};
    
    // add vertices
    for (int i = 0; i < 5; i++) 
        graph_vertex_add(p_graph, &vertices[i]);

    // add edges
    graph_edge_add(p_graph, "A", "B", &edges[0]);
    graph_edge_add(p_graph, "B", "C", &edges[1]);
    graph_edge_add(p_graph, "C", "D", &edges[2]);
    graph_edge_add(p_graph, "D", "E", &edges[3]);
    graph_edge_add(p_graph, "A", "C", &edges[4]);
    graph_edge_add(p_graph, "A", "E", &edges[5]);
}
