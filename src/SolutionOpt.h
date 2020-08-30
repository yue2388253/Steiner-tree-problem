//
// Created by lam on 2020/8/27.
//

#ifndef STEINERTREE_SOLUTIONOPT_H
#define STEINERTREE_SOLUTIONOPT_H

#include "type_define.h"
#include "SolutionBase.h"

class SolutionOpt: public SolutionBase {
public:
    explicit SolutionOpt(const Graph& g): SolutionBase(g) {};
    unsigned int optimal_solution();
};


#endif //STEINERTREE_SOLUTIONOPT_H
