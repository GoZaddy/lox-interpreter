#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <unordered_map>
#include <string>
#include "token.h"
#include "util.h"
#include <iostream>
#include "lox_callable.h"
#include "lox_class.cpp"

typedef std::string string;

class Environment{
    private:
        std::unordered_map<string, string> values;
        std::unordered_map<string, LoxCallable*> funcMap;
        std::unordered_map<string, LoxClass*> classMap;
        std::unordered_map<string, LoxInstance*> instanceMap;
        Environment* enclosing;


        LoxCallable* getCallable(string key);

        LoxClass* getClass(string key);

        LoxInstance* getInstance(string key);

        Environment* ancestor(int distance); 

        string addInstance(LoxInstance* instance);

    public:
        friend class Interpreter;

        friend class LoxClass;

        Environment();
         
        Environment(Environment* enclosing);

        void define(string name, string value);

        void defineFunc(string name, LoxCallable* func);

        string get(Token name);

        void assign(Token name, string value);

        void addClass(string name, LoxClass* klass);

        string getAt(int distance, string name);

        void assignAt(int distance, Token name, string value);
        
};

#endif