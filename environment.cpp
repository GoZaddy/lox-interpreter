// #include "environment.h"
#include <sstream>

#include "declr.h"


Environment::Environment(std::string environmentKey){
    enclosing = nullptr;
    this->environmentKey = environmentKey;
}


Environment::Environment(Environment* enclosing, std::string environmentKey) {
    this->enclosing = enclosing;
    this->environmentKey = environmentKey;
}

void Environment::define(string name, rv value){
    this->values[name] = value;
}


rv Environment::get(Token name){
    if (values.find(name.lexeme) != values.end()){
        return values[name.lexeme];
    }

    if (enclosing != nullptr) return enclosing->get(name);

    throw Util::runtimeError(
        name,
         "Undefined variable '" + name.lexeme + "'."
    );
}

rv Environment::getAt(int distance, string name) {
    return ancestor(distance)->values[name];
}

void Environment::assignAt(int distance, Token name, rv value) {
    ancestor(distance)->values[name.lexeme] = value;
}

Environment* Environment::ancestor(int distance) {
    Environment* environment = this;
    for (int i = 0; i < distance; i++) {
        environment = environment->enclosing; 
    }

    return environment;
}



void Environment::assign(Token name, rv value) {
    if (values.find(name.lexeme) != values.end()) {
        values[name.lexeme] = value;
        return;
    }

    if (enclosing != nullptr) {
        enclosing->assign(name, value);
        return;
    }

    throw Util::runtimeError(name,
        "Undefined variable '" + name.lexeme + "'.");
}
