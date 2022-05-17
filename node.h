//
// Created by Sumeet Batra on 5/13/22.
// Overfit to tic tac toe, will generalize to all games later
//
#include <vector>
#include <array>
#include <random>
#include <unordered_map>

#ifndef MCTS_NODE_H
#define MCTS_NODE_H

static const int board_row_size = 3;
static const int board_col_size = 3;

static const float win = 1.0;
static const float loss = -1.0;
static const float draw = 0.0;

typedef std::array<std::array<char, 3>, 3> state;

struct game_result{
    // result of a game after we reach a terminal state
    bool result;
    float score;
    game_result(bool t, float s): result(t), score(s){};
    ~game_result()= default;;
};

struct game_move { // cell of the 3 x 3 tic tac toe board
    std::uint8_t x, y;
    char action;
    game_move (std::uint8_t x, std::uint8_t  y, char act): x(x), y(y), action(act) {
        assert(x <= 2 && y <= 2);
        assert(action == 'x' || action == 'o');
    };
    ~game_move()= default;;
    // == operator in order to hash into unordered maps
    bool operator==(const game_move &other) const {
        return x == other.x && y == other.y && action == other.action;
    }
};

// this allows us to hash the game_move custom object as a key into unordered maps
namespace std {
    // https://stackoverflow.com/questions/17016175/c-unordered-map-using-a-custom-class-type-as-the-key
    template<>
    struct hash<game_move> {
        std::size_t operator()(const game_move &m) const {
            // compute individual hashes for x, y, and action
            size_t res = 17;
            res = res * 31 + hash<uint8_t>()(m.x);
            res = res * 31 + hash<uint8_t>()(m.y);
            res = res * 31 + hash<char>()(m.action);
            return res;
        }
    };
}

class Node {

public:
    int visit_count;
    float win_count;
    float avg_win_rate;
    bool terminal;
    char turn; // who's turn is it in this node
    game_move last_move; // the move taken to reach this state
    game_result end_game_result = game_result(NULL, NULL); // only terminal nodes will store this

    std::shared_ptr<Node> parent;
    std::unordered_map<game_move, std::shared_ptr<Node>> children;
    std::vector<game_move> unexpanded;

    Node(state s, game_move last_move, std::shared_ptr<Node> parent = nullptr, bool terminal = false);
    ~Node();

    const state get_state() const;
    std::vector<game_move> get_actions() const;
    game_move random_action();
    bool is_terminal();
    void print();

private:
    state _state; // 3 x 3 state of the game
    std::mt19937 rng;
};


#endif //MCTS_NODE_H
