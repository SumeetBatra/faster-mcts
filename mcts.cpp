#include "mcts.h"
#include "cmath"

#include <iostream>

MCTS::MCTS(std::shared_ptr<TTT_Node> &root): root(root), rng((std::random_device())()) {};
MCTS::~MCTS() = default;

double MCTS::ucb(const std::shared_ptr<TTT_Node> &node) {
    auto exploit = (double)node->avg_win_rate;
    auto explore = explore_rate * sqrt( log((double)node->parent->visit_count) / ((double)node->visit_count + 1e-9) );
    return exploit + explore;
}

std::shared_ptr<TTT_Node> MCTS::best_child_ucb(std::shared_ptr<TTT_Node> &curr){
    // the best child node using the ucb equation
    assert(!curr->children.empty());

    double best_score = -1e9;
    std::shared_ptr<TTT_Node> best_child;

    for(const auto & [move, child]: curr->children) {
        double score = ucb(child);
        if(score > best_score) {
            best_score = score;
            best_child = child;
        }
    }

    return best_child;
}

std::shared_ptr<TTT_Node> MCTS::select() {
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

std::shared_ptr<TTT_Node> MCTS::expand(std::shared_ptr<TTT_Node> &leaf) {
    // expand a child of the leaf node if an unexpanded child exists
    if(leaf->is_terminal()) {
        return leaf;
    }
    auto s = leaf->get_state();
    std::uniform_int_distribution<int> uni(0, leaf->unexpanded.size() - 1);
    auto action_idx = uni(rng);
    auto action = leaf->unexpanded[action_idx];
    leaf->unexpanded.erase(leaf->unexpanded.begin() + action_idx); // remove the used action from unexpanded since it will be expanded
    state new_state = s;
    new_state[action.move.x][action.move.y] = action.act;
    TTT_Action a(game_move(action.move.x, action.move.y), leaf->turn);
    auto child = std::make_shared<TTT_Node>(new_state, a, leaf);
    child->unexpanded = child->get_actions();
    leaf->children.insert(std::make_pair(a, child));
    return child;
}

std::shared_ptr<TTT_Node> MCTS::simulate(const std::shared_ptr<TTT_Node> &leaf) {
    auto current = leaf;
    while(!current->is_terminal()){
        auto random_act = current->random_action();
        auto new_state = current->get_state();
        new_state[random_act.move.x][random_act.move.y] = random_act.act;
        auto new_child = std::make_shared<TTT_Node>(new_state, random_act, current);
        current = new_child;
    }
    return current;
}

void MCTS::backprop(std::shared_ptr<TTT_Node> &terminal) {
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

std::shared_ptr<TTT_Node> MCTS::best_child(const std::shared_ptr<TTT_Node> &node) {
    // return the best child node by node value only (used during test time)
    assert(!node->children.empty());
    using pair_type = decltype(node->children)::value_type;
    auto max  = std::max_element(node->children.begin(), node->children.end(),
                                 [](const pair_type &a, const pair_type &b)
                                 {
                                    return a.second->avg_win_rate < b.second->avg_win_rate;
                                 });
    return max->second;
}

bool MCTS::is_valid(const std::shared_ptr<TTT_Node> &current_node, TTT_Action& a) {
    return current_node->get_state()[a.move.x][a.move.y] == '.';
}

std::shared_ptr<TTT_Node> MCTS::take_action(const std::shared_ptr<TTT_Node> &current_node, TTT_Action& a) {
    if(is_valid(current_node, a)){
        return current_node->children[a];
    }else{
        std::cout << "The provided move is not a valid one from this state. Please try again." << std::endl;
        return current_node;
    }
}
