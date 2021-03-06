//
// Created by lam on 2020/8/27.
//

#ifndef STEINERTREE_TYPE_DEFINE_H
#define STEINERTREE_TYPE_DEFINE_H

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <limits>

using namespace boost;

struct vertex_weight_t {
    typedef vertex_property_tag kind;
};

struct vertex_terminal_t {
    typedef vertex_property_tag kind;
};

typedef property<vertex_weight_t, unsigned int, property<vertex_terminal_t, bool>> VertexProperty;
typedef property<edge_weight_t, unsigned int> EdgeProperty;

// define graph structure.
typedef adjacency_list<vecS, vecS, directedS, VertexProperty, EdgeProperty> Graph;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::edge_descriptor Edge;

const float FLOAT_MAX = std::numeric_limits<float>::max();

#endif //STEINERTREE_TYPE_DEFINE_H
