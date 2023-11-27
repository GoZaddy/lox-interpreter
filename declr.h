#pragma once


#include <iostream>
#include "util.h"
#include <cstdlib>
#include "types.h"
#include <vector>
#include "return.cpp"
#include <unordered_map>
#include <chrono>

// forward declrs
class LoxClass;
class LoxFunction;
class Interpreter;
class LoxInstance;
class Resolver;
class Environment;



class LoxCallable{
    public:
        virtual int arity() = 0;
		virtual rv call(Interpreter* interpreter, std::vector<rv> arguments) = 0;
};




class LoxFunction: public LoxCallable{
    private:
        Environment* closure;
    public:
        Functionvp declaration;

        bool isInitializer;
        

        LoxFunction(Functionvp declaration, Environment* closure, bool isInitializer);

        LoxFunction* bind(LoxInstance* instance);

        rv call(Interpreter* interpreter, std::vector<rv> arguments);

        int arity();

        std::string toString();
};










typedef std::chrono::milliseconds milliseconds;

class Clock : public LoxCallable{
    public:
        int arity();

        rv call(
            Interpreter* interpreter,
            std::vector<rv> arguments
        );

        std::string toString();
};









typedef std::string string;

class Environment{
    private:
        std::unordered_map<string, string> values;
        std::unordered_map<string, LoxCallable*> funcMap;
        std::unordered_map<string, LoxClass*> classMap;
        std::unordered_map<string, LoxInstance*> instanceMap;
        Environment* enclosing;

        // environment keys are useful for referencing objects created in nested scopes.
        // This supports features like closures
        std::string environmentKey;


        LoxCallable* getCallable(string key);

        LoxClass* getClass(string key);

        LoxCallable* getClassMethod(string key);

        LoxInstance* getInstance(string key);

        Environment* ancestor(int distance); 

        

    public:
        friend class Interpreter;

        friend class LoxClass;

        Environment(std::string environmentKey="");
         
        Environment(Environment* enclosing, std::string environmentKey="");

        void define(string name, string value);

        void defineFunc(string name, LoxCallable* func);

        string get(Token name);

        void assign(Token name, string value);

        void addClass(string name, LoxClass* klass);

        string getAt(int distance, string name);

        void assignAt(int distance, Token name, string value);

        string addInstance(LoxInstance* instance);
        
};












class Interpreter : public ExprVisv, public StmtVisv{
    private:
        Environment* environment; // TODO: I wonder if this causes issues

        std::unordered_map<Exprvp, int> locals;

        rv evaluate(Exprvp expr);
        bool isTruthy(rv object);

        bool isStringLiteral(string literal);

        bool isNumberLiteral(string literal);

        bool isCallable(string expr);

        bool isClassMethod(string expr);

        bool isClass(string expr);

        bool isInstance(string expr);

        bool isEqual(rv a, rv b);

        void checkNumberOperand(Token operatorToken, rv operand);

        void checkNumberOperands(Token operatorToken, rv left, rv right);

        string stripTrailingZeroes(rv text);

        void execute(Stmtvp stmt);

        rv lookUpVariable(Token name, Exprvp expr);

        
    public:
        Environment* globals;

        friend class LoxClass;
        

        Interpreter();
        void interpret(std::vector<Stmtvp> statements);

        void executeBlock(
            std::vector<Stmtvp> statements,
            Environment* environment
        );


        rv visit(Litv* expr);

        rv visit(Logicalvp expr);

        rv visit(Groupv* expr);

        rv visit(Unav* expr);

        rv visit(Binv* expr);

        rv visit(Callvp expr);

        rv visit(Getvp expr);

        rv visit(Setvp expr);

        rv visit(Variablevp expr);

        rv visit(Assignvp expr);

        rv visit(Expressionvp stmt);

        rv visit(Functionvp stmt);

        rv visit(Ifvp stmt);

        rv visit(Printvp stmt);

        rv visit(Returnvp stmt);

        rv visit(Varvp stmt);

        rv visit(Whilevp stmt);

        rv visit(Blockvp stmt);

        rv visit(Classvp stmt);

        void resolve(Exprvp expr, int depth);

        rv visit(Thisvp expr);

        rv visit(Supervp expr);

};











class LoxClass: public LoxCallable{
    private:
        std::unordered_map<string, LoxFunction*> methods;
        LoxClass* superclass;
    public:
        friend class Environment;
        friend class LoxInstance;

        std::string name;

        LoxClass(std::string name, LoxClass* superclass, std::unordered_map<string, LoxFunction*> methods);

        std::string toString();

        rv call(Interpreter* interpreter, std::vector<rv> arguments);

        rv findMethod(string name);

        int arity();
};













class LoxInstance {
    private:
        LoxClass* klass;
        std::unordered_map<string, string> fields;
    
    public:
        LoxInstance(LoxClass* klass);

        string get(Token name);

        void set(Token name, string value);

        std::string toString();
};













#include "helper/custom_stack.h"
typedef std::unordered_map<std::string, bool> Mapsb;

enum FunctionType {
    NONE_FUNCTION,
    FUNCTION,
    INITIALIZER,
    METHOD
};

enum ClassType {
    NONE_CLASS,
    CLASS_,
    SUBCLASS
};

class Resolver: ExprVisv, StmtVisv {
    private:
        Interpreter* interpreter;

        Stack<Mapsb*>* scopes;

        FunctionType currentFunction;
        ClassType currentClass;

        void resolve(Stmtvp stmt);

        void resolve(Exprvp expr);

        void beginScope();

        void endScope();

        void declare(Token name);

        void define(Token name);

        void resolveLocal(Exprvp expr, Token name);

        void resolveFunction(Functionvp function, FunctionType type);

    public:
        Resolver(Interpreter* interpreter);

        void resolve(std::vector<Stmtvp> statements);

        rv visit(Litv* expr);

        rv visit(Logicalvp expr);

        rv visit(Groupv* expr);

        rv visit(Unav* expr);

        rv visit(Binv* expr);

        rv visit(Callvp expr);

        rv visit(Variablevp expr);

        rv visit(Assignvp expr);

        rv visit(Expressionvp stmt);

        rv visit(Functionvp stmt);

        rv visit(Ifvp stmt);

        rv visit(Printvp stmt);

        rv visit(Returnvp stmt);

        rv visit(Varvp stmt);

        rv visit(Whilevp stmt);

        rv visit(Blockvp stmt);

        rv visit(Classvp stmt); 

        rv visit(Getvp expr);

        rv visit(Setvp expr);

        rv visit(Thisvp expr);

        rv visit(Supervp expr);


};