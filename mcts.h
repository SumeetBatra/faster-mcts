//
// Created by Sumeet Batra on 5/30/22.
//
#include "node.h"
#include <random>

#ifndef MCTS_MCTS_H
#define MCTS_MCTS_H


template<class A, class S, class N>
class MCTS {
public:
    MCTS(std::shared_ptr<N>& root);
    virtual ~MCTS();

    std::shared_ptr<N> root;
    int terminal_count = 0;

    std::shared_ptr<N> select();
    std::shared_ptr<N> expand(std::shared_ptr<N>& leaf);
    std::shared_ptr<N> take_action(const std::shared_ptr<N>& current_node, A& action); // for users to play against mcts
    static std::shared_ptr<N> simulate(const std::shared_ptr<N>& leaf);
    static std::shared_ptr<N> best_child(const std::shared_ptr<N>& node);
    static void backprop(std::shared_ptr<N> &terminal);

private:
    double ucb(const std::shared_ptr<N>& node);
    std::shared_ptr<N> best_child_ucb(std::shared_ptr<N>& curr);
    bool is_valid(const std::shared_ptr<N>& current_node, A& action);

    std::mt19937 rng;
    double explore_rate = sqrt(2.0);

};


template<class A, class S, class N> MCTS<A, S, N>::MCTS(std::shared_ptr<N> &root): root(root),  rng((std::random_device())()){}
template<class A, class S, class N> MCTS<A, S, N>::~MCTS() = default;

template<class A, class S, class N> double MCTS<A, S, N>::ucb(const std::shared_ptr<N> &node) {
    auto exploit = (double)node->avg_win_rate;
    auto explore = explore_rate * sqrt( log((double)node->parent->visit_count) / ((double)node->visit_count + 1e-9) );
    return exploit + explore;
}

template<class A, class S, class N> std::shared_ptr<N> MCTS<A, S, N>::best_child_ucb(std::shared_ptr<N> &curr) {
    // the best child node using the ucb equation
    assert(!curr->children.empty());

    double best_score = -1e9;
    std::shared_ptr<N> best_child;

    for(const auto& [move, child]: curr->children) {
        double score = ucb(child);
        if(score > best_score){
            best_score = score;
            best_child = child;
        }
    }

    return best_child;
}

template<class A, class S, class N> std::shared_ptr<N> MCTS<A, S, N>::select() {
    auto current = root;
    while(current->unexpanded.empty() && !current->terminal) { // keep recursing down as long as the current node doesn't have unexpanded children
        auto best_ch = best_child_ucb(current);
        current = best_ch;
    }
    if(current->terminal) {
        terminal_count++;
    }
    return current;
}

template<class A, class S, class N> std::shared_ptr<N> MCTS<A, S, N>::expand(std::shared_ptr<N> &leaf) {
//     expand a child of the leaf node if an unexpanded child exists
    if(leaf->is_terminal()) {
        return leaf;
    }
    auto s = leaf->get_state();
    std::uniform_int_distribution<int> uni(0, leaf->unexpanded.size() - 1);
    auto action_idx = uni(rng);
    auto action = leaf->unexpanded[action_idx];
    leaf->unexpanded.erase(leaf->unexpanded.begin() + action_idx); // remove the used action from unexpanded since it will be expanded

    // construct the new child and add it to the children of leaf
    S new_state = s;
    new_state(action);
    A last_action = action;
    auto child = std::make_shared<N>(new_state, last_action, leaf);
    child->unexpanded = child->get_actions();
    leaf->children.insert(std::make_pair(last_action, child));
    return child;
}

template<class A, class S, class N> std::shared_ptr<N> MCTS<A, S, N>::simulate(const std::shared_ptr<N> &leaf) {
    auto current = leaf;
    while(!current->is_terminal()) {
        auto random_act = current->random_action();
        auto new_state = current->get_state();
        new_state(random_act);
        auto new_child = std::make_shared<N>(new_state, random_act, current);;
        current = new_child;
    }
    return current;
}

template<class A, class S, class N> void MCTS<A, S, N>::backprop(std::shared_ptr<N> &terminal) {
    auto winner = terminal->last_move.act;
    terminal->visit_count++;
    terminal->avg_win_rate = terminal->win_count / (float)terminal->visit_count;
    auto current = terminal->parent;
    auto score = terminal->end_game_result.score;
    while(current) {
        current->win_count += current->last_move.act == winner ? score : -score;
        current->visit_count++;
        current->avg_win_rate = current->win_count / (float)current->visit_count;
        current = current->parent;
    }
}

template<class A, class S, class N> std::shared_ptr<N> MCTS<A, S, N>::best_child(
        const std::shared_ptr<N> &node) {
    // return the best child node by node value only (used during test time)
    assert(!node->children.empty());
    using pair_type = typename decltype(node->children)::value_type; // not sure why we need the "typename" before decltype
    auto max  = std::max_element(node->children.begin(), node->children.end(),
                                 [](const pair_type &a, const pair_type &b)
                                 {
                                     return a.second->avg_win_rate < b.second->avg_win_rate;
                                 });
    return max->second;

}

template<class A, class S, class N> bool MCTS<A, S, N>::is_valid(const std::shared_ptr<N> &current_node, A &action) {
    return current_node->get_state().is_valid_action(action);
}

template<class A, class S, class N> std::shared_ptr<N> MCTS<A, S, N>::take_action(
        const std::shared_ptr<N> &current_node, A &action) {
    if(is_valid(current_node, action)){
        return current_node->children[action];
    }else{
        std::cout << "The provided move is not a valid one from this state. Please try again." << std::endl;
        return current_node;
    }
}


#endif //MCTS_MCTS_H
