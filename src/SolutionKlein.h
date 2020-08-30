//
// Created by lam on 2020/8/27.
//

#ifndef STEINERTREE_SOLUTIONKLEIN_H
#define STEINERTREE_SOLUTIONKLEIN_H
#include <utility>
#include <vector>
#include <tuple>
#include <boost/graph/dijkstra_shortest_paths.hpp>

#include "type_define.h"

using namespace boost;
using namespace std;

class SolutionKlein {
public:
    explicit SolutionKlein(const Graph& graph);

    struct Result {
        float quotient_cost;
        vector<int> trees_to_be_merged;
        vector<int> new_nodes;

        Result(float c, vector<int> t, vector<int> n):
                quotient_cost(c), trees_to_be_merged(std::move(t)), new_nodes(std::move(n)) {};
    };

    float klein_solution();
    Result calculate_min_quotient_cost(int node_index, const vector<vector<int>>& trees);
    Result calculate_cost(int node_index, const vector<vector<int>>& trees,
                          const vector<int>& indies);

    static vector<vector<int>> generate_subsets(const vector<int>& indies, int min_num = 2);
    static vector<vector<int>> generate_subsets_helper(const vector<int>& indies, int num);

private:
    const Graph& g;
    float cost;

    // distance_map[i][j] represents the distance from i to j (including the quotient_cost of j)
    vector<vector<int>> distance_map;

    // predecessors[i][j] represents rooted from i, the predecessor (parent) of j along the shortest path.
    vector<vector<Vertex>> predecessors;
};

template <class PredecessorMap>
class recorder : public dijkstra_visitor<>
{
public:
    recorder(PredecessorMap p)
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

#endif //STEINERTREE_SOLUTIONKLEIN_H
