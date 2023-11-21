#include "environment.h"


LoxCallable* Environment::getCallable(string key){
    if (funcMap.find(key) != funcMap.end()){
        return funcMap[key];
    }

    if (enclosing != nullptr) return enclosing->getCallable(key);

    std::cerr << "internal error: invalid callable key" << key << endl;
    return nullptr;
}

Environment::Environment(){
    enclosing = nullptr;
}

Environment::Environment(Environment* enclosing) {
    this->enclosing = enclosing;
}

void Environment::define(string name, string value){
    this->values[name] = value;
}

void Environment::defineFunc(string name, LoxCallable* func){
    define(name, "()"+name);  // TODO: we should probably implement recursive assign as in assign()
    funcMap["()"+name] = func;
}

string Environment::get(Token name){
    if (values.find(name.lexeme) != values.end()){
        return values[name.lexeme];
    }

    if (enclosing != nullptr) return enclosing->get(name);

    throw Util::runtimeError(
        name,
         "Undefined variable '" + name.lexeme + "'."
    );
}

string Environment::getAt(int distance, string name) {
    return ancestor(distance)->values[name];
}

void Environment::assignAt(int distance, Token name, string value) {
    ancestor(distance)->values[name.lexeme] = value;
}

Environment* Environment::ancestor(int distance) {
    Environment* environment = this;
    for (int i = 0; i < distance; i++) {
        environment = environment->enclosing; 
    }

    return environment;
}



void Environment::assign(Token name, string value) {
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