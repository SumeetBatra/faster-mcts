#include "node.h"
#include <random>
#ifndef MCTS_MCTS_H
#define MCTS_MCTS_H

class MCTS {

public:
    MCTS(std::shared_ptr<Node> &root);
    ~MCTS();

    std::shared_ptr<Node> root;

    std::shared_ptr<Node> select();
    std::shared_ptr<Node> expand(std::shared_ptr<Node> &leaf);
    std::shared_ptr<Node> simulate(const std::shared_ptr<Node> &leaf);
    std::shared_ptr<Node> best_child(const std::shared_ptr<Node> &node);
    std::shared_ptr<Node> take_action(const std::shared_ptr<Node> &current_node, game_move m); // for users to play against mcts
    void backprop(std::shared_ptr<Node> &terminal);

    int terminal_count = 0;

private:
    double ucb(const std::shared_ptr<Node> &node);
    std::shared_ptr<Node> best_child_ucb(std::shared_ptr<Node> &curr);
    bool is_valid(const std::shared_ptr<Node> &current_node, game_move m);

    std::mt19937 rng;
    double explore_rate = sqrt(2.0);
};

#endif //MCTS_MCTS_H
