cmake_minimum_required(VERSION 3.10)

add_compile_options(-O2)

project(SimpleSimplex)

include_directories(include)

add_library(SimpleSimplex
    include/fraction.h
    include/utils.h
    include/simple_simplex.h
    src/fraction.c
    src/utils.c
    src/simple_simplex.c
    )

add_executable(out
    src/main.c
    )

target_link_libraries(out SimpleSimplex)
