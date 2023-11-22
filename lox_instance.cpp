// #include "lox_instance.h"

#include "declr.h"

std::string LoxInstance::toString() {
    return klass->name + " instance";
}

LoxInstance::LoxInstance(LoxClass* klass) {
    this->klass = klass;
}

string LoxInstance::get(string name){
    if (fields.find(name) != fields.end()) {
      return fields[name];
    }

    throw Util::runtimeError(name, 
        "Undefined property '" + name + "'.");
}