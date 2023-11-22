#ifndef NATIVE_FUNCS_H
#define NATIVE_FUNCS_H

#include "interpreter.h"
#include "lox_callable.h"
#include <string>
#include "types.h"
#include <vector>
#include <chrono>

typedef std::chrono::milliseconds milliseconds;

class Clock : public LoxCallable{
    public:
        int arity();

        rv call(
            Interpreter* interpreter,
            std::vector<rv> arguments
        );

        std::string toString();
};


#endif
