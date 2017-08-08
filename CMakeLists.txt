cmake_minimum_required(VERSION 3.7)

set(CMAKE_C_STANDARD 11)

project(scheme)

set(SOURCE_FILES src/scm.h src/list.c src/bool.c src/number.c src/port.c src/read.c src/print.c src/eval.c src/main.c src/list.h src/port.h src/bool.h src/read.h src/symbol.h src/symbol.c src/eval.h src/print.h src/number.h src/char.h src/char.c src/str.h src/str.c src/env.c src/error.h src/error.c src/system.h src/system.c src/fun.h src/fun.c src/pcc32.h src/pcc32.c src/lib/builtinlib.h src/lib/builtinlib.c src/lib/libpcc32.h src/lib/libpcc32.c)

add_executable(scheme ${SOURCE_FILES})