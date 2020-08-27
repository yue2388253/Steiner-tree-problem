//
// Created by lam on 2020/8/27.
//

#ifndef STEINERTREE_SOLUTIONKLEIN_H
#define STEINERTREE_SOLUTIONKLEIN_H
#include <utility>
#include <vector>
#include <tuple>

#include "type_define.h"

using namespace boost;
using namespace std;

class SolutionKlein {
public:
    struct Result {
        unsigned int cost;
        vector<int> trees_to_be_merged;
        vector<int> new_nodes;

        Result(unsigned int c, vector<int> t, vector<int> n):
                cost(c), trees_to_be_merged(std::move(t)), new_nodes(std::move(n)) {};
    };

    unsigned int klein_solution(const Graph& g);
    Result calculate_cost(int node_index, vector<vector<int>> trees, const Graph& g);
};


#endif //STEINERTREE_SOLUTIONKLEIN_H
