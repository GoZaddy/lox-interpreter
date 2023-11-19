#ifndef LOX_FUNCTION
#define LOX_FUNCTION

#include "lox_callable.cpp"
#include "stmt.cpp"
#include "types.h"
#include "interpreter.cpp"

class LoxFunction: public LoxCallable{
    public:
        Functionvp declaration;

        LoxFunction(Functionvp declaration){
            this->declaration = declaration;
        }

        rv call(Interpreter::Interpreter& interpreter, std::vector<rv> arguments){
            Environment* environment = new Environment(interpreter.globals);
            for (int i = 0; i < declaration->params.size(); i++) {
            environment->define(declaration->params[i].lexeme,
                arguments[i]);
            }

            interpreter.executeBlock(declaration->body, environment);
            delete environment; // free memory
            return "";
        }

        int arity(){
            return declaration->params.size();
        }

        std::string toString() {
            return "<fn " + declaration->name.lexeme + ">";
        }


};

#endif