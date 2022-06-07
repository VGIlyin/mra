cmake_minimum_required(VERSION 3.21)
project(gosling)

set(CMAKE_CXX_STANDARD 14)

add_executable(gosling main.cpp lexer.cpp lexer.h token.cpp token.h Parser.cpp Parser.h)
