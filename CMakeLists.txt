cmake_minimum_required(VERSION 3.15)
project(assembler VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(assembler src/main.cpp src/lex.cpp src/parser.cpp)
