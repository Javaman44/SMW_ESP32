cmake_minimum_required(VERSION 3.30)
project(SMW_EditorAndGame)

# used by sonarlint
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set C++ version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Inclure FetchContent
include(FetchContent)

# add level converter folder
add_subdirectory(editor)

# Add mario_app folder
add_subdirectory(game)