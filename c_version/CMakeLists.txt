cmake_minimum_required(VERSION 3.10)

project(SimpleSimplex)

include_directories(include)

add_library(SimpleSimplex
    include/fraction.h
    src/fraction.c
    )

add_executable(out
    src/main.c
    )

target_link_libraries(out SimpleSimplex)
