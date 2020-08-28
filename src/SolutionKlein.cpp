//
// Created by lam on 2020/8/27.
//

#include "SolutionKlein.h"
#include <vector>
#include <algorithm>
#include <limits>
#include <unordered_set>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace std;

float SolutionKlein::klein_solution(const Graph& g) {
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
    float res = 0;
    while (trees.size() != 1) {
        float cost_min = std::numeric_limits<float>::max();
        vector<int> indies_trees_to_be_merged;
        vector<int> nodes_to_be_added;
        for (int i = 0; i < num_vertex; ++i) {
            auto r = calculate_min_cost(i, trees, g);
            if (r.cost < cost_min) {
                cost_min = r.cost;
                indies_trees_to_be_merged.clear();
                nodes_to_be_added.clear();
                indies_trees_to_be_merged = r.trees_to_be_merged;
                nodes_to_be_added = r.new_nodes;
            }
        }

        // merge the trees and r.new_nodes
        assert(indies_trees_to_be_merged.size() > 1);
        sort(indies_trees_to_be_merged.begin(),
             indies_trees_to_be_merged.end());
        vector<int>& first_tree = trees[indies_trees_to_be_merged.front()];
        for (int i = indies_trees_to_be_merged.size() - 1; i > 0; --i) {
            int idx = indies_trees_to_be_merged[i];
            first_tree.insert(first_tree.end(), trees[idx].begin(), trees[idx].end());
            trees.erase(trees.begin() + idx);
        }

        std::unordered_set<int> t(first_tree.begin(), first_tree.end());
        t.insert(nodes_to_be_added.begin(), nodes_to_be_added.end());
        first_tree.clear();
        first_tree = vector<int>(t.begin(), t.end());

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

    // TODO: should nodes in trees be considered?
    if (is_in_tree) return SolutionKlein::Result{std::numeric_limits<float>::max(), {}, {}};

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
    SolutionKlein::Result res(FLOAT_MAX, {}, {});

    int num_trees = indies.size();
    auto v_weights = get(vertex_weight_t(), g);
    unsigned int node_weight = v_weights[node_index];
    vector<int> distances(num_trees, UINT16_MAX);
    vector<vector<int>> node_to_be_added(num_trees);
    for (int i = 0; i < num_trees; ++i) {
        int& distance = distances[i];
        vector<int>& nodes_to_tree = node_to_be_added[i];
        for (int target_node_index: trees[indies[i]]) {
            if (distance_map[target_node_index] < distance) {
                distance = distance_map[target_node_index];

                nodes_to_tree.clear();
                Vertex p = predecessor[target_node_index];
                while (p != graph_traits<Graph>::null_vertex() && static_cast<int>(p) != node_index) {
                    nodes_to_tree.push_back(static_cast<int>(p));
                    p = predecessor[p];
                }
            }
        }
        assert(distance != UINT16_MAX);
    }
    std::unordered_set<int> nodes_set;
    for (const auto& nodes: node_to_be_added) {
        for (const auto &node: nodes) {
            nodes_set.insert(node);
        }
    }
    unsigned int distances2trees = 0;
    for (int i = 0; i < num_trees; ++i) {
        distances2trees += distances[i];
    }
    res.cost = static_cast<float>(node_weight + distances2trees) / static_cast<float>(num_trees);
    res.trees_to_be_merged = indies;
    res.new_nodes = vector<int>(nodes_set.begin(), nodes_set.end());

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
    vector<vector<int>> res;
    if (num == 0 || indies.size() < num)   return res;
    if (num == 1) {
        for (auto index: indies) {
            res.push_back({index});
        }
        return res;
    }
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

