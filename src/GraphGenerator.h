//
// Created by lam on 2020/8/30.
//

#ifndef STEINERTREE_GRAPHGENERATOR_H
#define STEINERTREE_GRAPHGENERATOR_H

#include "type_define.h"

using namespace boost;
using namespace std;


std::shared_ptr<Graph> generate_graph(int num_nodes, int num_terminals);
std::shared_ptr<Graph> create_my_graph();


#endif //STEINERTREE_GRAPHGENERATOR_H
