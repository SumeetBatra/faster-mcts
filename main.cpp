//
// Created by Sumeet Batra on 5/13/22.
//
#include "node.h"
#include "mcts.h"
#include <iostream>

static const int num_simulations = 1000000;


void play_game(MCTS mcts) {
    int x,y;
    char action;
    auto current_state = mcts.root;

    while(!current_state->terminal){
        current_state->print();
        std::cout << "Make a move: ";
        std::cin >> x >> y >> action;
        game_move m(x, y, action);
        current_state = mcts.take_action(current_state, m); // user makes a move
        if(!current_state->is_terminal()){
            current_state = mcts.best_child(current_state); // mcts makes a move
        }
    }
    std::cout << "GAME OVER!" << std::endl;
    current_state->print();

}


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
    root_ptr->unexpanded = root_ptr->get_actions();
    MCTS mcts(root_ptr);
    mcts.expand(root_ptr);

    for(int i = 0; i < num_simulations; ++i) {
        auto leaf = mcts.select();
        auto new_leaf = mcts.expand(leaf);
        auto terminal_node = mcts.simulate(new_leaf);
//        terminal_node->print();
        mcts.backprop(terminal_node);
    }

    std::cout << "Number of times select() hit a terminal state: " << mcts.terminal_count << " out of " <<
        num_simulations << " number of simulations " << 100.0 * (float)mcts.terminal_count / (float) num_simulations <<
        "%" << std::endl;

//    std::cout << "Simulating a game..." << std::endl;
//    auto curr = mcts.root;
//    while(!curr->is_terminal()){
//        auto best_child = mcts.best_child(curr);
//        best_child->print();
//        std::cout << "is terminal? " << best_child->is_terminal() << std::endl;
//        curr = best_child;
//    }

    play_game(mcts);

    return 1;
}