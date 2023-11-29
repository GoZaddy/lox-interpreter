// #include "lox_function.h"

#include "declr.h"


LoxFunction::LoxFunction(Functionvp declaration, Environment* closure, bool isInitializer){
    this->isInitializer = isInitializer;
    this->closure = closure;
    this->declaration = declaration;
}


LoxFunction* LoxFunction::bind(LoxInstance* instance) {
    Environment* environment = new Environment(closure);
    environment->define("this", instance);
    return new LoxFunction(declaration, environment, isInitializer);
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
        if (isInitializer) return closure->getAt(0, "this");
        // delete environment; // free memory // commenting out so closures can work
        return returnValue.value;
    }
    
    if (isInitializer) return closure->getAt(0, "this");
    return nullptr;
}

int LoxFunction::arity(){
    return declaration->params.size();
}

std::string LoxFunction::toString() {
    return "<fn " + declaration->name.lexeme + ">";
}

Type LoxFunction::getType() const {
    return LOX_FUNCTION;
}