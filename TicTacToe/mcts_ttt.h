#include "../mcts.h"
#include "ttt_node.h"
#include <random>
#ifndef MCTS_MCTS_TTT_H
#define MCTS_MCTS_TTT_H

class MCTS_TTT: public MCTS<TTT_Action, TTT_State, TTT_Node> {

public:
    MCTS_TTT(std::shared_ptr<TTT_Node> &root);
    ~MCTS_TTT();

    std::shared_ptr<TTT_Node> root;

private:
    std::mt19937 rng;
};

#endif //MCTS_MCTS_TTT_H
