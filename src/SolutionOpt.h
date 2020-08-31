//
// Created by lam on 2020/8/27.
//

#ifndef STEINERTREE_SOLUTIONOPT_H
#define STEINERTREE_SOLUTIONOPT_H

#include "type_define.h"
#include "SolutionBase.h"
#include <utility>
#include <vector>
#include <unordered_map>

using namespace std;

class SolutionOpt: public SolutionBase {
public:
    struct State {
        vector<int> s;

        explicit State(vector<int> s): s(std::move(s)) {};
        State(): s(vector<int>(SolutionOpt::num_terminals, 0)) {};
        void set(int i);
        void reset(int i);

        State operator+(const int& k);
        State operator-(const int& k);
        State& operator+=(const int& k);
        // ++s;
        State& operator++();
        // s++;
        const State operator++(int);
        State& operator-=(const int& k);
        // --s;
        State& operator--();
        // s--;
        const State operator--(int);
        State operator&(const State& other);
        State operator^(const State& other);
        bool operator==(const State& other) const {
            return s == other.s;
        }
        bool operator<(const State& rhs) const {
            return s < rhs.s;
        }
        bool operator<=(const State& rhs) const {
            return s <= rhs.s;
        }
        operator int() const {
            return std::any_of(s.begin(), s.end(),
                               [](int i) {return i > 0;});
        }
    };

    struct StateCompare {
    public:
        size_t operator()(const State& s) const {
            return boost::hash<vector<int>>()(s.s);
        }
    };

    struct StateEqual {
    public:
        bool operator()(const State& lhs, const State& rhs) const {
            return lhs.s == rhs.s;
        }
    };

    explicit SolutionOpt(const Graph& g);

    /* Reference:   https://www.cnblogs.com/nosta/p/10416451.html
     *              https://www.cnblogs.com/cqbzcsq/p/12903771.html
     * using dynamic programming
     * f[i,s] = min (f[i,t]  + f[i′,s−t] +e[i′,i])
     * f[i,s] = min (f[i′,s] + e[i′,i])
     * where f[i, s] denotes the cost of the tree rooted from node i with state s
     * where s denotes that which terminals are in the tree.
     */
    float solution() override;

private:
    vector<std::unordered_map<State, float, StateCompare, StateEqual>> dp;
    vector<int> terminals_index;
    queue<int> que;
    int num_nodes = 0;
    static int num_terminals;
};


#endif //STEINERTREE_SOLUTIONOPT_H
