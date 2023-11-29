cmake_minimum_required(VERSION 3.26)
project(lox_interpreter)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(helper)

add_executable(lox_interpreter
        helper/custom_stack.h
        helper/custom_stack.tpp
        helper/object.cpp
        helper/object.h
        declr.h
        environment.cpp
        expr.cpp
        interpreter.cpp
        lox_class.cpp
        lox_function.cpp
        lox_instance.cpp
        main.cpp
        native_funcs.cpp
        parser.cpp
        resolver.cpp
        return.cpp
        scanner.cpp
        scanner.h
        stmt.cpp
        token.cpp
        token.h
        types.h
        util.cpp
        util.h)
