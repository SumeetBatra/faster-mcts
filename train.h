//
// Created by Sumeet Batra on 6/3/22.
//
#include "TicTacToe/ttt_node.h"
#include "TicTacToe/mcts_ttt.h"
#include "mcts.h"
#include <thread>
#include <chrono>
#ifndef MCTS_TRAIN_H
#define MCTS_TRAIN_H

template <typename MCTS>
struct args{
    // args for the mcts simulate() method
    args(MCTS& mcts, int& num_simulations): mcts(mcts), num_simulations(num_simulations){};
    MCTS mcts;
    int num_simulations;
};


template <typename MCTS>
void simulate(args<MCTS> a) {
    auto mcts = a.mcts;
    int num_simulations = a.num_simulations;
    for(int i = 0; i < num_simulations; ++i) {
        auto leaf = mcts.select() ;
        auto new_leaf = mcts.expand(leaf);
        auto terminal_node = mcts.simulate(new_leaf);
        mcts.backprop(terminal_node);
    }
}

template <typename MCTS>
void train(MCTS mcts, int num_simulations=500000, int num_workers=2) {
    auto start = std::chrono::high_resolution_clock::now();
    auto root_ptr = mcts.root;
    root_ptr->visit_count++;
    root_ptr->unexpanded = root_ptr->get_actions();
    mcts.expand(root_ptr); // first expand the root node

    // create the worker threads and run the simulations
    for (int t = 0; t < num_workers; ++t) {
        args<MCTS> arguments(std::ref(mcts), std::ref(num_simulations));
        auto th = std::thread(simulate<MCTS>, arguments);
        th.join();
    }

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Duration: " << duration.count() << " microseconds." << std::endl;
}


#endif //MCTS_TRAIN_H
