//
// Created by lam on 2020/8/27.
//

#include "SolutionKlein.h"
#include <vector>
#include <algorithm>
#include <boost/graph/dijkstra_shortest_paths.hpp>

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
            auto r = calculate_min_cost(i, trees, g);
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
             greater<>());
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

// Note that node |node_index| might be in the trees.
SolutionKlein::Result SolutionKlein::calculate_min_cost(int node_index, const vector<vector<int>>& trees, const Graph& g) {
    std::unordered_map<int, int> node2tree_index;
    for (int i = 0; i < trees.size(); ++i) {
        for (auto& index: trees[i]) {
            node2tree_index[index] = i;
        }
    }

    vector<int> trees_index(trees.size());
    for (int i = 0; i < trees.size(); ++i) {
        trees_index[i] = i;
    }
    bool is_in_tree = false;
    if (node2tree_index.find(node_index) != node2tree_index.end()) {
        int tree_index = node2tree_index[node_index];
        trees_index.erase(trees_index.begin() + tree_index);
        is_in_tree = true;
    }

    // generate subsets from trees_index
    vector<vector<int>> subsets_vec;
    if (is_in_tree) {
        subsets_vec = SolutionKlein::generate_subsets(trees_index, 1);
    } else {
        subsets_vec = SolutionKlein::generate_subsets(trees_index, 2);
    }

    std::vector<int> d(num_vertices(g));
    vector<Vertex> p(num_vertices(g), graph_traits<Graph>::null_vertex()); //the predecessor array
    dijkstra_shortest_paths(g, node_index, distance_map(&d[0]).
            visitor(make_predecessor_recorder(&p[0])));
    SolutionKlein::Result res(UINT16_MAX, {}, {});
    for (const auto& indies: subsets_vec) {
        auto local = calculate_cost(node_index, trees, indies, g, d, p);
        if (local.cost < res.cost) {
            res = local;
        }
    }
    assert(res.cost != UINT16_MAX);

    return res;
}

// TODO:
// Note that indies.size() may be 1 since node |node_index| is in trees
SolutionKlein::Result SolutionKlein::calculate_cost(int node_index, const vector<vector<int>>& trees,
                                                    const vector<int>& indies, const Graph& g,
                                                    const vector<int>& distance_map, const vector<Vertex>& predecessor) {
    SolutionKlein::Result res(UINT16_MAX, {}, {});

    int num_trees = indies.size();
    auto v_weights = get(vertex_weight_t(), g);
    unsigned int node_weight = v_weights[node_index];
    vector<int> distances(num_trees, UINT16_MAX);
    for (int i = 0; i < num_trees; ++i) {
        int& distance = distances[i];
        for (int target_node_index: trees[indies[i]]) {
            if (distance_map[target_node_index] < distance) {
                distance = distance_map[target_node_index];
            }
        }
        assert(distance != UINT16_MAX);
    }
    unsigned int distances2trees = 0;
    for (int i = 0; i < num_trees; ++i) {
        distances2trees += distances[i];
    }
    res.cost = static_cast<float>(node_weight + distances2trees) / static_cast<float>(num_trees);

    // TODO: modify the remaining members of res

    return res;
}

// TODO: to be optimized. Tip: using dynamic programming can help.
vector<vector<int>> SolutionKlein::generate_subsets(const vector<int>& indies, int min_num) {
    vector<vector<int>> res;
    for (int i = min_num; i <= indies.size(); ++i) {
         auto r = SolutionKlein::generate_subsets_helper(indies, i);
         res.insert(res.end(), r.begin(), r.end());
    }
    return res;
}

vector<vector<int>> SolutionKlein::generate_subsets_helper(const vector<int>& indies, int num) {
    assert(indies.size() >= num);
    vector<vector<int>> res;
    if (num == 0)   return res;
    for (int i = 0; i < indies.size(); ++i) {
        vector<int> rem(indies.begin()+i+1, indies.end());
        vector<vector<int>> rem_res = generate_subsets_helper(rem, num-1);
        for (auto r: rem_res) {
            r.insert(r.begin(), indies[i]);
            res.push_back(r);
        }
    }
    return res;
}

