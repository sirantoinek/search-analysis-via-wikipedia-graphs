cmake_minimum_required(VERSION 3.28)
project(Search_Analysis_via_Wikipedia_Graphs)

set(CMAKE_CXX_STANDARD 14)

add_executable(Search_Analysis_via_Wikipedia_Graphs
        src/main.cpp
        src/Algorithms.h
        src/Gui.h
        src/WikiDatabase.cpp)