#include "util.h"

#include <iostream>


void Util::report(int line, std::string where, std::string message){
    std::cerr << "[line " << line << "] Error" << where << ": " << message;
}

void Util::error(int line, std::string message){
    report(line, "", message);
}