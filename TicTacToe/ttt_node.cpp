//
// Created by Sumeet Batra on 5/13/22.
//

#include "ttt_node.h"
#include <iostream>




/*
 * TTT_State member function implementations
 */

TTT_State::TTT_State(state s): _state(s) {}



void TTT_State::operator()(TTT_Action &a) {
    assert(_state[a.move.x][a.move.y] == '.');
    _state[a.move.x][a.move.y] = a.act;
}

bool TTT_State::is_valid_action(TTT_Action &action) const {
    return _state[action.move.x][action.move.y] == '.';
}

void TTT_State::print(){
    for(auto &row: _state) {
        for(auto & col: row){
            std::cout << col;
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}


/*
 * TTT_Action member function implementations
 */

TTT_Action::TTT_Action(game_move m, char act): move(m), act(act) {}

TTT_Action::TTT_Action(const TTT_Action &other): move(other.move), act(other.act) {}

TTT_Action::~TTT_Action() noexcept {}

void TTT_Action::execute(TTT_State& state) {
    state(*this);
}

void TTT_Action::print() {
    std::cout << "x y turn: " << move.x << " " << move.y << " " << act << std::endl;
}

bool TTT_Action::operator==(const TTT_Action &other) const {
    return move.x == other.move.x && move.y == other.move.y && act == other.act;
}



/*
 * TTT_Node member function implementations
 */




TTT_Node::TTT_Node(TTT_State s, TTT_Action last_move, std::shared_ptr<TTT_Node> parent, bool terminal):
Node<TTT_Action, TTT_State>(s, last_move, parent, terminal), _state(s), last_move(last_move), parent(parent), terminal(terminal){
    visit_count = 0;
    win_count = 0;
    avg_win_rate = 0;
    turn = last_move.act == 'x' ? 'o' : 'x'; // if the last move was an 'x', then it'TTT_Node o'TTT_Node turn in this node
};

TTT_Node::~TTT_Node() = default;;

std::vector<TTT_Action> TTT_Node::get_actions() const {
    std::vector<TTT_Action> actions;
    for(int i=0; i < board_row_size; ++i) {
        for(int j = 0; j < board_col_size; ++j) {
            if(_state.get_state()[i][j] == '.'){
                actions.emplace_back(TTT_Action(game_move(i, j), turn));
//                actions.emplace_back(game_move(i, j, turn));
            }
        }
    }
    return actions;
}

TTT_Action TTT_Node::random_action() {
    auto actions = get_actions();
    std::uniform_int_distribution<int> uni(0, actions.size()-1);
    int action_idx = uni(rng);
    return actions[action_idx];
}

bool TTT_Node::is_terminal() {
    // check if the game is over via draw or one side wins
    // draw = 0.5 score, win = +1, loss = -1
    // assume opponent is 'o' and we are 'x'?

    // check for draw -> board is full
    int empty_cell_count = 0;
    bool is_terminal = false;
    float score;

    for(int i = 0; i < board_row_size; ++i) {
        for(int j = 0; j < board_col_size; ++j) {
            if(_state.get_state()[i][j] == '.') {
                empty_cell_count++;
            }
        }
    }
    if(!empty_cell_count) {
        score = draw;
        is_terminal = true;
    }

    //check for cross of either x'TTT_Node or o'TTT_Node

    //check row
    int last_move_row = last_move.move.x;
    char last_act = last_move.act;
    for(int j = 0; j < board_col_size; ++j) {
        if(_state.get_state()[last_move_row][j] != last_act) {
            break;
        }
        if(j == board_col_size - 1) { // 3 x'TTT_Node or o'TTT_Node in a row
            score = win;
            is_terminal = true;
        }
    }

    // check col
    int last_move_col = last_move.move.y;
    for(int i = 0; i < board_row_size; ++i) {
        if(_state.get_state()[i][last_move_col] != last_act) {
            break;
        }
        if(i == board_row_size - 1) {
            score = win;
            is_terminal = true;
        }
    }

    //check diagonal
    if(last_move.move.x == last_move.move.y) {
        for(int i = 0; i < board_row_size; ++i) {
            if(_state.get_state()[i][i] != last_act) {
                break;
            }
            if(i == board_row_size - 1) {
                score = win;
                is_terminal = true;
            }
        }
    }

    // check anti-diagonal
    if(last_move.move.x + last_move.move.y == board_row_size - 1){
        for(int i = 0; i < board_row_size; i++) {
            if(_state.get_state()[i][(board_row_size-1)-i] != last_act){
                break;
            }
            if(i == board_row_size - 1) {
                score = win;
                is_terminal = true;
            }
        }
    }
    win_count += score;
    this->terminal = is_terminal;
    end_game_result = game_result(terminal, score);
    return is_terminal;
}

void TTT_Node::print(){
    _state.print();
}
