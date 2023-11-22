// #include "environment.h"
#include <sstream>

#include "declr.h"



LoxCallable* Environment::getCallable(string key){
    if (funcMap.find(key) != funcMap.end()){
        return funcMap[key];
    }

    if (enclosing != nullptr) return enclosing->getCallable(key);

    std::cerr << "internal error: invalid callable key" << key << endl;
    return nullptr;
}


LoxClass* Environment::getClass(string key){
    if (classMap.find(key) != classMap.end()){
        return classMap[key];
    }

    if (enclosing != nullptr) return enclosing->getClass(key);

    std::cerr << "internal error: invalid class key" << key << endl;
    return nullptr;
}


LoxInstance* Environment::getInstance(string key){
    if (instanceMap.find(key) != instanceMap.end()){
        return instanceMap[key];
    }

    if (enclosing != nullptr) return enclosing->getInstance(key);

    std::cerr << "internal error: invalid class key" << key << endl;
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


void Environment::addClass(string name, LoxClass* klass){
    string key = "(class)"+name;
    define(name, key);  // TODO: we should probably implement recursive assign as in assign()
    classMap[key] = klass;
}

string Environment::addInstance(LoxInstance* instance){
    stringstream ss;
    ss << "(instance) " << instance;

    instanceMap[ss.str()] = instance;

    return ss.str();
}


