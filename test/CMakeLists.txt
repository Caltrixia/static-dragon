cmake_minimum_required(VERSION 3.20)

project(StaticDragonTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(test1 test1.cpp)
add_executable(test2 test2.cpp)