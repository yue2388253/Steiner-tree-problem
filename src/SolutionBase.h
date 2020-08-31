//
// Created by lam on 2020/8/30.
//

#ifndef STEINERTREE_SOLUTIONBASE_H
#define STEINERTREE_SOLUTIONBASE_H

#include <boost/graph/dijkstra_shortest_paths.hpp>
#include "type_define.h"

using namespace boost;
using namespace std;

class SolutionBase {
public:
    explicit SolutionBase(const Graph& g);
    virtual ~SolutionBase() = default;;
    virtual float solution() = 0;

    const Graph& g;
    float cost;

    property_map<Graph, vertex_weight_t>::const_type vertex_weights;

    // distance_map[i][j] represents the distance from i to j (including the quotient_cost of j)
    vector<vector<int>> distance_map;

    // predecessors[i][j] represents rooted from i, the predecessor (parent) of j along the shortest path.
    vector<vector<Vertex>> predecessors;
};

template <class PredecessorMap>
class recorder : public dijkstra_visitor<>
{
public:
    explicit recorder(PredecessorMap p)
            : m_predecessor(p) { }

    template <class Edge, class Graph>
    void edge_relaxed(Edge e, Graph& g) {
        // set the parent of the target(e) to source(e)
        put(m_predecessor, target(e, g), source(e, g));
    }
protected:
    PredecessorMap m_predecessor;
};

template <class PredecessorMap>
recorder<PredecessorMap>
make_predecessor_recorder(PredecessorMap p) {
    return recorder < PredecessorMap > (p);
}

#endif //STEINERTREE_SOLUTIONBASE_H
