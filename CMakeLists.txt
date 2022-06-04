# cmake_minimum_required(VERSION <specify CMake version here>)
cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
project(mcts)

set(CMAKE_CXX_STANDARD 14)

add_library(mcts_base SHARED mcts.h node.cpp node.h)
add_library(mcts_ttt SHARED TicTacToe/mcts_ttt.cpp TicTacToe/mcts_ttt.h TicTacToe/ttt_node.cpp TicTacToe/ttt_node.h)
add_dependencies(mcts_ttt mcts_base)

add_executable(main main.cpp mcts.h train.h)

target_link_libraries(main PUBLIC mcts_ttt mcts_base)