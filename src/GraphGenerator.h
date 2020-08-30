//
// Created by lam on 2020/8/30.
//

#ifndef STEINERTREE_GRAPHGENERATOR_H
#define STEINERTREE_GRAPHGENERATOR_H

#include "type_define.h"

using namespace boost;
using namespace std;

class GraphGenerator {
public:
    static std::shared_ptr<Graph> generate_graph(int num_nodes, int num_terminals);
    static std::shared_ptr<Graph> create_my_graph();
};


#endif //STEINERTREE_GRAPHGENERATOR_H
