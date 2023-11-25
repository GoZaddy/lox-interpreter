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
    if (initializer != null) {
        (methods["init"])->bind(instance)->call(interpreter, arguments);
    }
    
    return interpreter->environment->addInstance(instance);
}

rv LoxClass::findMethod(string name) {
    // class method key => (.) class_key method_name
    // class_key => (class)class_name
    if (methods.find(name) != methods.end()) {
        return "(.) (class)"+this->name+" "+name;
    }

    if (superclass != nullptr) {
      return superclass->findMethod(name);
    }

    return null;
}


int LoxClass::arity(){
    rv initializer = findMethod("init");
    if (initializer == null) return 0;
    return methods["init"]->arity();
}