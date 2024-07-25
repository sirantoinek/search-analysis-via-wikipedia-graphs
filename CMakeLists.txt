cmake_minimum_required(VERSION 3.28)
project(search_analysis_via_wikipedia_graphs)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(search_analysis_via_wikipedia_graphs
        src/main.cpp
        src/Algorithms.h
        src/Gui.h
        src/WikiDatabase.cpp
        src/imgui_combo_autoselect.cpp
        src/imgui_combo_autoselect.cpp)

find_package(glad CONFIG REQUIRED)
target_link_libraries(search_analysis_via_wikipedia_graphs PUBLIC glad::glad)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(search_analysis_via_wikipedia_graphs PUBLIC glfw)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(search_analysis_via_wikipedia_graphs PUBLIC imgui::imgui)

