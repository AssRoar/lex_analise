cmake_minimum_required(VERSION 3.14)
project(lex_analise)

set(CMAKE_CXX_STANDARD 14)

add_executable(lex_analise main.cpp token.h variables.h syntax.h lex.h error.h TID.h TIF.h)