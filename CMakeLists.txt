cmake_minimum_required(VERSION 3.10)
project(IlocSim)

set(CMAKE_CXX_STANDARD 11)

add_executable(IlocSim main.cpp main.h iloc.cpp iloc.h)