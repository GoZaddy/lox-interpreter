// #include "lox_instance.h"

#include "declr.h"
#include <sstream>

std::string LoxInstance::toString() {
    return klass->name + " instance";
}

LoxInstance::LoxInstance(LoxClass* klass) {
    this->klass = klass;
}

rv LoxInstance::get(Token name){
    if (fields.find(name.lexeme) != fields.end()) {
      return fields[name.lexeme];
    }

    // findMethod returns a key for getting a the LoxFunction* ojbect for the method
    // get will return a key containing instance pointer value and the key for findMethod
    // this way we can get the LoxFunction* object and the LoxInstance* and bind the method to the instance in
    //Interpreter::visit(call)
    LoxFunction* method = klass->findMethod(name.lexeme);
    if (method != nullptr) {
        return method->bind(this);
    }

    throw Util::runtimeError(name, 
        "Undefined property '" + name.lexeme + "'.");
}

void LoxInstance::set(Token name, rv value) {
    fields[name.lexeme] =  value;
}

Type LoxInstance::getType() const {
    return LOX_INSTANCE;
}