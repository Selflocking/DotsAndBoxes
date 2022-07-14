cmake_minimum_required(VERSION 3.22)
project(dots_and_boxs)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)
include_directories(C:/mingw64/include/python3.9)

add_executable(dots_and_boxs
        board.cpp
        board.h
        define.h
        fun.cpp
        main.cpp
        Node.cpp Node.h UCT.cpp fun.h UCT.h)