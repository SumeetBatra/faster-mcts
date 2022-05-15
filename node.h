//
// Created by Sumeet Batra on 5/13/22.
// Overfit to tic tac toe, will generalize to all games later
//
#include <vector>
#include <array>
#include <random>

#ifndef MCTS_NODE_H
#define MCTS_NODE_H

static const int board_row_size = 3;
static const int board_col_size = 3;

typedef std::array<std::array<char, 3>, 3> state;


struct move { // cell of the 3 x 3 tic tac toe board
    std::uint8_t x, y;
    char action;
    move (std::uint8_t x, std::uint8_t  y, char act): x(x), y(y), action(act) {
        assert(x <= 2 && y <= 2);
        assert(action == 'x' || action == 'o');
    };
    ~move(){};
};

class Node {

public:
    int visit_count;
    float win_count;
    float avg_win_rate;
    bool terminal;
    char turn; // who's turn is it in this node
    move last_move; // the move taken to reach this state

    std::shared_ptr<Node> parent;
    std::vector<std::shared_ptr<Node>> children;

    Node(state s, move last_move, std::shared_ptr<Node> parent = nullptr, bool terminal = false);
    ~Node();

    const state get_state() const;
    std::vector<move> get_actions() const;
    move random_action();
    bool is_terminal();
    void print();

private:
    state _state; // 3 x 3 state of the game
    std::mt19937 rng;
};


#endif //MCTS_NODE_H
