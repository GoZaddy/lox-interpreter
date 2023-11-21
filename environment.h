#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <unordered_map>
#include <string>
#include "token.h"
#include "util.h"
#include <iostream>
#include "lox_callable.h"

typedef std::string string;

class Environment{
    private:
        std::unordered_map<string, string> values;
        std::unordered_map<string, LoxCallable*> funcMap;
        Environment* enclosing;


        LoxCallable* getCallable(string key);

        Environment* ancestor(int distance); 

    public:
        friend class Interpreter;

        Environment();
         
        Environment(Environment* enclosing);

        void define(string name, string value);

        void defineFunc(string name, LoxCallable* func);

        string get(Token name);

        void assign(Token name, string value);

        string getAt(int distance, string name);

        void assignAt(int distance, Token name, string value);
        
};

#endif