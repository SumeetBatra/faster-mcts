//
// Created by Sumeet Batra on 5/13/22.
// Overfit to tic tac toe, will generalize to all games later
//
#include <vector>
#include <array>
#include <random>
#include <unordered_map>
#include "../node.h"

#ifndef MCTS_NODE_H
#define MCTS_NODE_H

static const int board_row_size = 3;
static const int board_col_size = 3;

static const float win = 1.0;
static const float loss = -1.0;
static const float draw = 0.0;

typedef std::array<std::array<char, 3>, 3> state;

//struct game_move { // cell of the 3 x 3 tic tac toe board
//    std::uint8_t x, y;
//    char action;
//    game_move (std::uint8_t x, std::uint8_t  y, char act): x(x), y(y), action(act) {
//        assert(x <= 2 && y <= 2);
//        assert(action == 'x' || action == 'o');
//    };
//    ~game_move()= default;;
//    // == operator in order to hash into unordered maps
//    bool operator==(const game_move &other) const {
//        return x == other.x && y == other.y && action == other.action;
//    }
//};

class TTT_State;

struct game_move{
    std::uint8_t x, y;
    game_move(std::uint8_t x, std::uint8_t y): x(x), y(y){
        assert(x <= 2 && y <= 2);
    };
    ~game_move() = default;
};

class TTT_Action: public Action<TTT_State>{
public:
    char act;
    game_move move;

    TTT_Action(game_move m, char act);
    ~TTT_Action() override;

    void execute(TTT_State& state) override;
    void print() override;
    bool operator==(const TTT_Action& other) const;
};

class TTT_State: public State<state>{
public:
    TTT_State(state s);
    ~TTT_State() = default;

    void operator()(TTT_Action& a);
    void print() override;
    state get_state() const { return _state; }

private:
    state _state;
};

// this allows us to hash the game_move custom object as a key into unordered maps
namespace std {
    // https://stackoverflow.com/questions/17016175/c-unordered-map-using-a-custom-class-type-as-the-key
    template<>
    struct hash<TTT_Action> {
        std::size_t operator()(const TTT_Action &a) const {
            // compute individual hashes for x, y, and action
            size_t res = 17;
            res = res * 31 + hash<uint8_t>()(a.move.x);
            res = res * 31 + hash<uint8_t>()(a.move.y);
            res = res * 31 + hash<char>()(a.act);
            return res;
        }
    };
}

class TTT_Node {

public:
    int visit_count;
    float win_count;
    float avg_win_rate;
    bool terminal;
    char turn; // who's turn is it in this node
    TTT_Action last_move; // the move taken to reach this state
    game_result end_game_result = game_result(NULL, NULL); // only terminal nodes will store this

    std::shared_ptr<TTT_Node> parent;
    std::unordered_map<TTT_Action, std::shared_ptr<TTT_Node>> children;
    std::vector<TTT_Action> unexpanded;

    TTT_Node(TTT_State s, TTT_Action last_move, std::shared_ptr<TTT_Node> parent = nullptr, bool terminal = false);
    ~TTT_Node();

    const state get_state() const;
    std::vector<TTT_Action> get_actions() const;
    TTT_Action random_action();
    bool is_terminal();
    void print();

private:
    TTT_State _state; // 3 x 3 state of the game
    std::mt19937 rng;
};


#endif //MCTS_NODE_H
