// #include "lox_function.h"

#include "declr.h"


LoxFunction::LoxFunction(Functionvp declaration, Environment* closure){
    this->closure = closure;
    this->declaration = declaration;
}

rv LoxFunction::call(Interpreter* interpreter, std::vector<rv> arguments){
    Environment* environment = new Environment(closure);
    
    for (int i = 0; i < declaration->params.size(); i++) {
        environment->define(
            declaration->params[i].lexeme,
            arguments[i]
        );
    }

    try {
        interpreter->executeBlock(declaration->body, environment);
    } catch (ReturnException returnValue) {
        // delete environment; // free memory // commenting out so closures can work
        return returnValue.value;
    }
    
    return "nil";
}

int LoxFunction::arity(){
    return declaration->params.size();
}

std::string LoxFunction::toString() {
    return "<fn " + declaration->name.lexeme + ">";
}