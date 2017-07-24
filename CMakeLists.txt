cmake_minimum_required(VERSION 3.7)

set(CMAKE_C_STANDARD 11)

project(scheme)

set(SOURCE_FILES src/scm.h src/number.c src/bool.c src/list.c src/print.c src/main.c)

add_executable(scheme ${SOURCE_FILES})