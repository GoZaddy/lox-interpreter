#ifndef LOX_CLASS_H
#define LOX_CLASS_H

#include <string>
#include "lox_callable.h"

class LoxClass;

class LoxInstance{
    public:
        LoxInstance(LoxClass* klass);
        std::string toString();
};


class LoxClass: public LoxCallable{
    public:
        std::string name;

        LoxClass(std::string name);

        std::string toString();

        rv call(Interpreter* interpreter, std::vector<rv> arguments);

        int arity();
};

#endif