#ifndef UTIL_H
#define UTIL_H

#include <string>
#include "token.h"
#include "helper/object.h"




typedef Object* rv;


class Util{
    public:
        static bool hadError;
        static bool hadRuntimeError;

        static void report(int line, std::string where, std::string message);

        static void error(int line, std::string message);

        static void error(Token token, std::string message);

        static double doub(std::string input);

        static std::string runtimeError(Token token, std::string message);

        static bool isNumberLiteral(rv expr);

        static bool isStringLiteral(rv expr);

        static bool isCallable(rv expr);

        static bool isClassMethod(rv expr);

        static bool isClass(rv expr);

        static bool isInstance(rv expr);

        static bool isEqual(rv a, rv b);
        
        static bool isTruthy(rv object);

        static std::string GetString(rv expr);

        static double GetDouble(rv expr);

};

#endif