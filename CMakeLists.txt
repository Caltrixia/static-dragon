cmake_minimum_required(VERSION 3.20)

project(static-dragon 
        VERSION 1.0
        LANGUAGES CXX C

        )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(LLVM_DIR "/usr/lib/llvm-14/lib/cmake/llvm")
set(Clang_DIR "/usr/lib/llvm-14/lib/cmake/clang")

find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)

message(STATUS "LLVM version: ${LLVM_PACKAGE_VERSION}")
message(STATUS "LLVM dir: ${LLVM_DIR}")
message(STATUS "Clang dir: ${Clang_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${CLANG_INCLUDE_DIRS})

add_definitions(${LLVM_DEFINITIONS})

add_executable(
    static-dragon
    src/main.cpp
    src/SDASTConsumer.cpp
    src/SDFrontendAction.cpp
    src/SDRecursiveASTVisitor.cpp
    src/SDFunctionBodyVisitor.cpp
    src/SDReportGenerator.cpp
    src/SDFrondendActionFactory.cpp
    src/SDMetricsCollector.cpp
)

target_link_libraries(static-dragon
                    PRIVATE
                    clangTooling
                    clangFrontend
                    clangAST
                    clangASTMatchers
                    clangBasic
                    clangLex
)

target_include_directories(
    static-dragon
    PRIVATE
    ${LLVM_INCLUDE_DIRS}
    ${CLANG_INCLUDE_DIRS}
    include
)

target_compile_definitions(static-dragon PRIVATE PROJECT_ROOT="${CMAKE_SOURCE_DIR}")