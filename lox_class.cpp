// #include "lox_class.h"

#include "declr.h"



LoxClass::LoxClass(std::string name, LoxClass* superclass, std::unordered_map<string, LoxFunction*> methods) {
    this->name = name;
    this->methods = methods;
    this->superclass = superclass;
}

std::string LoxClass::toString() {
    return name;
}

rv LoxClass::call(Interpreter* interpreter, std::vector<rv> arguments){
    LoxInstance* instance = new LoxInstance(this);

    rv initializer = findMethod("init");
    if (initializer != nullptr) {
        (methods["init"])->bind(instance)->call(interpreter, arguments);
    }
    
    return instance;
}

LoxFunction* LoxClass::findMethod(string name) {
    // class method key => (.) class_key method_name
    // class_key => (class)class_name
    if (methods.find(name) != methods.end()) {
        return methods[name];
    }

    if (superclass != nullptr) {
      return superclass->findMethod(name);
    }

    return nullptr;
}


int LoxClass::arity(){
    rv initializer = findMethod("init");
    if (initializer == nullptr) return 0;
    return methods["init"]->arity();
}

Type LoxClass::getType() const{
    return LOX_CLASS;
}