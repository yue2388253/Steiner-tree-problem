#include <iostream>
#include <cassert>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

#include "type_define.h"
#include "SolutionKlein.h"
#include "SolutionOpt.h"

using namespace boost;

Graph create_graph() {
    // Make convenient labels for the vertices
    enum { A, B, C, D, E, N };
    const int num_vertices = N;
    const char* name = "ABCDE";
    unsigned int vertex_weights[] = {5, 6, 1, 2, 3};
    bool is_terminal[] = {false, false, true, true, false};
    assert(sizeof(vertex_weights) / sizeof(unsigned int) == num_vertices);

    // writing out the edges in the graph
    typedef std::pair<int, int> Edge;
    Edge edge_array[] =
            { Edge(A,B), Edge(A,D), Edge(C,A), Edge(D,C),
              Edge(C,E), Edge(B,D), Edge(D,E) };
    unsigned int edge_weights[] = {1, 2, 1, 2, 7, 3, 1};
    const int num_edges = sizeof(edge_array)/sizeof(edge_array[0]);
    assert(num_edges == sizeof(edge_weights) / sizeof(unsigned int));

    // declare a graph object
    Graph g(edge_array, edge_array + num_edges, edge_weights, num_vertices);

    property_map<Graph, vertex_weight_t>::type v_weights
            = get(vertex_weight_t(), g);
    property_map<Graph, vertex_terminal_t>::type v_terminals
            = get(vertex_terminal_t(), g);

    for (int i = 0; i < num_vertices; ++i) {
        boost::put(v_weights, i, vertex_weights[i]);
        boost::put(v_terminals, i, is_terminal[i]);
    }
    return g;
}

void access_graph_data (Graph g) {
    // The following codes demonstrate how to access the data of g.

    // get the property map for vertex indices
    typedef property_map<Graph, vertex_index_t>::type IndexMap;
    IndexMap index = get(vertex_index, g);
    property_map<Graph, vertex_weight_t>::type v_weights
            = get(vertex_weight_t(), g);
    auto v_terminals = get(vertex_terminal_t(), g);

    std::cout << "vertices(g) = ";
    typedef graph_traits<Graph>::vertex_iterator vertex_iter;
    std::pair<vertex_iter, vertex_iter> vp;
    for (vp = vertices(g); vp.first != vp.second; ++vp.first) {
        std::cout << index[*vp.first] <<  ":[";
        std::cout << v_weights[*vp.first] <<  ",";
        std::cout << v_terminals[*vp.first] <<  "] ";
    }
    std::cout << std::endl;

    std::cout << "edges(g) = ";
    graph_traits<Graph>::edge_iterator ei, ei_end;
    auto edge_weights = get(edge_weight_t(), g);
    for (tie(ei, ei_end) = edges(g); ei != ei_end; ++ei)
        std::cout << "[(" << index[source(*ei, g)]
                  << "," << index[target(*ei, g)] << ") "
                  << edge_weights[*ei] << "], ";
    std::cout << std::endl;
}

int main(int, char*[])
{
    Graph g = create_graph();

    // demonstrate how to access the data of the graph and print the graph
    access_graph_data(g);

    SolutionOpt s_opt;
    SolutionKlein s_klein;
    float cost_klein = static_cast<float>(s_klein.klein_solution(g));
    float cost_opt = static_cast<float>(s_opt.optimal_solution(g));
    if (cost_opt > 0) {
        std::cout << "approximation ratio: " << cost_klein / cost_opt << std::endl;
    }

    return 0;
}
