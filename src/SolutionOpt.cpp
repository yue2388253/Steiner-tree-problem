//
// Created by lam on 2020/8/27.
//

#include "SolutionOpt.h"

using namespace std;
using namespace boost;

const int dx[] = {-1, 1, 0, 0};
const int dy[] = {0, 0, -1, 1};

int SolutionOpt::num_terminals = 0;

void SolutionOpt::State::set(int i) {
    assert(i < s.size());
    s[i] = 1;
}

void SolutionOpt::State::reset(int i) {
    assert(i < s.size());
    s[i] = 0;
}

SolutionOpt::State SolutionOpt::State::operator+(const int &k) {
    assert(k >= 0);
    SolutionOpt::State res = *this;
    int carry = k;
    for (int i = res.s.size() - 1; i >= 0 && carry > 0; --i) {
        res.s[i] += carry;
        carry = res.s[i] / 2;
        res.s[i] %= 2;
    }
    assert(carry == 0);
    return res;
}

SolutionOpt::State SolutionOpt::State::operator-(const int &k) {
    assert(k >= 0);
    SolutionOpt::State res = *this;
    int carry = k;
    for (int i = res.s.size() - 1; i >= 0 && carry > 0; --i) {
        res.s[i] -= carry;
        carry = (abs(res.s[i]) + 1) / 2;
        res.s[i] = abs(res.s[i]) % 2;
    }
    assert(carry == 0);
    return res;
}

SolutionOpt::State &SolutionOpt::State::operator+=(const int &k) {
    *this = *this + k;
    return *this;
}

SolutionOpt::State &SolutionOpt::State::operator-=(const int &k) {
    *this = *this - k;
    return *this;
}

SolutionOpt::State &SolutionOpt::State::operator++() {
    *this += 1;
    return *this;
}

SolutionOpt::State &SolutionOpt::State::operator--() {
    *this -= 1;
    return *this;
}

const SolutionOpt::State SolutionOpt::State::operator++(int) {
    SolutionOpt::State res = *this;
    ++*this;
    return res;
}

const SolutionOpt::State SolutionOpt::State::operator--(int) {
    SolutionOpt::State res = *this;
    --*this;
    return res;
}

SolutionOpt::State SolutionOpt::State::operator&(const State& other) {
    assert(this->s.size() == other.s.size());
    SolutionOpt::State res = *this;
    for (int i = 0; i < this->s.size(); ++i) {
        res.s[i] = this->s[i] & other.s[i];
    }
    return res;
}

SolutionOpt::State SolutionOpt::State::operator^(const State& other) {
    assert(this->s.size() == other.s.size());
    SolutionOpt::State res = *this;
    for (int i = 0; i < this->s.size(); ++i) {
        res.s[i] = this->s[i] ^ other.s[i];
    }
    return res;
}

SolutionOpt::SolutionOpt(const Graph &graph): SolutionBase(graph) {
    num_nodes = num_vertices(g);
    auto terminals = get(vertex_terminal_t(), g);
    for (int i = 0; i < num_nodes; ++i) {
        if (terminals[i]) {
            num_terminals++;
            terminals_index.push_back(i);
        }
    }
    dp.clear();
    dp.resize(num_nodes);
    for (int i = 0; i < num_terminals; ++i) {
        int index = terminals[i];
        State s;
        s.set(i);
        dp[index][s] = 0;
    }
}

float SolutionOpt::solution() {
    // TODO
//    State all(vector<int>(num_terminals, 1));
//
//    State s;
//    for (s.set(0); s <= all; ++s) {
//        for (int i = 0; i < num_nodes; ++i) {
//            for (State t = (s-1) & s; t; t = (t-1) & s) {
//                if (dp[i].find(t) != dp[i].end() &&
//                        dp[i].find(s^t) != dp[i].end()) {
//                    float tmp = dp[i][t] + dp[i][s^t] - vertex_weights[i];
//                    if (dp[i].find(s) != dp[i].end()) {
//                        dp[i][s] = min(dp[i][s], tmp);
//                    } else {
//                        dp[i][s] = tmp;
//                    }
//                }
//            }
//        }
//    }

    return 1;
}