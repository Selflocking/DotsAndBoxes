cmake_minimum_required(VERSION 3.22)
project(dots_and_boxs)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(dots_and_boxs
        board.cpp
        board.h
        define.h
        fun.h
        main.cpp
        stupidAI.h)