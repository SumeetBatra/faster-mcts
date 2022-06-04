//
// Created by Sumeet Batra on 5/13/22.
//
#include "TicTacToe/ttt_node.h"
#include "TicTacToe/mcts_ttt.h"
#include "train.h"
#include <iostream>

static const int num_simulations = 1000000;


void play_game(MCTS_TTT mcts) {
    int x,y;
    char action;
    auto current_state = mcts.root;

    while(!current_state->terminal){
        current_state->print();
        std::cout << "Make a move: ";
        std::cin >> x >> y >> action;
        TTT_Action a(game_move(x, y), action);
        current_state = mcts.take_action(current_state, a); // user makes a move
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
    TTT_Node root(state, TTT_Action(game_move(0, 0), 'o'));
    root.visit_count++;
    root.print();
    auto root_ptr = std::make_shared<TTT_Node>(root);
    root_ptr->unexpanded = root_ptr->get_actions();
    MCTS_TTT mcts(root_ptr);
    train(mcts);

//    std::cout << "Number of times select() hit a terminal state: " << mcts.terminal_count << " out of " <<
//        num_simulations << " number of simulations " << 100.0 * (float)mcts.terminal_count / (float) num_simulations <<
//        "%" << std::endl;


    play_game(mcts);

    return 1;
}