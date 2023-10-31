#ifndef UTIL_H
#define UTIL_H

#include <string>


namespace Util {
    void report(int line, std::string where, std::string message);

    void error(int line, std::string message);
}


#endif