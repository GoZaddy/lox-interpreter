#ifndef RETURN_CPP
#define RETURN_CPP

#include "types.h"

class ReturnException{
    public:
        rv value;

        ReturnException(rv value) {
            this->value = value;
        }
};

#endif