cmake_minimum_required(VERSION 3.22)
project(dots_and_boxs)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(dots_and_boxs
        board.cpp
        board.h
        define.h
        assess.cpp
        assess.h
        fun.cpp
        fun.h 
        main.cpp
        UCT.cpp UCT.h  )