//
// Created by Sumeet Batra on 5/13/22.
//

#include "node.h"
#include <iostream>

Node::Node(state s, game_move last_move,  std::shared_ptr<Node> parent, bool terminal): _state(s), last_move(last_move), parent(parent), terminal(terminal), rng((std::random_device())()){
    visit_count = 0;
    win_count = 0;
    avg_win_rate = 0;
    turn = last_move.action == 'x' ? 'o' : 'x'; // if the last move was an 'x', then it's o's turn in this node
};

Node::~Node() = default;;

const state Node::get_state() const{
    return _state;
}

std::vector<game_move> Node::get_actions() const {
    std::vector<game_move> actions;
    for(int i=0; i < board_row_size; ++i) {
        for(int j = 0; j < board_col_size; ++j) {
            if(_state[i][j] == '.'){
                actions.emplace_back(game_move(i, j, turn));
            }
        }
    }
    return actions;
}

game_move Node::random_action() {
    auto actions = get_actions();
    std::uniform_int_distribution<int> uni(0, actions.size()-1);
    int action_idx = uni(rng);
    return actions[action_idx];
}

bool Node::is_terminal() {
    // check if the game is over via draw or one side wins
    // draw = 0.5 score, win = +1, loss = -1
    // assume opponent is 'o' and we are 'x'?

    // check for draw -> board is full
    int empty_cell_count = 0;
    bool is_terminal = false;
    float score;

    for(int i = 0; i < board_row_size; ++i) {
        for(int j = 0; j < board_col_size; ++j) {
            if(_state[i][j] == '.') {
                empty_cell_count++;
            }
        }
    }
    if(!empty_cell_count) {
        score = draw;
        is_terminal = true;
    }

    //check for cross of either x's or o's

    //check row
    int last_move_row = last_move.x;
    char last_act = last_move.action;
    for(int j = 0; j < board_col_size; ++j) {
        if(_state[last_move_row][j] != last_act) {
            break;
        }
        if(j == board_col_size - 1) { // 3 x's or o's in a row
            score = last_move.action == turn ? win : loss;
            is_terminal = true;
        }
    }

    // check col
    int last_move_col = last_move.y;
    for(int i = 0; i < board_row_size; ++i) {
        if(_state[i][last_move_col] != last_act) {
            break;
        }
        if(i == board_row_size - 1) {
            score = last_move.action == turn ? win : loss;
            is_terminal = true;
        }
    }

    //check diagonal
    if(last_move.x == last_move.y) {
        for(int i = 0; i < board_row_size; ++i) {
            if(_state[i][i] != last_act) {
                break;
            }
            if(i == board_row_size - 1) {
                score = last_move.action == turn ? win : loss;
                is_terminal = true;
            }
        }
    }

    // check anti-diagonal
    if(last_move.x + last_move.y == board_row_size - 1){
        for(int i = 0; i < board_row_size; i++) {
            if(_state[i][(board_row_size-1)-i] != last_act){
                break;
            }
            if(i == board_row_size - 1) {
                score = last_move.action == turn ? win : loss;
                is_terminal = true;
            }
        }
    }
    win_count += score;
    this->terminal = is_terminal;
    end_game_result = game_result(terminal, score);
    return is_terminal;
}


void Node::print(){
    for(auto &row: _state) {
        for(auto & col: row){
            std::cout << col;
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}