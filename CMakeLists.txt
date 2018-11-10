cmake_minimum_required(VERSION 3.10)
project(UROP)

set(CMAKE_CXX_STANDARD 11)

add_executable(UROP main.cpp allocation.cpp allocation.h layer.cpp layer.h)