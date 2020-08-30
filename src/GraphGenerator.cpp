//
// Created by lam on 2020/8/30.
//

#include "GraphGenerator.h"
#include "type_define.h"

#include <vector>

using namespace boost;
using namespace std;


std::shared_ptr<Graph> generate_graph(int num_nodes, int num_terminals) {
    std::shared_ptr<Graph> res(new Graph(num_nodes));

    // TODO:


    return res;
}


std::shared_ptr<Graph> create_my_graph() {
    // Make convenient labels for the vertices
    enum { A, B, C, D, E, N };
    const int num_vertices = N;
    unsigned int vertex_weights[] = {5, 6, 1, 2, 3};
    bool is_terminal[] = {false, true, false, false, true};
    assert(sizeof(vertex_weights) / sizeof(unsigned int) == num_vertices);

    // writing out the edges in the graph
    typedef std::pair<int, int> MyEdge;
    vector<MyEdge> edge_array =
            { MyEdge(A,B), MyEdge(A,D), MyEdge(C,A), MyEdge(D,C),
              MyEdge(C,E), MyEdge(B,D), MyEdge(D,E) };
    vector<unsigned int> edge_weights = {1, 2, 1, 2, 7, 3, 1};
    const int num_edges = edge_array.size();
    assert(num_edges == edge_weights.size());

    vector<MyEdge> new_edge_array;
    vector<unsigned int> new_edge_weights;
    for (int i = 0; i < num_edges; ++i) {
        int src = edge_array[i].first;
        int dst = edge_array[i].second;
        unsigned int w = edge_weights[i];
        new_edge_array.emplace_back(MyEdge(src, dst));
        new_edge_weights.emplace_back(w + vertex_weights[dst]);
        new_edge_array.emplace_back(MyEdge(dst, src));
        new_edge_weights.emplace_back(w + vertex_weights[src]);
    }

    // declare a graph object
    std::shared_ptr<Graph> res(new Graph(new_edge_array.begin(), new_edge_array.end(), new_edge_weights.begin(), num_vertices));
    auto& g = *res;

    property_map<Graph, vertex_weight_t>::type v_weights
            = get(vertex_weight_t(), g);
    property_map<Graph, vertex_terminal_t>::type v_terminals
            = get(vertex_terminal_t(), g);

    for (int i = 0; i < num_vertices; ++i) {
        boost::put(v_weights, i, vertex_weights[i]);
        boost::put(v_terminals, i, is_terminal[i]);
    }
    return res;
}
