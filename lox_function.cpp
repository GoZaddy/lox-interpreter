#include "lox_function.h"

LoxFunction::LoxFunction(Functionvp declaration, Environment* closure){
    this->closure = closure;
    this->declaration = declaration;
}

rv LoxFunction::call(Interpreter* interpreter, std::vector<rv> arguments){
    Environment* environment = new Environment(closure);
    // cout << "environment: " << environment << endl;
    
    for (int i = 0; i < declaration->params.size(); i++) {
        environment->define(
            declaration->params[i].lexeme,
            arguments[i]
        );
    }

    try {
        interpreter->executeBlock(declaration->body, environment);
    } catch (ReturnException returnValue) {
        delete environment; // free memory
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