//
// Created by lam on 2020/8/27.
//

#include "SolutionKlein.h"
#include <vector>
#include <algorithm>

using namespace std;

unsigned int SolutionKlein::klein_solution(const Graph& g) {
    // Initialize the algorithm
    int num_vertex = g.vertex_set().size();
    vector<vector<int>> trees;
    vector<bool> is_in_trees(num_vertex, false);
    auto v_terminals = get(vertex_terminal_t(), g);
    auto e_weights = get(edge_weight_t(), g);
    for (int i = 0; i < num_vertex; ++i) {
        if (v_terminals[i]) {
            trees.push_back({i});
            is_in_trees[i] = true;
        }
    }

    // start iteration until there is only one tree.
    unsigned int res = 0;
    while (trees.size() != 1) {
        unsigned int cost_min = UINT16_MAX;
        vector<int> indies_trees_to_be_merged;
        vector<int> nodes_to_be_added;
        for (int i = 0; i < num_vertex; ++i) {
            auto r = calculate_cost(i, trees, g);
            if (r.cost < cost_min) {
                indies_trees_to_be_merged.clear();
                nodes_to_be_added.clear();
                indies_trees_to_be_merged = r.trees_to_be_merged;
                nodes_to_be_added = r.new_nodes;
            }
        }

        // merge the trees and r.new_nodes
        assert(indies_trees_to_be_merged.size() > 1);
        sort(indies_trees_to_be_merged.begin(),
             indies_trees_to_be_merged.end(),
             [](int i, int j){return i > j;});
        vector<int>& first_tree = trees[indies_trees_to_be_merged[0]];
        for (int i = indies_trees_to_be_merged.size(); i > 0; --i) {
            int idx = indies_trees_to_be_merged[i];
            first_tree.insert(first_tree.end(), trees[idx].begin(), trees[idx].end());
            trees.erase(trees.begin() + indies_trees_to_be_merged[i]);
        }
        first_tree.insert(first_tree.end(),
                          nodes_to_be_added.begin(),
                          nodes_to_be_added.end());

        res += cost_min;
    }
    return res;
}

// TODO: Calculate the quotient cost of node |node_index|
// Note that node |node_index| might be in the trees.
SolutionKlein::Result SolutionKlein::calculate_cost(int node_index, vector<vector<int>> trees, const Graph& g) {
    return Result(1, {}, {});
}
