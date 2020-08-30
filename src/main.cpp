#include <iostream>
#include <cassert>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

#include "type_define.h"
#include "SolutionKlein.h"
#include "SolutionOpt.h"
#include "GraphGenerator.h"

using namespace boost;

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
    auto p = create_my_graph();
    const Graph& g = *p;

    // demonstrate how to access the data of the graph and print the graph
    access_graph_data(g);

    SolutionOpt s_opt;
    SolutionKlein s_klein(g);
    float cost_klein = static_cast<float>(s_klein.klein_solution());
    float cost_opt = static_cast<float>(s_opt.optimal_solution(g));
    if (cost_opt > 0) {
        std::cout << "approximation ratio: " << cost_klein / cost_opt << std::endl;
    }

    return 0;
}
