cmake_minimum_required(VERSION 3.20)

project(his-dragon 
        VERSION 1.0
        LANGUAGES CXX

        )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(
    his-dragon
    src/main.cpp
)


target_include_directories(
    his-dragon
    PRIVATE
    ${LLVM_INCLUDE_DIRS}
    ${CLANG_INCLUDE_DIRS}
)

target_link_libraries(
        his-dragon
        PRIVATE
        clangTooling
        clangBasic
        clangAST
        clangFrontend
)