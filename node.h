//
// Created by Sumeet Batra on 5/23/22.
//
#include <iostream>
#include <vector>
#include <unordered_map>
#include <random>

#ifndef MCTS_ABSTRACT_NODE_H
#define MCTS_ABSTRACT_NODE_H


struct game_result{
    // result of a game after we reach a terminal state
    bool result;
    float score;
    game_result(bool t, float s): result(t), score(s){};
    ~game_result()= default;;
};


class Printable{
    virtual void print() const = 0;
};

template<typename T, typename A>
class State: protected Printable{
    friend std::ostream& operator<<(std::ostream& stream, State<T, A> s){
        s.print(stream);
        return stream ;
    }
public:
    virtual void operator()(const A& action) = 0;
    virtual bool is_valid_action(const A& action) const = 0;
};

template<typename T>
class Action: protected Printable{
    friend std::ostream& operator<<(std::ostream& stream, Action<T>& a) {
        a.print(stream);
        return stream;
    }

public:
    virtual void execute(T& state) = 0;
//    Action(Action<T>& other) = 0; // copy constructor
    virtual ~Action() = default;
};

template<class A, class S>
class Node: protected Printable {
public:
    int visit_count;
    float win_count;
    float avg_win_rate;
    bool terminal;
    char turn; // who's turn is it in this node
    A last_action; // the action taken to reach this state
    game_result end_game_result = game_result(NULL, NULL); // only terminal nodes will store this

    std::shared_ptr<Node<A, S>> parent;
    std::unordered_map<A, std::shared_ptr<Node<A, S>>> children;
    std::vector<A> unexpanded;

    Node<A, S>(S state, A last_move, std::shared_ptr<Node<A,S>> parent = nullptr, bool terminal = false);
    virtual ~Node();
    virtual std::vector<A> get_actions() const = 0;
    virtual A random_action() = 0;
    virtual bool is_terminal() = 0;

    const S get_state() const;

private:
    S _state;
    std::mt19937 rng;
};

/*
 *  Node member function implementations
 */

template<class A, class S>Node<A,S>::Node(S state, A last_move, std::shared_ptr<Node<A,S>> parent, bool terminal):
    _state(state), last_action(last_move), parent(parent), terminal(terminal), rng((std::random_device())()) {
        visit_count = 0;
        win_count = 0;
        avg_win_rate = 0;
    };

template<class A, class S>Node<A,S>::~Node() = default;

template<class A, class S> const S Node<A,S>::get_state() const {
    return _state;
}



#endif //MCTS_ABSTRACT_NODE_H
