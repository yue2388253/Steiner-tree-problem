//
// Created by lam on 2020/8/30.
//

#include "GraphGenerator.h"
#include "type_define.h"

#include <random>
#include <vector>
#include <cstdlib>

using namespace boost;
using namespace std;

const int MAXIMUM_VERTEX_WEIGHT = 10;
const int MAXIMUM_EDGE_WEIGHT = 10;
const int MINIMUM_EDGE_NUM = -1;
const int AVG_DEGREE = 2;


// Generate a random connected graph with |num_nodes| nodes and |num_terminals| terminals.
std::shared_ptr<Graph> GraphGenerator::generate_graph(int num_nodes, int num_terminals) {
    assert(num_nodes >= num_terminals);
    srand(time(NULL));
    std::shared_ptr<Graph> res(new Graph(num_nodes));
    Graph& g = *res;

    vector<bool> is_terminals(num_nodes, false);
    for (int i = 0; i < num_terminals; ++i) {
        is_terminals[i] = true;
    }
    shuffle(is_terminals.begin(), is_terminals.end(), std::mt19937(std::random_device()()));

    vector<unsigned int> vertex_weights(num_nodes, 0);
    for (int i = 0; i < num_nodes; ++i) {
        vertex_weights[i] = rand() % MAXIMUM_VERTEX_WEIGHT + 1;
    }

    property_map<Graph, vertex_weight_t>::type v_weights
            = get(vertex_weight_t(), g);
    property_map<Graph, vertex_terminal_t>::type v_terminals
            = get(vertex_terminal_t(), g);

    for (int i = 0; i < num_nodes; ++i) {
        boost::put(v_weights, i, vertex_weights[i]);
        boost::put(v_terminals, i, is_terminals[i]);
    }

    int num_edges = num_nodes * AVG_DEGREE;
    if (MINIMUM_EDGE_NUM != -1)    num_edges = MINIMUM_EDGE_NUM;
    assert(num_edges >= num_nodes);

    vector<vector<int>> edges(num_nodes, vector<int>(num_nodes, 0));
    auto my_add_edge = [&](int src, int dst) {
        if (edges[src][dst])    return false;
        unsigned int weight = rand() % MAXIMUM_EDGE_WEIGHT + 1;
        add_edge(src, dst, weight + vertex_weights[dst], g);
        add_edge(dst, src, weight + vertex_weights[src], g);
        edges[src][dst] = 1;
        edges[dst][src] = 1;
        return true;
    };

    // connect all the nodes together firstly.
    for (int i = 1; i < num_nodes; ++i) {
        int src = rand() % i;
        int dst = i;
        my_add_edge(src, dst);
        num_edges--;
    }

    // add random edge.
    while (num_edges) {
        int src = rand() % num_nodes;
        int dst = -1;
        while (dst == -1 || dst == src) {
            dst = rand() % num_nodes;
        }
        bool ok = my_add_edge(src, dst);
        if (ok) num_edges--;
    }

    return res;
}

std::shared_ptr<Graph> GraphGenerator::create_my_graph() {
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
    std::shared_ptr<Graph> res(
            new Graph(new_edge_array.begin(), new_edge_array.end(),
                      new_edge_weights.begin(), num_vertices));
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
