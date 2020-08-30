//
// Created by lam on 2020/8/30.
//

#include <vector>
#include "SolutionBase.h"

using namespace std;

SolutionBase::SolutionBase(const Graph& graph): g(graph), cost(0) {
    int num_vertex = num_vertices(g);
    distance_map.resize(num_vertex);
    predecessors.resize(num_vertex);
    for (int i = 0; i < num_vertex; ++i) {
        std::vector<int> d(num_vertex);
        vector<Vertex> p(num_vertices(g), graph_traits<Graph>::null_vertex()); //the predecessor array
        dijkstra_shortest_paths(g, i, boost::distance_map(&d[0]).
                visitor(make_predecessor_recorder(&p[0])));
        distance_map[i] = std::move(d);
        predecessors[i] = std::move(p);
    }
}
