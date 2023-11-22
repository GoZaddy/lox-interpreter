#ifndef LOX_INSTANCE_H
#define LOX_INSTANCE_H

#include <string>

#include "lox_class.h"

class LoxInstance {
    private:
        LoxClass* klass;
    
    public:
        LoxInstance(LoxClass* klass);

        std::string toString();
};

#endif