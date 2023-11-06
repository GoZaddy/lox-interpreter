#include "util.h"

#include <iostream>


void Util::report(int line, std::string where, std::string message){
    std::cerr << "[line " << line << "] Error" << where << ": " << message;
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