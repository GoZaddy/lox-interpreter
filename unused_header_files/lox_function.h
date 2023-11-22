#ifndef LOX_FUNCTION_H
#define LOX_FUNCTION_H

#include "lox_callable.h"
#include "stmt.cpp"
#include "types.h"
#include "interpreter.h"

class LoxFunction: public LoxCallable{
    private:
        Environment* closure;
    public:
        Functionvp declaration;
        

        LoxFunction(Functionvp declaration, Environment* closure);

        rv call(Interpreter* interpreter, std::vector<rv> arguments);

        int arity();

        std::string toString();
};

#endif