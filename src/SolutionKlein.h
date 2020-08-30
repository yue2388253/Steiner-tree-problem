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
#include "SolutionBase.h"

using namespace boost;
using namespace std;

class SolutionKlein: public SolutionBase {
public:
    explicit SolutionKlein(const Graph& graph) : SolutionBase(graph) {};

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
};

#endif //STEINERTREE_SOLUTIONKLEIN_H
