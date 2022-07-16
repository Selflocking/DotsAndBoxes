cmake_minimum_required(VERSION 3.22)
project(dots_and_boxs)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(dots_and_boxs
        main.cpp
        define.h
        board.cpp
        board.h
        fun.cpp
        fun.h
        Node.cpp
        Node.h
        UCT.cpp
        UCT.h)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--stack,1024000")