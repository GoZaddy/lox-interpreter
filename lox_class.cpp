// #include "lox_class.h"

#include "declr.h"



LoxClass::LoxClass(std::string name) {
    this->name = name;
}

std::string LoxClass::toString() {
    return name;
}

rv LoxClass::call(Interpreter* interpreter, std::vector<rv> arguments){
    LoxInstance* instance = new LoxInstance(this);
    
    return interpreter->environment->addInstance(instance);
}

int LoxClass::arity(){
    return 0;
}