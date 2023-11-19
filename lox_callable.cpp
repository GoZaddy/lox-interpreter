#ifndef LOX_CALLABLE_H
#define LOX_CALLABLE_H

#include <string>
#include <vector>
#include "types.h"
#include "interpreter.cpp"

class LoxCallable{
    public:
        virtual int arity() = 0;
		virtual rv call(Interpreter::Interpreter interpreter, std::vector<rv> arguments) = 0;
};

#endif