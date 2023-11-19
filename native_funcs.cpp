#ifndef NATIVE_FUNCS
#define NATIVE_FUNCS

#include "lox_callable.cpp"
#include <string>
#include "types.h"
#include "interpreter.cpp"
#include <vector>
#include <chrono>

typedef std::chrono::milliseconds milliseconds;

class Clock : public LoxCallable{
    public:
        int arity() { return 0; }

        rv call(
            Interpreter::Interpreter interpreter,
            std::vector<rv> arguments
        ) {
            milliseconds ms = std::chrono::duration_cast< milliseconds >(
                std::chrono::system_clock::now().time_since_epoch()
            );
            return std::to_string(ms.count());
        }

        std::string toString() { return "<native fn>"; }
};


#endif
