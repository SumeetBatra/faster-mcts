//
// Created by Sumeet Batra on 5/13/22.
//
#include "node.h"
#include "mcts.h"
#include <iostream>

static const int num_simulations = 1;


int main(int argc, char* argv[]) {
    std::array<std::array<char, 3>, 3> state {{
            {'.', '.', '.'},
            {'.', '.', '.'},
            {'.', '.', '.'},
    }};
    Node root(state, game_move(0, 0, 'o'));
    root.visit_count++;
    root.print();
    auto root_ptr = std::make_shared<Node>(root);
    MCTS mcts(root_ptr);
    mcts.expand(root_ptr);

    for(int i = 0; i < num_simulations; ++i) {
        auto leaf = mcts.select();
        auto new_leaf = mcts.expand(leaf);
        auto terminal_node = mcts.simulate(new_leaf);
//        terminal_node->print();
        mcts.backprop(terminal_node);
    }

    std::cout << "Simulating a game..." << std::endl;
    auto curr = mcts.root;
    while(curr){
        auto best_child = mcts.best_child(curr);
        best_child->print();
        std::cout << "is terminal? " << best_child->is_terminal() << std::endl;
        curr = best_child;
    }

    return 1;
}