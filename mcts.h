#include "TicTacToe/ttt_node.h"
#include <random>
#ifndef MCTS_MCTS_H
#define MCTS_MCTS_H

class MCTS {

public:
    MCTS(std::shared_ptr<TTT_Node> &root);
    ~MCTS();

    std::shared_ptr<TTT_Node> root;

    std::shared_ptr<TTT_Node> select();
    std::shared_ptr<TTT_Node> expand(std::shared_ptr<TTT_Node> &leaf);
    std::shared_ptr<TTT_Node> take_action(const std::shared_ptr<TTT_Node> &current_node, TTT_Action& a); // for users to play against mcts
    static std::shared_ptr<TTT_Node> simulate(const std::shared_ptr<TTT_Node> &leaf);
    static std::shared_ptr<TTT_Node> best_child(const std::shared_ptr<TTT_Node> &node);
    static void backprop(std::shared_ptr<TTT_Node> &terminal);

    int terminal_count = 0;

private:
    double ucb(const std::shared_ptr<TTT_Node> &node);
    std::shared_ptr<TTT_Node> best_child_ucb(std::shared_ptr<TTT_Node> &curr);
    bool is_valid(const std::shared_ptr<TTT_Node> &current_node, TTT_Action& a);

    std::mt19937 rng;
    double explore_rate = sqrt(2.0);
};

#endif //MCTS_MCTS_H
