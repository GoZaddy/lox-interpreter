#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <unordered_map>
#include <string>
#include "token.h"
#include "util.h"

typedef std::string string;

class Environment {
    private:
        std::unordered_map<string, string> values;

    public:
        void define(string name, string value){
            values[name] = value;
        }

        string get(Token name){
            if (values.find(name.lexeme) != values.end()){
                return values[name.lexeme];
            }


            throw Util::runtimeError(
                name,
                "Undefined variable '" + name.lexeme + "'."
            );
        }
};

#endif