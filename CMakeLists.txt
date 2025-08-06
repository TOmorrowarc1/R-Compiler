cmake_minimum_required(VERSION 3.10)
project(RCompiler)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

set(EXECUTABLE
  main.cpp
  src/Lexer/lexer.cpp
)

set(SOURCES_MAIN
  header/Lexer
)

add_executable(code ${EXECUTABLE})

target_include_directories(code PUBLIC ${SOURCES_MAIN})
