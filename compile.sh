#!/bin/bash
g++ -w -std=c++11 helper/object.cpp main.cpp scanner.cpp token.cpp util.cpp parser.cpp expr.cpp stmt.cpp interpreter.cpp return.cpp lox_function.cpp native_funcs.cpp environment.cpp resolver.cpp lox_class.cpp lox_instance.cpp lox_callable.cpp
