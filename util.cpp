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