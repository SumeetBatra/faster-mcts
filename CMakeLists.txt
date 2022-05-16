# cmake_minimum_required(VERSION <specify CMake version here>)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
project(mcts)

set(CMAKE_CXX_STANDARD 14)

add_library(mcts mcts.cpp mcts.h node.cpp node.h)

add_executable(main main.cpp)

target_link_libraries(main PUBLIC mcts)