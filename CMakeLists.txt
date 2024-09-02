cmake_minimum_required(VERSION 3.30)
project(SMW_EditorAndGame)

# set C++ version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# add level converter folder
add_subdirectory(editor)

# Add mario_app folder
add_subdirectory(game)