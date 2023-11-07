#ifndef UTIL_H
#define UTIL_H

#include <string>
#include "token.h"


class Util{
    public:
        static bool hadError;
        static bool hadRuntimeError;

        static void report(int line, std::string where, std::string message);

        static void error(int line, std::string message);

        static void error(Token token, std::string message);

        static double doub(std::string input);

        static std::string runtimeError(Token token, std::string message);

};

#endif