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
    void backprop(std::shared_ptr<Node> &terminal);

private:
    double ucb(const std::shared_ptr<Node> &node);
    std::shared_ptr<Node> best_child_ucb(std::shared_ptr<Node> &curr);

    std::mt19937 rng;
    double explore_rate = sqrt(2.0);
};

#endif //MCTS_MCTS_H
