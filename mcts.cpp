#include "mcts.h"
#include "cmath"

#include <iostream>

MCTS::MCTS(std::shared_ptr<Node> &root): root(root), rng((std::random_device())()) {};
MCTS::~MCTS() = default;

double MCTS::ucb(std::shared_ptr<Node> &node) {
    auto score = node->avg_win_rate + explore_rate * sqrt( log((float)node->parent->visit_count) / ((float)node->visit_count + 1e-9) );
    return score;
}

std::shared_ptr<Node> MCTS::best_child_ucb(std::shared_ptr<Node> &curr){
    // the best child node using the ucb equation
    assert(!curr->children.empty());

    double best_score = -1e9;
    std::shared_ptr<Node> best_child;
    for(auto&& child: curr->children) {
        double score = ucb(child);
        if(score > best_score) {
            best_score = score;
            best_child = child;
        }
    }
    return best_child;
}

std::shared_ptr<Node> MCTS::select() {
    auto curr_node = root;
    while(!curr_node->children.empty()){
        auto best_ch = best_child(curr_node);
        curr_node = best_ch;
    }
    return curr_node;
}

std::shared_ptr<Node> MCTS::expand(std::shared_ptr<Node> &leaf) {
    // expand all the children of the leaf node
    auto s = leaf->get_state();
    auto actions = leaf->get_actions();
    for(auto&& action: actions){
        state new_state = s;
        new_state[action.x][action.y] = leaf->turn;
        auto child = std::make_unique<Node>(new_state, move(action.x, action.y, leaf->turn), leaf);
        leaf->children.emplace_back(std::move(child));
    }

    // select a random child node of the leaf node
    int num_children = leaf->children.size();
    std::uniform_int_distribution<int> uni(0, num_children-1);
    auto child_idx = uni(rng);
    return leaf->children[child_idx];
}

std::shared_ptr<Node> MCTS::simulate(const std::shared_ptr<Node> &leaf) {
    auto current = leaf;
    while(!current->is_terminal()){
        auto random_act = current->random_action();
        auto new_state = current->get_state();
        new_state[random_act.x][random_act.y] = random_act.action;
        auto new_child = std::make_shared<Node>(new_state, random_act, current);
        current = new_child;
    }
    return current;
}

void MCTS::backprop(std::shared_ptr<Node> &terminal) {
    auto winner = terminal->last_move.action;
    terminal->visit_count++;
    terminal->avg_win_rate = terminal->win_count / (float)terminal->visit_count;
    auto current = terminal->parent;
    auto val = terminal->win_count;
    while(current) {
        if(val == 0.5) { // draw, give 0.5 to each player
            current->win_count += val;
        }else{ // give +1 only to the winning player
            current->win_count += winner == current->last_move.action ? val : 0.0;
        }
        current->win_count += val;
        current->visit_count++;
        current->avg_win_rate = current->win_count / (float)current->visit_count;
        current = current->parent;
    }
}

std::shared_ptr<Node> MCTS::best_child(const std::shared_ptr<Node> &node) {
    // return the best child node by node value only (used during test time)
    assert(!node->children.empty());
    auto max  = std::max_element(node->children.begin(), node->children.end(),
                                 [](const std::shared_ptr<Node> &a, const std::shared_ptr<Node> &b)
                                 {
                                    return a->avg_win_rate < b->avg_win_rate;
                                 });
    return *max;
}
