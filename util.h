#ifndef UTIL_H
#define UTIL_H

#include <string>
#include "token.h"


namespace Util {
    void report(int line, std::string where, std::string message);

    void error(int line, std::string message);

    void error(Token token, std::string message);
}


#endif