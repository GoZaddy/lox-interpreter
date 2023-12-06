#include "util.h"

#include <iostream>


void Util::report(int line, std::string where, std::string message){
    std::cerr << "[line " << line << "] Error" << where << ": " << message << std::endl;
    hadError = true;
}

void Util::error(int line, std::string message){
    report(line, "", message);
}

void Util::error(Token token, std::string message){
    if (token.type == END_OF_FILE){
        report(token.line, "at end", message);
    } else {
        report(token.line, " at '"+token.lexeme+"'", message);
    }
}

std::string Util::runtimeError(Token token, std::string message){
    std::cerr << message << "\n[line " << token.line << "]" << std::endl;
    hadRuntimeError = true;
    return "Runtime error!";
}

double Util::doub(std::string input){
    return std::stod(input);
}

bool Util::isNumberLiteral(rv expr){
    if (expr == nullptr) return false;
    return expr->getType() == DOUBLE_T;
}

bool Util::isStringLiteral(rv expr){
    if (expr == nullptr) return false;
    return expr->getType() == STRING_T;
}

std::string Util::GetString(rv expr){
    return ((String*) expr)->getValue();
}

double Util::GetDouble(rv expr){
    return ((Double*) expr)->getValue();
}

bool Util::isCallable(rv expr){
    if (expr == nullptr) return false;
    Type type = expr->getType();
    return type == LOX_FUNCTION || type == LOX_CLASS || type == LOX_CALLABLE;
}

// bool Util::isClassMethod(rv expr);

bool Util::isClass(rv expr){
    if (expr == nullptr) return false;
    return expr->getType() == LOX_CLASS;
}

bool Util::isInstance(rv expr){
    if (expr == nullptr) return false;
    return expr->getType() == LOX_INSTANCE;
}

bool Util::isEqual(rv a, rv b){
    if (a == nullptr || b == nullptr) return false;

    Type typeA = a->getType();
    Type typeB = b->getType();



    if (typeA != typeB){
        return false;
    }

    if (typeA == STRING_T){
        return (GetString(a) == GetString(b));
    }

    if (typeA == DOUBLE_T){
        return (GetDouble(a) == GetDouble(b));
    }

    if (typeA == BOOLEAN){
        return ((Boolean*) a)->getValue() == ((Boolean*) b)->getValue();
    }

    return a == b;
}

bool Util::isTruthy(rv object){
    if (object == nullptr) return false;
    if (object->getType() == BOOLEAN){
        return ((Boolean*) object)->getValue();
    }
    return true;
}